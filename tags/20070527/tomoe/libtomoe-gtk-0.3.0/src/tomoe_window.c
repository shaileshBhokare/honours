/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2005 Takuro Ashie
 *  Copyright (C) 2006 Juernjakob Harder
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <glib/gi18n.h>
#include <tomoe/tomoe.h>

#include "tomoe_window.h"
#include "tomoe_canvas.h"
#include "tomoe_stroke_search.h"
#include "tomoe_char_table.h"
#include "tomoe_dictionaries_page.h"
#include "tomoe_reading_search_page.h"
#include "tomoe_dictionary_page.h"

#define TOMOE_DATA_VIEW_ICON LIBTOMOEGTKDATADIR"/tomoe-data-view.png"
#define TOMOE_DICTIONARY_ICON LIBTOMOEGTKDATADIR"/tomoe-dictionary.png"
#define TOMOE_HANDWRITING_ICON LIBTOMOEGTKDATADIR"/tomoe-handwriting.png"
#define TOMOE_SEARCHING_ICON LIBTOMOEGTKDATADIR"/tomoe-search.png"

static void tomoe_window_class_init       (TomoeWindowClass    *klass);
static void tomoe_window_init             (TomoeWindow         *object);
static void tomoe_window_dispose          (GObject             *object);

static GtkDialogClass *parent_class = NULL;

GType
tomoe_window_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeWindowClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_window_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeWindow),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_window_init,
        };

        type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "TomoeWindow",
                                       &info, (GTypeFlags) 0);
        parent_class = g_type_class_ref (GTK_TYPE_WINDOW);
    }

    return type;
}

static void
tomoe_window_class_init (TomoeWindowClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkDialogClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_window_dispose;
}

static void
tomoe_window_init (TomoeWindow *window)
{
    GtkWidget   *handwriting_page, *dictionaries_page, *reading_search_page;
    GtkWidget   *notebook, *page;
    TomoeCanvas *canvas;
    tomoe_array *list;

#ifdef ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, LIBTOMOE_GTK_LOCALEDIR);
    textdomain (GETTEXT_PACKAGE);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif

    /* load libtomoe */
    tomoe_init ();
    
    window->config = tomoe_config_new (g_getenv("TOMOE_CONFIG_FILE"));
    tomoe_config_load (window->config);
    list = tomoe_config_getDictList (window->config);
    window->database = tomoe_db_new ();
    tomoe_db_loadDictList (window->database, list);

    gtk_window_set_title (GTK_WINDOW (window),
                          _("Handwriting recognition"));

    /* FIXME! */
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

    /* create search pages view notebook */
    notebook = gtk_notebook_new ();
    window->notebook = notebook;
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_LEFT);
    gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), TRUE);
    gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook), FALSE);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (window)->vbox), notebook);
    gtk_widget_show (notebook);

    /* TODO gtk warnings at destroy */
    handwriting_page = tomoe_stroke_search_new ();
    window->handwriting_page = handwriting_page;
    canvas = TOMOE_CANVAS (tomoe_stroke_search_get_canvas (TOMOE_STROKE_SEARCH (handwriting_page)));
    tomoe_canvas_set_database (canvas, window->database);
    gtk_widget_show (handwriting_page);
    tomoe_window_append_page (window, handwriting_page, 
                              gtk_image_new_from_file (TOMOE_HANDWRITING_ICON),
                              _("Handwriting recognition"));

    reading_search_page = tomoe_reading_search_page_new ();
    window->reading_search_page = reading_search_page;
    tomoe_reading_search_page_set_database (TOMOE_READING_SEARCH_PAGE (reading_search_page), window->database);
    gtk_widget_show (reading_search_page);
    tomoe_window_append_page (window, reading_search_page, 
                              gtk_image_new_from_file (TOMOE_SEARCHING_ICON),
                              _("Search with reading "));

    dictionaries_page = tomoe_dictionaries_page_new (window->database, window->config);
    window->dictionaries_page = dictionaries_page;
    gtk_widget_show (dictionaries_page);
    tomoe_window_append_page (window, dictionaries_page,
                              gtk_image_new_from_file (TOMOE_DICTIONARY_ICON),
                              _("List view of Dictionaries"));

    page = tomoe_dictionary_page_new (window->database, window->config);
    window->dictionary_page = page;
    gtk_widget_show (page);
    tomoe_window_append_page (window, page,
                              gtk_image_new_from_file (TOMOE_DATA_VIEW_ICON),
                              _("List view of contents in each dictionary"));
}

static void
tomoe_window_dispose (GObject *object)
{
    TomoeWindow *window = TOMOE_WINDOW (object);

    if (window->database)
    {
        tomoe_db_save (window->database);
        tomoe_db_free (window->database);
        window->database = NULL;
    }
    
    if (window->config)
    {
        tomoe_config_save (window->config);
        tomoe_config_free (window->config);
        window->config = NULL;
    }
    tomoe_term ();

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

GtkWidget *
tomoe_window_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_WINDOW,
                                    NULL));
}

GtkWidget *
tomoe_window_get_canvas (TomoeWindow *window)
{
    g_return_val_if_fail (TOMOE_IS_WINDOW (window), NULL);
    g_return_val_if_fail (TOMOE_IS_STROKE_SEARCH (window->handwriting_page), NULL);

    return tomoe_stroke_search_get_canvas (TOMOE_STROKE_SEARCH (window->handwriting_page));
}
void
tomoe_window_append_page (TomoeWindow *window, GtkWidget *page, GtkWidget *label, const gchar *label_text)
{
    GtkTooltips *tip;
    GtkWidget *event_box;

    g_return_if_fail (TOMOE_IS_WINDOW (window));

    if (!page || !GTK_IS_WIDGET (page)) return;

    event_box = gtk_event_box_new ();
    gtk_event_box_set_visible_window (GTK_EVENT_BOX (event_box), FALSE);
    gtk_container_add (GTK_CONTAINER (event_box), label);
    gtk_widget_show (event_box);
    gtk_widget_show (label);

    gtk_notebook_append_page (GTK_NOTEBOOK (window->notebook), page, event_box);

    if (label_text) {
        tip = gtk_tooltips_new ();
        gtk_tooltips_set_tip (tip, event_box, label_text, NULL);
    }
}
/*
 * vi:ts=4:nowrap:ai:expandtab
 */
