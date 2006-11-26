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
#  include <config.h>
#endif

#include <stdlib.h>

#include "tomoe_char_table.h"
#include "tomoe_canvas.h"

enum {
    SELECTED_SIGNAL,
    LAST_SIGNAL,
};

typedef struct _TomoeCharTablePriv TomoeCharTablePriv;
struct _TomoeCharTablePriv
{
    TomoeCharTableLayout layout;

    TomoeCanvas *canvas;

    GdkPixmap   *pixmap;
    guint        padding;
    gint         selected;
    gint         prelighted;
    GList       *layout_list;
};
#define TOMOE_CHAR_TABLE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CHAR_TABLE, TomoeCharTablePriv))

static void   tomoe_char_table_class_init           (TomoeCharTableClass *klass);
static void   tomoe_char_table_init                 (TomoeCharTable      *object);
static void   tomoe_char_table_dispose              (GObject           *object);
static void   tomoe_char_table_size_request         (GtkWidget         *widget,
                                                     GtkRequisition    *requisition);
static gint   tomoe_char_table_configure_event      (GtkWidget         *widget,
                                                     GdkEventConfigure *event);
static gint   tomoe_char_table_expose_event         (GtkWidget         *widget,
                                                     GdkEventExpose    *event);
static gint   tomoe_char_table_button_press_event   (GtkWidget         *widget,
                                                     GdkEventButton    *event);
static gint   tomoe_char_table_button_release_event (GtkWidget         *widget,
                                                     GdkEventButton    *event);
static gint   tomoe_char_table_motion_notify_event  (GtkWidget         *widget,
                                                     GdkEventMotion    *event);
static gint   tomoe_char_table_leave_notify_event   (GtkWidget         *widget,
                                                     GdkEventCrossing  *event);
static void   tomoe_char_table_draw                 (TomoeCharTable *view);

/* Callback functions for related tomoe canvas */
static void   on_canvas_find  (TomoeCanvas *canvas,
                               gpointer     user_data);
static void   on_canvas_clear (TomoeCanvas *canvas,
                               gpointer     user_data);

static guint view_signals[LAST_SIGNAL] = { 0 };
static GtkDrawingAreaClass *parent_class = NULL;

GType
tomoe_char_table_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeCharTableClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_char_table_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeCharTable),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_char_table_init,
        };

        type = g_type_register_static (GTK_TYPE_DRAWING_AREA,
                                       "TomoeCharTable",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_char_table_class_init (TomoeCharTableClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class     = GTK_WIDGET_CLASS (klass);

    parent_class = (GtkDrawingAreaClass *) g_type_class_peek_parent (klass);

    view_signals[SELECTED_SIGNAL] =
      g_signal_new ("selected",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCharTableClass, selected),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);

    gobject_class->dispose             = tomoe_char_table_dispose;
    widget_class->size_request         = tomoe_char_table_size_request;
    widget_class->configure_event      = tomoe_char_table_configure_event;
    widget_class->expose_event         = tomoe_char_table_expose_event;
    widget_class->button_press_event   = tomoe_char_table_button_press_event;
    widget_class->button_release_event = tomoe_char_table_button_release_event;
    widget_class->motion_notify_event  = tomoe_char_table_motion_notify_event;
    widget_class->leave_notify_event   = tomoe_char_table_leave_notify_event;

    klass->selected                    = NULL;

	g_type_class_add_private (gobject_class, sizeof(TomoeCharTablePriv));
}

static void
tomoe_char_table_init (TomoeCharTable *view)
{
    GtkWidget *widget = GTK_WIDGET (view);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    PangoFontDescription *font_desc;
    gint size;

    gtk_widget_set_events (widget,
                           GDK_EXPOSURE_MASK |
                           GDK_ENTER_NOTIFY_MASK |
                           GDK_LEAVE_NOTIFY_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK |
                           GDK_POINTER_MOTION_HINT_MASK);

    /* Set default font with large scale */
    font_desc = pango_font_description_copy (widget->style->font_desc);
    size = pango_font_description_get_size(font_desc);
    pango_font_description_set_size(font_desc, size * PANGO_SCALE_XX_LARGE);
    gtk_widget_modify_font (widget, font_desc);
    pango_font_description_free (font_desc);

    priv->layout      = TOMOE_CHAR_TABLE_LAYOUT_SINGLE_HORIZONTAL;
    priv->canvas      = NULL;
    priv->pixmap      = NULL;
    priv->padding     = 2;
    priv->selected    = -1;
    priv->prelighted  = -1;
    priv->layout_list = NULL;
}

static void
tomoe_char_table_dispose (GObject *object)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (object);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    tomoe_char_table_set_canvas (view, NULL);

    if (priv->pixmap) {
        g_object_unref(G_OBJECT (priv->pixmap));
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

static void
tomoe_char_table_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    PangoFontMetrics *metrics;
    PangoContext *context;
    gint ascent, descent, char_width, digit_width, char_pixels;
  
    gtk_widget_ensure_style (widget);
    context = gtk_widget_get_pango_context (widget);
    metrics = pango_context_get_metrics (context,
                                         widget->style->font_desc,
                                         pango_context_get_language (context));

    /* width */
    char_width = pango_font_metrics_get_approximate_char_width (metrics);
    digit_width = pango_font_metrics_get_approximate_digit_width (metrics);
    char_pixels = PANGO_PIXELS (MAX (char_width, digit_width) * PANGO_SCALE_XX_LARGE);
    requisition->width = char_pixels + priv->padding * 2;

    /* height */
    ascent  = pango_font_metrics_get_ascent (metrics);
    descent = pango_font_metrics_get_descent (metrics);
    requisition->height = PANGO_PIXELS (ascent + descent) + priv->padding * 2;

    pango_font_metrics_unref (metrics);
}

GtkWidget *
tomoe_char_table_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_CHAR_TABLE,
                                    NULL));
}

void
tomoe_char_table_set_canvas (TomoeCharTable *view, TomoeCanvas *canvas)
{
    TomoeCharTablePriv *priv;

    g_return_if_fail (TOMOE_IS_CHAR_TABLE (view));

    priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    if (priv->canvas) {
        g_signal_handlers_disconnect_by_func (
            G_OBJECT (priv->canvas),
            (gpointer) on_canvas_find,
            (gpointer) view);
        g_object_remove_weak_pointer (G_OBJECT (canvas),
                                      (gpointer*) &priv->canvas);
    }

    priv->canvas = canvas;

    if (canvas)
    {
        g_object_add_weak_pointer (G_OBJECT (canvas),
                                   (gpointer*) &priv->canvas);

        g_signal_connect_after (G_OBJECT (canvas), "find",
                                G_CALLBACK (on_canvas_find), (gpointer) view);
        g_signal_connect_after (G_OBJECT (canvas), "clear",
                                G_CALLBACK (on_canvas_clear), (gpointer) view);
    }
}

tomoe_char *
tomoe_char_table_get_selected (TomoeCharTable *view)
{
    GtkWidget *widget;
    TomoeCharTablePriv *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR_TABLE (view), NULL);

    widget = GTK_WIDGET (view);
    priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    if (!priv->canvas)
        return NULL;

    if (priv->selected < 0)
        return NULL;

    return tomoe_canvas_get_nth_candidate (priv->canvas, priv->selected);
}

TomoeCharTableLayout
tomoe_char_table_get_layout (TomoeCharTable *view)
{
    TomoeCharTablePriv *priv;

    g_return_val_if_fail (TOMOE_IS_CHAR_TABLE (view), 0);

    priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    return priv->layout;
}

void
tomoe_char_table_set_layout (TomoeCharTable *view, TomoeCharTableLayout layout)
{
    TomoeCharTablePriv *priv;

    g_return_if_fail (TOMOE_IS_CHAR_TABLE (view));

    priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    /*
     * FIXME! check the value!
     * TomoeCharTableLayout should be implemented as GObject's property.
     */
    priv->layout = layout;
}

static gint
tomoe_char_table_configure_event (GtkWidget *widget,
                                  GdkEventConfigure *event)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
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
tomoe_char_table_expose_event (GtkWidget *widget, GdkEventExpose *event)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
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
char_id_from_coordinate (TomoeCharTable *view, gint x, gint y)
{
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    PangoContext *context;
    PangoFontMetrics *metrics;
    gint char_width, digit_width, ascent, descent;
    gint max_char_width, max_char_height;
    gint columns, rows;
    gint i, begin = 0, end = 0;
    GList *node;

    context = gtk_widget_get_pango_context (GTK_WIDGET (view));
    metrics = pango_context_get_metrics (context,
                                         GTK_WIDGET (view)->style->font_desc,
                                         pango_context_get_language (context));

    /* Get max char size */
    char_width = pango_font_metrics_get_approximate_char_width (metrics);
    digit_width = pango_font_metrics_get_approximate_digit_width (metrics);
    max_char_width = PANGO_PIXELS (MAX (char_width, digit_width) * PANGO_SCALE_XX_LARGE);

    ascent  = pango_font_metrics_get_ascent (metrics);
    descent = pango_font_metrics_get_descent (metrics);
    max_char_height = PANGO_PIXELS (ascent + descent);

    /* Calculate columns for horizontal layout */
    columns = ((GTK_WIDGET (view)->allocation.width) - priv->padding)
        / (max_char_width + priv->padding);
    columns = columns <= 0 ? 1 : columns;

    /* Calculate rows for vertical layout */
    rows = ((GTK_WIDGET (view)->allocation.height) - priv->padding)
        / (max_char_height + priv->padding);
    rows = rows <= 0 ? 1 : rows;

    for (i = 0, node = priv->layout_list;
         node;
         i++, node = g_list_next (node))
    {
        PangoLayout *layout = PANGO_LAYOUT (node->data);
        gint char_width = 0, char_height = 0, pad = priv->padding;

        pango_layout_get_pixel_size (layout, &char_width, &char_height);

        if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_SINGLE_HORIZONTAL) {
            end = end + ((pad * 2) + char_width);
            if (x >= begin && x < end)
                return i;

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_SINGLE_VERTICAL) {
            end = end + ((pad * 2) + char_height);
            if (y >= begin && y < end)
                return i;

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_HORIZONTAL) {
            gint area_x, area_y;

            area_x = (max_char_width + priv->padding)  * (i % columns) + priv->padding;
            area_y = (max_char_height + priv->padding) * (i / columns) + priv->padding;

            if (x >= area_x && x < area_x + max_char_width  + (gint) priv->padding &&
                y >= area_y && y < area_y + max_char_height + (gint) priv->padding)
            {
                return i;
            }

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_VERTICAL) {
            gint area_x, area_y;

            area_x = (max_char_width + priv->padding)  * (i / rows) + priv->padding;
            area_y = (max_char_height + priv->padding) * (i % rows) + priv->padding;

            if (x >= area_x && x < (gint) area_x + max_char_width  + (gint) priv->padding &&
                y >= area_y && y < (gint) area_y + max_char_height + (gint) priv->padding)
            {
                return i;
            }

        } else {
            /* Shoudn't reach */;
        }

        begin = end;
    }

    return -1;
}

static gint
tomoe_char_table_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    gint prev_selected;

    prev_selected = priv->selected;
    priv->selected = char_id_from_coordinate (view,
                                              (gint) event->x,
                                              (gint) event->y);

    if (prev_selected != priv->selected)
        tomoe_char_table_draw (view);
    if (priv->selected >= 0)
        g_signal_emit (G_OBJECT (widget),
                       view_signals[SELECTED_SIGNAL], 0);

    return FALSE;
}

static gint
tomoe_char_table_button_release_event (GtkWidget *widget, GdkEventButton *event)
{
    return FALSE;
}

static gint
tomoe_char_table_motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
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
    priv->prelighted = char_id_from_coordinate (view, x, y);

    if (prev_prelighted != priv->prelighted) {
        tomoe_char_table_draw (view);
    }

    return FALSE;
}

static gint
tomoe_char_table_leave_notify_event (GtkWidget *widget, GdkEventCrossing *event)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (widget);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    gint prev_prelighted;

    prev_prelighted = priv->prelighted;
    priv->prelighted = -1;

    if (prev_prelighted != priv->prelighted) {
        tomoe_char_table_draw (view);
    }

    return FALSE;
}

static void
tomoe_char_table_draw (TomoeCharTable *view)
{
    GtkWidget *widget = GTK_WIDGET (view);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
    PangoContext *context;
    PangoFontMetrics *metrics;
    gint char_width, digit_width, ascent, descent, columns, rows;
    gint max_char_width, max_char_height, x_pos, y_pos;
    guint i;
    gint begin_pos = 0, end_pos = 0;
    GList *node;

    context = gtk_widget_get_pango_context (widget);
    metrics = pango_context_get_metrics (context,
                                         widget->style->font_desc,
                                         pango_context_get_language (context));

    /* Get max char size */
    char_width = pango_font_metrics_get_approximate_char_width (metrics);
    digit_width = pango_font_metrics_get_approximate_digit_width (metrics);
    max_char_width = PANGO_PIXELS (MAX (char_width, digit_width) * PANGO_SCALE_XX_LARGE);

    ascent  = pango_font_metrics_get_ascent (metrics);
    descent = pango_font_metrics_get_descent (metrics);
    max_char_height = PANGO_PIXELS (ascent + descent);

    /* Calculate y position of characters for single horizontal layout */
    y_pos = (GTK_WIDGET (view)->allocation.height - max_char_height) / 2;

    /* Calculate x position of characters for single vertical layout */
    x_pos = (GTK_WIDGET (view)->allocation.width - max_char_width) / 2;

    /* Calculate columns for horizontal layout */
    columns = ((GTK_WIDGET (view)->allocation.width) - priv->padding)
        / (max_char_width + priv->padding);
    columns = columns <= 0 ? 1 : columns;

    /* Calculate rows for vertical layout */
    rows = ((GTK_WIDGET (view)->allocation.height) - priv->padding)
        / (max_char_height + priv->padding);
    rows = rows <= 0 ? 1 : rows;

    pango_font_metrics_unref (metrics);

    /* Fill background */
    gdk_draw_rectangle (priv->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    /* Draw characters */
    for (i = 0, node = priv->layout_list;
         node;
         i++, node = g_list_next (node))
    {
        PangoLayout *layout = PANGO_LAYOUT (node->data);
        gint char_width = 20, char_height = 20;
        gint outer_x, outer_y, outer_width, outer_height;
        gint inner_x, inner_y;
        gboolean selected = (gint) i == priv->selected;

        pango_layout_get_pixel_size (layout, &char_width, &char_height);

        if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_SINGLE_HORIZONTAL) {
            outer_x = begin_pos;
            outer_y = 0;
            outer_width  = (priv->padding * 2) + char_width;
            outer_height = widget->allocation.height;
            inner_x = outer_x + priv->padding;
            inner_y = y_pos;
            end_pos = outer_x + outer_width;

            if (end_pos > widget->allocation.width) {
                // FIXME! shoule be scrollable
                break;
            }

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_SINGLE_VERTICAL) {
            outer_x      = 0;
            outer_y      = begin_pos;
            outer_width  = widget->allocation.width;
            outer_height = (priv->padding * 2) + char_height;
            inner_x = x_pos;
            inner_y = outer_y + priv->padding;
            end_pos = outer_y + outer_height;

            if (end_pos > widget->allocation.height) {
                // FIXME! shoule be scrollable
                break;
            }

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_HORIZONTAL) {
            outer_x      = (max_char_width + priv->padding) * (i % columns) + priv->padding;
            outer_y      = (max_char_height + priv->padding) * (i / columns) + priv->padding;
            outer_width  = max_char_width;
            outer_height = max_char_height;
            inner_x      = outer_x + (max_char_width  - char_width)  / 2;
            inner_y      = outer_y + (max_char_height - char_height) / 2;

            if (outer_y + outer_height > widget->allocation.height) {
                // FIXME! should be scrollable
                break;
            }

        } else if (priv->layout == TOMOE_CHAR_TABLE_LAYOUT_VERTICAL) {
            /* Not implemented yet */
            outer_x      = (max_char_width  + priv->padding) * (i / rows) + priv->padding;
            outer_y      = (max_char_height + priv->padding) * (i % rows) + priv->padding;
            outer_width  = max_char_width;
            outer_height = max_char_height;
            inner_x      = outer_x + (max_char_width  - char_width) / 2;
            inner_y      = outer_y + (max_char_height - char_height) / 2;

            if (outer_x + outer_width > widget->allocation.width) {
                // FIXME! should be scrollable
                break;
            }

        } else {
            /* Should't reach */
            break;
        }

        gdk_draw_rectangle (priv->pixmap,
                            selected ? widget->style->bg_gc[GTK_STATE_SELECTED]
                                     : widget->style->white_gc,
                            TRUE,
                            outer_x, outer_y,
                            outer_width, outer_height);

        gdk_draw_layout (priv->pixmap,
                         selected ? widget->style->white_gc
                                  : widget->style->black_gc,
                         inner_x, inner_y,
                         layout);

        if ((gint) i == priv->prelighted)
            gtk_paint_shadow (widget->style, priv->pixmap,
                              GTK_STATE_PRELIGHT, GTK_SHADOW_OUT,
                              NULL, NULL, NULL,
                              outer_x, outer_y,
                              outer_width, outer_height);

        begin_pos = end_pos;
    }

    /* Draw off-screen pixmap to foreground */
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      0, 0,
                      0, 0,
                      widget->allocation.width, widget->allocation.height);
}


/*
 *  Callback functions for related TomoeCanvas.
 */
static void
on_canvas_find (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (user_data);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);
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
        tomoe_char* chr;

        chr = tomoe_canvas_get_nth_candidate (priv->canvas, i);
        if (!chr)
            continue;
        layout = gtk_widget_create_pango_layout (widget, tomoe_char_getCode (chr));

        priv->layout_list = g_list_append (priv->layout_list, layout);
    }

    tomoe_char_table_draw (view);
}

static void
on_canvas_clear (TomoeCanvas *canvas, gpointer user_data)
{
    TomoeCharTable *view = TOMOE_CHAR_TABLE (user_data);
    TomoeCharTablePriv *priv = TOMOE_CHAR_TABLE_GET_PRIVATE (view);

    if (priv->layout_list) {
        g_list_foreach (priv->layout_list, (GFunc) g_object_unref, NULL);
        g_list_free (priv->layout_list);
        priv->layout_list = NULL;
    }
    priv->selected = -1;
    priv->prelighted = -1;

    tomoe_char_table_draw (view);
}
