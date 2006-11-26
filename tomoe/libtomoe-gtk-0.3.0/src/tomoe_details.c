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

#include <glib/gi18n.h>
#include <tomoe/tomoe.h>

#include "tomoe_details.h"
#include "tomoe_canvas.h"
#include "tomoe_edit_char.h"
#include "tomoe_edit_strokes.h"
#include "tomoe_edit_meta.h"

enum {
    PROPERTY_EDITABLE_COLUMN,
    PROPERTY_NAME_COLUMN,
    PROPERTY_VALUE_COLUMN,
    COLUMN_COUNT
};

enum {
    CHARACTER_PROPERTY,
    STROKE_PROPERTY,
    READINGS_PROPERTY,
    N_PROPERTIES 
};

static void tomoe_details_class_init       (TomoeDetailsClass    *klass);
static void tomoe_details_init             (TomoeDetails         *object);
static void tomoe_details_dispose          (GObject              *object);

static void _show_details                  (TomoeDetails         *dialog);
static void _update_sensitive              (TomoeDetails         *window);

static void on_close_button_clicked        (GtkButton            *button,
                                            gpointer              user_data);
static void on_edit_char_button_clicked    (GtkButton            *button,
                                            gpointer              user_data);
static void on_edit_strokes_button_clicked (GtkButton            *button,
                                            gpointer              user_data);
static void on_edit_meta_button_clicked    (GtkButton            *button,
                                            gpointer              user_data);
static void on_property_value_edited       (GtkCellRendererText  *renderer,
                                            const gchar          *path_str,
                                            const gchar          *new_text,
                                            gpointer              user_data);

static GtkDialogClass *parent_class = NULL;

GType
tomoe_details_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeDetailsClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_details_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeDetails),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_details_init,
        };

        type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "TomoeDetails",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_details_class_init (TomoeDetailsClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkDialogClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_details_dispose;
}

static void
tomoe_details_init (TomoeDetails *window)
{
    GtkWidget *hbox, *main_vbox, *vbox, *alignment, *button;
    GtkWidget *label, *canvas, *meta, *frame, *tree_view;
    GtkListStore *list_store;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;

    gtk_window_set_title (GTK_WINDOW (window),
                          _("Character Details"));

    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_modal (GTK_WINDOW (window), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (window), 5);
    gtk_dialog_set_default_response (GTK_DIALOG (window), GTK_RESPONSE_DELETE_EVENT);
    gtk_dialog_set_has_separator (GTK_DIALOG (window), FALSE);
    /*gtk_dialog_add_button (GTK_DIALOG (window), GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE);*/

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (window)->vbox), hbox);
    gtk_widget_set_size_request (hbox, 400, 320);
    gtk_widget_show (hbox);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), main_vbox,
                        TRUE, TRUE, 0);
    gtk_widget_show (main_vbox);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_close_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (_("Edit Character"));
    window->edit_char_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_edit_char_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (_("Edit Strokes"));
    window->edit_strokes_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_edit_strokes_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (_("Edit Meta"));
    window->edit_meta_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_edit_meta_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    /* character / strokes area */
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (main_vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_show (hbox);
    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (hbox), frame, FALSE, FALSE, 4);
    gtk_widget_show (frame);

    canvas = tomoe_canvas_new ();
    window->canvas = canvas;
    gtk_widget_set_size_request (canvas, 100, 100);
    tomoe_canvas_lock (TOMOE_CANVAS (canvas), TRUE);
    gtk_container_add (GTK_CONTAINER (frame), canvas);
    /*
    g_signal_connect (G_OBJECT (canvas), "stroke-added",
                      G_CALLBACK (on_canvas_stroke_added), (gpointer) page);
    */
    gtk_widget_show (canvas);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
    gtk_widget_show (vbox);

    label = gtk_label_new (_("Basic information"));
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 2);
    gtk_widget_show (label);

    /* basic informations */
    list_store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);
    window->basic_prop = list_store;
    tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree_view), FALSE);
    /* gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_GRID_LINES_VERTICAL); */
    gtk_box_pack_start (GTK_BOX (vbox), tree_view, FALSE, FALSE, 4);
    gtk_widget_show (tree_view);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Property",
                                                       renderer,
                                                       "text", PROPERTY_NAME_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

    renderer = gtk_cell_renderer_text_new ();
    g_signal_connect (G_OBJECT (renderer), "edited",
                      G_CALLBACK (on_property_value_edited), window);
    column = gtk_tree_view_column_new_with_attributes ("Value",
                                                       renderer,
                                                       "editable", PROPERTY_EDITABLE_COLUMN,
                                                       "text", PROPERTY_VALUE_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

    /* set initial values to list store */
    gtk_list_store_append (list_store, &iter);
    gtk_list_store_set (list_store, &iter,
                        PROPERTY_EDITABLE_COLUMN, TRUE,
                        PROPERTY_NAME_COLUMN, _("Character"),
                        PROPERTY_VALUE_COLUMN, NULL,
                        -1);
    gtk_list_store_append (list_store, &iter);
    gtk_list_store_set (list_store, &iter,
                        PROPERTY_EDITABLE_COLUMN, FALSE,
                        PROPERTY_NAME_COLUMN, _("Strokes"),
                        PROPERTY_VALUE_COLUMN, _("No stroke information"),
                        -1);
    gtk_list_store_append (list_store, &iter);
    gtk_list_store_set (list_store, &iter,
                        PROPERTY_EDITABLE_COLUMN, TRUE,
                        PROPERTY_NAME_COLUMN, _("Readings"),
                        PROPERTY_VALUE_COLUMN, NULL,
                        -1);
    g_object_unref (list_store);

    /* meta data area */
    meta = gtk_text_view_new ();
    window->meta_view = meta;
    gtk_text_view_set_editable (GTK_TEXT_VIEW (meta), FALSE);
    gtk_box_pack_start (GTK_BOX (main_vbox), meta, TRUE, TRUE, 4);
    gtk_widget_show (meta);
    
#if 0
    list_store = gtk_list_store_new (COLUMN_COUNT, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING);
    window->sub_prop = list_store;
    tree_view = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list_store));
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW (tree_view), FALSE);
    /* gtk_tree_view_set_grid_lines (GTK_TREE_VIEW (tree_view), GTK_TREE_VIEW_GRID_LINES_VERTICAL); */
    gtk_box_pack_start (GTK_BOX (main_vbox), tree_view, FALSE, FALSE, 4);
    gtk_widget_show (tree_view);

    renderer = gtk_cell_renderer_text_new ();
    column = gtk_tree_view_column_new_with_attributes ("Property",
                                                       renderer,
                                                       "text", PROPERTY_NAME_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);

    renderer = gtk_cell_renderer_text_new ();
    g_signal_connect (G_OBJECT (renderer), "edited",
                      G_CALLBACK (on_property_value_edited), window);
    column = gtk_tree_view_column_new_with_attributes ("Value",
                                                       renderer,
                                                       "editable", PROPERTY_EDITABLE_COLUMN,
                                                       "text", PROPERTY_VALUE_COLUMN,
                                                       NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view), column);
#endif
}

static void
tomoe_details_dispose (GObject *object)
{
    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

GtkWidget *
tomoe_details_new (tomoe_char *chr)
{
    GtkWidget *w = GTK_WIDGET(g_object_new (TOMOE_TYPE_DETAILS,
                                            NULL));
    TOMOE_DETAILS (w)->character = chr;
    _show_details (TOMOE_DETAILS (w));
    _update_sensitive (TOMOE_DETAILS (w));
    return w;
}

static void
_show_details (TomoeDetails *dialog)
{
    GtkTextBuffer *buf = gtk_text_buffer_new (NULL);
    tomoe_array *readings = tomoe_char_getReadings (dialog->character);
    const char *meta = tomoe_char_getMeta (dialog->character);
    char *chr;
    GtkTreeIter iter;
    gboolean editable = tomoe_char_is_editable (dialog->character);

    tomoe_canvas_set_preview_glyph (TOMOE_CANVAS (dialog->canvas), 
                                    tomoe_char_getGlyph (dialog->character));

    gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (dialog->basic_prop),
                                   &iter, NULL, CHARACTER_PROPERTY);
    gtk_list_store_set (dialog->basic_prop, &iter,
                        PROPERTY_VALUE_COLUMN, tomoe_char_getCode (dialog->character),
                        PROPERTY_EDITABLE_COLUMN, editable,
                        -1);
    if (tomoe_char_getGlyph (dialog->character))
    {
        chr = g_markup_printf_escaped 
            ("%d", tomoe_char_getGlyph (dialog->character)->stroke_num);
        gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (dialog->basic_prop),
                                       &iter, NULL, STROKE_PROPERTY);
        gtk_list_store_set (dialog->basic_prop, &iter,
                            PROPERTY_VALUE_COLUMN, chr,
                            -1);
        g_free (chr);
    }

    if (readings)
    {
        int j;
        int reading_num = tomoe_array_size (readings);
        gchar **str_array = g_new0 (gchar *, reading_num + 1);
        gchar *readings_text;
        str_array[reading_num] = NULL;
        for (j = 0; j < reading_num; j++)
        {
            str_array[j]  = (gchar *)tomoe_array_getConst (readings, j);
        }
        readings_text = g_strjoinv (" ", str_array);
        gtk_tree_model_iter_nth_child (GTK_TREE_MODEL (dialog->basic_prop),
                                       &iter, NULL, READINGS_PROPERTY);
        gtk_list_store_set (dialog->basic_prop, &iter,
                            PROPERTY_VALUE_COLUMN, readings_text,
                            PROPERTY_EDITABLE_COLUMN, editable,
                            -1);
        g_free (str_array);
        g_free (readings_text);
    }

    gtk_text_view_set_buffer (GTK_TEXT_VIEW (dialog->meta_view), buf);

    if (meta)
        gtk_text_buffer_insert_at_cursor (buf, meta, -1);
}

static void
_update_sensitive (TomoeDetails *dialog)
{
    gboolean editable = tomoe_char_is_editable (dialog->character);
    gtk_widget_set_sensitive (dialog->edit_char_button,    editable);
    gtk_widget_set_sensitive (dialog->edit_strokes_button, editable);
    gtk_widget_set_sensitive (dialog->edit_meta_button,    editable);
}

static void
on_close_button_clicked (GtkButton *button, gpointer user_data)
{
    GtkDialog *dlg = GTK_DIALOG (user_data);

    g_return_if_fail (GTK_IS_DIALOG (dlg));

    gtk_dialog_response (dlg, GTK_RESPONSE_DELETE_EVENT);
}

static void
on_edit_char_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDetails *dlg = TOMOE_DETAILS (user_data);
    GtkWidget    *wnd = tomoe_edit_char_new (dlg->character, NULL); /*FIXME*/

    gtk_dialog_run (GTK_DIALOG (wnd));
    gtk_widget_destroy (wnd);
    if (tomoe_char_get_modified (dlg->character))
        _show_details (dlg);
}

static void
on_edit_strokes_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDetails *dlg = TOMOE_DETAILS (user_data);
    GtkWidget    *wnd = tomoe_edit_strokes_new (dlg->character);

    gtk_dialog_run (GTK_DIALOG (wnd));
    gtk_widget_destroy (wnd);
    if (tomoe_char_get_modified (dlg->character))
        _show_details (dlg);
}

static void
on_edit_meta_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeDetails *dlg = TOMOE_DETAILS (user_data);
    GtkWidget    *wnd = tomoe_edit_meta_new (dlg->character);

    gtk_dialog_run (GTK_DIALOG (wnd));
    gtk_widget_destroy (wnd);
    if (tomoe_char_get_modified (dlg->character))
        _show_details (dlg);
}
static void
on_property_value_edited (GtkCellRendererText  *renderer,
                          const gchar          *path_str,
                          const gchar          *new_text,
                          gpointer              user_data)
{
    GtkTreePath *path;
    GtkTreeIter iter;
    gint index;
    gchar **readings = NULL;
    TomoeDetails *dlg = TOMOE_DETAILS (user_data);

    path = gtk_tree_path_new_from_string (path_str);
    index = gtk_tree_path_get_indices (path)[0];
    gtk_tree_path_free (path);

    gtk_tree_model_get_iter_from_string (GTK_TREE_MODEL (dlg->basic_prop), &iter, path_str);

    switch (index)
    {
        case CHARACTER_PROPERTY:
            tomoe_char_setCode (dlg->character, new_text);
            gtk_list_store_set (dlg->basic_prop, &iter,
                                PROPERTY_VALUE_COLUMN, new_text,
                                -1);
            break;
        case STROKE_PROPERTY: /* stroke number should not modify directly*/
            break;
        case READINGS_PROPERTY:
            readings = g_strsplit (new_text, " ", -1);
            if (readings)
            {
                guint i;
                tomoe_array *reading_array = NULL;
                reading_array = tomoe_array_new ((tomoe_compare_fn) tomoe_string_compare,
                                                 NULL, 
                                                 (tomoe_free_fn) g_free);
                for (i = 0; i < g_strv_length (readings); i++)
                {
                    tomoe_array_append (reading_array, g_strdup (readings[i]));
                }
                g_strfreev (readings);
                tomoe_char_setReadings (dlg->character, reading_array);
                tomoe_array_free (reading_array);
            }
            else
            {
                tomoe_char_setReadings (dlg->character, NULL);
            }
            gtk_list_store_set (dlg->basic_prop, &iter,
                                PROPERTY_VALUE_COLUMN, new_text,
                                -1);
            break;
        default:
            break;
    }
}

/*
 * vi:ts=4:nowrap:ai:expandtab
 */
