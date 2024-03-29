/* GStreamer
 * Copyright (C) 1999,2000 Erik Walthinsen <omega@cse.ogi.edu>
 *                    2004 Wim Taymans <wim@fluendo.com>
 *
 * gstsystemclock.c: Default clock, uses the system clock
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

/**
 * SECTION:gstsystemclock
 * @short_description: Default clock that uses the current system time
 * @see_also: #GstClock
 *
 * The GStreamer core provides a GstSystemClock based on the system time.
 * Asynchronous callbacks are scheduled from an internal thread.
 *
 * Clock implementors are encouraged to subclass this systemclock as it
 * implements the async notification.
 *
 * Subclasses can however override all of the important methods for sync and
 * async notifications to implement their own callback methods or blocking
 * wait operations.
 *
 * Last reviewed on 2006-03-08 (0.10.4)
 */

#include "gst_private.h"
#include "gstinfo.h"
#include "gstsystemclock.h"
#include "gstenumtypes.h"
#include "gstpoll.h"

#include <errno.h>

#ifdef __SYMBIAN32__
#include <glib_global.h>
#endif

/* Define this to get some extra debug about jitter from each clock_wait */
#undef WAIT_DEBUGGING

struct _GstSystemClockPrivate
{
  GstClockType clock_type;
  GstPoll *timer;
  gint wakeup_count;            /* the number of entries with a pending wakeup */
  gboolean async_wakeup;        /* if the wakeup was because of a async list change */
};

#define GST_SYSTEM_CLOCK_GET_PRIVATE(obj)  \
   (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GST_TYPE_SYSTEM_CLOCK, \
        GstSystemClockPrivate))

#ifdef HAVE_POSIX_TIMERS
# ifdef HAVE_MONOTONIC_CLOCK
#  define DEFAULT_CLOCK_TYPE GST_CLOCK_TYPE_MONOTONIC
# else
#  define DEFAULT_CLOCK_TYPE GST_CLOCK_TYPE_REALTIME
# endif
#else
#define DEFAULT_CLOCK_TYPE GST_CLOCK_TYPE_REALTIME
#endif

enum
{
  PROP_0,
  PROP_CLOCK_TYPE,
  /* FILL ME */
};

/* the one instance of the systemclock */
static GstClock *_the_system_clock = NULL;

static void gst_system_clock_class_init (GstSystemClockClass * klass);
static void gst_system_clock_init (GstSystemClock * clock);
static void gst_system_clock_dispose (GObject * object);
static void gst_system_clock_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_system_clock_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static GstClockTime gst_system_clock_get_internal_time (GstClock * clock);
static guint64 gst_system_clock_get_resolution (GstClock * clock);
static GstClockReturn gst_system_clock_id_wait_jitter (GstClock * clock,
    GstClockEntry * entry, GstClockTimeDiff * jitter);
static GstClockReturn gst_system_clock_id_wait_jitter_unlocked
    (GstClock * clock, GstClockEntry * entry, GstClockTimeDiff * jitter,
    gboolean restart);
static GstClockReturn gst_system_clock_id_wait_async (GstClock * clock,
    GstClockEntry * entry);
static void gst_system_clock_id_unschedule (GstClock * clock,
    GstClockEntry * entry);
static void gst_system_clock_async_thread (GstClock * clock);
static gboolean gst_system_clock_start_async (GstSystemClock * clock);
static void gst_system_clock_add_wakeup (GstSystemClock * sysclock);

static GStaticMutex _gst_sysclock_mutex = G_STATIC_MUTEX_INIT;

static GstClockClass *parent_class = NULL;

/* static guint gst_system_clock_signals[LAST_SIGNAL] = { 0 }; */

G_DEFINE_TYPE (GstSystemClock, gst_system_clock, GST_TYPE_CLOCK);

static void
gst_system_clock_class_init (GstSystemClockClass * klass)
{
  GObjectClass *gobject_class;
  GstClockClass *gstclock_class;

  gobject_class = (GObjectClass *) klass;
  gstclock_class = (GstClockClass *) klass;

  parent_class = g_type_class_peek_parent (klass);

  g_type_class_add_private (klass, sizeof (GstSystemClockPrivate));

  gobject_class->dispose = gst_system_clock_dispose;
  gobject_class->set_property = gst_system_clock_set_property;
  gobject_class->get_property = gst_system_clock_get_property;

  g_object_class_install_property (gobject_class, PROP_CLOCK_TYPE,
      g_param_spec_enum ("clock-type", "Clock type",
          "The type of underlying clock implementation used",
          GST_TYPE_CLOCK_TYPE, DEFAULT_CLOCK_TYPE, G_PARAM_READWRITE));

  gstclock_class->get_internal_time = gst_system_clock_get_internal_time;
  gstclock_class->get_resolution = gst_system_clock_get_resolution;
  gstclock_class->wait_jitter = gst_system_clock_id_wait_jitter;
  gstclock_class->wait_async = gst_system_clock_id_wait_async;
  gstclock_class->unschedule = gst_system_clock_id_unschedule;
}

static void
gst_system_clock_init (GstSystemClock * clock)
{
  GST_OBJECT_FLAG_SET (clock,
      GST_CLOCK_FLAG_CAN_DO_SINGLE_SYNC |
      GST_CLOCK_FLAG_CAN_DO_SINGLE_ASYNC |
      GST_CLOCK_FLAG_CAN_DO_PERIODIC_SYNC |
      GST_CLOCK_FLAG_CAN_DO_PERIODIC_ASYNC);

  clock->priv = GST_SYSTEM_CLOCK_GET_PRIVATE (clock);

  clock->priv->clock_type = DEFAULT_CLOCK_TYPE;
  clock->priv->timer = gst_poll_new_timer ();

#if 0
  /* Uncomment this to start the async clock thread straight away */
  GST_OBJECT_LOCK (clock);
  gst_system_clock_start_async (clock);
  GST_OBJECT_UNLOCK (clock);
#endif
}

static void
gst_system_clock_dispose (GObject * object)
{
  GstClock *clock = (GstClock *) object;
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK_CAST (clock);
  GList *entries;

  /* else we have to stop the thread */
  GST_OBJECT_LOCK (clock);
  sysclock->stopping = TRUE;
  /* unschedule all entries */
  for (entries = clock->entries; entries; entries = g_list_next (entries)) {
    GstClockEntry *entry = (GstClockEntry *) entries->data;

    GST_CAT_DEBUG (GST_CAT_CLOCK, "unscheduling entry %p", entry);
    entry->status = GST_CLOCK_UNSCHEDULED;
  }
  g_list_free (clock->entries);
  clock->entries = NULL;
  GST_CLOCK_BROADCAST (clock);
  gst_system_clock_add_wakeup (sysclock);
  GST_OBJECT_UNLOCK (clock);

  if (sysclock->thread)
    g_thread_join (sysclock->thread);
  sysclock->thread = NULL;
  GST_CAT_DEBUG (GST_CAT_CLOCK, "joined thread");

  gst_poll_free (sysclock->priv->timer);

  G_OBJECT_CLASS (parent_class)->dispose (object);

  if (_the_system_clock == clock) {
    _the_system_clock = NULL;
    GST_CAT_DEBUG (GST_CAT_CLOCK, "disposed system clock");
  }
}

static void
gst_system_clock_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK (object);

  switch (prop_id) {
    case PROP_CLOCK_TYPE:
      sysclock->priv->clock_type = g_value_get_enum (value);
      GST_CAT_DEBUG (GST_CAT_CLOCK, "clock-type set to %d",
          sysclock->priv->clock_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_system_clock_get_property (GObject * object, guint prop_id, GValue * value,
    GParamSpec * pspec)
{
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK (object);

  switch (prop_id) {
    case PROP_CLOCK_TYPE:
      g_value_set_enum (value, sysclock->priv->clock_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/**
 * gst_system_clock_obtain:
 *
 * Get a handle to the default system clock. The refcount of the
 * clock will be increased so you need to unref the clock after
 * usage.
 *
 * Returns: the default clock.
 *
 * MT safe.
 */
#ifdef __SYMBIAN32__
EXPORT_C
#endif

GstClock *
gst_system_clock_obtain (void)
{
  GstClock *clock;

  g_static_mutex_lock (&_gst_sysclock_mutex);
  clock = _the_system_clock;

  if (clock == NULL) {
    GST_CAT_DEBUG (GST_CAT_CLOCK, "creating new static system clock");
    clock = g_object_new (GST_TYPE_SYSTEM_CLOCK,
        "name", "GstSystemClock", NULL);

    /* we created the global clock; take ownership so
     * we can hand out instances later */
    gst_object_ref_sink (clock);

    _the_system_clock = clock;
    g_static_mutex_unlock (&_gst_sysclock_mutex);
  } else {
    g_static_mutex_unlock (&_gst_sysclock_mutex);
    GST_CAT_DEBUG (GST_CAT_CLOCK, "returning static system clock");
  }

  /* we ref it since we are a clock factory. */
  gst_object_ref (clock);
  return clock;
}

static void
gst_system_clock_remove_wakeup (GstSystemClock * sysclock)
{
  g_return_if_fail (sysclock->priv->wakeup_count > 0);

  sysclock->priv->wakeup_count--;
  if (sysclock->priv->wakeup_count == 0) {
    /* read the control socket byte when we removed the last wakeup count */
    GST_CAT_DEBUG (GST_CAT_CLOCK, "reading control");
    while (!gst_poll_read_control (sysclock->priv->timer)) {
      g_warning ("gstsystemclock: read control failed, trying again\n");
    }
    GST_CLOCK_BROADCAST (sysclock);
  }
  GST_CAT_DEBUG (GST_CAT_CLOCK, "wakeup count %d",
      sysclock->priv->wakeup_count);
}

static void
gst_system_clock_add_wakeup (GstSystemClock * sysclock)
{
  /* only write the control socket for the first wakeup */
  if (sysclock->priv->wakeup_count == 0) {
    GST_CAT_DEBUG (GST_CAT_CLOCK, "writing control");
    while (!gst_poll_write_control (sysclock->priv->timer)) {
      g_warning
          ("gstsystemclock: write control failed in wakeup_async, trying again : %d:%s\n",
          errno, g_strerror (errno));
    }
  }
  sysclock->priv->wakeup_count++;
  GST_CAT_DEBUG (GST_CAT_CLOCK, "wakeup count %d",
      sysclock->priv->wakeup_count);
}

static void
gst_system_clock_wait_wakeup (GstSystemClock * sysclock)
{
  while (sysclock->priv->wakeup_count > 0) {
    GST_CLOCK_WAIT (sysclock);
  }
}

/* this thread reads the sorted clock entries from the queue.
 *
 * It waits on each of them and fires the callback when the timeout occurs.
 *
 * When an entry in the queue was canceled before we wait for it, it is
 * simply skipped.
 *
 * When waiting for an entry, it can become canceled, in that case we don't
 * call the callback but move to the next item in the queue.
 *
 * MT safe.
 */
static void
gst_system_clock_async_thread (GstClock * clock)
{
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK_CAST (clock);

  GST_CAT_DEBUG (GST_CAT_CLOCK, "enter system clock thread");
  GST_OBJECT_LOCK (clock);
  /* signal spinup */
  GST_CLOCK_BROADCAST (clock);
  /* now enter our (almost) infinite loop */
  while (!sysclock->stopping) {
    GstClockEntry *entry;
    GstClockTime requested;
    GstClockReturn res;

    /* check if something to be done */
    while (clock->entries == NULL) {
      GST_CAT_DEBUG (GST_CAT_CLOCK, "no clock entries, waiting..");
      /* wait for work to do */
      GST_CLOCK_WAIT (clock);
      GST_CAT_DEBUG (GST_CAT_CLOCK, "got signal");
      /* clock was stopping, exit */
      if (sysclock->stopping)
        goto exit;
    }

    /* see if we have a pending wakeup because the order of the list
     * changed. */
    if (sysclock->priv->async_wakeup) {
      GST_CAT_DEBUG (GST_CAT_CLOCK, "clear async wakeup");
      gst_system_clock_remove_wakeup (sysclock);
      sysclock->priv->async_wakeup = FALSE;
    }

    /* pick the next entry */
    entry = clock->entries->data;
    /* if it was unscheduled, just move on to the next entry */
    if (entry->status == GST_CLOCK_UNSCHEDULED) {
      GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p was unscheduled", entry);
      goto next_entry;
    }

    requested = entry->time;

    /* now wait for the entry, we already hold the lock */
    res =
        gst_system_clock_id_wait_jitter_unlocked (clock, (GstClockID) entry,
        NULL, FALSE);

    switch (res) {
      case GST_CLOCK_UNSCHEDULED:
        /* entry was unscheduled, move to the next */
        GST_CAT_DEBUG (GST_CAT_CLOCK, "async entry %p unscheduled", entry);
        goto next_entry;
      case GST_CLOCK_OK:
      case GST_CLOCK_EARLY:
      {
        /* entry timed out normally, fire the callback and move to the next
         * entry */
        GST_CAT_DEBUG (GST_CAT_CLOCK, "async entry %p timed out", entry);
        if (entry->func) {
          /* unlock before firing the callback */
          GST_OBJECT_UNLOCK (clock);
          entry->func (clock, entry->time, (GstClockID) entry,
              entry->user_data);
          GST_OBJECT_LOCK (clock);
        }
        if (entry->type == GST_CLOCK_ENTRY_PERIODIC) {
          GST_CAT_DEBUG (GST_CAT_CLOCK, "updating periodic entry %p", entry);
          /* adjust time now */
          entry->time = requested + entry->interval;
          /* and resort the list now */
          clock->entries =
              g_list_sort (clock->entries, gst_clock_id_compare_func);
          /* and restart */
          continue;
        } else {
          GST_CAT_DEBUG (GST_CAT_CLOCK, "moving to next entry");
          goto next_entry;
        }
      }
      case GST_CLOCK_BUSY:
        /* somebody unlocked the entry but is was not canceled, This means that
         * either a new entry was added in front of the queue or some other entry
         * was canceled. Whatever it is, pick the head entry of the list and
         * continue waiting. */
        GST_CAT_DEBUG (GST_CAT_CLOCK, "async entry %p needs restart", entry);

        /* we set the entry back to the OK state. This is needed so that the
         * _unschedule() code can see if an entry is currently being waited
         * on (when its state is BUSY). */
        entry->status = GST_CLOCK_OK;
        continue;
      default:
        GST_CAT_DEBUG (GST_CAT_CLOCK,
            "strange result %d waiting for %p, skipping", res, entry);
        g_warning ("%s: strange result %d waiting for %p, skipping",
            GST_OBJECT_NAME (clock), res, entry);
        goto next_entry;
    }
  next_entry:
    /* we remove the current entry and unref it */
    clock->entries = g_list_remove (clock->entries, entry);
    gst_clock_id_unref ((GstClockID) entry);
  }
exit:
  /* signal exit */
  GST_CLOCK_BROADCAST (clock);
  GST_OBJECT_UNLOCK (clock);
  GST_CAT_DEBUG (GST_CAT_CLOCK, "exit system clock thread");
}

#ifdef HAVE_POSIX_TIMERS
static inline clockid_t
clock_type_to_posix_id (GstClockType clock_type)
{
#ifdef HAVE_MONOTONIC_CLOCK
  if (clock_type == GST_CLOCK_TYPE_MONOTONIC)
    return CLOCK_MONOTONIC;
  else
#endif
    return CLOCK_REALTIME;
}
#endif

/* MT safe */
static GstClockTime
gst_system_clock_get_internal_time (GstClock * clock)
{
#ifdef HAVE_POSIX_TIMERS
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK_CAST (clock);
  clockid_t ptype;
  struct timespec ts;

  ptype = clock_type_to_posix_id (sysclock->priv->clock_type);

  if (G_UNLIKELY (clock_gettime (ptype, &ts)))
    return GST_CLOCK_TIME_NONE;

  return GST_TIMESPEC_TO_TIME (ts);
#else
  GTimeVal timeval;

  g_get_current_time (&timeval);

  return GST_TIMEVAL_TO_TIME (timeval);
#endif
}

static guint64
gst_system_clock_get_resolution (GstClock * clock)
{
#ifdef HAVE_POSIX_TIMERS
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK_CAST (clock);
  clockid_t ptype;
  struct timespec ts;

  ptype = clock_type_to_posix_id (sysclock->priv->clock_type);

  if (G_UNLIKELY (clock_getres (ptype, &ts)))
    return GST_CLOCK_TIME_NONE;

  return GST_TIMESPEC_TO_TIME (ts);
#else
  return 1 * GST_USECOND;
#endif
}

/* synchronously wait on the given GstClockEntry.
 *
 * We do this by blocking on the global clock GCond variable with
 * the requested time as a timeout. This allows us to unblock the
 * entry by signaling the GCond variable.
 *
 * Note that signaling the global GCond unlocks all waiting entries. So
 * we need to check if an unlocked entry has changed when it unlocks.
 *
 * Entries that arrive too late are simply not waited on and a
 * GST_CLOCK_EARLY result is returned.
 *
 * should be called with LOCK held.
 *
 * MT safe.
 */
static GstClockReturn
gst_system_clock_id_wait_jitter_unlocked (GstClock * clock,
    GstClockEntry * entry, GstClockTimeDiff * jitter, gboolean restart)
{
  GstSystemClock *sysclock = GST_SYSTEM_CLOCK_CAST (clock);
  GstClockTime entryt, real, now;
  GstClockTimeDiff diff;

  /* need to call the overridden method because we want to sync against the time
   * of the clock, whatever the subclass uses as a clock. */
  real = GST_CLOCK_GET_CLASS (clock)->get_internal_time (clock);
  now = gst_clock_adjust_unlocked (clock, real);

  /* get the time of the entry */
  entryt = GST_CLOCK_ENTRY_TIME (entry);

  if (jitter) {
    *jitter = GST_CLOCK_DIFF (entryt, now);
  }
  /* the diff of the entry with the clock is the amount of time we have to
   * wait */
  diff = entryt - now;

  GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p"
      " time %" GST_TIME_FORMAT
      " now %" GST_TIME_FORMAT
      " real %" GST_TIME_FORMAT
      " diff (time-now) %" G_GINT64_FORMAT,
      entry,
      GST_TIME_ARGS (entryt), GST_TIME_ARGS (now), GST_TIME_ARGS (real), diff);

  if (diff > 0) {
#ifdef WAIT_DEBUGGING
    GstClockTime final;
#endif

    while (entry->status != GST_CLOCK_UNSCHEDULED) {
      gint pollret;

      /* mark the entry as busy */
      entry->status = GST_CLOCK_BUSY;
      GST_OBJECT_UNLOCK (clock);

      /* now wait on the entry, it either times out or the fd is written. */
      pollret = gst_poll_wait (sysclock->priv->timer, diff);

      /* another thread can read the fd before we get the lock */
      GST_OBJECT_LOCK (clock);
      if (entry->status == GST_CLOCK_UNSCHEDULED) {
        GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p unlocked", entry);
        gst_system_clock_remove_wakeup (sysclock);
      } else {
        if (pollret != 0) {
          /* some other id got unlocked */
          if (!restart) {
            /* this can happen if the entry got unlocked because of an async
             * entry was added to the head of the async queue. */
            GST_CAT_DEBUG (GST_CAT_CLOCK, "wakeup waiting for entry %p", entry);
            break;
          }

          /* mark ourselves as EARLY, we release the lock and we could be
           * unscheduled ourselves but we don't want the unscheduling thread
           * to write on the control socket (it does that when an entry has a
           * BUSY status). */
          entry->status = GST_CLOCK_EARLY;

          /* wait till all the entries got woken up */
          gst_system_clock_wait_wakeup (sysclock);

          /* we released the lock in the wait, recheck our status, we don't need
           * to remove the wakeup count because we marked the entry as EARLY
           * before releasing the object lock. */
          if (entry->status == GST_CLOCK_UNSCHEDULED) {
            GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p got unscheduled", entry);
            break;
          }

          GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p needs to be restarted",
              entry);
        } else {
          GST_CAT_DEBUG (GST_CAT_CLOCK, "entry %p unlocked after timeout",
              entry);
        }

        /* reschedule if gst_poll_wait returned early or we have to reschedule after
         * an unlock*/
        real = GST_CLOCK_GET_CLASS (clock)->get_internal_time (clock);
        now = gst_clock_adjust_unlocked (clock, real);
        diff = entryt - now;

        if (diff <= 0) {
          /* timeout, this is fine, we can report success now */
          entry->status = GST_CLOCK_OK;

          GST_CAT_DEBUG (GST_CAT_CLOCK,
              "entry %p finished, diff %" G_GINT64_FORMAT, entry, diff);

#ifdef WAIT_DEBUGGING
          final = gst_system_clock_get_internal_time (clock);
          GST_CAT_DEBUG (GST_CAT_CLOCK, "Waited for %" G_GINT64_FORMAT
              " got %" G_GINT64_FORMAT " diff %" G_GINT64_FORMAT
              " %g target-offset %" G_GINT64_FORMAT " %g", entryt, now,
              now - entryt,
              (double) (GstClockTimeDiff) (now - entryt) / GST_SECOND,
              (final - target),
              ((double) (GstClockTimeDiff) (final - target)) / GST_SECOND);
#endif
          break;
        } else {
          GST_CAT_DEBUG (GST_CAT_CLOCK,
              "entry %p restart, diff %" G_GINT64_FORMAT, entry, diff);
        }
      }
    }
  } else if (diff == 0) {
    entry->status = GST_CLOCK_OK;
  } else {
    entry->status = GST_CLOCK_EARLY;
  }
  return entry->status;
}

static GstClockReturn
gst_system_clock_id_wait_jitter (GstClock * clock, GstClockEntry * entry,
    GstClockTimeDiff * jitter)
{
  GstClockReturn ret;

  GST_OBJECT_LOCK (clock);
  if (G_UNLIKELY (entry->status == GST_CLOCK_UNSCHEDULED))
    goto was_unscheduled;

  ret = gst_system_clock_id_wait_jitter_unlocked (clock, entry, jitter, TRUE);
  GST_OBJECT_UNLOCK (clock);

  return ret;

  /* ERRORS */
was_unscheduled:
  {
    GST_OBJECT_UNLOCK (clock);
    return GST_CLOCK_UNSCHEDULED;
  }
}

/* Start the async clock thread. Must be called with the object lock
 * held */
static gboolean
gst_system_clock_start_async (GstSystemClock * clock)
{
  GError *error = NULL;

  if (G_LIKELY (clock->thread != NULL))
    return TRUE;                /* Thread already running. Nothing to do */

  clock->thread = g_thread_create ((GThreadFunc) gst_system_clock_async_thread,
      clock, TRUE, &error);
  if (G_UNLIKELY (error))
    goto no_thread;

  /* wait for it to spin up */
  GST_CLOCK_WAIT (clock);

  return TRUE;

  /* ERRORS */
no_thread:
  {
    g_warning ("could not create async clock thread: %s", error->message);
    g_error_free (error);
  }
  return FALSE;
}

/* Add an entry to the list of pending async waits. The entry is inserted
 * in sorted order. If we inserted the entry at the head of the list, we
 * need to signal the thread as it might either be waiting on it or waiting
 * for a new entry.
 *
 * MT safe.
 */
static GstClockReturn
gst_system_clock_id_wait_async (GstClock * clock, GstClockEntry * entry)
{
  GstSystemClock *sysclock;
  GstClockEntry *head;

  sysclock = GST_SYSTEM_CLOCK_CAST (clock);

  GST_CAT_DEBUG (GST_CAT_CLOCK, "adding async entry %p", entry);

  GST_OBJECT_LOCK (clock);
  /* Start the clock async thread if needed */
  if (G_UNLIKELY (!gst_system_clock_start_async (sysclock)))
    goto thread_error;

  if (G_UNLIKELY (entry->status == GST_CLOCK_UNSCHEDULED))
    goto was_unscheduled;

  if (clock->entries)
    head = clock->entries->data;
  else
    head = NULL;

  /* need to take a ref */
  gst_clock_id_ref ((GstClockID) entry);
  /* insert the entry in sorted order */
  clock->entries = g_list_insert_sorted (clock->entries, entry,
      gst_clock_id_compare_func);

  /* only need to send the signal if the entry was added to the
   * front, else the thread is just waiting for another entry and
   * will get to this entry automatically. */
  if (clock->entries->data == entry) {
    GST_CAT_DEBUG (GST_CAT_CLOCK, "async entry added to head");
    if (head == NULL) {
      /* the list was empty before, signal the cond so that the async thread can
       * start taking a look at the queue */
      GST_CAT_DEBUG (GST_CAT_CLOCK, "first entry, sending signal");
      GST_CLOCK_BROADCAST (clock);
    } else {
      if (head->status == GST_CLOCK_BUSY) {
        /* the async thread was waiting for an entry, unlock the wait so that it
         * looks at the new head entry instead, we only need to do this once */
        if (!sysclock->priv->async_wakeup) {
          GST_CAT_DEBUG (GST_CAT_CLOCK, "wakeup async thread");
          sysclock->priv->async_wakeup = TRUE;
          gst_system_clock_add_wakeup (sysclock);
        }
      }
    }
  }
  GST_OBJECT_UNLOCK (clock);

  return GST_CLOCK_OK;

  /* ERRORS */
thread_error:
  {
    /* Could not start the async clock thread */
    GST_OBJECT_UNLOCK (clock);
    return GST_CLOCK_ERROR;
  }
was_unscheduled:
  {
    GST_OBJECT_UNLOCK (clock);
    return GST_CLOCK_UNSCHEDULED;
  }
}

/* unschedule an entry. This will set the state of the entry to GST_CLOCK_UNSCHEDULED
 * and will signal any thread waiting for entries to recheck their entry.
 * We cannot really decide if the signal is needed or not because the entry
 * could be waited on in async or sync mode.
 *
 * MT safe.
 */
static void
gst_system_clock_id_unschedule (GstClock * clock, GstClockEntry * entry)
{
  GstSystemClock *sysclock;

  sysclock = GST_SYSTEM_CLOCK_CAST (clock);

  GST_CAT_DEBUG (GST_CAT_CLOCK, "unscheduling entry %p", entry);

  GST_OBJECT_LOCK (clock);
  if (entry->status == GST_CLOCK_BUSY) {
    /* the entry was being busy, wake up all entries so that they recheck their
     * status. We cannot wake up just one entry because allocating such a
     * datastructure for each entry would be too heavy and unlocking an entry
     * is usually done when shutting down or some other exceptional case. */
    GST_CAT_DEBUG (GST_CAT_CLOCK, "entry was BUSY, doing wakeup");
    gst_system_clock_add_wakeup (sysclock);
  }
  /* when it leaves the poll, it'll detect the unscheduled */
  entry->status = GST_CLOCK_UNSCHEDULED;
  GST_OBJECT_UNLOCK (clock);
}
