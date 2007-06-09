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

#include <stdlib.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <tomoe/tomoe.h>

#include "tomoe_dictionary_page.h"
#include "tomoe_details.h"

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

static void tomoe_dictionary_page_class_init       (TomoeDictionaryPageClass    *klass);
static void tomoe_dictionary_page_init             (TomoeDictionaryPage         *object);
static void tomoe_dictionary_page_dispose          (GObject                     *object);
static void tomoe_dictionary_page_set_sensitive    (TomoeDictionaryPage         *page);

static int  _get_selection                         (TomoeDictionaryPage         *page);
static void _populate                              (TomoeDictionaryPage         *page);
static void _populate_dict_list                    (TomoeDictionaryPage         *page);
static tomoe_dict *_get_active_dict                (TomoeDictionaryPage         *page);
static void _show_details_window_for_character_index (TomoeDictionaryPage       *page,
                                                      gint                       index);

static void on_dict_list_changed                   (GtkComboBox                 *dict_list,
                                                    gpointer                     user_data);
static void on_selection_changed                   (GtkTreeSelection            *selection,
                                                    gpointer                     user_data);
static void on_add_button_clicked                  (GtkButton                   *button,
                                                    gpointer                     user_data);
static void on_delete_button_clicked               (GtkButton                   *button,
                                                    gpointer                     user_data);
static void on_details_button_clicked              (GtkButton                   *button,
                                                    gpointer                     user_data);
static gboolean on_tree_view_button_press          (GtkWidget                   *widget,
                                                    GdkEventButton              *event,
                                                    gpointer                     user_data);

/*static guint dictionary_page_signals[LAST_SIGNAL] = { 0 };*/
static GtkTableClass *parent_class = NULL;

GType
tomoe_dictionary_page_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeDictionaryPageClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_dictionary_page_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictionaryPage),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_dictionary_page_init,
        };

        type = g_type_register_static (GTK_TYPE_TABLE,
                                       "TomoeDictionaryPage",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

GtkWidget *
tomoe_dictionary_page_new (tomoe_db *db, tomoe_config *config)
{
    GtkWidget *w = GTK_WIDGET(g_object_new (TOMOE_TYPE_DICTIONARY_PAGE,
                                            NULL));
    TOMOE_DICTIONARY_PAGE (w)->database = db;
    TOMOE_DICTIONARY_PAGE (w)->config = config;
    _populate_dict_list (TOMOE_DICTIONARY_PAGE (w));
    _populate (TOMOE_DICTIONARY_PAGE (w));
    return w;
}

static void
tomoe_dictionary_page_class_init (TomoeDictionaryPageClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    /*GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);*/

    parent_class = (GtkTableClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_dictionary_page_dispose;
}

static void
tomoe_dictionary_page_init (TomoeDictionaryPage *page)
{
    GtkWidget *main_vbox, *vbox, *hbox, *list, *button, *alignment;
    GtkWidget *scrolled_window;
    GtkWidget *label, *droplist;
    GtkCellRenderer   *renderer;
    GtkTreeViewColumn *column;

    gtk_table_resize (GTK_TABLE (page), 1, 1);
    gtk_table_set_homogeneous (GTK_TABLE (page), TRUE);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
    gtk_table_attach_defaults (GTK_TABLE (page), hbox,
                               0, 1, 0, 1);
    gtk_widget_show (hbox);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), main_vbox,
                        TRUE, TRUE, 0);
    gtk_widget_set_size_request (main_vbox, 320, 320);
    gtk_widget_show (main_vbox);

    /* dictionary list */
    page->store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (page->store));
    g_signal_connect (G_OBJECT (list), "button-press-event",
                      G_CALLBACK (on_tree_view_button_press), page);

    page->char_list = list;
    page->char_sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
    gtk_tree_selection_set_mode (page->char_sel, GTK_SELECTION_SINGLE);

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), 
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
					 GTK_SHADOW_ETCHED_IN);
    gtk_container_add (GTK_CONTAINER (scrolled_window), list);
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
    gtk_box_pack_start (GTK_BOX (main_vbox), scrolled_window, TRUE, TRUE, 0);
    g_signal_connect (G_OBJECT (page->char_sel), "changed",
                      G_CALLBACK (on_selection_changed),
                      (gpointer) page);
    gtk_widget_show (list);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_ADD);
    page->add_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_add_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_EDIT); /*FIXME*/
    page->details_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_details_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
    page->delete_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_delete_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);

    /* default user database area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (main_vbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);
    gtk_widget_show (hbox);

    label = gtk_label_new (_("Dictionary: "));
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 4);
    gtk_widget_show (label);

    droplist = gtk_combo_box_new_text ();
    page->dict_list = droplist;
    gtk_box_pack_start (GTK_BOX (hbox), droplist, TRUE, TRUE, 4);
    g_signal_connect (G_OBJECT (droplist), "changed",
                      G_CALLBACK (on_dict_list_changed),
                      (gpointer) page);
    gtk_widget_show (droplist);

    tomoe_dictionary_page_set_sensitive (page);
}

static void
tomoe_dictionary_page_dispose (GObject *object)
{
    TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (object);

    if (page->store)
    {
        g_object_unref (page->store);
        page->store = NULL;
    }

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_dictionary_page_set_sensitive (TomoeDictionaryPage *page)
{
    gboolean     selected = (0 <= _get_selection (page));
    gboolean     editable;

    editable = tomoe_dict_getEditable (_get_active_dict (page));

    gtk_widget_set_sensitive (page->delete_button, selected & editable);
    gtk_widget_set_sensitive (page->details_button, selected);
    gtk_widget_set_sensitive (page->add_button, editable);
}

static int
_get_selection (TomoeDictionaryPage *page)
{
    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (page->char_list));
    GtkTreeModel     *model = NULL;
    GtkTreeIter       iter;

    if (gtk_tree_selection_get_selected (sel, &model, &iter))
    {
        GtkTreePath *path = gtk_tree_model_get_path (model, &iter);
        gint *ind = gtk_tree_path_get_indices (path);
        return ind ? ind[0] : -1;
    } else return -1;
}

static void
_populate (TomoeDictionaryPage *page)
{
    int i;
    GtkTreeIter  iter;
    tomoe_dict  *dict;

    if (!page->store)
        return;

    dict = _get_active_dict (page);
    if (!dict) return;

    gtk_list_store_clear (page->store);

    for (i = 0; i < tomoe_dict_getSize (dict); i++)
    {
        tomoe_char *chr = (tomoe_char*)tomoe_dict_charByIndex (dict, i);
        const int buf_size = 256;
        gchar readings_buf[buf_size]; /* FIXME dynamic */
        gchar strokes_text[32];
        int strokes = 0;
        int j;
        tomoe_array *readings = tomoe_char_getReadings (chr);

        if (tomoe_char_getGlyph (chr))
            strokes = tomoe_char_getGlyph (chr)->stroke_num;

        if (strokes > 0)
            g_sprintf (strokes_text, "%d", strokes);
        else
            g_strlcpy (strokes_text, "?", 32);

        if (tomoe_array_size (readings) > 0)
        {
            g_strlcpy (readings_buf, (gchar*)tomoe_array_get (readings, 0), buf_size);
            for (j = 1; j < tomoe_array_size (readings); j++)
            {
                g_strlcat (readings_buf, " ", buf_size);
                g_strlcat (readings_buf, (gchar*)tomoe_array_get (readings, j), buf_size);
            }
        }
        else
            g_strlcpy (readings_buf, "?", buf_size);

        gtk_list_store_append (page->store, &iter);
        gtk_list_store_set (page->store, &iter,
                            CHAR_COLUMN, tomoe_char_getCode (chr),
                            STROKECOUNT_COLUMN, strokes_text,
                            READING_COLUMN, readings_buf,
                            -1);
    }

}

static void
_populate_dict_list (TomoeDictionaryPage *page)
{
    int i;
    tomoe_array *dictlist;

    if (!page->database || !page->config || !page->dict_list)
        return;

    dictlist = tomoe_db_getDictList (page->database);

    for (i = 0; i < tomoe_array_size (dictlist); i++)
    {
        tomoe_dict *dict = (tomoe_dict*)tomoe_array_get (dictlist, i);
        GtkComboBox *def = GTK_COMBO_BOX (page->dict_list);
        if (tomoe_dict_getName (dict))
            gtk_combo_box_append_text (def, tomoe_dict_getName (dict));
        else
            gtk_combo_box_append_text (def, tomoe_dict_getFilename (dict));
    }

    /*
    gtk_combo_box_set_active (GTK_COMBO_BOX (page->dict_list),
                              tomoe_config_getDefaultUserDB (page->config));
    */
    gtk_combo_box_set_active (GTK_COMBO_BOX (page->dict_list), 0);
    /* TODO save active in settings */
}

static tomoe_dict *
_get_active_dict (TomoeDictionaryPage *page)
{
    tomoe_array  *dict_list;
    int           active;
    dict_list = tomoe_db_getDictList (page->database);
    active = gtk_combo_box_get_active (GTK_COMBO_BOX (page->dict_list));
    return (tomoe_dict*)tomoe_array_get (dict_list, active);
}

static void
_show_details_window_for_character_index (TomoeDictionaryPage *page, gint index)
{
    GtkWidget           *wnd;
    tomoe_char          *chr;

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (page));

    chr = tomoe_dict_charByIndex (_get_active_dict (page), index);

    tomoe_char_set_modified (chr, FALSE);
    wnd = tomoe_details_new (chr);
    gtk_dialog_run (GTK_DIALOG (wnd));
    gtk_widget_destroy (wnd);
    if (tomoe_char_get_modified (chr))
        _populate (page); /* FIXME */
}

static void
on_dict_list_changed (GtkComboBox *dict_list, gpointer user_data)
{
    TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (user_data);

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (page));

    _populate (page);
    tomoe_dictionary_page_set_sensitive (page);
}

static void
on_selection_changed (GtkTreeSelection *selection, gpointer user_data)
{
    TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (user_data);

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (page));

    tomoe_dictionary_page_set_sensitive (page);
}

static void
on_add_button_clicked (GtkButton *button, gpointer user_data)
{
    tomoe_char *chr;
    TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (user_data);
    tomoe_dict *dict;

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (page));

    dict = _get_active_dict (page);
    chr = tomoe_char_new (NULL);
    tomoe_char_setCode (chr, "??");
    tomoe_dict_addChar (dict, chr);
    _populate (page);
}

static void
on_delete_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (user_data);
    int index;
    tomoe_dict *dict;

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (page));

    index = _get_selection (page);
    if (index < 0) return;

    dict = _get_active_dict (page);
    tomoe_dict_removeByIndex (dict, index);
    _populate (page);
}

static void
on_details_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDictionaryPage *page;
    gint index;

    g_return_if_fail (TOMOE_IS_DICTIONARY_PAGE (user_data));

    page = TOMOE_DICTIONARY_PAGE (user_data);
    index = _get_selection (page);
    if (index < 0) return;

    _show_details_window_for_character_index (page, index);
}

static gboolean
on_tree_view_button_press (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    g_return_val_if_fail (TOMOE_IS_DICTIONARY_PAGE (user_data), FALSE);

    if (event->type == GDK_2BUTTON_PRESS && event->button == 1)
    {
        GtkTreePath *path = NULL;
        gtk_tree_view_get_path_at_pos (GTK_TREE_VIEW (widget),
                                       (gint) event->x, (gint) event->y,
                                       &path, NULL,
                                       NULL, NULL);
        if (path)
        {
            gint index;
            TomoeDictionaryPage *page = TOMOE_DICTIONARY_PAGE (user_data);

            index = gtk_tree_path_get_indices (path)[0];
            gtk_tree_path_free (path);
            _show_details_window_for_character_index (page, index);
        }
    }
    return FALSE;
}

/*
 * vi:ts=4:nowrap:ai:expandtab
 */
