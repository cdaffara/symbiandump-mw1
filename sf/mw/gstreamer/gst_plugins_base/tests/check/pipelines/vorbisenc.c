/* GStreamer
 *
 * unit test for vorbisenc
 *
 * Copyright (C) 2006 Andy Wingo <wingo at pobox.com>
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

#include <gst/check/gstcheck.h>
#include <gst/check/gstbufferstraw.h>

#ifndef GST_DISABLE_PARSE

#define TIMESTAMP_OFFSET G_GINT64_CONSTANT(3249870963)

static void
check_buffer_timestamp (GstBuffer * buffer, GstClockTime timestamp)
{
  fail_unless (GST_BUFFER_TIMESTAMP (buffer) == timestamp,
      "expected timestamp %" GST_TIME_FORMAT
      ", but got timestamp %" GST_TIME_FORMAT,
      GST_TIME_ARGS (timestamp), GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (buffer)));
}

static void
check_buffer_duration (GstBuffer * buffer, GstClockTime duration)
{
  fail_unless (GST_BUFFER_DURATION (buffer) == duration,
      "expected duration %" GST_TIME_FORMAT
      ", but got duration %" GST_TIME_FORMAT,
      GST_TIME_ARGS (duration), GST_TIME_ARGS (GST_BUFFER_DURATION (buffer)));
}

static void
check_buffer_granulepos (GstBuffer * buffer, gint64 granulepos)
{
  GstClockTime clocktime;

  fail_unless (GST_BUFFER_OFFSET_END (buffer) == granulepos,
      "expected granulepos %" G_GUINT64_FORMAT
      ", but got granulepos %" G_GUINT64_FORMAT,
      granulepos, GST_BUFFER_OFFSET_END (buffer));

  /* contrary to what we record as TIMESTAMP, we can use OFFSET to check
   * the granulepos correctly here */
  clocktime = gst_util_uint64_scale (GST_BUFFER_OFFSET_END (buffer), 44100,
      GST_SECOND);

  fail_unless (clocktime == GST_BUFFER_OFFSET (buffer),
      "expected OFFSET set to clocktime %" GST_TIME_FORMAT
      ", but got %" GST_TIME_FORMAT,
      GST_TIME_ARGS (clocktime), GST_TIME_ARGS (GST_BUFFER_OFFSET (buffer)));
}

/* this check is here to check that the granulepos we derive from the timestamp
   is about correct. This is "about correct" because you can't precisely go from
   timestamp to granulepos due to the downward-rounding characteristics of
   gst_util_uint64_scale, so you check if granulepos is equal to the number, or
   the number plus one. */
static void
check_buffer_granulepos_from_endtime (GstBuffer * buffer, GstClockTime endtime)
{
  gint64 granulepos, expected;

  granulepos = GST_BUFFER_OFFSET_END (buffer);
  expected = gst_util_uint64_scale (endtime, 44100, GST_SECOND);

  fail_unless (granulepos == expected || granulepos == expected + 1,
      "expected granulepos %" G_GUINT64_FORMAT
      " or %" G_GUINT64_FORMAT
      ", but got granulepos %" G_GUINT64_FORMAT,
      expected, expected + 1, granulepos);
}

GST_START_TEST (test_granulepos_offset)
{
  GstElement *bin;
  GstPad *pad;
  gchar *pipe_str;
  GstBuffer *buffer;
  GError *error = NULL;

  pipe_str = g_strdup_printf ("audiotestsrc timestamp-offset=%" G_GUINT64_FORMAT
      " ! audio/x-raw-int,rate=44100"
      " ! audioconvert ! vorbisenc ! fakesink", TIMESTAMP_OFFSET);

  bin = gst_parse_launch (pipe_str, &error);
  fail_unless (bin != NULL, "Error parsing pipeline: %s",
      error ? error->message : "(invalid error)");
  g_free (pipe_str);

  /* get the pad */
  {
    GstElement *sink = gst_bin_get_by_name (GST_BIN (bin), "fakesink0");

    fail_unless (sink != NULL, "Could not get fakesink out of bin");
    pad = gst_element_get_pad (sink, "sink");
    fail_unless (pad != NULL, "Could not get pad out of fakesink");
    gst_object_unref (sink);
  }

  gst_buffer_straw_start_pipeline (bin, pad);

  /* header packets should have timestamp == NONE, granulepos 0 */
  buffer = gst_buffer_straw_get_buffer (bin, pad);
  GST_DEBUG ("Got buffer in test");
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);
  GST_DEBUG ("Unreffed buffer in test");

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  {
    GstClockTime next_timestamp;
    gint64 last_granulepos = 0;

    /* first buffer should have timestamp of TIMESTAMP_OFFSET, granulepos to
     * match the timestamp of the end of the last sample in the output buffer.
     * Note that one cannot go timestamp->granulepos->timestamp and get the same
     * value due to loss of precision with granulepos. vorbisenc does take care
     * to timestamp correctly based on the offset of the input data however, so
     * it does do sub-granulepos timestamping. */
    buffer = gst_buffer_straw_get_buffer (bin, pad);
    last_granulepos = GST_BUFFER_OFFSET_END (buffer);
    check_buffer_timestamp (buffer, TIMESTAMP_OFFSET);
    /* don't really have a good way of checking duration... */
    check_buffer_granulepos_from_endtime (buffer,
        TIMESTAMP_OFFSET + GST_BUFFER_DURATION (buffer));

    next_timestamp = TIMESTAMP_OFFSET + GST_BUFFER_DURATION (buffer);

    gst_buffer_unref (buffer);

    /* check continuity with the next buffer */
    buffer = gst_buffer_straw_get_buffer (bin, pad);
    check_buffer_timestamp (buffer, next_timestamp);
    check_buffer_duration (buffer,
        gst_util_uint64_scale (GST_BUFFER_OFFSET_END (buffer), GST_SECOND,
            44100)
        - gst_util_uint64_scale (last_granulepos, GST_SECOND, 44100));
    check_buffer_granulepos_from_endtime (buffer,
        next_timestamp + GST_BUFFER_DURATION (buffer));

    gst_buffer_unref (buffer);
  }

  gst_buffer_straw_stop_pipeline (bin, pad);

  gst_object_unref (pad);
  gst_object_unref (bin);
}

GST_END_TEST;

GST_START_TEST (test_timestamps)
{
  GstElement *bin;
  GstPad *pad;
  gchar *pipe_str;
  GstBuffer *buffer;
  GError *error = NULL;

  pipe_str = g_strdup_printf ("audiotestsrc"
      " ! audio/x-raw-int,rate=44100 ! audioconvert ! vorbisenc ! fakesink");

  bin = gst_parse_launch (pipe_str, &error);
  fail_unless (bin != NULL, "Error parsing pipeline: %s",
      error ? error->message : "(invalid error)");
  g_free (pipe_str);

  /* get the pad */
  {
    GstElement *sink = gst_bin_get_by_name (GST_BIN (bin), "fakesink0");

    fail_unless (sink != NULL, "Could not get fakesink out of bin");
    pad = gst_element_get_pad (sink, "sink");
    fail_unless (pad != NULL, "Could not get pad out of fakesink");
    gst_object_unref (sink);
  }

  gst_buffer_straw_start_pipeline (bin, pad);

  /* check header packets */
  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  {
    GstClockTime next_timestamp;
    gint64 last_granulepos;

    /* first buffer has timestamp 0 */
    buffer = gst_buffer_straw_get_buffer (bin, pad);
    last_granulepos = GST_BUFFER_OFFSET_END (buffer);
    check_buffer_timestamp (buffer, 0);
    /* don't really have a good way of checking duration... */
    check_buffer_granulepos_from_endtime (buffer, GST_BUFFER_DURATION (buffer));

    next_timestamp = GST_BUFFER_DURATION (buffer);

    gst_buffer_unref (buffer);

    /* check continuity with the next buffer */
    buffer = gst_buffer_straw_get_buffer (bin, pad);
    check_buffer_timestamp (buffer, next_timestamp);
    check_buffer_duration (buffer,
        gst_util_uint64_scale (GST_BUFFER_OFFSET_END (buffer), GST_SECOND,
            44100)
        - gst_util_uint64_scale (last_granulepos, GST_SECOND, 44100));
    check_buffer_granulepos_from_endtime (buffer,
        next_timestamp + GST_BUFFER_DURATION (buffer));

    gst_buffer_unref (buffer);
  }

  gst_buffer_straw_stop_pipeline (bin, pad);

  gst_object_unref (pad);
  gst_object_unref (bin);
}

GST_END_TEST;

static gboolean
drop_second_data_buffer (GstPad * droppad, GstBuffer * buffer, gpointer unused)
{
  return !(GST_BUFFER_OFFSET (buffer) == 1024);
}

GST_START_TEST (test_discontinuity)
{
  GstElement *bin;
  GstPad *pad, *droppad;
  gchar *pipe_str;
  GstBuffer *buffer;
  GError *error = NULL;
  guint drop_id;

  pipe_str = g_strdup_printf ("audiotestsrc samplesperbuffer=1024"
      " ! audio/x-raw-int,rate=44100" " ! audioconvert ! vorbisenc ! fakesink");

  bin = gst_parse_launch (pipe_str, &error);
  fail_unless (bin != NULL, "Error parsing pipeline: %s",
      error ? error->message : "(invalid error)");
  g_free (pipe_str);

  /* the plan: same as test_timestamps, but dropping a buffer and seeing if
     vorbisenc correctly notes the discontinuity */

  /* get the pad to use to drop buffers */
  {
    GstElement *sink = gst_bin_get_by_name (GST_BIN (bin), "vorbisenc0");

    fail_unless (sink != NULL, "Could not get vorbisenc out of bin");
    droppad = gst_element_get_pad (sink, "sink");
    fail_unless (droppad != NULL, "Could not get pad out of vorbisenc");
    gst_object_unref (sink);
  }

  /* get the pad */
  {
    GstElement *sink = gst_bin_get_by_name (GST_BIN (bin), "fakesink0");

    fail_unless (sink != NULL, "Could not get fakesink out of bin");
    pad = gst_element_get_pad (sink, "sink");
    fail_unless (pad != NULL, "Could not get pad out of fakesink");
    gst_object_unref (sink);
  }

  drop_id = gst_pad_add_buffer_probe (droppad,
      G_CALLBACK (drop_second_data_buffer), NULL);
  gst_buffer_straw_start_pipeline (bin, pad);

  /* check header packets */
  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  buffer = gst_buffer_straw_get_buffer (bin, pad);
  check_buffer_timestamp (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_duration (buffer, GST_CLOCK_TIME_NONE);
  check_buffer_granulepos (buffer, 0);
  gst_buffer_unref (buffer);

  /* two phases: continuous granulepos values up to 1024, then a first
     discontinuous granulepos whose granulepos corresponds to a gap ending at
     2048. */
  {
    GstClockTime next_timestamp = 0;
    gint64 last_granulepos = 0;

    while (last_granulepos < 1024) {
      buffer = gst_buffer_straw_get_buffer (bin, pad);
      last_granulepos = GST_BUFFER_OFFSET_END (buffer);
      check_buffer_timestamp (buffer, next_timestamp);
      fail_if (GST_BUFFER_IS_DISCONT (buffer), "expected continuous buffer");
      next_timestamp += GST_BUFFER_DURATION (buffer);
      gst_buffer_unref (buffer);
    }

    fail_unless (last_granulepos == 1024,
        "unexpected granulepos: %" G_GUINT64_FORMAT, last_granulepos);
  }

  {
    buffer = gst_buffer_straw_get_buffer (bin, pad);
    /* The first buffer after the discontinuity will produce zero output
     * samples (because of the overlap/add), so it won't increment the 
     * granulepos, which should be 2048 after the discontinuity.
     */
    fail_unless (GST_BUFFER_OFFSET_END (buffer) == 2048,
        "expected granulepos after gap: %" G_GUINT64_FORMAT,
        GST_BUFFER_OFFSET_END (buffer));
    fail_unless (GST_BUFFER_IS_DISCONT (buffer),
        "expected discontinuous buffer");
    gst_buffer_unref (buffer);
  }

  gst_buffer_straw_stop_pipeline (bin, pad);
  gst_pad_remove_buffer_probe (droppad, drop_id);

  gst_object_unref (droppad);
  gst_object_unref (pad);
  gst_object_unref (bin);
}

GST_END_TEST;

#endif /* #ifndef GST_DISABLE_PARSE */

static Suite *
vorbisenc_suite (void)
{
  Suite *s = suite_create ("vorbisenc");
  TCase *tc_chain = tcase_create ("general");

  suite_add_tcase (s, tc_chain);
#ifndef GST_DISABLE_PARSE
  tcase_add_test (tc_chain, test_granulepos_offset);
  tcase_add_test (tc_chain, test_timestamps);
  tcase_add_test (tc_chain, test_discontinuity);
#endif

  return s;
}

int
main (int argc, char **argv)
{
  int nf;

  Suite *s = vorbisenc_suite ();
  SRunner *sr = srunner_create (s);

  gst_check_init (&argc, &argv);

  srunner_run_all (sr, CK_NORMAL);
  nf = srunner_ntests_failed (sr);
  srunner_free (sr);

  return nf;
}
