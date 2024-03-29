/* GStreamer
 *
 * Copyright (C) 2008 Sebastian Dröge <sebastian.droege@collabora.co.uk>.
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

#ifndef __GST_BIT_READER_H__
#define __GST_BIT_READER_H__

#include <gst/gst.h>

G_BEGIN_DECLS

/**
 * GstBitReader:
 * @data: Data from which the bit reader will read
 * @size: Size of @data in bytes
 * @byte: Current byte position
 * @bit: Bit position in the current byte
 *
 * A bit reader instance.
 */
typedef struct {
  const guint8 *data;
  guint size;

  guint byte;  /* Byte position */
  guint bit;   /* Bit position in the current byte */
} GstBitReader;
#ifdef __SYMBIAN32__
IMPORT_C
#endif


GstBitReader * gst_bit_reader_new (const guint8 *data, guint size);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

GstBitReader * gst_bit_reader_new_from_buffer (const GstBuffer *buffer);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

void gst_bit_reader_free (GstBitReader *reader);
#ifdef __SYMBIAN32__
IMPORT_C
#endif


void gst_bit_reader_init (GstBitReader *reader, const guint8 *data, guint size);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

void gst_bit_reader_init_from_buffer (GstBitReader *reader, const GstBuffer *buffer);
#ifdef __SYMBIAN32__
IMPORT_C
#endif


gboolean gst_bit_reader_set_pos (GstBitReader *reader, guint pos);
#ifdef __SYMBIAN32__
IMPORT_C
#endif


guint gst_bit_reader_get_pos (const GstBitReader *reader);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

guint gst_bit_reader_get_remaining (const GstBitReader *reader);
#ifdef __SYMBIAN32__
IMPORT_C
#endif


gboolean gst_bit_reader_skip (GstBitReader *reader, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_skip_to_byte (GstBitReader *reader);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_get_bits_uint8 (GstBitReader *reader, guint8 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_get_bits_uint16 (GstBitReader *reader, guint16 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_get_bits_uint32 (GstBitReader *reader, guint32 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_get_bits_uint64 (GstBitReader *reader, guint64 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_peek_bits_uint8 (const GstBitReader *reader, guint8 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_peek_bits_uint16 (const GstBitReader *reader, guint16 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_peek_bits_uint32 (const GstBitReader *reader, guint32 *val, guint nbits);
#ifdef __SYMBIAN32__
IMPORT_C
#endif

gboolean gst_bit_reader_peek_bits_uint64 (const GstBitReader *reader, guint64 *val, guint nbits);

/**
 * GST_BIT_READER_INIT:
 * @data: Data from which the #GstBitReader should read
 * @size: Size of @data in bytes
 *
 * A #GstBitReader must be initialized with this macro, before it can be
 * used. This macro can used be to initialize a variable, but it cannot
 * be assigned to a variable. In that case you have to use
 * gst_bit_reader_init().
 *
 * Since: 0.10.22
 */
#define GST_BIT_READER_INIT(data, size) {data, size, 0, 0}

/**
 * GST_BIT_READER_INIT_FROM_BUFFER:
 * @buffer: Buffer from which the #GstBitReader should read
 *
 * A #GstBitReader must be initialized with this macro, before it can be
 * used. This macro can used be to initialize a variable, but it cannot
 * be assigned to a variable. In that case you have to use
 * gst_bit_reader_init().
 *
 * Since: 0.10.22
 */
#define GST_BIT_READER_INIT_FROM_BUFFER(buffer) {GST_BUFFER_DATA (buffer), GST_BUFFER_SIZE (buffer), 0, 0}

G_END_DECLS

#endif /* __GST_BIT_READER_H__ */
