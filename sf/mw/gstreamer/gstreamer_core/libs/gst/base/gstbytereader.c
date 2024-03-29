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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstbytereader.h"

#include <string.h>

/**
 * SECTION:gstbytereader
 * @short_description: Reads different integer, string and floating point
 *     types from a memory buffer
 *
 * #GstByteReader provides a byte reader that can read different integer and
 * floating point types from a memory buffer. It provides functions for reading
 * signed/unsigned, little/big endian integers of 8, 16, 24, 32 and 64 bits
 * and functions for reading little/big endian floating points numbers of
 * 32 and 64 bits. It also provides functions to read NUL-terminated strings
 * in various character encodings.
 */

/**
 * gst_byte_reader_new:
 * @data: Data from which the #GstByteReader should read
 * @size: Size of @data in bytes
 *
 * Create a new #GstByteReader instance, which will read from @data.
 *
 * Returns: a new #GstByteReader instance
 *
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

GstByteReader *
gst_byte_reader_new (const guint8 * data, guint size)
{
  GstByteReader *ret = g_slice_new0 (GstByteReader);

  ret->data = data;
  ret->size = size;

  return ret;
}

/**
 * gst_byte_reader_new_from_buffer:
 * @buffer: Buffer from which the #GstByteReader should read
 *
 * Create a new #GstByteReader instance, which will read from the
 * #GstBuffer @buffer.
 *
 * Returns: a new #GstByteReader instance
 *
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

GstByteReader *
gst_byte_reader_new_from_buffer (const GstBuffer * buffer)
{
  g_return_val_if_fail (GST_IS_BUFFER (buffer), NULL);

  return gst_byte_reader_new (GST_BUFFER_DATA (buffer),
      GST_BUFFER_SIZE (buffer));
}

/**
 * gst_byte_reader_free:
 * @reader: a #GstByteReader instance
 *
 * Frees a #GstByteReader instance, which was previously allocated by
 * gst_byte_reader_new() or gst_byte_reader_new_from_buffer().
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

void
gst_byte_reader_free (GstByteReader * reader)
{
  g_return_if_fail (reader != NULL);

  g_slice_free (GstByteReader, reader);
}

/**
 * gst_byte_reader_init:
 * @reader: a #GstByteReader instance
 * @data: Data from which the #GstByteReader should read
 * @size: Size of @data in bytes
 *
 * Initializes a #GstByteReader instance to read from @data. This function
 * can be called on already initialized instances.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

void
gst_byte_reader_init (GstByteReader * reader, const guint8 * data, guint size)
{
  g_return_if_fail (reader != NULL);

  reader->data = data;
  reader->size = size;
  reader->byte = 0;
}

/**
 * gst_byte_reader_init_from_buffer:
 * @reader: a #GstByteReader instance
 * @buffer: Buffer from which the #GstByteReader should read
 *
 * Initializes a #GstByteReader instance to read from @buffer. This function
 * can be called on already initialized instances.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

void
gst_byte_reader_init_from_buffer (GstByteReader * reader,
    const GstBuffer * buffer)
{
  g_return_if_fail (GST_IS_BUFFER (buffer));

  gst_byte_reader_init (reader, GST_BUFFER_DATA (buffer),
      GST_BUFFER_SIZE (buffer));
}

/**
 * gst_byte_reader_set_pos:
 * @reader: a #GstByteReader instance
 * @pos: The new position in bytes
 *
 * Sets the new position of a #GstByteReader instance to @pos in bytes.
 *
 * Returns: %TRUE if the position could be set successfully, %FALSE
 * otherwise.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_set_pos (GstByteReader * reader, guint pos)
{
  g_return_val_if_fail (reader != NULL, FALSE);

  if (pos > reader->size)
    return FALSE;

  reader->byte = pos;

  return TRUE;
}

/**
 * gst_byte_reader_get_pos:
 * @reader: a #GstByteReader instance
 *
 * Returns the current position of a #GstByteReader instance in bytes.
 *
 * Returns: The current position of @reader in bytes.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

guint
gst_byte_reader_get_pos (const GstByteReader * reader)
{
  g_return_val_if_fail (reader != NULL, 0);

  return reader->byte;
}

/**
 * gst_byte_reader_get_remaining:
 * @reader: a #GstByteReader instance
 *
 * Returns the remaining number of bytes of a #GstByteReader instance.
 *
 * Returns: The remaining number of bytes of @reader instance.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

guint
gst_byte_reader_get_remaining (const GstByteReader * reader)
{
  g_return_val_if_fail (reader != NULL, 0);

  return reader->size - reader->byte;
}

/**
 * gst_byte_reader_skip:
 * @reader: a #GstByteReader instance
 * @nbytes: the number of bytes to skip
 *
 * Skips @nbytes bytes of the #GstByteReader instance.
 *
 * Returns: %TRUE if @nbytes bytes could be skipped, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_skip (GstByteReader * reader, guint nbytes)
{
  g_return_val_if_fail (reader != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < nbytes)
    return FALSE;

  reader->byte += nbytes;

  return TRUE;
}

/**
 * gst_byte_reader_get_uint8:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint8 to store the result
 *
 * Read an unsigned 8 bit integer into @val and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int8:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint8 to store the result
 *
 * Read a signed 8 bit integer into @val and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint8:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint8 to store the result
 *
 * Read a signed 8 bit integer into @val but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int8:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint8 to store the result
 *
 * Read a signed 8 bit integer into @val but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint16_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint16 to store the result
 *
 * Read an unsigned 16 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int16_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint16 to store the result
 *
 * Read a signed 16 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint16_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint16 to store the result
 *
 * Read a signed 16 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int16_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint16 to store the result
 *
 * Read a signed 16 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint16_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint16 to store the result
 *
 * Read an unsigned 16 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int16_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint16 to store the result
 *
 * Read a signed 16 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint16_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint16 to store the result
 *
 * Read a signed 16 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int16_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint16 to store the result
 *
 * Read a signed 16 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint24_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read an unsigned 24 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int24_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 24 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint24_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read a signed 24 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int24_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 24 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint24_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read an unsigned 24 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int24_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 24 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint24_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read a signed 24 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int24_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 24 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */


/**
 * gst_byte_reader_get_uint32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read an unsigned 32 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 32 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read a signed 32 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 32 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read an unsigned 32 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 32 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint32 to store the result
 *
 * Read a signed 32 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint32 to store the result
 *
 * Read a signed 32 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint64 to store the result
 *
 * Read an unsigned 64 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint64 to store the result
 *
 * Read a signed 64 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint64 to store the result
 *
 * Read a signed 64 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint64 to store the result
 *
 * Read a signed 64 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_uint64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint64 to store the result
 *
 * Read an unsigned 64 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_int64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint64 to store the result
 *
 * Read a signed 64 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_uint64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #guint64 to store the result
 *
 * Read a signed 64 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_int64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gint64 to store the result
 *
 * Read a signed 64 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

#define GST_BYTE_READER_READ_INTS(bits) \
__declspec(dllexport) gboolean \
gst_byte_reader_get_uint##bits##_le (GstByteReader *reader, guint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_LE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_get_uint##bits##_be (GstByteReader *reader, guint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_BE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_get_int##bits##_le (GstByteReader *reader, gint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_LE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_get_int##bits##_be (GstByteReader *reader, gint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_BE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
__declspec(dllexport) gboolean \
gst_byte_reader_peek_uint##bits##_le (GstByteReader *reader, guint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_LE (&reader->data[reader->byte]); \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_peek_uint##bits##_be (GstByteReader *reader, guint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_BE (&reader->data[reader->byte]); \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_peek_int##bits##_le (GstByteReader *reader, gint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_LE (&reader->data[reader->byte]); \
  return TRUE; \
} \
\
__declspec(dllexport) gboolean \
gst_byte_reader_peek_int##bits##_be (GstByteReader *reader, gint##bits *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_UINT##bits##_BE (&reader->data[reader->byte]); \
  return TRUE; \
}


GST_BYTE_READER_READ_INTS (16);
GST_BYTE_READER_READ_INTS (32);
GST_BYTE_READER_READ_INTS (64);
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_uint8 (GstByteReader * reader, guint8 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (reader->byte >= reader->size)
    return FALSE;

  *val = GST_READ_UINT8 (&reader->data[reader->byte]);
  reader->byte++;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_int8 (GstByteReader * reader, gint8 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (reader->byte >= reader->size)
    return FALSE;

  *val = GST_READ_UINT8 (&reader->data[reader->byte]);
  reader->byte++;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_uint8 (GstByteReader * reader, guint8 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (reader->byte >= reader->size)
    return FALSE;

  *val = GST_READ_UINT8 (&reader->data[reader->byte]);
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_int8 (GstByteReader * reader, gint8 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (reader->byte >= reader->size)
    return FALSE;

  *val = GST_READ_UINT8 (&reader->data[reader->byte]);
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_uint24_le (GstByteReader * reader, guint32 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  *val = GST_READ_UINT24_LE (&reader->data[reader->byte]);
  reader->byte += 3;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_uint24_be (GstByteReader * reader, guint32 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  *val = GST_READ_UINT24_BE (&reader->data[reader->byte]);
  reader->byte += 3;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_int24_le (GstByteReader * reader, gint32 * val)
{
  guint32 ret;

  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  ret = GST_READ_UINT24_LE (&reader->data[reader->byte]);
  if (ret & 0x00800000)
    ret |= 0xff000000;

  reader->byte += 3;

  *val = ret;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_get_int24_be (GstByteReader * reader, gint32 * val)
{
  guint32 ret;

  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  ret = GST_READ_UINT24_BE (&reader->data[reader->byte]);
  if (ret & 0x00800000)
    ret |= 0xff000000;

  reader->byte += 3;

  *val = ret;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_uint24_le (GstByteReader * reader, guint32 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  *val = GST_READ_UINT24_LE (&reader->data[reader->byte]);
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_uint24_be (GstByteReader * reader, guint32 * val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  *val = GST_READ_UINT24_BE (&reader->data[reader->byte]);
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_int24_le (GstByteReader * reader, gint32 * val)
{
  guint32 ret;

  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  ret = GST_READ_UINT24_LE (&reader->data[reader->byte]);
  if (ret & 0x00800000)
    ret |= 0xff000000;

  *val = ret;
  return TRUE;
}
#ifdef __SYMBIAN32__
EXPORT_C
#endif


gboolean
gst_byte_reader_peek_int24_be (GstByteReader * reader, gint32 * val)
{
  guint32 ret;

  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < 3)
    return FALSE;

  ret = GST_READ_UINT24_BE (&reader->data[reader->byte]);
  if (ret & 0x00800000)
    ret |= 0xff000000;

  *val = ret;
  return TRUE;
}

/**
 * gst_byte_reader_get_float32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gfloat to store the result
 *
 * Read a 32 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_float32_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gfloat to store the result
 *
 * Read a 32 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_float32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gfloat to store the result
 *
 * Read a 32 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_float32_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gfloat to store the result
 *
 * Read a 32 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_float64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gdouble to store the result
 *
 * Read a 64 bit little endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_float64_le:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gdouble to store the result
 *
 * Read a 64 bit little endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_get_float64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gdouble to store the result
 *
 * Read a 64 bit big endian integer into @val
 * and update the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

/**
 * gst_byte_reader_peek_float64_be:
 * @reader: a #GstByteReader instance
 * @val: Pointer to a #gdouble to store the result
 *
 * Read a 64 bit big endian integer into @val
 * but keep the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */

#define GST_BYTE_READER_READ_FLOATS(bits, type, TYPE) \
    __declspec(dllexport) gboolean \
gst_byte_reader_get_float##bits##_le (GstByteReader *reader, g##type *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_##TYPE##_LE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
__declspec(dllexport) gboolean \
gst_byte_reader_get_float##bits##_be (GstByteReader *reader, g##type *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_##TYPE##_BE (&reader->data[reader->byte]); \
  reader->byte += bits / 8; \
  return TRUE; \
} \
__declspec(dllexport) gboolean \
gst_byte_reader_peek_float##bits##_le (GstByteReader *reader, g##type *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_##TYPE##_LE (&reader->data[reader->byte]); \
  return TRUE; \
} \
__declspec(dllexport) gboolean \
gst_byte_reader_peek_float##bits##_be (GstByteReader *reader, g##type *val) \
{ \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (val != NULL, FALSE); \
  \
  if (gst_byte_reader_get_remaining (reader) < bits / 8) \
    return FALSE; \
  \
  *val = GST_READ_##TYPE##_BE (&reader->data[reader->byte]); \
  return TRUE; \
}

GST_BYTE_READER_READ_FLOATS (32, float, FLOAT);
GST_BYTE_READER_READ_FLOATS (64, double, DOUBLE);

/**
 * gst_byte_reader_get_data:
 * @reader: a #GstByteReader instance
 * @size: Size in bytes
 * @val: Pointer to a #guint8 to store the result
 *
 * Returns a constant pointer to the current data
 * position if at least @size bytes are left and
 * updates the current position.
 *
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_get_data (GstByteReader * reader, guint size,
    const guint8 ** val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < size)
    return FALSE;

  *val = reader->data + reader->byte;
  reader->byte += size;
  return TRUE;
}

/**
 * gst_byte_reader_peek_data:
 * @reader: a #GstByteReader instance
 * @size: Size in bytes
 * @val: Pointer to a #guint8 to store the result
 *
 * Returns a constant pointer to the current data
 * position if at least @size bytes are left and
 * keeps the current position.
 *
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 * 
 * Since: 0.10.22
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_peek_data (GstByteReader * reader, guint size,
    const guint8 ** val)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (val != NULL, FALSE);

  if (gst_byte_reader_get_remaining (reader) < size)
    return FALSE;

  *val = reader->data + reader->byte;
  return TRUE;
}

/**
 * gst_byte_reader_dup_data:
 * @reader: a #GstByteReader instance
 * @size: Size in bytes
 * @val: Pointer to a #guint8 to store the result
 *
 * Returns a newly-allocated copy of the current data
 * position if at least @size bytes are left and
 * updates the current position.
 *
 * Returns: %TRUE if successful, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_dup_data (GstByteReader * reader, guint size, guint8 ** val)
{
  const guint8 *cval = NULL;

  if (!gst_byte_reader_get_data (reader, size, &cval))
    return FALSE;

  *val = g_memdup (cval, size);
  return TRUE;
}

/**
 * gst_byte_reader_masked_scan_uint32:
 * @reader: a #GstByteReader
 * @mask: mask to apply to data before matching against @pattern
 * @pattern: pattern to match (after mask is applied)
 * @offset: offset from which to start scanning, relative to the current
 *     position
 * @size: number of bytes to scan from offset
 *
 * Scan for pattern @pattern with applied mask @mask in the byte reader data,
 * starting from offset @offset relative to the current position.
 *
 * The bytes in @pattern and @mask are interpreted left-to-right, regardless
 * of endianness.  All four bytes of the pattern must be present in the
 * byte reader data for it to match, even if the first or last bytes are masked
 * out.
 *
 * It is an error to call this function without making sure that there is
 * enough data (offset+size bytes) in the byte reader.
 *
 * Returns: offset of the first match, or -1 if no match was found.
 *
 * Example:
 * <programlisting>
 * // Assume the reader contains 0x00 0x01 0x02 ... 0xfe 0xff
 *
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffffffff, 0x00010203, 0, 256);
 * // -> returns 0
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffffffff, 0x00010203, 1, 255);
 * // -> returns -1
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffffffff, 0x01020304, 1, 255);
 * // -> returns 1
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffff, 0x0001, 0, 256);
 * // -> returns -1
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffff, 0x0203, 0, 256);
 * // -> returns 0
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffff0000, 0x02030000, 0, 256);
 * // -> returns 2
 * gst_byte_reader_masked_scan_uint32 (reader, 0xffff0000, 0x02030000, 0, 4);
 * // -> returns -1
 * </programlisting>
 *
 * Since: 0.10.24
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

guint
gst_byte_reader_masked_scan_uint32 (GstByteReader * reader, guint32 mask,
    guint32 pattern, guint offset, guint size)
{
  const guint8 *data;
  guint32 state;
  guint i;

  g_return_val_if_fail (size > 0, -1);
  g_return_val_if_fail ((guint64) offset + size <= reader->size - reader->byte,
      -1);

  /* we can't find the pattern with less than 4 bytes */
  if (G_UNLIKELY (size < 4))
    return -1;

  data = reader->data + reader->byte + offset;

  /* set the state to something that does not match */
  state = ~pattern;

  /* now find data */
  for (i = 0; i < size; i++) {
    /* throw away one byte and move in the next byte */
    state = ((state << 8) | data[i]);
    if (G_UNLIKELY ((state & mask) == pattern)) {
      /* we have a match but we need to have skipped at
       * least 4 bytes to fill the state. */
      if (G_LIKELY (i >= 3))
        return offset + i - 3;
    }
  }

  /* nothing found */
  return -1;
}

#define GST_BYTE_READER_SCAN_STRING(bits) \
static guint \
gst_byte_reader_scan_string_utf##bits (GstByteReader * reader) \
{ \
  guint len, off, max_len; \
  \
  max_len = (reader->size - reader->byte) / sizeof (guint##bits); \
  \
  /* need at least a single NUL terminator */ \
  if (max_len < 1) \
    return 0; \
  \
  len = 0; \
  off = reader->byte; \
  /* endianness does not matter if we are looking for a NUL terminator */ \
  while (GST_READ_UINT##bits##_LE (&reader->data[off]) != 0) { \
    ++len; \
    off += sizeof (guint##bits); \
    /* have we reached the end without finding a NUL terminator? */ \
    if (len == max_len) \
      return 0; \
  } \
  /* return size in bytes including the NUL terminator (hence the +1) */ \
  return (len + 1) * sizeof (guint##bits); \
}

#define GST_READ_UINT8_LE GST_READ_UINT8
GST_BYTE_READER_SCAN_STRING (8);
#undef GST_READ_UINT8_LE
GST_BYTE_READER_SCAN_STRING (16);
GST_BYTE_READER_SCAN_STRING (32);

#define GST_BYTE_READER_SKIP_STRING(bits) \
__declspec(dllexport) gboolean \
gst_byte_reader_skip_string_utf##bits (GstByteReader * reader) \
{ \
  guint size; /* size in bytes including the terminator */ \
  \
  g_return_val_if_fail (reader != NULL, FALSE); \
  \
  size = gst_byte_reader_scan_string_utf##bits (reader); \
  reader->byte += size; \
  return (size > 0); \
}

/**
 * gst_byte_reader_skip_string:
 * @reader: a #GstByteReader instance
 *
 * Skips a NUL-terminated string in the #GstByteReader instance, advancing
 * the current position to the byte after the string. This will work for
 * any NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
/**
 * gst_byte_reader_skip_string_utf8:
 * @reader: a #GstByteReader instance
 *
 * Skips a NUL-terminated string in the #GstByteReader instance, advancing
 * the current position to the byte after the string. This will work for
 * any NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc. No input checking for valid UTF-8 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_SKIP_STRING (8);

/**
 * gst_byte_reader_skip_string_utf16:
 * @reader: a #GstByteReader instance
 *
 * Skips a NUL-terminated UTF-16 string in the #GstByteReader instance,
 * advancing the current position to the byte after the string.
 *
 * No input checking for valid UTF-16 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_SKIP_STRING (16);

/**
 * gst_byte_reader_skip_string_utf32:
 * @reader: a #GstByteReader instance
 *
 * Skips a NUL-terminated UTF-32 string in the #GstByteReader instance,
 * advancing the current position to the byte after the string.
 *
 * No input checking for valid UTF-32 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_SKIP_STRING (32);

/**
 * gst_byte_reader_peek_string:
 * @reader: a #GstByteReader instance
 * @str: Pointer to a #gchar to store the result
 *
 * Returns a constant pointer to the current data position if there is
 * a NUL-terminated string in the data (this could be just a NUL terminator).
 * The current position will be maintained. This will work for any
 * NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
/**
 * gst_byte_reader_peek_string_utf8:
 * @reader: a #GstByteReader instance
 * @str: Pointer to a #gchar to store the result
 *
 * Returns a constant pointer to the current data position if there is
 * a NUL-terminated string in the data (this could be just a NUL terminator).
 * The current position will be maintained. This will work for any
 * NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc.
 *
 * No input checking for valid UTF-8 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be skipped, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_peek_string_utf8 (GstByteReader * reader, const gchar ** str)
{
  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (str != NULL, FALSE);

  if (gst_byte_reader_scan_string_utf8 (reader) > 0) {
    *str = (const gchar *) (reader->data + reader->byte);
  } else {
    *str = NULL;
  }
  return (*str != NULL);
}

/**
 * gst_byte_reader_get_string_utf8:
 * @reader: a #GstByteReader instance
 * @str: Pointer to a #gchar to store the result
 *
 * Returns a constant pointer to the current data position if there is
 * a NUL-terminated string in the data (this could be just a NUL terminator),
 * advancing the current position to the byte after the string. This will work
 * for any NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc.
 *
 * No input checking for valid UTF-8 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be found, %FALSE otherwise.
 *
 * Since: 0.10.24
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

gboolean
gst_byte_reader_get_string_utf8 (GstByteReader * reader, const gchar ** str)
{
  guint size;                   /* size in bytes including the terminator */

  g_return_val_if_fail (reader != NULL, FALSE);
  g_return_val_if_fail (str != NULL, FALSE);

  size = gst_byte_reader_scan_string_utf8 (reader);
  if (size == 0) {
    *str = NULL;
    return FALSE;
  }

  *str = (const gchar *) (reader->data + reader->byte);
  reader->byte += size;
  return TRUE;
}

#define GST_BYTE_READER_DUP_STRING(bits,type) \
__declspec(dllexport) gboolean \
gst_byte_reader_dup_string_utf##bits (GstByteReader * reader, type ** str) \
{ \
  guint size; /* size in bytes including the terminator */ \
  \
  g_return_val_if_fail (reader != NULL, FALSE); \
  g_return_val_if_fail (str != NULL, FALSE); \
  \
  size = gst_byte_reader_scan_string_utf##bits (reader); \
  if (size == 0) { \
    *str = NULL; \
    return FALSE; \
  } \
  *str = g_memdup (reader->data + reader->byte, size); \
  reader->byte += size; \
  return TRUE; \
}

/**
 * gst_byte_reader_dup_string_utf8:
 * @reader: a #GstByteReader instance
 * @str: address of a string pointer to store the result
 *
 * FIXME:Reads (copies) a NUL-terminated string in the #GstByteReader instance,
 * advancing the current position to the byte after the string. This will work
 * for any NUL-terminated string with a character width of 8 bits, so ASCII,
 * UTF-8, ISO-8859-N etc. No input checking for valid UTF-8 is done.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Returns: %TRUE if a string could be read into @str, %FALSE otherwise. The
 *     string put into @str must be freed with g_free() when no longer needed.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_DUP_STRING (8, gchar);

/**
 * gst_byte_reader_dup_string_utf16:
 * @reader: a #GstByteReader instance
 * @str: address of a #guint16 pointer to store the result
 *
 * Returns a newly-allocated copy of the current data position if there is
 * a NUL-terminated UTF-16 string in the data (this could be an empty string
 * as well), and advances the current position.
 *
 * No input checking for valid UTF-16 is done. This function is endianness
 * agnostic - you should not assume the UTF-16 characters are in host
 * endianness.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Note: there is no peek or get variant of this function to ensure correct
 * byte alignment of the UTF-16 string.
 *
 * Returns: %TRUE if a string could be read, %FALSE otherwise. The
 *     string put into @str must be freed with g_free() when no longer needed.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_DUP_STRING (16, guint16);

/**
 * gst_byte_reader_dup_string_utf32:
 * @reader: a #GstByteReader instance
 * @str: address of a #guint32 pointer to store the result
 *
 * Returns a newly-allocated copy of the current data position if there is
 * a NUL-terminated UTF-32 string in the data (this could be an empty string
 * as well), and advances the current position.
 *
 * No input checking for valid UTF-32 is done. This function is endianness
 * agnostic - you should not assume the UTF-32 characters are in host
 * endianness.
 *
 * This function will fail if no NUL-terminator was found in in the data.
 *
 * Note: there is no peek or get variant of this function to ensure correct
 * byte alignment of the UTF-32 string.
 *
 * Returns: %TRUE if a string could be read, %FALSE otherwise. The
 *     string put into @str must be freed with g_free() when no longer needed.
 *
 * Since: 0.10.24
 */
GST_BYTE_READER_DUP_STRING (32, guint32);
