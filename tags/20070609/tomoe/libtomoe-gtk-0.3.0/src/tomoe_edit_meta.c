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

#include "tomoe_edit_meta.h"
#include "tomoe_details.h"

enum {
    CHAR_COLUMN,
    STROKECOUNT_COLUMN,
    READING_COLUMN,
    COLUMN_COUNT
};

static void tomoe_edit_meta_class_init       (TomoeEditMetaClass    *klass);
static void tomoe_edit_meta_init             (TomoeEditMeta         *object);
static void tomoe_edit_meta_dispose          (GObject               *object);
static void tomoe_edit_meta_set_sensitive    (TomoeEditMeta         *page);

static int  _get_selection                   (TomoeEditMeta         *page);
static void _populate                        (TomoeEditMeta         *page);

static void on_selection_changed             (GtkTreeSelection      *selection,
                                              gpointer               user_data);
static void on_save_button_clicked           (GtkButton             *button,
                                              gpointer               user_data);
static void on_abort_button_clicked          (GtkButton             *button,
                                              gpointer               user_data);
static void on_add_button_clicked            (GtkButton             *button,
                                              gpointer               user_data);
static void on_delete_button_clicked         (GtkButton             *button,
                                              gpointer               user_data);
static void on_edit_button_clicked           (GtkButton             *button,
                                              gpointer               user_data);

static GtkTableClass *parent_class = NULL;

GType
tomoe_edit_meta_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeEditMetaClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_edit_meta_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeEditMeta),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_edit_meta_init,
        };

        type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "TomoeEditMeta",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

GtkWidget *
tomoe_edit_meta_new (tomoe_char *chr)
{
    GtkWidget *w = GTK_WIDGET(g_object_new (TOMOE_TYPE_EDIT_META,
                                            NULL));
    TOMOE_EDIT_META (w)->character = chr;
    _populate (TOMOE_EDIT_META (w));
    return w;
}

static void
tomoe_edit_meta_class_init (TomoeEditMetaClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkTableClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_edit_meta_dispose;
}

static void
tomoe_edit_meta_init (TomoeEditMeta *dialog)
{
    GtkWidget *main_vbox, *vbox, *hbox, *list, *button, *alignment;
    GtkCellRenderer   *renderer;
    GtkTreeViewColumn *column;


    gtk_window_set_title (GTK_WINDOW (dialog),
                          _("Edit meta information"));

    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_DELETE_EVENT);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), hbox);
    gtk_widget_show (hbox);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), main_vbox,
                        TRUE, TRUE, 0);
    gtk_widget_set_size_request (main_vbox, 320, 320);
    gtk_widget_show (main_vbox);

    /* meta list */
    dialog->meta_store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    list = gtk_tree_view_new_with_model (GTK_TREE_MODEL (dialog->meta_store));
    dialog->meta_sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
    gtk_tree_selection_set_mode (dialog->meta_sel, GTK_SELECTION_SINGLE);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Type",
                                                       renderer,
                                                       "text", CHAR_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);
    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Value",
                                                       renderer,
                                                       "text", STROKECOUNT_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (list), column);

    gtk_box_pack_start (GTK_BOX (main_vbox), list, TRUE, TRUE, 0);
    g_signal_connect (G_OBJECT (dialog->meta_sel), "changed",
                      G_CALLBACK (on_selection_changed),
                      (gpointer) dialog);
    gtk_widget_show (list);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_SAVE);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_save_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_abort_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_ADD);
    dialog->add_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_add_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_EDIT);
    dialog->edit_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_edit_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_REMOVE);
    dialog->delete_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_delete_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    tomoe_edit_meta_set_sensitive (dialog);
}

static void
tomoe_edit_meta_dispose (GObject *object)
{
    TomoeEditMeta *meta = TOMOE_EDIT_META (object);

    if (meta->meta_store)
    {
        g_object_unref (meta->meta_store);
        meta->meta_store = NULL;
    }

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_edit_meta_set_sensitive (TomoeEditMeta *page)
{
    gboolean selected = (0 <= _get_selection (page));

    gtk_widget_set_sensitive (page->delete_button, selected);
    gtk_widget_set_sensitive (page->edit_button,   selected);
}

static int
_get_selection (TomoeEditMeta *page)
{
    /* Not implemented yet */
    return -1;
}

static void
_populate (TomoeEditMeta *page)
{
}

static void
on_selection_changed (GtkTreeSelection *selection, gpointer user_data)
{
    TomoeEditMeta *page = TOMOE_EDIT_META (user_data);

    g_return_if_fail (TOMOE_IS_EDIT_META (page));

    tomoe_edit_meta_set_sensitive (page);
}

static void
on_save_button_clicked (GtkButton *button, gpointer user_data)
{
    GtkDialog *dlg = GTK_DIALOG (user_data);

    g_return_if_fail (GTK_IS_DIALOG (dlg));

    gtk_dialog_response (dlg, GTK_RESPONSE_DELETE_EVENT);
}

static void
on_abort_button_clicked (GtkButton *button, gpointer user_data)
{
    GtkDialog *dlg = GTK_DIALOG (user_data);

    g_return_if_fail (GTK_IS_DIALOG (dlg));

    gtk_dialog_response (dlg, GTK_RESPONSE_DELETE_EVENT);
}

static void
on_add_button_clicked (GtkButton *button, gpointer user_data)
{
}

static void
on_delete_button_clicked (GtkButton *button, gpointer user_data)
{
}

static void
on_edit_button_clicked (GtkButton *button, gpointer user_data)
{
}

 
