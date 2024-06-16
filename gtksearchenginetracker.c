/*
 * Copyright (C) 2009-2011 Nokia <ivan.frade@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 *
 * Authors: Jürg Billeter <juerg.billeter@codethink.co.uk>
 *          Martyn Russell <martyn@lanedo.com>
 *
 * Based on nautilus-search-engine-tracker.c
 */

#include "config.h"

#include <string.h>

#include <gio/gio.h>
#include <gmodule.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "gtksearchenginetracker.h"

#define DBUS_SERVICE_RESOURCES   "org.freedesktop.Tracker1"
#define DBUS_PATH_RESOURCES      "/org/freedesktop/Tracker1/Resources"
#define DBUS_INTERFACE_RESOURCES "org.freedesktop.Tracker1.Resources"

#define DBUS_SERVICE_STATUS      "org.freedesktop.Tracker1"
#define DBUS_PATH_STATUS         "/org/freedesktop/Tracker1/Status"
#define DBUS_INTERFACE_STATUS    "org.freedesktop.Tracker1.Status"

/* Time in second to wait for service before deciding it's not available */
#define WAIT_TIMEOUT_SECONDS 1

/* Time in second to wait for query results to come back */
#define QUERY_TIMEOUT_SECONDS 10

/* If defined, we use fts:match, this has to be enabled in Tracker to
 * work which it usually is. The alternative is to undefine it and
 * use filename matching instead. This doesn't use the content of the
 * file however.
 */
#undef FTS_MATCHING

/*
 * GtkSearchEngineTracker object
 */
struct _GtkSearchEngineTrackerPrivate
{
  GCancellable *cancellable;
  GtkQuery *query;
  gboolean query_pending;
};

G_DEFINE_TYPE (GtkSearchEngineTracker, _gtk_search_engine_tracker, GTK_TYPE_SEARCH_ENGINE);

static void
finalize (GObject *object)
{
  GtkSearchEngineTracker *tracker;

  g_debug ("Finalizing GtkSearchEngineTracker");

  tracker = GTK_SEARCH_ENGINE_TRACKER (object);

  if (tracker->priv->cancellable)
    {
      g_cancellable_cancel (tracker->priv->cancellable);
      g_object_unref (tracker->priv->cancellable);
      tracker->priv->cancellable = NULL;
    }

  if (tracker->priv->query)
    {
      g_object_unref (tracker->priv->query);
      tracker->priv->query = NULL;
    }

  G_OBJECT_CLASS (_gtk_search_engine_tracker_parent_class)->finalize (object);
}

static void
get_query_results (GtkSearchEngineTracker *engine,
                   const gchar            *sparql,
                   GAsyncReadyCallback     callback,
                   gpointer                user_data)
{
}

/* Stolen from libtracker-common */
static GList *
string_list_to_gslist (gchar **strv)
{
  GList *list;
  gsize i;

  list = NULL;

  for (i = 0; strv[i]; i++)
    list = g_list_prepend (list, g_strdup (strv[i]));

  return g_list_reverse (list);
}

/* Stolen from libtracker-sparql */
static gchar *
sparql_escape_string (const gchar *literal)
{
  GString *str;
  const gchar *p;

  g_return_val_if_fail (literal != NULL, NULL);

  str = g_string_new ("");
  p = literal;

  while (TRUE)
     {
      gsize len;

      if (!((*p) != '\0'))
        break;

      len = strcspn ((const gchar *) p, "\t\n\r\b\f\"\\");
      g_string_append_len (str, (const gchar *) p, (gssize) ((glong) len));
      p = p + len;

      switch (*p)
        {
        case '\t':
          g_string_append (str, "\\t");
          break;
        case '\n':
          g_string_append (str, "\\n");
          break;
        case '\r':
          g_string_append (str, "\\r");
          break;
        case '\b':
          g_string_append (str, "\\b");
          break;
        case '\f':
          g_string_append (str, "\\f");
          break;
        case '"':
          g_string_append (str, "\\\"");
          break;
        case '\\':
          g_string_append (str, "\\\\");
          break;
        default:
          continue;
        }

      p++;
     }
  return g_string_free (str, FALSE);
 }

static void
sparql_append_string_literal (GString     *sparql,
                              const gchar *str)
{
  gchar *s;

  s = sparql_escape_string (str);

  g_string_append_c (sparql, '"');
  g_string_append (sparql, s);
  g_string_append_c (sparql, '"');

  g_free (s);
}

static void
sparql_append_string_literal_lower_case (GString     *sparql,
                                         const gchar *str)
{
  gchar *s;

  s = g_utf8_strdown (str, -1);
  sparql_append_string_literal (sparql, s);
  g_free (s);
}

static void
query_callback (GObject      *object,
                GAsyncResult *res,
                gpointer      user_data)
{
}

static void
gtk_search_engine_tracker_start (GtkSearchEngine *engine)
{
  GtkSearchEngineTracker *tracker;
  gchar *search_text;
#ifdef FTS_MATCHING
  gchar *location_uri;
#endif
  GString *sparql;

  tracker = GTK_SEARCH_ENGINE_TRACKER (engine);

  if (tracker->priv->query_pending)
    {
      g_debug ("Attempt to start a new search while one is pending, doing nothing");
      return;
    }

  if (tracker->priv->query == NULL)
    {
      g_debug ("Attempt to start a new search with no GtkQuery, doing nothing");
      return;
    }

  search_text = _gtk_query_get_text (tracker->priv->query);

#ifdef FTS_MATCHING
  location_uri = _gtk_query_get_location (tracker->priv->query);
  /* Using FTS: */
  sparql = g_string_new ("SELECT nie:url(?urn) "
                         "WHERE {"
                         "  ?urn a nfo:FileDataObject ;"
                         "  tracker:available true ; "
                         "  fts:match ");
  sparql_append_string_literal (sparql, search_text);

  if (location_uri)
    {
      g_string_append (sparql, " . FILTER (fn:starts-with(nie:url(?urn),");
      sparql_append_string_literal (sparql, location_uri);
      g_string_append (sparql, "))");
    }

  g_string_append (sparql, " } ORDER BY DESC(fts:rank(?urn)) ASC(nie:url(?urn))");
#else  /* FTS_MATCHING */
  /* Using filename matching: */
  sparql = g_string_new ("SELECT nie:url(?urn) "
                         "WHERE {"
                         "  ?urn a nfo:FileDataObject ;"
                         "    tracker:available true ."
                         "  FILTER (fn:contains(fn:lower-case(nfo:fileName(?urn)),");
  sparql_append_string_literal_lower_case (sparql, search_text);

  g_string_append (sparql,
                   "))"
                   "} ORDER BY DESC(nie:url(?urn)) DESC(nfo:fileName(?urn))");
#endif /* FTS_MATCHING */

  tracker->priv->query_pending = TRUE;

  get_query_results (tracker, sparql->str, query_callback, tracker);

  g_string_free (sparql, TRUE);
  g_free (search_text);
}

static void
gtk_search_engine_tracker_stop (GtkSearchEngine *engine)
{
  GtkSearchEngineTracker *tracker;

  tracker = GTK_SEARCH_ENGINE_TRACKER (engine);

  if (tracker->priv->query && tracker->priv->query_pending)
    {
      g_cancellable_cancel (tracker->priv->cancellable);
      tracker->priv->query_pending = FALSE;
    }
}

static gboolean
gtk_search_engine_tracker_is_indexed (GtkSearchEngine *engine)
{
  return TRUE;
}

static void
gtk_search_engine_tracker_set_query (GtkSearchEngine *engine,
                                     GtkQuery        *query)
{
  GtkSearchEngineTracker *tracker;

  tracker = GTK_SEARCH_ENGINE_TRACKER (engine);

  if (query)
    g_object_ref (query);

  if (tracker->priv->query)
    g_object_unref (tracker->priv->query);

  tracker->priv->query = query;
}

static void
_gtk_search_engine_tracker_class_init (GtkSearchEngineTrackerClass *class)
{
  GObjectClass *gobject_class;
  GtkSearchEngineClass *engine_class;

  gobject_class = G_OBJECT_CLASS (class);
  gobject_class->finalize = finalize;

  engine_class = GTK_SEARCH_ENGINE_CLASS (class);
  engine_class->set_query = gtk_search_engine_tracker_set_query;
  engine_class->start = gtk_search_engine_tracker_start;
  engine_class->stop = gtk_search_engine_tracker_stop;
  engine_class->is_indexed = gtk_search_engine_tracker_is_indexed;

  g_type_class_add_private (gobject_class,
                            sizeof (GtkSearchEngineTrackerPrivate));
}

static void
_gtk_search_engine_tracker_init (GtkSearchEngineTracker *engine)
{
  engine->priv = G_TYPE_INSTANCE_GET_PRIVATE (engine,
                                              GTK_TYPE_SEARCH_ENGINE_TRACKER,
                                              GtkSearchEngineTrackerPrivate);
}


GtkSearchEngine *
_gtk_search_engine_tracker_new (void)
{
  return NULL;
}
