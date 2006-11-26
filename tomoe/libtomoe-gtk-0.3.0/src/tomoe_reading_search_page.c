/*
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

#include <glib/gprintf.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "tomoe_reading_search_page.h"
#include "tomoe_char_table.h"

enum {
    HOGE_SIGNAL,
    LAST_SIGNAL,
};

enum {
    CHAR_COLUMN,
    STROKECOUNT_COLUMN,
    READING_COLUMN,
    COLUMN_COUNT
};

static void tomoe_reading_search_page_class_init       (TomoeReadingSearchPageClass    *klass);
static void tomoe_reading_search_page_init             (TomoeReadingSearchPage         *object);
static void tomoe_reading_search_page_dispose          (GObject                       *object);
static void tomoe_reading_search_page_set_sensitive    (TomoeReadingSearchPage         *page);

static void on_find_button_clicked        (GtkButton           *button,
                                           gpointer             user_data);
static void on_input_entry_activate	  (GtkEntry            *entry,
					   gpointer            *user_data);

/*static guint reading_search_page_signals[LAST_SIGNAL] = { 0 };*/
static GtkTableClass *parent_class = NULL;

GType
tomoe_reading_search_page_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeReadingSearchPageClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_reading_search_page_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeReadingSearchPage),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_reading_search_page_init,
        };

        type = g_type_register_static (GTK_TYPE_TABLE,
                                       "TomoeReadingSearchPage",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

GtkWidget *
tomoe_reading_search_page_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_READING_SEARCH_PAGE,
                                    NULL));
}

void
tomoe_reading_search_page_set_database (TomoeReadingSearchPage *page, tomoe_db *database)
{
    page->database = database;
}

static void
tomoe_reading_search_page_class_init (TomoeReadingSearchPageClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    /*GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);*/

    parent_class = (GtkTableClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_reading_search_page_dispose;
}

static void
tomoe_reading_search_page_init (TomoeReadingSearchPage *page)
{
    GtkWidget *hbox, *vbox, *input, *button, *list, *label;
    GtkCellRenderer   *renderer;
    GtkTreeViewColumn *column;
    GtkWidget *scrolled_window;

    gtk_table_resize (GTK_TABLE (page), 1, 1);
    gtk_table_set_homogeneous (GTK_TABLE (page), FALSE);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 8);
    gtk_table_attach_defaults (GTK_TABLE (page), vbox, 0, 1, 0, 1);
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    label = gtk_label_new (_("Reading:"));
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 4);
    gtk_widget_show (label);

    input = gtk_entry_new ();
    page->input = input;
    gtk_box_pack_start (GTK_BOX (hbox), input, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (page->input), "activate",
                      G_CALLBACK (on_input_entry_activate),
                      (gpointer) page);
    gtk_widget_show (input);

    button = gtk_button_new_from_stock (GTK_STOCK_FIND);
    page->find_button = button;
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_find_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);

    /* result view */
    page->result_store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (page->result_store));
    page->result_list = list;
    page->result_sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
    gtk_tree_selection_set_mode (page->result_sel, GTK_SELECTION_SINGLE);
    
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
					 GTK_SHADOW_ETCHED_IN);
    gtk_container_add (GTK_CONTAINER (scrolled_window), list);
    gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 4);
    gtk_widget_show (scrolled_window);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Character"),
                                                       renderer,
                                                       "text", CHAR_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Stroke count"),
                                                       renderer,
                                                       "text", STROKECOUNT_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Reading"),
                                                       renderer,
                                                       "text", READING_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
    gtk_widget_set_size_request (list, 400, 320);
    /*
    g_signal_connect (G_OBJECT (page->char_sel), "changed",
                      G_CALLBACK (on_selection_changed),
                      (gpointer) page);
    */
    gtk_widget_show (list);

    gtk_widget_show (hbox);
    gtk_widget_show (vbox);
    tomoe_reading_search_page_set_sensitive (page);
}

static void
tomoe_reading_search_page_dispose (GObject *object)
{
    TomoeReadingSearchPage *page = TOMOE_READING_SEARCH_PAGE (object);

    if (page->result_store)
    {
        g_object_unref (page->result_store);
        page->result_store = NULL;
    }

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_reading_search_page_set_sensitive (TomoeReadingSearchPage *page)
{
}

static void
tomoe_reading_search_page_start_search (TomoeReadingSearchPage *page)
{
    tomoe_array *result;
    const gchar *reading;
    int i;

    g_return_if_fail (GTK_IS_TREE_VIEW (page->result_list));
    g_return_if_fail (GTK_IS_LIST_STORE (page->result_store));
    g_return_if_fail (GTK_IS_ENTRY (page->input));

    gtk_list_store_clear (page->result_store);

    reading = gtk_entry_get_text (GTK_ENTRY (page->input));
    result = tomoe_db_searchByReading (page->database, reading);

    for (i = 0; i < tomoe_array_size (result); i++)
    {
        tomoe_char *chr = (tomoe_char*)tomoe_array_get (result, i);
        gchar **str_array;
        gchar *strokes_text;
        gchar *readings_text;
        int strokes = 0;
        int j, reading_num;
        GtkTreeIter iter;
        tomoe_array *readings = tomoe_char_getReadings (chr);

        if (tomoe_char_getGlyph (chr))
            strokes = tomoe_char_getGlyph (chr)->stroke_num;

        if (strokes > 0)
            strokes_text = g_strdup_printf ("%d", strokes);
        else
            strokes_text = g_strdup ("?");

        reading_num = tomoe_array_size (readings);
        if (reading_num > 0)
        {
	        str_array = g_new0 (gchar *, reading_num + 1);
            str_array[reading_num] = NULL;
            for (j = 0; j < reading_num; j++)
            {
                str_array[j]  = (gchar *)tomoe_array_getConst (readings, j);
            }
            readings_text = g_strjoinv (" ", str_array);
            g_free (str_array);
        }
        else
            readings_text = g_strdup ("?");

        gtk_list_store_append (page->result_store, &iter);
        gtk_list_store_set (page->result_store, &iter,
                            CHAR_COLUMN, tomoe_char_getCode (chr),
                            STROKECOUNT_COLUMN, strokes_text,
                            READING_COLUMN, readings_text,
                            -1);
        g_free (readings_text);
        g_free (strokes_text);
    }

    tomoe_array_free (result);
}

static void
on_input_entry_activate (GtkEntry *entry, gpointer *user_data)
{
    TomoeReadingSearchPage *page = TOMOE_READING_SEARCH_PAGE (user_data);

    g_return_if_fail (TOMOE_IS_READING_SEARCH_PAGE (page));

    tomoe_reading_search_page_start_search (page);
}

static void
on_find_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeReadingSearchPage *page = TOMOE_READING_SEARCH_PAGE (user_data);

    g_return_if_fail (TOMOE_IS_READING_SEARCH_PAGE (page));

    tomoe_reading_search_page_start_search (page);
}
/*
 * vi:ts=4:nowrap:ai:expandtab
 */
