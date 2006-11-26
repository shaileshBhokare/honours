 
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
#include <gtk/gtk.h>
#include <tomoe/tomoe.h>

#include "tomoe_edit_strokes.h"
#include "tomoe_canvas.h"

enum {
    HOGE_SIGNAL,
    LAST_SIGNAL,
};

static void tomoe_edit_strokes_class_init       (TomoeEditStrokesClass    *klass);
static void tomoe_edit_strokes_init             (TomoeEditStrokes         *object);
static void tomoe_edit_strokes_dispose          (GObject                    *object);
static void tomoe_edit_strokes_set_sensitive    (TomoeEditStrokes         *page);

static void on_ok_button_clicked          (GtkButton           *button,
                                           gpointer             user_data);
static void on_cancel_button_clicked      (GtkButton           *button,
                                           gpointer             user_data);
static void on_go_back_button_clicked     (GtkButton           *button,
                                           gpointer             user_data);
static void on_clear_button_clicked       (GtkButton           *button,
                                           gpointer             user_data);
static void on_normalize_button_clicked   (GtkButton           *button,
                                           gpointer             user_data);
static void on_canvas_stroke_added        (TomoeCanvas          *canvas,
                                           gpointer             user_data);

static GtkDialogClass *parent_class = NULL;

GType
tomoe_edit_strokes_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeEditStrokesClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_edit_strokes_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeEditStrokes),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_edit_strokes_init,
        };

        type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "TomoeEditStrokes",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

GtkWidget *
tomoe_edit_strokes_new (tomoe_char *chr)
{
    GtkWidget *w = GTK_WIDGET(g_object_new (TOMOE_TYPE_EDIT_STROKES,
                                            NULL));
    TOMOE_EDIT_STROKES (w)->character = chr;
    return w;
}


static void
tomoe_edit_strokes_class_init (TomoeEditStrokesClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkDialogClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_edit_strokes_dispose;
}

static void
tomoe_edit_strokes_init (TomoeEditStrokes *dialog)
{
    GtkWidget *vbox, *hbox, *button, *alignment, *canvas, *frame;

    gtk_window_set_title (GTK_WINDOW (dialog),
                          _("Edit Strokes"));

    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_DELETE_EVENT);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), hbox);
    gtk_widget_show (hbox);

    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 4);
    gtk_widget_show (frame);

    canvas = tomoe_canvas_new ();
    dialog->canvas = canvas;
    gtk_container_add (GTK_CONTAINER (frame), canvas);
    g_signal_connect (G_OBJECT (canvas), "stroke-added",
                      G_CALLBACK (on_canvas_stroke_added), (gpointer) dialog);
    gtk_widget_show (canvas);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_OK);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_ok_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_cancel_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
    dialog->go_back_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_go_back_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_CLEAR);
    dialog->clear_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_clear_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_ZOOM_FIT);
    dialog->normalize_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_normalize_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (button);

    tomoe_edit_strokes_set_sensitive (dialog);
}

static void
tomoe_edit_strokes_dispose (GObject *object)
{
    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_edit_strokes_set_sensitive (TomoeEditStrokes *page)
{
    gboolean editable = tomoe_canvas_has_stroke (TOMOE_CANVAS (page->canvas));

    gtk_widget_set_sensitive (page->go_back_button,   editable);
    gtk_widget_set_sensitive (page->clear_button,     editable);
    gtk_widget_set_sensitive (page->normalize_button, editable);
}

static void
on_ok_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeEditStrokes *dlg = TOMOE_EDIT_STROKES (user_data);
    tomoe_glyph *glyph;

    g_return_if_fail (TOMOE_IS_EDIT_STROKES (dlg));
    g_return_if_fail (TOMOE_IS_CANVAS (dlg->canvas));

    glyph = tomoe_canvas_get_glyph (TOMOE_CANVAS (dlg->canvas));
    tomoe_char_setGlyph (dlg->character, glyph);

    gtk_dialog_response (GTK_DIALOG (dlg), GTK_RESPONSE_DELETE_EVENT);
}

static void
on_cancel_button_clicked (GtkButton *button, gpointer user_data)
{
    GtkDialog *dlg = GTK_DIALOG (user_data);

    g_return_if_fail (GTK_IS_DIALOG (dlg));

    gtk_dialog_response (dlg, GTK_RESPONSE_DELETE_EVENT);
}

static void
on_go_back_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeEditStrokes *page = TOMOE_EDIT_STROKES (user_data);

    g_return_if_fail (TOMOE_IS_EDIT_STROKES (page));
    g_return_if_fail (TOMOE_IS_CANVAS (page->canvas));

    tomoe_canvas_revert (TOMOE_CANVAS (page->canvas));
    tomoe_canvas_find (TOMOE_CANVAS (page->canvas));
    tomoe_edit_strokes_set_sensitive (page);
}

static void
on_clear_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeEditStrokes *page = TOMOE_EDIT_STROKES (user_data);

    g_return_if_fail (TOMOE_IS_EDIT_STROKES (page));
    g_return_if_fail (TOMOE_IS_CANVAS (page->canvas));

    tomoe_canvas_clear (TOMOE_CANVAS (page->canvas));
    tomoe_edit_strokes_set_sensitive (page);
}

static void
on_canvas_stroke_added (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeEditStrokes *page = TOMOE_EDIT_STROKES (user_data);
    tomoe_edit_strokes_set_sensitive (page);
}

static void
on_normalize_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeEditStrokes *page = TOMOE_EDIT_STROKES (user_data);

    g_return_if_fail (TOMOE_EDIT_STROKES (page));
    g_return_if_fail (TOMOE_IS_CANVAS (page->canvas));

    tomoe_canvas_normalize (TOMOE_CANVAS (page->canvas));
}

