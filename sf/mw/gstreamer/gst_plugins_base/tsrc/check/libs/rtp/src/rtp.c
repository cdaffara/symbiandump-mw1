/* GStreamer unit tests for the RTP support library
 *
 * Copyright (C) 2007 Tim-Philipp Müller <tim centricular net>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst_global.h>
#include <gst/check/gstcheck.h>
#include <glib_global.h>



#define LOG_FILE "c:\\logs\\gstrtp_logs.txt" 
#include "std_log_result.h" 
#define LOG_FILENAME_LINE __FILE__, __LINE__


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

#include <gst/rtp/gstrtpbuffer.h>
#include <gst/rtp/gstrtcpbuffer.h>
#include <string.h>

#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(raised_critical,gstcheck,gboolean)
#define _gst_check_raised_critical (*GET_GSTREAMER_WSD_VAR_NAME(raised_critical,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_raised_critical;
#endif

#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(raised_warning,gstcheck,gboolean)
#define _gst_check_raised_warning (*GET_GSTREAMER_WSD_VAR_NAME(raised_warning,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_raised_warning;
#endif

#if EMULATOR
static GET_GLOBAL_VAR_FROM_TLS(expecting_log,gstcheck,gboolean)
#define _gst_check_expecting_log (*GET_GSTREAMER_WSD_VAR_NAME(expecting_log,gstcheck,g)())
#else 
IMPORT_C extern gboolean _gst_check_expecting_log;
#endif


#define RTP_HEADER_LEN 12

void test_rtp_buffer()
{
  GstBuffer *buf;
  guint8 *data;

    xmlfile = "gst_rtp_test_rtp_buffer";
    std_log(LOG_FILENAME_LINE, "Test Started test_rtp_buffer");

  /* check GstRTPHeader structure alignment and packing */
  buf = (GstBuffer *) gst_rtp_buffer_new_allocate ((guint)16, (guint8)4, (guint8)0);
  fail_unless (buf != NULL);
  fail_unless_equals_int (GST_BUFFER_SIZE (buf), RTP_HEADER_LEN + 16 + 4);
  data = GST_BUFFER_DATA (buf);

  /* check defaults */
  fail_unless_equals_int (gst_rtp_buffer_get_version (buf), 2);
  fail_unless (gst_rtp_buffer_get_padding (buf) == FALSE);
  fail_unless (gst_rtp_buffer_get_extension (buf) == FALSE);
  fail_unless_equals_int (gst_rtp_buffer_get_csrc_count (buf), 0);
  fail_unless (gst_rtp_buffer_get_marker (buf) == FALSE);
  fail_unless (gst_rtp_buffer_get_payload_type (buf) == 0);
  fail_unless_equals_int (GST_READ_UINT16_BE (data), 0x8000);

  /* check version in bitfield */
  gst_rtp_buffer_set_version (buf, 3);
  fail_unless_equals_int (gst_rtp_buffer_get_version (buf), 3);
  fail_unless_equals_int ((data[0] & 0xC0) >> 6, 3);
  gst_rtp_buffer_set_version (buf, 2);
  fail_unless_equals_int (gst_rtp_buffer_get_version (buf), 2);
  fail_unless_equals_int ((data[0] & 0xC0) >> 6, 2);

  /* check padding bit */
  gst_rtp_buffer_set_padding (buf, TRUE);
  fail_unless (gst_rtp_buffer_get_padding (buf) == TRUE);
  fail_unless_equals_int ((data[0] & 0x20) >> 5, 1);
  gst_rtp_buffer_set_padding (buf, FALSE);
  fail_unless (gst_rtp_buffer_get_padding (buf) == FALSE);
  fail_unless_equals_int ((data[0] & 0x20) >> 5, 0);

  /* check marker bit */
  gst_rtp_buffer_set_marker (buf, TRUE);
  fail_unless (gst_rtp_buffer_get_marker (buf) == TRUE);
  fail_unless_equals_int ((data[1] & 0x80) >> 7, 1);
  gst_rtp_buffer_set_marker (buf, FALSE);
  fail_unless (gst_rtp_buffer_get_marker (buf) == FALSE);
  fail_unless_equals_int ((data[1] & 0x80) >> 7, 0);

  /* check sequence offset */
  gst_rtp_buffer_set_seq (buf, 0xF2C9);
  fail_unless_equals_int (gst_rtp_buffer_get_seq (buf), 0xF2C9);
  fail_unless_equals_int (GST_READ_UINT16_BE (data + 2), 0xF2C9);
  gst_rtp_buffer_set_seq (buf, 0);
  fail_unless_equals_int (gst_rtp_buffer_get_seq (buf), 0);
  fail_unless_equals_int (GST_READ_UINT16_BE (data + 2), 0);

  /* check timestamp offset */
  gst_rtp_buffer_set_timestamp (buf, 432191);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 4), 432191);
  fail_unless_equals_int (gst_rtp_buffer_get_timestamp (buf), 432191);
  gst_rtp_buffer_set_timestamp (buf, 0);
  fail_unless_equals_int (gst_rtp_buffer_get_timestamp (buf), 0);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 4), 0);

  /* check ssrc offset */
  gst_rtp_buffer_set_ssrc (buf, 0xf04043C2);
  fail_unless_equals_int (gst_rtp_buffer_get_ssrc (buf), (gint) 0xf04043c2);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 4 + 4), (gint) 0xf04043c2);
  gst_rtp_buffer_set_ssrc (buf, 0);
  fail_unless_equals_int (gst_rtp_buffer_get_ssrc (buf), 0);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 4 + 4), 0);

  /* check csrc bits */
  fail_unless_equals_int (gst_rtp_buffer_get_csrc_count (buf), 0);
  ASSERT_CRITICAL (gst_rtp_buffer_get_csrc (buf, 0));
  fail_unless_equals_int (data[0] & 0xf, 0);
  gst_buffer_unref (buf);

  /* and again, this time with CSRCs */
  buf = (GstBuffer *) gst_rtp_buffer_new_allocate ((guint)16, (guint8)4, (guint8)3);
  fail_unless (buf != NULL);
  fail_unless_equals_int (GST_BUFFER_SIZE (buf),
      RTP_HEADER_LEN + 16 + 4 + 4 * 3);

  data = GST_BUFFER_DATA (buf);

  fail_unless_equals_int (gst_rtp_buffer_get_csrc_count (buf), 3);
  ASSERT_CRITICAL (gst_rtp_buffer_get_csrc (buf, 3));
  fail_unless_equals_int (data[0] & 0xf, 3);
  fail_unless_equals_int (gst_rtp_buffer_get_csrc (buf, 0), 0);
  fail_unless_equals_int (gst_rtp_buffer_get_csrc (buf, 1), 0);
  fail_unless_equals_int (gst_rtp_buffer_get_csrc (buf, 2), 0);

  data += RTP_HEADER_LEN;       /* skip the other header stuff */
  gst_rtp_buffer_set_csrc (buf, 0, 0xf7c0);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 0 * 4), 0xf7c0);
  gst_rtp_buffer_set_csrc (buf, 1, 0xf7c1);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 1 * 4), 0xf7c1);
  gst_rtp_buffer_set_csrc (buf, 2, 0xf7c2);
  fail_unless_equals_int (GST_READ_UINT32_BE (data + 2 * 4), 0xf7c2);
  ASSERT_CRITICAL (gst_rtp_buffer_set_csrc (buf, 3, 0xf123));
  gst_buffer_unref (buf);
    std_log(LOG_FILENAME_LINE, "Test Successful");
      create_xml(0);
}



void test_rtp_buffer_set_extension_data()
{
  GstBuffer *buf;
  guint8 *data;
  guint16 bits;
  guint size;
  gpointer pointer;
  
      xmlfile = "gst_rtp_test_rtp_buffer_set_extension_data";
    std_log(LOG_FILENAME_LINE, "Test Started test_rtp_buffer_set_extension_data");

  /* check GstRTPHeader structure alignment and packing */
  buf = (GstBuffer *) gst_rtp_buffer_new_allocate ((guint)4, (guint8)0, (guint8)0);
  data = GST_BUFFER_DATA (buf);

  /* should be impossible to set the extension data */
  ASSERT_WARNING (fail_unless (gst_rtp_buffer_set_extension_data (buf, 0, 4) == FALSE));
  fail_unless (gst_rtp_buffer_get_extension (buf) == FALSE);

  /* should be possible to set the extension data */
  fail_unless (gst_rtp_buffer_set_extension_data (buf, 270, 0) == TRUE);
  fail_unless (gst_rtp_buffer_get_extension (buf) == TRUE);
  gst_rtp_buffer_get_extension_data (buf, &bits, &pointer, &size);
  fail_unless (bits == 270);
  fail_unless (size == 0);
  fail_unless (pointer == GST_BUFFER_DATA (buf) + 16);
  pointer = (gpointer)gst_rtp_buffer_get_payload ((GstBuffer *) buf);
  fail_unless (pointer == GST_BUFFER_DATA (buf) + 16);
  gst_buffer_unref (buf);

  buf = (GstBuffer *)gst_rtp_buffer_new_allocate ((guint)20, (guint8)0, (guint8)0);
  data = GST_BUFFER_DATA (buf);
  fail_unless (gst_rtp_buffer_get_extension (buf) == FALSE);
  fail_unless (gst_rtp_buffer_set_extension_data (buf, 333, 2) == TRUE);
  fail_unless (gst_rtp_buffer_get_extension (buf) == TRUE);
  gst_rtp_buffer_get_extension_data (buf, &bits, &pointer, &size);
  fail_unless (bits == 333);
  fail_unless (size == 2);
  fail_unless (pointer == GST_BUFFER_DATA (buf) + 16);
  pointer = (gpointer)gst_rtp_buffer_get_payload ((GstBuffer *)buf);
  fail_unless (pointer == GST_BUFFER_DATA (buf) + 24);
  gst_buffer_unref (buf);
    std_log(LOG_FILENAME_LINE, "Test Successful");
      create_xml(0);
}

void test_rtp_seqnum_compare()
{
    xmlfile = "test_rtp_seqnum_compare";
    std_log(LOG_FILENAME_LINE, "Test Started test_rtp_seqnum_compare");
    
#define ASSERT_COMP(a,b,c) fail_unless (gst_rtp_buffer_compare_seqnum ((guint16)a,(guint16)b) == c);
  ASSERT_COMP (0xfffe, 0xfffd, -1);
  ASSERT_COMP (0xffff, 0xfffe, -1);
  ASSERT_COMP (0x0000, 0xffff, -1);
  ASSERT_COMP (0x0001, 0x0000, -1);
  ASSERT_COMP (0x0002, 0x0001, -1);

  ASSERT_COMP (0xffff, 0xfffd, -2);
  ASSERT_COMP (0x0000, 0xfffd, -3);
  ASSERT_COMP (0x0001, 0xfffd, -4);
  ASSERT_COMP (0x0002, 0xfffd, -5);

  ASSERT_COMP (0x7ffe, 0x7ffd, -1);
  ASSERT_COMP (0x7fff, 0x7ffe, -1);
  ASSERT_COMP (0x8000, 0x7fff, -1);
  ASSERT_COMP (0x8001, 0x8000, -1);
  ASSERT_COMP (0x8002, 0x8001, -1);

  ASSERT_COMP (0x7fff, 0x7ffd, -2);
  ASSERT_COMP (0x8000, 0x7ffd, -3);
  ASSERT_COMP (0x8001, 0x7ffd, -4);
  ASSERT_COMP (0x8002, 0x7ffd, -5);

  ASSERT_COMP (0x7ffd, 0xffff, -0x7ffe);
  ASSERT_COMP (0x7ffe, 0x0000, -0x7ffe);
  ASSERT_COMP (0x7fff, 0x0001, -0x7ffe);
  ASSERT_COMP (0x7fff, 0x0000, -0x7fff);
  ASSERT_COMP (0x8000, 0x0001, -0x7fff);
  ASSERT_COMP (0x8001, 0x0002, -0x7fff);

  ASSERT_COMP (0xfffd, 0x7ffe, -0x7fff);
  ASSERT_COMP (0xfffe, 0x7fff, -0x7fff);
  ASSERT_COMP (0xffff, 0x8000, -0x7fff);
  ASSERT_COMP (0x0000, 0x8001, -0x7fff);
  ASSERT_COMP (0x0001, 0x8002, -0x7fff);

  ASSERT_COMP (0xfffe, 0x7ffe, -0x8000);
  ASSERT_COMP (0xffff, 0x7fff, -0x8000);
  ASSERT_COMP (0x0000, 0x8000, -0x8000);
  ASSERT_COMP (0x0001, 0x8001, -0x8000);

  ASSERT_COMP (0x7ffe, 0xfffe, -0x8000);
  ASSERT_COMP (0x7fff, 0xffff, -0x8000);
  ASSERT_COMP (0x8000, 0x0000, -0x8000);
  ASSERT_COMP (0x8001, 0x0001, -0x8000);

  ASSERT_COMP (0x0001, 0x0002, 1);
  ASSERT_COMP (0x0000, 0x0001, 1);
  ASSERT_COMP (0xffff, 0x0000, 1);
  ASSERT_COMP (0xfffe, 0xffff, 1);
  ASSERT_COMP (0xfffd, 0xfffe, 1);

  ASSERT_COMP (0x0000, 0x0002, 2);
  ASSERT_COMP (0xffff, 0x0002, 3);
  ASSERT_COMP (0xfffe, 0x0002, 4);
  ASSERT_COMP (0xfffd, 0x0002, 5);

  ASSERT_COMP (0x8001, 0x8002, 1);
  ASSERT_COMP (0x8000, 0x8001, 1);
  ASSERT_COMP (0x7fff, 0x8000, 1);
  ASSERT_COMP (0x7ffe, 0x7fff, 1);
  ASSERT_COMP (0x7ffd, 0x7ffe, 1);

  ASSERT_COMP (0x8000, 0x8002, 2);
  ASSERT_COMP (0x7fff, 0x8002, 3);
  ASSERT_COMP (0x7ffe, 0x8002, 4);
  ASSERT_COMP (0x7ffd, 0x8002, 5);

  ASSERT_COMP (0xfffe, 0x7ffd, 0x7fff);
  ASSERT_COMP (0xffff, 0x7ffe, 0x7fff);
  ASSERT_COMP (0x0000, 0x7fff, 0x7fff);
  ASSERT_COMP (0x0001, 0x8000, 0x7fff);
  ASSERT_COMP (0x0002, 0x8001, 0x7fff);

  ASSERT_COMP (0x7ffe, 0xfffd, 0x7fff);
  ASSERT_COMP (0x7fff, 0xfffe, 0x7fff);
  ASSERT_COMP (0x8000, 0xffff, 0x7fff);
  ASSERT_COMP (0x8001, 0x0000, 0x7fff);
  ASSERT_COMP (0x8002, 0x0001, 0x7fff);
#undef ASSERT_COMP
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
    create_xml(0);
}


void test_rtcp_buffer()
{
  GstBuffer *buf;
  GstRTCPPacket packet;
  guint8 *data;
  
  xmlfile = "test_rtcp_buffer";
  std_log(LOG_FILENAME_LINE, "Test Started test_rtcp_buffer");

  buf = gst_rtcp_buffer_new (1400);
  fail_unless (buf != NULL);
  fail_unless_equals_int (GST_BUFFER_SIZE (buf), 1400);
  data = GST_BUFFER_DATA (buf);

  fail_unless (gst_rtcp_buffer_get_first_packet (buf, &packet) == FALSE);
  fail_unless (gst_rtcp_buffer_get_packet_count (buf) == 0);
  fail_unless (gst_rtcp_buffer_validate (buf) == FALSE);

  /* add an SR packet */
  fail_unless (gst_rtcp_buffer_add_packet (buf, GST_RTCP_TYPE_SR,
          &packet) == TRUE);

  fail_unless (gst_rtcp_packet_get_padding (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_count (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_type (&packet) == GST_RTCP_TYPE_SR);
  fail_unless (gst_rtcp_packet_get_length (&packet) == 6);

  gst_rtcp_packet_sr_set_sender_info (&packet, 0x44556677,
      G_GUINT64_CONSTANT (1), 0x11111111, 101, 123456);
  {
    guint32 ssrc;
    guint64 ntptime;
    guint32 rtptime;
    guint32 packet_count;
    guint32 octet_count;

    gst_rtcp_packet_sr_get_sender_info (&packet, &ssrc, &ntptime, &rtptime,
        &packet_count, &octet_count);

    fail_unless (ssrc == 0x44556677);
    fail_unless (ntptime == G_GUINT64_CONSTANT (1));
    fail_unless (rtptime == 0x11111111);
    fail_unless (packet_count == 101);
    fail_unless (octet_count == 123456);
  }

  /* go to first packet, this should be the packet we just added */
  fail_unless (gst_rtcp_buffer_get_first_packet (buf, &packet) == TRUE);

  fail_unless (gst_rtcp_packet_get_padding (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_count (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_type (&packet) == GST_RTCP_TYPE_SR);
  fail_unless (gst_rtcp_packet_get_length (&packet) == 6);

  fail_unless (gst_rtcp_packet_move_to_next (&packet) == FALSE);

  /* add some SDES */
  fail_unless (gst_rtcp_buffer_add_packet (buf, GST_RTCP_TYPE_SDES,
          &packet) == TRUE);
  fail_unless (gst_rtcp_packet_sdes_add_item (&packet, 0xff658743) == TRUE);
  fail_unless (gst_rtcp_packet_sdes_add_entry (&packet, GST_RTCP_SDES_CNAME,
          sizeof ("test@foo.bar"), (guint8 *) "test@foo.bar") == TRUE);

  /* add some BYE */
  fail_unless (gst_rtcp_buffer_add_packet (buf, GST_RTCP_TYPE_BYE,
          &packet) == TRUE);
  fail_unless (gst_rtcp_packet_bye_add_ssrc (&packet, 0x5613212f) == TRUE);
  fail_unless (gst_rtcp_packet_bye_add_ssrc (&packet, 0x00112233) == TRUE);
  fail_unless (gst_rtcp_packet_bye_get_ssrc_count (&packet) == 2);

  fail_unless (gst_rtcp_packet_get_padding (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_count (&packet) == 2);
  fail_unless (gst_rtcp_packet_get_type (&packet) == GST_RTCP_TYPE_BYE);
  fail_unless (gst_rtcp_packet_get_length (&packet) == 2);

  /* move to SDES */
  fail_unless (gst_rtcp_buffer_get_first_packet (buf, &packet) == TRUE);
  fail_unless (gst_rtcp_packet_move_to_next (&packet) == TRUE);

  fail_unless (gst_rtcp_packet_get_padding (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_count (&packet) == 1);
  fail_unless (gst_rtcp_packet_get_type (&packet) == GST_RTCP_TYPE_SDES);
  fail_unless (gst_rtcp_packet_get_length (&packet) == 5);

  /* remove the SDES */
  fail_unless (gst_rtcp_packet_remove (&packet) == TRUE);

  /* we are now at the BYE packet */
  fail_unless (gst_rtcp_packet_get_padding (&packet) == 0);
  fail_unless (gst_rtcp_packet_get_count (&packet) == 2);
  fail_unless (gst_rtcp_packet_get_type (&packet) == GST_RTCP_TYPE_BYE);
  fail_unless (gst_rtcp_packet_get_length (&packet) == 2);

  /* close and validate */
  gst_rtcp_buffer_end (buf);
  fail_unless (gst_rtcp_buffer_validate (buf) == TRUE);
  gst_buffer_unref (buf);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
    create_xml(0);
}

void test_rtp_buffer_validate_corrupt()
{
  GstBuffer *buf;
  guint8 corrupt_rtp_packet[58] = {
    0x90, 0x7a, 0xbf, 0x28, 0x3a, 0x8a, 0x0a, 0xf4, 0x69, 0x6b, 0x76, 0xc0,
    0x21, 0xe0, 0xe0, 0x60, 0x81, 0x10, 0x84, 0x30, 0x21, 0x52, 0x06, 0xc2,
    0xb8, 0x30, 0x10, 0x4c, 0x08, 0x62, 0x67, 0xc2, 0x6e, 0x1a, 0x53, 0x3f,
    0xaf, 0xd6, 0x1b, 0x29, 0x40, 0xe0, 0xa5, 0x83, 0x01, 0x4b, 0x04, 0x02,
    0xb0, 0x97, 0x63, 0x08, 0x10, 0x4b, 0x43, 0x85, 0x37, 0x2c
  };

  xmlfile = "test_rtp_buffer_validate_corrupt";
  std_log(LOG_FILENAME_LINE, "Test Started test_rtp_buffer_validate_corrupt");
  buf = gst_buffer_new ();
  GST_BUFFER_DATA (buf) = corrupt_rtp_packet;
  GST_BUFFER_SIZE (buf) = sizeof (corrupt_rtp_packet);
  fail_if (gst_rtp_buffer_validate (buf));
  gst_buffer_unref (buf);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
    create_xml(0);
}


void test_rtp_buffer_list()
{
  GstBuffer *rtp_header;
  GstBuffer *rtp_payload;
  GstBufferList *list = NULL;
  GstBufferListIterator *it;
  guint i;

  xmlfile = "test_rtp_buffer_list";
  std_log(LOG_FILENAME_LINE, "Test Started test_rtp_buffer_list");
  
  list = gst_buffer_list_new ();
  it = gst_buffer_list_iterate (list);

  /* Creating a list of two RTP packages */

  /* Create first group to hold the rtp header and the payload */
  gst_buffer_list_iterator_add_group (it);
  rtp_header = gst_rtp_buffer_new_allocate (0, 0, 0);
  gst_buffer_list_iterator_add (it, rtp_header);
  rtp_payload = gst_buffer_new_and_alloc (42);
  gst_buffer_list_iterator_add (it, rtp_payload);

  /* Create second group to hold an rtp header and a payload */
  gst_buffer_list_iterator_add_group (it);
  rtp_header = gst_rtp_buffer_new_allocate (0, 0, 0);
  gst_buffer_list_iterator_add (it, rtp_header);
  rtp_payload = gst_buffer_new_and_alloc (42);
  gst_buffer_list_iterator_add (it, rtp_payload);

  gst_buffer_list_iterator_free (it);

  /* Test SEQ number */
  i = gst_rtp_buffer_list_set_seq (list, 1024);
  fail_if (1026 != i);
  fail_if (!gst_rtp_buffer_list_validate (list));

  /* Timestamp */
  gst_rtp_buffer_list_set_timestamp (list, 432191);
  fail_unless_equals_int (gst_rtp_buffer_list_get_timestamp (list), 432191);

  /* SSRC */
  gst_rtp_buffer_list_set_ssrc (list, 0xf04043C2);
  fail_unless_equals_int (gst_rtp_buffer_list_get_ssrc (list),
      (gint) 0xf04043c2);

  /* Payload type */
  gst_rtp_buffer_list_set_payload_type (list, 127);
  fail_unless_equals_int (gst_rtp_buffer_list_get_payload_type (list), 127);

  gst_buffer_list_unref (list);
  
  std_log(LOG_FILENAME_LINE, "Test Successful");
    create_xml(0);
}


void (*fn[]) (void) = {
test_rtp_buffer,
test_rtp_buffer_set_extension_data,
test_rtp_buffer_validate_corrupt,
test_rtp_seqnum_compare,
test_rtp_buffer_list,
test_rtcp_buffer
};

char *args[] = {
"test_rtp_buffer",
"test_rtp_buffer_set_extension_data",
"test_rtp_buffer_validate_corrupt",
"test_rtp_seqnum_compare",
"test_rtp_buffer_list",
"test_rtcp_buffer"
};

GST_CHECK_MAIN (gst_rtp);


#if 0
void main()
    {
    gst_check_init (NULL, NULL);
    test_rtp_buffer();
    test_rtp_buffer_set_extension_data();
    }
    
#endif 


