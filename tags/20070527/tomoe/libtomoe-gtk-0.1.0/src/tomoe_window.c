/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 *  Copyright (C) 2005 Takuro Ashie
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

#include "tomoe_window.h"
#include "tomoe_canvas.h"
#include "tomoe_candidates_view.h"

enum {
    HOGE_SIGNAL,
    LAST_SIGNAL,
};

static void tomoe_window_class_init       (TomoeWindowClass    *klass);
static void tomoe_window_init             (TomoeWindow         *object);
static void tomoe_window_dispose          (GObject             *object);
static void tomoe_window_set_sensitive    (TomoeWindow         *window);

/* callbacks for child widgets */
static void on_canvas_stroke_added        (TomoeCanvas         *canvas,
                                           gpointer             user_data);
static void on_find_button_clicked        (GtkButton           *button,
                                           gpointer             user_data);
static void on_go_back_button_clicked     (GtkButton           *button,
                                           gpointer             user_data);
static void on_clear_button_clicked       (GtkButton           *button,
                                           gpointer             user_data);
static void on_normalize_button_clicked   (GtkButton           *button,
                                           gpointer             user_data);

/*static guint window_signals[LAST_SIGNAL] = { 0 };*/
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
    }

    return type;
}

static void
tomoe_window_class_init (TomoeWindowClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    /*GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);*/

    parent_class = (GtkDialogClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_window_dispose;
}

static void
tomoe_window_init (TomoeWindow *window)
{
    GtkWidget *main_vbox, *hbox, *vbox, *frame, *alignment;
    GtkWidget *canvas, *button, *view;

#ifdef ENABLE_NLS
    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, LIBTOMOE_GTK_LOCALEDIR);
    textdomain (GETTEXT_PACKAGE);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif

    gtk_window_set_title (GTK_WINDOW (window),
                          _("Handwriting recognition"));
#if 0 /* by dai */
    gtk_window_set_icon_from_file (GTK_WINDOW (window),
                                   SCIM_ICONDIR "/scim-tomoe.png",
                                   NULL);
#endif
    /* FIXME! */
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

    gtk_dialog_add_buttons (GTK_DIALOG (window),
                            GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
                            NULL);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 8);
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox), main_vbox,
                        TRUE, TRUE, 0);
    gtk_widget_show (main_vbox);

    hbox = gtk_hbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (main_vbox), hbox,
                        TRUE, TRUE, 0);
    gtk_widget_show (hbox);

    /* canvas */
    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (hbox), frame, TRUE, TRUE, 0);
    gtk_widget_show (frame);

    canvas = tomoe_canvas_new ();
    window->canvas = canvas;
    gtk_container_add (GTK_CONTAINER (frame), canvas);
    g_signal_connect (G_OBJECT (canvas), "stroke-added",
                      G_CALLBACK (on_canvas_stroke_added), (gpointer) window);
    gtk_widget_show (canvas);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    window->button_area = vbox;
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

    button = gtk_button_new_from_stock (GTK_STOCK_FIND);
    window->find_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_find_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
    window->go_back_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_go_back_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_CLEAR);
    window->clear_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_clear_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    button = gtk_button_new_from_stock (GTK_STOCK_ZOOM_FIT);
    window->normalize_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_normalize_button_clicked),
                      (gpointer) window);
    gtk_widget_show (button);

    // candidates view
    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 4);
    gtk_widget_show (frame);

    view = tomoe_candidates_view_new ();
    window->candidates_view = view;
    tomoe_candidates_view_set_canvas (TOMOE_CANDIDATES_VIEW (view),
                                      TOMOE_CANVAS (canvas));
    gtk_container_add (GTK_CONTAINER (frame), view);
    gtk_widget_show (view);

    tomoe_window_set_sensitive (window);
}

static void
tomoe_window_dispose (GObject *object)
{
    TomoeWindow *window = TOMOE_WINDOW (window);

	if (G_OBJECT_CLASS(parent_class)->dispose)
		G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_window_set_sensitive (TomoeWindow *window)
{
    gboolean editable = tomoe_canvas_has_stroke (TOMOE_CANVAS (window->canvas));

    gtk_widget_set_sensitive (window->find_button,      editable);
    gtk_widget_set_sensitive (window->go_back_button,   editable);
    gtk_widget_set_sensitive (window->clear_button,     editable);
    gtk_widget_set_sensitive (window->normalize_button, editable);
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

    return window->canvas;
}

static void
on_canvas_stroke_added (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeWindow *window = TOMOE_WINDOW (user_data);

    tomoe_window_set_sensitive (window);
}

static void
on_find_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeWindow *window = TOMOE_WINDOW (user_data);

    g_return_if_fail (TOMOE_IS_WINDOW (window));
    g_return_if_fail (TOMOE_IS_CANVAS (window->canvas));

    tomoe_canvas_find (TOMOE_CANVAS (window->canvas));
}

static void
on_go_back_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeWindow *window = TOMOE_WINDOW (user_data);

    g_return_if_fail (TOMOE_IS_WINDOW (window));
    g_return_if_fail (TOMOE_IS_CANVAS (window->canvas));

    tomoe_canvas_revert (TOMOE_CANVAS (window->canvas));
    tomoe_window_set_sensitive (window);
}

static void
on_clear_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeWindow *window = TOMOE_WINDOW (user_data);

    g_return_if_fail (TOMOE_IS_WINDOW (window));
    g_return_if_fail (TOMOE_IS_CANVAS (window->canvas));

    tomoe_canvas_clear (TOMOE_CANVAS (window->canvas));
    tomoe_window_set_sensitive (window);
}

static void
on_normalize_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeWindow *window = TOMOE_WINDOW (user_data);

    g_return_if_fail (TOMOE_IS_WINDOW (window));
    g_return_if_fail (TOMOE_IS_CANVAS (window->canvas));

    tomoe_canvas_normalize (TOMOE_CANVAS (window->canvas));
    tomoe_window_set_sensitive (window);
}
