/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
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

#include <gst/gst_private.h>
#include <gst/gstversion.h>
#include <gst/gstplugin.h>

#ifdef __SYMBIAN32__
IMPORT_C
#endif
extern gboolean gst_mem_index_plugin_init (GstPlugin * plugin);

#ifndef GST_DISABLE_LOADSAVE
extern gboolean gst_file_index_plugin_init (GstPlugin * plugin);
#endif

static gboolean
plugin_init (GstPlugin * plugin)
{
  gboolean res = TRUE;

  res &= gst_mem_index_plugin_init (plugin);
#if defined(HAVE_MMAP) && !defined(GST_DISABLE_LOADSAVE)
  res &= gst_file_index_plugin_init (plugin);
#endif

  return res;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "coreindexers",
    "GStreamer core indexers",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN);
#ifdef __SYMBIAN32__
EXPORT_C
#endif
GstPluginDesc* _GST_PLUGIN_DESC()
{
	return &gst_plugin_desc;
}
