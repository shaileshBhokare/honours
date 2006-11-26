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
#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <tomoe/tomoe.h>

#include "tomoe_dictionaries_page.h"

enum {
    HOGE_SIGNAL,
    LAST_SIGNAL,
};

enum {
    LOAD_COLUMN,
    SAVE_COLUMN,
    NAME_COLUMN,
    COLUMN_COUNT
};

static void tomoe_dictionaries_page_class_init       (TomoeDictionariesPageClass    *klass);
static void tomoe_dictionaries_page_init             (TomoeDictionariesPage         *object);
static void tomoe_dictionaries_page_dispose          (GObject                       *object);
static void tomoe_dictionaries_page_set_sensitive    (TomoeDictionariesPage         *page);

static void _populate                                (TomoeDictionariesPage         *page);
static int  _get_selection                           (TomoeDictionariesPage         *page);

static void on_selection_changed                     (GtkTreeSelection              *selection,
                                                      gpointer                       user_data);
static void on_add_button_clicked                    (GtkButton                     *button,
                                                      gpointer                       user_data);
static void on_remove_button_clicked                 (GtkButton                     *button,
                                                      gpointer                       user_data);
static void on_use_check_toggled                     (GtkCellRendererToggle         *cell_renderer,
                                                      gchar                         *path,
                                                      gpointer                       user_data);

static GtkTableClass *parent_class = NULL;

GType
tomoe_dictionaries_page_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeDictionariesPageClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_dictionaries_page_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDictionariesPage),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_dictionaries_page_init,
        };

        type = g_type_register_static (GTK_TYPE_TABLE,
                                       "TomoeDictionariesPage",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

GtkWidget *
tomoe_dictionaries_page_new (tomoe_db *db, tomoe_config *config)
{
    GtkWidget* w = GTK_WIDGET(g_object_new (TOMOE_TYPE_DICTIONARIES_PAGE,
                                            NULL));
    TOMOE_DICTIONARIES_PAGE (w)->database = db;
    TOMOE_DICTIONARIES_PAGE (w)->config = config;
    _populate (TOMOE_DICTIONARIES_PAGE (w));
    return w;
}

static void
tomoe_dictionaries_page_class_init (TomoeDictionariesPageClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkTableClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_dictionaries_page_dispose;
}

static void
tomoe_dictionaries_page_init (TomoeDictionariesPage *page)
{
    GtkWidget *vbox, *hbox, *list, *button, *alignment;
    GtkWidget *scrolled_window;
    GtkWidget *label;
    GtkCellRenderer   *renderer;
    GtkTreeViewColumn *column;
    GtkTreeSelection  *sel;

    gtk_table_resize (GTK_TABLE (page), 1, 1);
    gtk_table_set_homogeneous (GTK_TABLE (page), TRUE);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
    gtk_table_attach_defaults (GTK_TABLE (page), hbox,
                               0, 1, 0, 1);
    gtk_widget_show (hbox);

    /* dictionary list */
    page->store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);

    list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (page->store));
    page->dict_list = list;
    sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));

    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_ALWAYS);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
					 GTK_SHADOW_ETCHED_IN);

    renderer = gtk_cell_renderer_toggle_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Use"),
                                                       renderer,
                                                       "active", LOAD_COLUMN,
                                                       NULL);
    g_signal_connect (G_OBJECT (renderer), "toggled",
                      G_CALLBACK (on_use_check_toggled),
                      (gpointer) page);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Editable"),
                                                       renderer,
                                                       "text", SAVE_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes (_("Name"),
                                                       renderer,
                                                       "text", NAME_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);

    gtk_container_add (GTK_CONTAINER (scrolled_window), list);
    g_signal_connect (G_OBJECT (sel), "changed",
                      G_CALLBACK (on_selection_changed),
                      (gpointer) page);
    gtk_widget_show (list);
    gtk_box_pack_start (GTK_BOX (hbox), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show (scrolled_window);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_ADD);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_add_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);


#if 0
    button = gtk_button_new_from_stock (GTK_STOCK_EDIT); /*FIXME*/
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    gtk_widget_show (button);
#endif

    button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
    page->remove_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_remove_button_clicked),
                      (gpointer) page);
    gtk_widget_show (button);

    label = gtk_label_new (_("Please restart\nTomoe to apply"));
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 4);
    gtk_widget_show (label);

    _populate (page);
    tomoe_dictionaries_page_set_sensitive (page);
}

static void
tomoe_dictionaries_page_dispose (GObject *object)
{
    TomoeDictionariesPage *page = TOMOE_DICTIONARIES_PAGE (object);

    if (page->store)
    {
        g_object_unref (page->store);
        page->store = NULL;
    }

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_dictionaries_page_set_sensitive (TomoeDictionariesPage *page)
{
    tomoe_array *list;
    tomoe_dict_cfg *dict;
    gboolean removable;

    list = tomoe_config_getDictList (page->config);
    dict = tomoe_array_get (list, _get_selection (page));

    removable = (dict && dict->user);

    gtk_widget_set_sensitive (page->remove_button, removable);
}

static void
_populate (TomoeDictionariesPage *page)
{
    int i;
    GtkTreeIter  iter;
    tomoe_array *list;

    if (!page->database || !page->config || !page->store)
        return;

    list = tomoe_config_getDictList (page->config);
    gtk_list_store_clear (page->store);

    for (i = 0; i < tomoe_array_size (list); i++)
    {
        tomoe_dict_cfg *dict = (tomoe_dict_cfg*)tomoe_array_get (list, i);
        gchar *name;

        if (dict->user)
            name = dict->filename;
        else
            name = g_strdup_printf (_("%s (system)"), dict->filename);

        gtk_list_store_append (page->store, &iter);
        gtk_list_store_set (page->store, &iter,
                            LOAD_COLUMN, !dict->dontLoad,
                            SAVE_COLUMN, (((dict->user & dict->writeAccess) == 0) ? _("No") : _("Yes")),
                            NAME_COLUMN, name,
                            -1);

        if (!dict->user)
            g_free (name);
    }
}

static int
_get_selection (TomoeDictionariesPage *page)
{
    GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (page->dict_list));
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
on_selection_changed (GtkTreeSelection *selection, gpointer user_data)
{
    TomoeDictionariesPage *page = TOMOE_DICTIONARIES_PAGE (user_data);

    g_return_if_fail (TOMOE_IS_DICTIONARIES_PAGE (page));

    tomoe_dictionaries_page_set_sensitive (page);
}

static void
on_add_button_clicked (GtkButton *button, gpointer user_data)
{
    GtkWidget *dialog;
    GtkFileFilter *filter;
    TomoeDictionariesPage *page = TOMOE_DICTIONARIES_PAGE (user_data);

    dialog = gtk_file_chooser_dialog_new (_("Add TOMOE XML dictionary"),
                                          GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (page))),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    filter = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter, _("XML dictionary"));
    gtk_file_filter_add_mime_type (filter, "application/xml");
    gtk_file_filter_add_pattern (filter, "*.xml");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);
    gtk_file_chooser_set_filter (GTK_FILE_CHOOSER (dialog), filter);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        tomoe_dict_cfg *dict = (tomoe_dict_cfg*)calloc (1, sizeof (tomoe_dict_cfg));
        tomoe_array *list = tomoe_config_getDictList (page->config);

        dict->filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
        dict->user = TRUE;
        dict->dontLoad = FALSE;
        dict->writeAccess = TRUE;

        tomoe_array_append (list, dict);
        _populate (page); /* FIXME */
	tomoe_array_free (list);
    }

    gtk_widget_destroy (dialog);
}

static void
on_remove_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDictionariesPage *page = TOMOE_DICTIONARIES_PAGE (user_data);
    tomoe_array *list = tomoe_config_getDictList (page->config);
    tomoe_array_remove (list, _get_selection (page));
    _populate (page); /* FIXME */
}

static void
on_use_check_toggled (GtkCellRendererToggle *cell_renderer, gchar *path, gpointer user_data)
{
    TomoeDictionariesPage *page = TOMOE_DICTIONARIES_PAGE (user_data);
    tomoe_array *list = tomoe_config_getDictList (page->config);
    GtkTreePath *tp = gtk_tree_path_new_from_string (path);
    gint *ind = gtk_tree_path_get_indices (tp);
    tomoe_dict_cfg *dict = tomoe_array_get (list, ind ? ind[0] : -1);

    if (dict)
    {
        dict->dontLoad = gtk_cell_renderer_toggle_get_active (cell_renderer);
        _populate (page); /* FIXME */
    }
}
