/* GStreamer
 *
 * unit test for audioresample, based on the audioresample unit test
 *
 * Copyright (C) <2005> Thomas Vander Stichele <thomas at apestaart dot org>
 * Copyright (C) <2006> Tim-Philipp Müller <tim at centricular net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
#include <gst/gst_global.h>
#include <unistd.h>

#include <gst/check/gstcheck.h>

#include <gst/audio/audio.h>

#define LOG_FILE "c:\\logs\\audioresample_logs.txt" 
#include "std_log_result.h" 
#define LOG_FILENAME_LINE __FILE__, __LINE__

//char* xmlfile = "gstsystemclock";

void create_xml(int result)
{

    if(result)
    {
        assert_failed = 1;
    } 

    testResultXml(xmlfile);
    close_log_file();

    if(result)
    {
        exit (-1);
    }    

}

#include "libgstreamer_wsd_solution.h" 


#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(threads_running,gstcheck,gboolean)
#define _gst_check_threads_running (*GET_GSTREAMER_WSD_VAR_NAME(threads_running,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_threads_running;
#endif
#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(raised_critical,gstcheck,gboolean)
#define _gst_check_raised_critical (*GET_GSTREAMER_WSD_VAR_NAME(raised_critical,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_raised_critical;
#endif
//gboolean _gst_check_raised_warning = FALSE;
#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(raised_warning,gstcheck,gboolean)
#define _gst_check_raised_warning (*GET_GSTREAMER_WSD_VAR_NAME(raised_warning,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_raised_warning;
#endif
//gboolean _gst_check_expecting_log = FALSE;
#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(expecting_log,gstcheck,gboolean)
#define _gst_check_expecting_log (*GET_GSTREAMER_WSD_VAR_NAME(expecting_log,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_expecting_log;
#endif

#if EMULATOR
GET_GLOBAL_VAR_FROM_TLS(buffers,gstcheck,GList*)
#define buffers (*GET_GSTREAMER_WSD_VAR_NAME(buffers,gstcheck,g)())
#else 
IMPORT_C extern GList *buffers;
#endif


/* For ease of programming we use globals to keep refs for our floating
 * src and sink pads we create; otherwise we always have to do get_pad,
 * get_peer, and then remove references in every test function */
static GstPad *mysrcpad, *mysinkpad;

#define RESAMPLE_CAPS_FLOAT     \
    "audio/x-raw-float, "               \
    "channels = (int) [ 1, MAX ], "     \
    "rate = (int) [ 1,  MAX ], "        \
    "endianness = (int) BYTE_ORDER, "   \
    "width = (int) { 32, 64 }"

#define RESAMPLE_CAPS_INT       \
    "audio/x-raw-int, "                 \
    "channels = (int) [ 1, MAX ], "     \
    "rate = (int) [ 1,  MAX ], "        \
    "endianness = (int) BYTE_ORDER, "   \
    "width = (int) 16, "                \
    "depth = (int) 16, "                \
    "signed = (bool) TRUE"

#define RESAMPLE_CAPS_TEMPLATE_STRING   \
    RESAMPLE_CAPS_FLOAT " ; " \
    RESAMPLE_CAPS_INT

static GstStaticPadTemplate sinktemplate = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (RESAMPLE_CAPS_TEMPLATE_STRING)
    );
static GstStaticPadTemplate srctemplate = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (RESAMPLE_CAPS_TEMPLATE_STRING)
    );

static GstElement *
setup_audioresample (int channels, int inrate, int outrate, int width,
    gboolean fp)
{
  GstElement *audioresample;
  GstCaps *caps;
  GstStructure *structure;

  GST_DEBUG ("setup_audioresample");
  audioresample = gst_check_setup_element ("audioresample");

  if (fp)
    caps = gst_caps_from_string (RESAMPLE_CAPS_FLOAT);
  else
    caps = gst_caps_from_string (RESAMPLE_CAPS_INT);
  structure = gst_caps_get_structure (caps, 0);
  gst_structure_set (structure, "channels", G_TYPE_INT, channels,
      "rate", G_TYPE_INT, inrate, "width", G_TYPE_INT, width, NULL);
  if (!fp)
    gst_structure_set (structure, "depth", G_TYPE_INT, width, NULL);
  fail_unless (gst_caps_is_fixed (caps));

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PAUSED) == GST_STATE_CHANGE_SUCCESS,
      "could not set to paused");

  mysrcpad = gst_check_setup_src_pad (audioresample, &srctemplate, caps);
  gst_pad_set_caps (mysrcpad, caps);
  gst_caps_unref (caps);

  if (fp)
    caps = gst_caps_from_string (RESAMPLE_CAPS_FLOAT);
  else
    caps = gst_caps_from_string (RESAMPLE_CAPS_INT);
  structure = gst_caps_get_structure (caps, 0);
  gst_structure_set (structure, "channels", G_TYPE_INT, channels,
      "rate", G_TYPE_INT, outrate, "width", G_TYPE_INT, width, NULL);
  if (!fp)
    gst_structure_set (structure, "depth", G_TYPE_INT, width, NULL);
  fail_unless (gst_caps_is_fixed (caps));

  mysinkpad = gst_check_setup_sink_pad (audioresample, &sinktemplate, caps);
  /* this installs a getcaps func that will always return the caps we set
   * later */
  gst_pad_set_caps (mysinkpad, caps);
  gst_pad_use_fixed_caps (mysinkpad);

  gst_pad_set_active (mysinkpad, TRUE);
  gst_pad_set_active (mysrcpad, TRUE);

  gst_caps_unref (caps);

  return audioresample;
}

static void
cleanup_audioresample (GstElement * audioresample)
{
  GST_DEBUG ("cleanup_audioresample");

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_NULL) == GST_STATE_CHANGE_SUCCESS, "could not set to NULL");

  gst_pad_set_active (mysrcpad, FALSE);
  gst_pad_set_active (mysinkpad, FALSE);
  gst_check_teardown_src_pad (audioresample);
  gst_check_teardown_sink_pad (audioresample);
  gst_check_teardown_element (audioresample);
}

static void
fail_unless_perfect_stream (void)
{
  guint64 timestamp = 0L, duration = 0L;
  guint64 offset = 0L, offset_end = 0L;

  GList *l;
  GstBuffer *buffer;

  for (l = buffers; l; l = l->next) {
    buffer = GST_BUFFER (l->data);
    ASSERT_BUFFER_REFCOUNT (buffer, "buffer", 1);
    GST_DEBUG ("buffer timestamp %" G_GUINT64_FORMAT ", duration %"
        G_GUINT64_FORMAT " offset %" G_GUINT64_FORMAT " offset_end %"
        G_GUINT64_FORMAT,
        GST_BUFFER_TIMESTAMP (buffer),
        GST_BUFFER_DURATION (buffer),
        GST_BUFFER_OFFSET (buffer), GST_BUFFER_OFFSET_END (buffer));

    fail_unless_equals_uint64 (timestamp, GST_BUFFER_TIMESTAMP (buffer));
    fail_unless_equals_uint64 (offset, GST_BUFFER_OFFSET (buffer));
    duration = GST_BUFFER_DURATION (buffer);
    offset_end = GST_BUFFER_OFFSET_END (buffer);

    timestamp += duration;
    offset = offset_end;
    gst_buffer_unref (buffer);
  }
  g_list_free (buffers);
  buffers = NULL;
}

/* this tests that the output is a perfect stream if the input is */
static void
test_perfect_stream_instance (int inrate, int outrate, int samples,
    int numbuffers)
{
  GstElement *audioresample;
  GstBuffer *inbuffer, *outbuffer;
  GstCaps *caps;
  guint64 offset = 0;

  int i, j;
  gint16 *p;

  audioresample = setup_audioresample (2, inrate, outrate, 16, FALSE);
  caps = gst_pad_get_negotiated_caps (mysrcpad);
  fail_unless (gst_caps_is_fixed (caps));

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PLAYING) == GST_STATE_CHANGE_SUCCESS,
      "could not set to playing");

  for (j = 1; j <= numbuffers; ++j) {

    inbuffer = gst_buffer_new_and_alloc (samples * 4);
    GST_BUFFER_DURATION (inbuffer) = GST_FRAMES_TO_CLOCK_TIME (samples, inrate);
    GST_BUFFER_TIMESTAMP (inbuffer) = GST_BUFFER_DURATION (inbuffer) * (j - 1);
    GST_BUFFER_OFFSET (inbuffer) = offset;
    offset += samples;
    GST_BUFFER_OFFSET_END (inbuffer) = offset;

    gst_buffer_set_caps (inbuffer, caps);

    p = (gint16 *) GST_BUFFER_DATA (inbuffer);

    /* create a 16 bit signed ramp */
    for (i = 0; i < samples; ++i) {
      *p = -32767 + i * (65535 / samples);
      ++p;
      *p = -32767 + i * (65535 / samples);
      ++p;
    }

    /* pushing gives away my reference ... */
    fail_unless (gst_pad_push (mysrcpad, inbuffer) == GST_FLOW_OK);
    /* ... but it ends up being collected on the global buffer list */
    fail_unless_equals_int (g_list_length (buffers), j);
  }

  /* FIXME: we should make audioresample handle eos by flushing out the last
   * samples, which will give us one more, small, buffer */
  fail_if ((outbuffer = (GstBuffer *) buffers->data) == NULL);
  ASSERT_BUFFER_REFCOUNT (outbuffer, "outbuffer", 1);

  fail_unless_perfect_stream ();

  /* cleanup */
  gst_caps_unref (caps);
  cleanup_audioresample (audioresample);
}


/* make sure that outgoing buffers are contiguous in timestamp/duration and
 * offset/offsetend
 */
void test_perfect_stream()
{
  /* integral scalings */
    xmlfile = "test_perfect_stream";
  std_log(LOG_FILENAME_LINE, "Test Started test_perfect_stream");
  test_perfect_stream_instance (48000, 24000, 500, 20);
  test_perfect_stream_instance (48000, 12000, 500, 20);
  test_perfect_stream_instance (12000, 24000, 500, 20);
  test_perfect_stream_instance (12000, 48000, 500, 20);

  /* non-integral scalings */
  test_perfect_stream_instance (44100, 8000, 500, 20);
  test_perfect_stream_instance (8000, 44100, 500, 20);

  /* wacky scalings */
  test_perfect_stream_instance (12345, 54321, 500, 20);
  test_perfect_stream_instance (101, 99, 500, 20);
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



/* this tests that the output is a correct discontinuous stream
 * if the input is; ie input drops in time come out the same way */
static void
test_discont_stream_instance (int inrate, int outrate, int samples,
    int numbuffers)
{
  GstElement *audioresample;
  GstBuffer *inbuffer, *outbuffer;
  GstCaps *caps;
  GstClockTime ints;

  int i, j;
  gint16 *p;

  GST_DEBUG ("inrate:%d outrate:%d samples:%d numbuffers:%d",
      inrate, outrate, samples, numbuffers);

  audioresample = setup_audioresample (2, inrate, outrate, 16, FALSE);
  caps = gst_pad_get_negotiated_caps (mysrcpad);
  fail_unless (gst_caps_is_fixed (caps));

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PLAYING) == GST_STATE_CHANGE_SUCCESS,
      "could not set to playing");

  for (j = 1; j <= numbuffers; ++j) {

    inbuffer = gst_buffer_new_and_alloc (samples * 4);
    GST_BUFFER_DURATION (inbuffer) = samples * GST_SECOND / inrate;
    /* "drop" half the buffers */
    ints = GST_BUFFER_DURATION (inbuffer) * 2 * (j - 1);
    GST_BUFFER_TIMESTAMP (inbuffer) = ints;
    GST_BUFFER_OFFSET (inbuffer) = (j - 1) * 2 * samples;
    GST_BUFFER_OFFSET_END (inbuffer) = j * 2 * samples + samples;

    gst_buffer_set_caps (inbuffer, caps);

    p = (gint16 *) GST_BUFFER_DATA (inbuffer);

    /* create a 16 bit signed ramp */
    for (i = 0; i < samples; ++i) {
      *p = -32767 + i * (65535 / samples);
      ++p;
      *p = -32767 + i * (65535 / samples);
      ++p;
    }

    GST_DEBUG ("Sending Buffer time:%" G_GUINT64_FORMAT " duration:%"
        G_GINT64_FORMAT " discont:%d offset:%" G_GUINT64_FORMAT " offset_end:%"
        G_GUINT64_FORMAT, GST_BUFFER_TIMESTAMP (inbuffer),
        GST_BUFFER_DURATION (inbuffer), GST_BUFFER_IS_DISCONT (inbuffer),
        GST_BUFFER_OFFSET (inbuffer), GST_BUFFER_OFFSET_END (inbuffer));
    /* pushing gives away my reference ... */
    fail_unless (gst_pad_push (mysrcpad, inbuffer) == GST_FLOW_OK);

    /* check if the timestamp of the pushed buffer matches the incoming one */
    outbuffer = g_list_nth_data (buffers, g_list_length (buffers) - 1);
    fail_if (outbuffer == NULL);
    fail_unless_equals_uint64 (ints, GST_BUFFER_TIMESTAMP (outbuffer));
    GST_DEBUG ("Got Buffer time:%" G_GUINT64_FORMAT " duration:%"
        G_GINT64_FORMAT " discont:%d offset:%" G_GUINT64_FORMAT " offset_end:%"
        G_GUINT64_FORMAT, GST_BUFFER_TIMESTAMP (outbuffer),
        GST_BUFFER_DURATION (outbuffer), GST_BUFFER_IS_DISCONT (outbuffer),
        GST_BUFFER_OFFSET (outbuffer), GST_BUFFER_OFFSET_END (outbuffer));
    if (j > 1) {
      fail_unless (GST_BUFFER_IS_DISCONT (outbuffer),
          "expected discont for buffer #%d", j);
    }
  }

  /* cleanup */
  gst_caps_unref (caps);
  cleanup_audioresample (audioresample);
}

void test_discont_stream()
{
  /* integral scalings */
    xmlfile = "test_discont_stream";
  std_log(LOG_FILENAME_LINE, "Test Started test_discont_stream");
  test_discont_stream_instance (48000, 24000, 500, 20);
  test_discont_stream_instance (48000, 12000, 500, 20);
  test_discont_stream_instance (12000, 24000, 500, 20);
  test_discont_stream_instance (12000, 48000, 500, 20);

  /* non-integral scalings */
  test_discont_stream_instance (44100, 8000, 500, 20);
  test_discont_stream_instance (8000, 44100, 500, 20);

  /* wacky scalings */
  test_discont_stream_instance (12345, 54321, 500, 20);
  test_discont_stream_instance (101, 99, 500, 20);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}





void test_reuse()
{
  GstElement *audioresample;
  GstEvent *newseg;
  GstBuffer *inbuffer;
  GstCaps *caps;
  xmlfile = "test_reuse";
std_log(LOG_FILENAME_LINE, "Test Started test_reuse");
  audioresample = setup_audioresample (1, 9343, 48000, 16, FALSE);
  caps = gst_pad_get_negotiated_caps (mysrcpad);
  fail_unless (gst_caps_is_fixed (caps));

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PLAYING) == GST_STATE_CHANGE_SUCCESS,
      "could not set to playing");

  newseg = gst_event_new_new_segment (FALSE, 1.0, GST_FORMAT_TIME, 0, -1, 0);
  fail_unless (gst_pad_push_event (mysrcpad, newseg) != FALSE);

  inbuffer = gst_buffer_new_and_alloc (9343 * 4);
  memset (GST_BUFFER_DATA (inbuffer), 0, GST_BUFFER_SIZE (inbuffer));
  GST_BUFFER_DURATION (inbuffer) = GST_SECOND;
  GST_BUFFER_TIMESTAMP (inbuffer) = 0;
  GST_BUFFER_OFFSET (inbuffer) = 0;
  gst_buffer_set_caps (inbuffer, caps);

  /* pushing gives away my reference ... */
  fail_unless (gst_pad_push (mysrcpad, inbuffer) == GST_FLOW_OK);

  /* ... but it ends up being collected on the global buffer list */
  fail_unless_equals_int (g_list_length (buffers), 1);

  /* now reset and try again ... */
  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_NULL) == GST_STATE_CHANGE_SUCCESS, "could not set to NULL");

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PLAYING) == GST_STATE_CHANGE_SUCCESS,
      "could not set to playing");

  newseg = gst_event_new_new_segment (FALSE, 1.0, GST_FORMAT_TIME, 0, -1, 0);
  fail_unless (gst_pad_push_event (mysrcpad, newseg) != FALSE);

  inbuffer = gst_buffer_new_and_alloc (9343 * 4);
  memset (GST_BUFFER_DATA (inbuffer), 0, GST_BUFFER_SIZE (inbuffer));
  GST_BUFFER_DURATION (inbuffer) = GST_SECOND;
  GST_BUFFER_TIMESTAMP (inbuffer) = 0;
  GST_BUFFER_OFFSET (inbuffer) = 0;
  gst_buffer_set_caps (inbuffer, caps);

  fail_unless (gst_pad_push (mysrcpad, inbuffer) == GST_FLOW_OK);

  /* ... it also ends up being collected on the global buffer list. If we
   * now have more than 2 buffers, then audioresample probably didn't clean
   * up its internal buffer properly and tried to push the remaining samples
   * when it got the second NEWSEGMENT event */
  fail_unless_equals_int (g_list_length (buffers), 2);

  cleanup_audioresample (audioresample);
  gst_caps_unref (caps);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



void test_shutdown()
{
  GstElement *pipeline, *src, *cf1, *ar, *cf2, *sink;
  GstCaps *caps;
  guint i;
  xmlfile = "test_shutdown";
std_log(LOG_FILENAME_LINE, "Test Started test_shutdown");
  /* create pipeline, force audioresample to actually resample */
  pipeline = gst_pipeline_new (NULL);

  src = gst_check_setup_element ("audiotestsrc");
  cf1 = gst_check_setup_element ("capsfilter");
  ar = gst_check_setup_element ("audioresample");
  cf2 = gst_check_setup_element ("capsfilter");
  g_object_set (cf2, "name", "capsfilter2", NULL);
  sink = gst_check_setup_element ("fakesink");

  caps =
      gst_caps_new_simple ("audio/x-raw-int", "rate", G_TYPE_INT, 11025, NULL);
  g_object_set (cf1, "caps", caps, NULL);
  gst_caps_unref (caps);

  caps =
      gst_caps_new_simple ("audio/x-raw-int", "rate", G_TYPE_INT, 48000, NULL);
  g_object_set (cf2, "caps", caps, NULL);
  gst_caps_unref (caps);

  /* don't want to sync against the clock, the more throughput the better */
  g_object_set (src, "is-live", FALSE, NULL);
  g_object_set (sink, "sync", FALSE, NULL);

  gst_bin_add_many (GST_BIN (pipeline), src, cf1, ar, cf2, sink, NULL);
  fail_if (!gst_element_link_many (src, cf1, ar, cf2, sink, NULL));

  /* now, wait until pipeline is running and then shut it down again; repeat */
  for (i = 0; i < 20; ++i) {
    gst_element_set_state (pipeline, GST_STATE_PAUSED);
    gst_element_get_state (pipeline, NULL, NULL, -1);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_usleep (100);
    gst_element_set_state (pipeline, GST_STATE_NULL);
  }

  gst_object_unref (pipeline);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



static GstFlowReturn
live_switch_alloc_only_48000 (GstPad * pad, guint64 offset,
    guint size, GstCaps * caps, GstBuffer ** buf)
{
  GstStructure *structure;
  gint rate;
  gint channels;
  GstCaps *desired;

  structure = gst_caps_get_structure (caps, 0);
  fail_unless (gst_structure_get_int (structure, "rate", &rate));
  fail_unless (gst_structure_get_int (structure, "channels", &channels));

  if (rate < 48000)
    return GST_FLOW_NOT_NEGOTIATED;

  desired = gst_caps_copy (caps);
  gst_caps_set_simple (desired, "rate", G_TYPE_INT, 48000, NULL);

  *buf = gst_buffer_new_and_alloc (channels * 48000);
  gst_buffer_set_caps (*buf, desired);
  gst_caps_unref (desired);

  return GST_FLOW_OK;
}

static GstCaps *
live_switch_get_sink_caps (GstPad * pad)
{
  GstCaps *result;

  result = gst_caps_copy (GST_PAD_CAPS (pad));

  gst_caps_set_simple (result,
      "rate", GST_TYPE_INT_RANGE, 48000, G_MAXINT, NULL);

  return result;
}

static void
live_switch_push (int rate, GstCaps * caps)
{
  GstBuffer *inbuffer;
  GstCaps *desired;
  GList *l;

  desired = gst_caps_copy (caps);
  gst_caps_set_simple (desired, "rate", G_TYPE_INT, rate, NULL);
  gst_pad_set_caps (mysrcpad, desired);

  fail_unless (gst_pad_alloc_buffer_and_set_caps (mysrcpad,
          GST_BUFFER_OFFSET_NONE, rate * 4, desired, &inbuffer) == GST_FLOW_OK);

  /* When the basetransform hits the non-configured case it always
   * returns a buffer with exactly the same caps as we requested so the actual
   * renegotiation (if needed) will be done in the _chain*/
  fail_unless (inbuffer != NULL);
  GST_DEBUG ("desired: %" GST_PTR_FORMAT ".... got: %" GST_PTR_FORMAT,
      desired, GST_BUFFER_CAPS (inbuffer));
  fail_unless (gst_caps_is_equal (desired, GST_BUFFER_CAPS (inbuffer)));

  memset (GST_BUFFER_DATA (inbuffer), 0, GST_BUFFER_SIZE (inbuffer));
  GST_BUFFER_DURATION (inbuffer) = GST_SECOND;
  GST_BUFFER_TIMESTAMP (inbuffer) = 0;
  GST_BUFFER_OFFSET (inbuffer) = 0;

  /* pushing gives away my reference ... */
  fail_unless (gst_pad_push (mysrcpad, inbuffer) == GST_FLOW_OK);

  /* ... but it ends up being collected on the global buffer list */
  fail_unless_equals_int (g_list_length (buffers), 1);

  for (l = buffers; l; l = l->next) {
    GstBuffer *buffer = GST_BUFFER (l->data);

    gst_buffer_unref (buffer);
  }

  g_list_free (buffers);
  buffers = NULL;

  gst_caps_unref (desired);
}

void test_live_switch()
{
  GstElement *audioresample;
  GstEvent *newseg;
  GstCaps *caps;
  xmlfile = "test_live_switch";
std_log(LOG_FILENAME_LINE, "Test Started test_live_switch");
  audioresample = setup_audioresample (4, 48000, 48000, 16, FALSE);

  /* Let the sinkpad act like something that can only handle things of
   * rate 48000- and can only allocate buffers for that rate, but if someone
   * tries to get a buffer with a rate higher then 48000 tries to renegotiate
   * */
  gst_pad_set_bufferalloc_function (mysinkpad, live_switch_alloc_only_48000);
  gst_pad_set_getcaps_function (mysinkpad, live_switch_get_sink_caps);

  gst_pad_use_fixed_caps (mysrcpad);

  caps = gst_pad_get_negotiated_caps (mysrcpad);
  fail_unless (gst_caps_is_fixed (caps));

  fail_unless (gst_element_set_state (audioresample,
          GST_STATE_PLAYING) == GST_STATE_CHANGE_SUCCESS,
      "could not set to playing");

  newseg = gst_event_new_new_segment (FALSE, 1.0, GST_FORMAT_TIME, 0, -1, 0);
  fail_unless (gst_pad_push_event (mysrcpad, newseg) != FALSE);

  /* downstream can provide the requested rate, a buffer alloc will be passed
   * on */
  live_switch_push (48000, caps);

  /* Downstream can never accept this rate, buffer alloc isn't passed on */
  live_switch_push (40000, caps);

  /* Downstream can provide the requested rate but will re-negotiate */
  live_switch_push (50000, caps);

  cleanup_audioresample (audioresample);
  gst_caps_unref (caps);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



#ifndef GST_DISABLE_PARSE

static GMainLoop *loop;
static gint messages = 0;

static void
element_message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
{
  gchar *s;

  s = gst_structure_to_string (gst_message_get_structure (message));
  GST_DEBUG ("Received message: %s", s);
  g_free (s);

  messages++;
}

static void
eos_message_cb (GstBus * bus, GstMessage * message, gpointer user_data)
{
  GST_DEBUG ("Received eos");
  g_main_loop_quit (loop);
}

static void
test_pipeline (gint width, gboolean fp, gint inrate, gint outrate, gint quality)
{
  GstElement *pipeline;
  GstBus *bus;
  GError *error = NULL;
  gchar *pipe_str;

  pipe_str =
      g_strdup_printf
      ("audiotestsrc num-buffers=10 ! audioconvert ! audio/x-raw-%s,rate=%d,width=%d,channels=2 ! audioresample quality=%d ! audio/x-raw-%s,rate=%d,width=%d ! identity check-imperfect-timestamp=TRUE ! fakesink",
      (fp) ? "float" : "int", inrate, width, quality, (fp) ? "float" : "int",
      outrate, width);

  pipeline = gst_parse_launch (pipe_str, &error);
  fail_unless (pipeline != NULL, "Error parsing pipeline: %s",
      error ? error->message : "(invalid error)");
  g_free (pipe_str);

  bus = gst_element_get_bus (pipeline);
  fail_if (bus == NULL);
  gst_bus_add_signal_watch (bus);
  g_signal_connect (bus, "message::element", (GCallback) element_message_cb,
      NULL);
  g_signal_connect (bus, "message::eos", (GCallback) eos_message_cb, NULL);

  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* run until we receive EOS */
  loop = g_main_loop_new (NULL, FALSE);

  g_main_loop_run (loop);

  g_main_loop_unref (loop);
  loop = NULL;

  gst_element_set_state (pipeline, GST_STATE_NULL);

  fail_if (messages > 0, "Received imperfect timestamp messages");
  gst_object_unref (pipeline);
}

void test_pipelines()
{
  gint quality;
  xmlfile = "test_pipelines";
std_log(LOG_FILENAME_LINE, "Test Started test_pipelines");
  /* Test qualities 0, 5 and 10 */
  for (quality = 0; quality < 11; quality += 5) {
    test_pipeline (8, FALSE, 44100, 48000, quality);
    test_pipeline (8, FALSE, 48000, 44100, quality);

    test_pipeline (16, FALSE, 44100, 48000, quality);
    test_pipeline (16, FALSE, 48000, 44100, quality);

    test_pipeline (24, FALSE, 44100, 48000, quality);
    test_pipeline (24, FALSE, 48000, 44100, quality);

    test_pipeline (32, FALSE, 44100, 48000, quality);
    test_pipeline (32, FALSE, 48000, 44100, quality);

    test_pipeline (32, TRUE, 44100, 48000, quality);
    test_pipeline (32, TRUE, 48000, 44100, quality);

    test_pipeline (64, TRUE, 44100, 48000, quality);
    test_pipeline (64, TRUE, 48000, 44100, quality); 
  }
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



void test_preference_passthrough()
{
  GstStateChangeReturn ret;
  GstElement *pipeline, *src;
  GstStructure *s;
  GstMessage *msg;
  GstCaps *caps;
  GstPad *pad;
  GstBus *bus;
  GError *error = NULL;
  gint rate = 0;

  xmlfile = "test_preference_passthrough";
std_log(LOG_FILENAME_LINE, "Test Started test_preference_passthrough");
  pipeline = gst_parse_launch ("audiotestsrc num-buffers=1 name=src ! "
      "audioresample ! "
      "audio/x-raw-int,rate=8000,channels=1,width=16,depth=16,signed=(boolean)true,endianness=(int)BYTE_ORDER ! "
      "fakesink can-activate-pull=0 ", &error);
  fail_unless (pipeline != NULL, "Error parsing pipeline: %s",
      error ? error->message : "(invalid error)");

  ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
  fail_unless_equals_int (ret, GST_STATE_CHANGE_ASYNC);

  /* run until we receive EOS */
  bus = gst_element_get_bus (pipeline);
  fail_if (bus == NULL);
  msg = gst_bus_timed_pop_filtered (bus, -1, GST_MESSAGE_EOS);
  gst_message_unref (msg);
  gst_object_unref (bus);

  src = gst_bin_get_by_name (GST_BIN (pipeline), "src");
  fail_unless (src != NULL);
  pad = gst_element_get_static_pad (src, "src");
  fail_unless (pad != NULL);
  caps = gst_pad_get_negotiated_caps (pad);
  GST_LOG ("negotiated audiotestsrc caps: %" GST_PTR_FORMAT, caps);
  fail_unless (caps != NULL);
  s = gst_caps_get_structure (caps, 0);
  fail_unless (gst_structure_get_int (s, "rate", &rate));
  /* there's no need to resample, audiotestsrc supports any rate, so make
   * sure audioresample provided upstream with the right caps to negotiate
   * this correctly */
  fail_unless_equals_int (rate, 8000);
  gst_caps_unref (caps);
  gst_object_unref (pad);
  gst_object_unref (src);

  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
  create_xml(0);
}



#endif

//static Suite *
//audioresample_suite (void)
//{
//  Suite *s = suite_create ("audioresample");
//  TCase *tc_chain = tcase_create ("general");
//
//  suite_add_tcase (s, tc_chain);
//  tcase_add_test (tc_chain, test_perfect_stream);
//  tcase_add_test (tc_chain, test_discont_stream);
//  tcase_add_test (tc_chain, test_reuse);
//  tcase_add_test (tc_chain, test_shutdown);
//  tcase_add_test (tc_chain, test_live_switch);
//
//#ifndef GST_DISABLE_PARSE
//  tcase_set_timeout (tc_chain, 360);
//  tcase_add_test (tc_chain, test_pipelines);
//  tcase_add_test (tc_chain, test_preference_passthrough);
//#endif
//
//  return s;
//}

void (*fn[]) (void) = {
test_perfect_stream,
test_discont_stream,
test_reuse,
test_shutdown,
test_live_switch,
test_pipelines,
test_preference_passthrough
};

char *args[] = {
"test_perfect_stream",
"test_discont_stream",
"test_reuse",
"test_shutdown",
"test_live_switch",
"test_pipelines",
"test_preference_passthrough"
};

GST_CHECK_MAIN (audioresample);



//int main (int argc, char **argv)
//{
////gst_init(&argc, &argv);                
//gst_check_init (NULL, NULL); 
//test_perfect_stream();
//test_discont_stream();
//test_shutdown(); 
//test_reuse();
//
//}
