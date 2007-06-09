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
 * Lesser General Public License for more edit_char.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <string.h>
#include <glib/gi18n.h>
#include <tomoe/tomoe.h>

#include "tomoe_edit_char.h"
#include "tomoe_canvas.h"
#include "tomoe_edit_meta.h"

static void tomoe_edit_char_class_init       (TomoeEditCharClass    *klass);
static void tomoe_edit_char_init             (TomoeEditChar         *object);
static void tomoe_edit_char_dispose          (GObject               *object);
static void tomoe_edit_char_set_sensitive    (TomoeEditChar         *window);

static void on_ok_button_clicked             (GtkButton            *button,
                                              gpointer              user_data);
static void on_cancel_button_clicked         (GtkButton            *button,
                                              gpointer              user_data);
static void on_input_changed                 (GtkEntry             *input,
                                              gpointer              user_data);

static GtkDialogClass *parent_class = NULL;

GType
tomoe_edit_char_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeEditCharClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_edit_char_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeEditChar),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_edit_char_init,
        };

        type = g_type_register_static (GTK_TYPE_DIALOG,
                                       "TomoeEditChar",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_edit_char_class_init (TomoeEditCharClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    parent_class = (GtkDialogClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_edit_char_dispose;
}

static void
tomoe_edit_char_init (TomoeEditChar *dialog)
{
    GtkWidget *hbox, *vbox, *alignment;
    GtkWidget *widget;

    gtk_window_set_title (GTK_WINDOW (dialog),
                          _("Change character"));

    gtk_window_set_resizable (GTK_WINDOW (dialog), FALSE);
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (dialog), 5);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_DELETE_EVENT);
    gtk_dialog_set_has_separator (GTK_DIALOG (dialog), FALSE);

    vbox = gtk_vbox_new (FALSE, 4);
    gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), vbox);
    gtk_widget_set_size_request (vbox, 240, -1);
    gtk_widget_show (vbox);

    widget = gtk_label_new (_("New character:"));
    dialog->label = widget;
    gtk_box_pack_start (GTK_BOX (vbox), widget, FALSE, FALSE, 4);
    gtk_widget_show (widget);

    widget = gtk_entry_new ();
    dialog->input = widget;
    gtk_box_pack_start (GTK_BOX (vbox), widget, TRUE, TRUE, 4);
    g_signal_connect (G_OBJECT (widget), "changed",
                      G_CALLBACK (on_input_changed),
                      (gpointer) dialog);
    gtk_widget_show (widget);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);
    gtk_widget_show (hbox);

    widget = gtk_button_new_from_stock (GTK_STOCK_CANCEL);
    gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (widget), "clicked",
                      G_CALLBACK (on_cancel_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (widget);

    widget = gtk_button_new_from_stock (GTK_STOCK_OK);
    dialog->ok_button = widget;
    gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (widget), "clicked",
                      G_CALLBACK (on_ok_button_clicked),
                      (gpointer) dialog);
    gtk_widget_show (widget);
}

static void
tomoe_edit_char_dispose (GObject *object)
{
    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

GtkWidget *
tomoe_edit_char_new (tomoe_char *chr, tomoe_dict *dict)
{
    GtkWidget *w = GTK_WIDGET(g_object_new (TOMOE_TYPE_EDIT_CHAR,
                                            NULL));
    TOMOE_EDIT_CHAR (w)->character = chr;
    TOMOE_EDIT_CHAR (w)->dict = dict;
    gtk_entry_set_text (GTK_ENTRY (TOMOE_EDIT_CHAR (w)->input), 
                        tomoe_char_getCode (chr));
    return w;
}

static void
on_ok_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeEditChar *dlg = TOMOE_EDIT_CHAR (user_data);

    g_return_if_fail (TOMOE_IS_EDIT_CHAR (dlg));

    tomoe_char_setCode (dlg->character,
                        gtk_entry_get_text (GTK_ENTRY (dlg->input)));
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
on_input_changed (GtkEntry *input, gpointer user_data)
{
    TomoeEditChar *dlg = TOMOE_EDIT_CHAR (user_data);
    gboolean ok = FALSE;

    g_return_if_fail (TOMOE_IS_EDIT_CHAR (dlg));

    ok = (strcmp (gtk_entry_get_text (GTK_ENTRY (dlg->input)), "") != 0);

    gtk_widget_set_sensitive (dlg->ok_button, ok);
}
