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

#include "tomoe_stroke_search.h"
#include "tomoe_canvas.h"
#include "tomoe_char_table.h"

enum {
    HOGE_SIGNAL,
    LAST_SIGNAL,
};

static void tomoe_stroke_search_class_init       (TomoeStrokeSearchClass    *klass);
static void tomoe_stroke_search_init             (TomoeStrokeSearch         *object);
static void tomoe_stroke_search_dispose          (GObject                   *object);
static void tomoe_stroke_search_set_sensitive    (TomoeStrokeSearch         *strokeSearch);

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
#if 0
static void on_details_button_clicked     (GtkButton           *button,
                                           gpointer             user_data);
#endif
static gboolean on_candidate_selected     (TomoeCharTable      *table,
                                           GdkEventButton      *event,
                                           gpointer             user_data);

/*static guint stroke_search_signals[LAST_SIGNAL] = { 0 };*/
static GtkTableClass *parent_class = NULL;

GType
tomoe_stroke_search_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeStrokeSearchClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_stroke_search_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeStrokeSearch),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_stroke_search_init,
        };

        type = g_type_register_static (GTK_TYPE_TABLE,
                                       "TomoeStrokeSearch",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_stroke_search_class_init (TomoeStrokeSearchClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    /*GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);*/

    parent_class = (GtkTableClass *) g_type_class_peek_parent (klass);

    gobject_class->dispose = tomoe_stroke_search_dispose;
}

static void
tomoe_stroke_search_init (TomoeStrokeSearch *strokeSearch)
{
    GtkWidget *main_vbox, *hbox, *vbox, *frame, *alignment;
    GtkWidget *canvas, *button, *table;

    gtk_table_resize (GTK_TABLE (strokeSearch), 1, 1);
    gtk_table_set_homogeneous (GTK_TABLE (strokeSearch), TRUE);

    main_vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (main_vbox), 8);
    gtk_table_attach_defaults (GTK_TABLE (strokeSearch), main_vbox, 
                               0, 1, 0, 1);
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
    strokeSearch->canvas = canvas;
    gtk_container_add (GTK_CONTAINER (frame), canvas);
    g_signal_connect (G_OBJECT (canvas), "stroke-added",
                      G_CALLBACK (on_canvas_stroke_added),
                      (gpointer) strokeSearch);
    gtk_widget_show (canvas);

    /* button area */
    alignment = gtk_alignment_new (0.5, 0.5, 1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 0, 0, 8, 0);
    gtk_box_pack_start (GTK_BOX (hbox), alignment, FALSE, FALSE, 0);
    gtk_widget_show (alignment);

    vbox = gtk_vbox_new (FALSE, 0);
    strokeSearch->button_area = vbox;
    gtk_container_add (GTK_CONTAINER (alignment), vbox);
    gtk_widget_show (vbox);

	//create "Find" button handler
    button = gtk_button_new_from_stock (GTK_STOCK_FIND);
    strokeSearch->find_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_find_button_clicked),
                      (gpointer) strokeSearch);
    gtk_widget_show (button);

	//create "Backspace" button handler
    button = gtk_button_new_from_stock (GTK_STOCK_GO_BACK);
    strokeSearch->go_back_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_go_back_button_clicked),
                      (gpointer) strokeSearch);
    gtk_widget_show (button);

	//create "Clear" button handler
    button = gtk_button_new_from_stock (GTK_STOCK_CLEAR);
    strokeSearch->clear_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_clear_button_clicked),
                      (gpointer) strokeSearch);
    gtk_widget_show (button);

	//create "Fix best" button handler
    button = gtk_button_new_from_stock (GTK_STOCK_ZOOM_FIT);
    strokeSearch->normalize_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_normalize_button_clicked),
                      (gpointer) strokeSearch);
    gtk_widget_show (button);
#if 0
    button = gtk_button_new_from_stock (GTK_STOCK_EDIT);  /* FIXME */
    strokeSearch->details_button = button;
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 4);
    g_signal_connect (G_OBJECT (button), "clicked",
                      G_CALLBACK (on_details_button_clicked),
                      (gpointer) strokeSearch);
    gtk_widget_show (button);
#endif
    /* candidates view */
    frame = gtk_frame_new (NULL);
    gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 4);
    gtk_widget_show (frame);

    table = tomoe_char_table_new ();
#if 0
    /* test for table mode */
    gtk_widget_set_size_request (table, -1, 100);
    tomoe_char_table_set_layout (TOMOE_CHAR_TABLE (table),
                                 TOMOE_CHAR_TABLE_LAYOUT_HORIZONTAL);
#endif
    strokeSearch->candidates_view = table;
    tomoe_char_table_set_canvas (TOMOE_CHAR_TABLE (table),
                                 TOMOE_CANVAS (canvas));
    g_signal_connect (G_OBJECT (table), "button-release-event",
                      G_CALLBACK (on_candidate_selected), (gpointer) strokeSearch);
    gtk_container_add (GTK_CONTAINER (frame), table);
    gtk_widget_show (table);

    tomoe_stroke_search_set_sensitive (strokeSearch);

#if 0
    /* test for vertical mode */
    table = tomoe_char_table_new ();
    gtk_widget_set_size_request (table, 100, -1);
    tomoe_char_table_set_layout (TOMOE_CHAR_TABLE (table),
                                 TOMOE_CHAR_TABLE_LAYOUT_VERTICAL);
    tomoe_char_table_set_canvas (TOMOE_CHAR_TABLE (table),
                                 TOMOE_CANVAS (canvas));
    gtk_box_pack_start (GTK_BOX (hbox), table, TRUE, TRUE, 0);
    gtk_widget_show (table);
#endif
}

static void
tomoe_stroke_search_dispose (GObject *object)
{
    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

static void
tomoe_stroke_search_set_sensitive (TomoeStrokeSearch *strokeSearch)
{
    gboolean editable = tomoe_canvas_has_stroke (TOMOE_CANVAS (strokeSearch->canvas));
    tomoe_char *sel = tomoe_char_table_get_selected
        (TOMOE_CHAR_TABLE (strokeSearch->candidates_view));

    gtk_widget_set_sensitive (strokeSearch->find_button,      editable);
    gtk_widget_set_sensitive (strokeSearch->go_back_button,   editable);
    gtk_widget_set_sensitive (strokeSearch->clear_button,     editable);
    gtk_widget_set_sensitive (strokeSearch->normalize_button, editable);
#if 0
    gtk_widget_set_sensitive (strokeSearch->details_button,   (sel != NULL) ? 1 : 0);
#endif
}

GtkWidget *
tomoe_stroke_search_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_STROKE_SEARCH,
                                    NULL));
}

void
tomoe_stroke_search_clear (TomoeStrokeSearch *strokeSearch)
{
}

GtkWidget *
tomoe_stroke_search_get_canvas (TomoeStrokeSearch *strokeSearch)
{
    g_return_val_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch), NULL);

    return strokeSearch->canvas;
}

static void
on_canvas_stroke_added (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    tomoe_stroke_search_set_sensitive (strokeSearch);
}

static void
on_find_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    g_return_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch));
    g_return_if_fail (TOMOE_IS_CANVAS (strokeSearch->canvas));

    tomoe_canvas_find (TOMOE_CANVAS (strokeSearch->canvas));
}

static void
on_go_back_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    g_return_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch));
    g_return_if_fail (TOMOE_IS_CANVAS (strokeSearch->canvas));

    tomoe_canvas_revert (TOMOE_CANVAS (strokeSearch->canvas));
    tomoe_canvas_find (TOMOE_CANVAS (strokeSearch->canvas));
    tomoe_stroke_search_set_sensitive (strokeSearch);
}

static void
on_clear_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    g_return_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch));
    g_return_if_fail (TOMOE_IS_CANVAS (strokeSearch->canvas));

    tomoe_canvas_clear (TOMOE_CANVAS (strokeSearch->canvas));
    tomoe_stroke_search_set_sensitive (strokeSearch);
}

static void
on_normalize_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    g_return_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch));
    g_return_if_fail (TOMOE_IS_CANVAS (strokeSearch->canvas));

    tomoe_canvas_normalize (TOMOE_CANVAS (strokeSearch->canvas));
    tomoe_stroke_search_set_sensitive (strokeSearch);
}

#if 0
static void
on_details_button_clicked (GtkButton *button, gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);
    GtkWidget         *wnd;
    tomoe_char        *chr;

    g_return_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch));
    g_return_if_fail (TOMOE_IS_CHAR_TABLE (strokeSearch->candidates_view));

    chr = tomoe_char_table_get_selected
        (TOMOE_CHAR_TABLE (strokeSearch->candidates_view));

    wnd = GTK_WIDGET (tomoe_details_new (chr));
    gtk_dialog_run (GTK_DIALOG (wnd));
    gtk_widget_destroy (wnd);
}
#endif

/* TODO does not work */
static gboolean
on_candidate_selected (TomoeCharTable *table,
                       GdkEventButton *event,
                       gpointer user_data)
{
    TomoeStrokeSearch *strokeSearch = TOMOE_STROKE_SEARCH (user_data);

    g_return_val_if_fail (TOMOE_IS_STROKE_SEARCH (strokeSearch), FALSE);

    tomoe_stroke_search_set_sensitive (strokeSearch);
    return FALSE;
}
