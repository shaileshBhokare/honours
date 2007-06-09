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

#include <stdlib.h>

#include "tomoe_candidates_view.h"
#include "tomoe_canvas.h"

enum {
    SELECTED_SIGNAL,
    LAST_SIGNAL,
};

typedef struct _TomoeCandidatesViewPriv TomoeCandidatesViewPriv;
struct _TomoeCandidatesViewPriv
{
    TomoeCanvas *canvas;

    GdkPixmap   *pixmap;
    guint        padding;
    gint         selected;
    gint         prelighted;
    GList       *layout_list;
};
#define TOMOE_CANDIDATES_VIEW_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CANDIDATES_VIEW, TomoeCandidatesViewPriv))

static void   tomoe_candidates_view_class_init           (TomoeCandidatesViewClass *klass);
static void   tomoe_candidates_view_init                 (TomoeCandidatesView      *object);
static void   tomoe_candidates_view_dispose              (GObject           *object);
static gint   tomoe_candidates_view_configure_event      (GtkWidget         *widget,
                                                          GdkEventConfigure *event);
static gint   tomoe_candidates_view_expose_event         (GtkWidget         *widget,
                                                          GdkEventExpose    *event);
static gint   tomoe_candidates_view_button_press_event   (GtkWidget         *widget,
                                                          GdkEventButton    *event);
static gint   tomoe_candidates_view_button_release_event (GtkWidget         *widget,
                                                          GdkEventButton    *event);
static gint   tomoe_candidates_view_motion_notify_event  (GtkWidget         *widget,
                                                          GdkEventMotion    *event);
static gint   tomoe_candidates_view_leave_notify_event   (GtkWidget         *widget,
                                                          GdkEventCrossing  *event);
static void   tomoe_candidates_view_draw                 (TomoeCandidatesView *view);

static void   on_canvas_find  (TomoeCanvas *canvas,
                               gpointer     user_data);
static void   on_canvas_clear (TomoeCanvas *canvas,
                               gpointer     user_data);

static guint view_signals[LAST_SIGNAL] = { 0 };
static GtkDrawingAreaClass *parent_class = NULL;

GType
tomoe_candidates_view_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeCandidatesViewClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_candidates_view_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeCandidatesView),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_candidates_view_init,
        };

        type = g_type_register_static (GTK_TYPE_DRAWING_AREA,
                                       "TomoeCandidatesView",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_candidates_view_class_init (TomoeCandidatesViewClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);

    parent_class = (GtkDrawingAreaClass *) g_type_class_peek_parent (klass);

    view_signals[SELECTED_SIGNAL] =
      g_signal_new ("selected",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCandidatesViewClass, selected),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);

    gobject_class->dispose             = tomoe_candidates_view_dispose;
    widget_class->configure_event      = tomoe_candidates_view_configure_event;
    widget_class->expose_event         = tomoe_candidates_view_expose_event;
    widget_class->button_press_event   = tomoe_candidates_view_button_press_event;
    widget_class->button_release_event = tomoe_candidates_view_button_release_event;
    widget_class->motion_notify_event  = tomoe_candidates_view_motion_notify_event;
    widget_class->leave_notify_event   = tomoe_candidates_view_leave_notify_event;

    klass->selected                    = NULL;

	g_type_class_add_private (gobject_class, sizeof(TomoeCandidatesViewPriv));
}

static void
tomoe_candidates_view_init (TomoeCandidatesView *view)
{
    GtkWidget *widget = GTK_WIDGET (view);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    PangoFontDescription *font_desc;

    gtk_widget_set_events (widget,
                           GDK_EXPOSURE_MASK |
                           GDK_ENTER_NOTIFY_MASK |
                           GDK_LEAVE_NOTIFY_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK |
                           GDK_POINTER_MOTION_HINT_MASK);

    // FIXME!
    gtk_drawing_area_size (GTK_DRAWING_AREA (view), 30, 30);

    font_desc = pango_font_description_from_string ("Sans 18");
    gtk_widget_modify_font (widget, font_desc);

    priv->canvas      = NULL;
    priv->pixmap      = NULL;
    priv->padding     = 2;
    priv->selected    = -1;
    priv->prelighted  = -1;
    priv->layout_list = NULL;
}

static void
tomoe_candidates_view_dispose (GObject *object)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (view);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);

    tomoe_candidates_view_set_canvas (view, NULL);

    if (priv->pixmap) {
        g_object_unref(priv->pixmap);
        priv->pixmap = NULL;
    }

    if (priv->layout_list) {
        g_list_foreach (priv->layout_list, (GFunc) g_object_unref, NULL);
        g_list_free (priv->layout_list);
        priv->layout_list = NULL;
    }

	if (G_OBJECT_CLASS(parent_class)->dispose)
		G_OBJECT_CLASS(parent_class)->dispose(object);
}

GtkWidget *
tomoe_candidates_view_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_CANDIDATES_VIEW,
                                    NULL));
}

void
tomoe_candidates_view_set_canvas (TomoeCandidatesView *view,
                                  TomoeCanvas         *canvas)
{
    TomoeCandidatesViewPriv *priv;

    g_return_if_fail (TOMOE_IS_CANDIDATES_VIEW (view));

    priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);

    if (priv->canvas) {
        g_signal_handlers_disconnect_by_func (
            G_OBJECT (priv->canvas),
            (gpointer) on_canvas_find,
            (gpointer) view);
        g_object_remove_weak_pointer (G_OBJECT (canvas),
                                      (gpointer*) &priv->canvas);
    }

    priv->canvas = canvas;
    g_object_add_weak_pointer (G_OBJECT (canvas),
                               (gpointer*) &priv->canvas);

    g_signal_connect_after (G_OBJECT (canvas), "find",
                            G_CALLBACK (on_canvas_find), (gpointer) view);
    g_signal_connect_after (G_OBJECT (canvas), "clear",
                            G_CALLBACK (on_canvas_clear), (gpointer) view);
}

gchar *
tomoe_candidates_view_get_selected_letter (TomoeCandidatesView *view)
{
    GtkWidget *widget;
    TomoeCandidatesViewPriv *priv;

    g_return_val_if_fail (TOMOE_IS_CANDIDATES_VIEW (view), NULL);

    widget = GTK_WIDGET (view);
    priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);

    if (!priv->canvas)
        return NULL;

    if (priv->selected < 0)
        return NULL;

    return tomoe_canvas_get_nth_candidate (priv->canvas, priv->selected);
}

static gint
tomoe_candidates_view_configure_event (GtkWidget *widget,
                                       GdkEventConfigure *event)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (widget);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    gboolean retval = FALSE;

    if (GTK_WIDGET_CLASS(parent_class)->configure_event)
        retval = GTK_WIDGET_CLASS(parent_class)->configure_event (widget,
                                                                  event);

    if (priv->pixmap)
        g_object_unref(priv->pixmap);

    priv->pixmap = gdk_pixmap_new(widget->window,
                                  widget->allocation.width,
                                  widget->allocation.height,
                                  -1);

    gdk_draw_rectangle (priv->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      0, 0,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);

    return retval;
}

static gint
tomoe_candidates_view_expose_event (GtkWidget *widget,
                                    GdkEventExpose *event)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (widget);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    gboolean retval = FALSE;

    if (GTK_WIDGET_CLASS(parent_class)->expose_event)
        retval = GTK_WIDGET_CLASS(parent_class)->expose_event (widget,
                                                               event);

    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height);

    return retval;
}

static gint
candidate_id_from_coordinate (TomoeCandidatesView *view, gint x, gint y)
{
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    gint i, left = 0, right = 0;
    GList *node;

    for (i = 0, node = priv->layout_list;
         node;
         i++, node = g_list_next (node))
    {
        PangoLayout *layout = PANGO_LAYOUT (node->data);
        gint char_width = 0, char_height = 0, pad = priv->padding;

        pango_layout_get_pixel_size (layout, &char_width, &char_height);

        right = right + ((pad * 2) + char_width);

        if (x >= left && x < right) {
            return i;
            break;
        }

        left = right;
    }

    return -1;
}

static gint
tomoe_candidates_view_button_press_event (GtkWidget *widget,
                                          GdkEventButton *event)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (widget);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    gint prev_selected;

    prev_selected = priv->selected;
    priv->selected = candidate_id_from_coordinate (view,
                                                   (gint) event->x,
                                                   (gint) event->y);

    if (prev_selected != priv->selected)
        tomoe_candidates_view_draw (view);
    if (priv->selected >= 0)
        g_signal_emit (G_OBJECT (widget),
                       view_signals[SELECTED_SIGNAL], 0);

    return FALSE;
}

static gint
tomoe_candidates_view_button_release_event (GtkWidget *widget,
                                            GdkEventButton *event)
{
    return FALSE;
}

static gint
tomoe_candidates_view_motion_notify_event (GtkWidget *widget,
                                           GdkEventMotion *event)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (widget);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    GdkModifierType state;
    gint x, y, prev_prelighted;

    if (event->is_hint) {
        gdk_window_get_pointer(event->window, &x, &y, &state);
    } else {
        x = (int) event->x;
        y = (int) event->y;
        state = (GdkModifierType) event->state;
    }

    prev_prelighted = priv->prelighted;
    priv->prelighted = candidate_id_from_coordinate (view, x, y);

    if (prev_prelighted != priv->prelighted) {
        tomoe_candidates_view_draw (view);
    }

    return FALSE;
}

static gint
tomoe_candidates_view_leave_notify_event (GtkWidget *widget,
                                          GdkEventCrossing *event)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (widget);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    gint prev_prelighted;

    prev_prelighted = priv->prelighted;
    priv->prelighted = -1;

    if (prev_prelighted != priv->prelighted) {
        tomoe_candidates_view_draw (view);
    }

    return FALSE;
}

static void
tomoe_candidates_view_draw (TomoeCandidatesView *view)
{
    GtkWidget *widget = GTK_WIDGET (view);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    guint i, left = 0;
    GList *node;

    gdk_draw_rectangle (priv->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    for (i = 0, node = priv->layout_list;
         node;
         i++, node = g_list_next (node))
    {
        PangoLayout *layout = PANGO_LAYOUT (node->data);
        gint pad = priv->padding, char_width = 20, char_height = 20;
        gint right;

        pango_layout_get_pixel_size (layout, &char_width, &char_height);
        right = left + ((pad * 2) + char_width);

        if (right < widget->allocation.width) {
            gboolean selected = (gint) i == priv->selected;

            gdk_draw_rectangle (priv->pixmap,
                                selected ? widget->style->bg_gc[GTK_STATE_SELECTED]
                                         : widget->style->white_gc,
                                TRUE,
                                left, 0,
                                (pad * 2) + char_width,
                                widget->allocation.height);

            gdk_draw_layout (priv->pixmap,
                             selected ? widget->style->white_gc
                                      : widget->style->black_gc,
                             left + pad, pad,
                             layout);

            if ((gint) i == priv->prelighted)
                gtk_paint_shadow (widget->style, priv->pixmap,
                                  GTK_STATE_PRELIGHT, GTK_SHADOW_OUT,
                                  NULL, NULL, NULL,
                                  left, 0,
                                  (pad * 2) + char_width,
                                  widget->allocation.height);

            left = right;
        } else {
            break;
        }
    }

    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      0, 0,
                      0, 0,
                      widget->allocation.width, widget->allocation.height);
}

static void
on_canvas_find (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (user_data);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);
    GtkWidget *widget = GTK_WIDGET (view);
    guint n_candidates = 0;
    unsigned int i = 0;

    if (priv->layout_list) {
        g_list_foreach (priv->layout_list, (GFunc) g_object_unref, NULL);
        g_list_free (priv->layout_list);
        priv->layout_list = NULL;
    }
    priv->selected = -1;
    priv->prelighted = -1;

    if (priv->canvas)
        n_candidates = tomoe_canvas_get_number_of_candidates (priv->canvas);

    for (i = 0; i < n_candidates; i++) {
        PangoLayout *layout;
        gchar *str;

        str = tomoe_canvas_get_nth_candidate (priv->canvas, i);
        if (!str)
            continue;
        layout = gtk_widget_create_pango_layout (widget, str);

        priv->layout_list = g_list_append (priv->layout_list, layout);

        g_free (str);
    }

    tomoe_candidates_view_draw (view);
}

static void
on_canvas_clear (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeCandidatesView *view = TOMOE_CANDIDATES_VIEW (user_data);
    TomoeCandidatesViewPriv *priv = TOMOE_CANDIDATES_VIEW_GET_PRIVATE (view);

    if (priv->layout_list) {
        g_list_foreach (priv->layout_list, (GFunc) g_object_unref, NULL);
        g_list_free (priv->layout_list);
        priv->layout_list = NULL;
    }
    priv->selected = -1;
    priv->prelighted = -1;

    tomoe_candidates_view_draw (view);
}
