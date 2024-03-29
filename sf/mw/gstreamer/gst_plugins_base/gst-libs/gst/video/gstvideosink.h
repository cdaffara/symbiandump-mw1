/*
 *  GStreamer Video sink.
 *
 *  Copyright (C) <2003> Julien Moutte <julien@moutte.net>
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
 
#ifndef __GST_VIDEO_SINK_H__
#define __GST_VIDEO_SINK_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS
  
#define GST_TYPE_VIDEO_SINK (gst_video_sink_get_type())
#define GST_VIDEO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_VIDEO_SINK, GstVideoSink))
#define GST_VIDEO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_VIDEO_SINK, GstVideoSinkClass))
#define GST_IS_VIDEO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_VIDEO_SINK))
#define GST_IS_VIDEO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_VIDEO_SINK))
#define GST_VIDEO_SINK_GET_CLASS(klass) \
  (G_TYPE_INSTANCE_GET_CLASS ((klass), GST_TYPE_VIDEO_SINK, GstVideoSinkClass))

/**
 * GST_VIDEO_SINK_CAST:
 * @obj: a #GstVideoSink or derived object
 *
 * Cast @obj to a #GstVideoSink without runtime type check.
 *
 * Since: 0.10.12
 */
#define GST_VIDEO_SINK_CAST(obj)  ((GstVideoSink *) (obj))

/**
 * GST_VIDEO_SINK_PAD:
 * @obj: a #GstVideoSink
 *
 * Get the sink #GstPad of @obj.
 */
#define GST_VIDEO_SINK_PAD(obj) GST_BASE_SINK_PAD(obj)

#define GST_VIDEO_SINK_WIDTH(obj) (GST_VIDEO_SINK_CAST (obj)->width)
#define GST_VIDEO_SINK_HEIGHT(obj) (GST_VIDEO_SINK_CAST (obj)->height)
  
typedef struct _GstVideoSink GstVideoSink;
typedef struct _GstVideoSinkClass GstVideoSinkClass;
typedef struct _GstVideoRectangle GstVideoRectangle;

/**
 * GstVideoRectangle:
 * @x: X coordinate of rectangle's top-left point
 * @y: Y coordinate of rectangle's top-left point
 * @w: width of the rectangle
 * @h: height of the rectangle
 *
 * Helper structure representing a rectangular area.
 */
struct _GstVideoRectangle {
  gint x;
  gint y;
  gint w;
  gint h;
};

struct _GstVideoSink {
  GstBaseSink element;
  
  gint width, height;
  
  gpointer _gst_reserved[GST_PADDING];
};

struct _GstVideoSinkClass {
  GstBaseSinkClass parent_class;
      
  gpointer _gst_reserved[GST_PADDING];
};
#ifdef __SYMBIAN32__
IMPORT_C
#endif


GType gst_video_sink_get_type (void);
#ifdef __SYMBIAN32__
IMPORT_C
#endif


void gst_video_sink_center_rect (GstVideoRectangle src, GstVideoRectangle dst,
                                 GstVideoRectangle *result, gboolean scaling);

G_END_DECLS

#endif  /* __GST_VIDEO_SINK_H__ */
