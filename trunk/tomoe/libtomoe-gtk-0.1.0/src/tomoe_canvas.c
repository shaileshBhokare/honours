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
#include <math.h>
#include <tomoe/tomoe.h>

#include "tomoe_canvas.h"

#define TOMOE_CANVAS_DEFAULT_SIZE 300
#define TOMOE_CANVAS_DEFAULT_RATE 0.7

enum {
    FIND_SIGNAL,
    CLEAR_SIGNAL,
    NORMALIZE_SIGNAL,
    STROKE_ADDED_SIGNAL,
    STROKE_REVERTED_SIGNAL,
    LAST_SIGNAL,
};

typedef struct _TomoeCanvasPriv TomoeCanvasPriv;
struct _TomoeCanvasPriv
{
    guint            size;
    GdkGC           *handwrite_line_gc;
    GdkGC           *adjust_line_gc;
    GdkGC           *annotate_gc;
    GdkGC           *axis_gc;
    GdkPixmap       *pixmap;
    GList           *stroke;
    GList           *stroke_list;
    candidate      **candidates;
    unsigned int     candidates_len;
    gint             auto_find_time;
    guint            auto_find_id;
};
#define TOMOE_CANVAS_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), TOMOE_TYPE_CANVAS, TomoeCanvasPriv))

static void   tomoe_canvas_class_init           (TomoeCanvasClass  *klass);
static void   tomoe_canvas_init                 (TomoeCanvas       *object);
static void   tomoe_canvas_dispose              (GObject           *object);
static gint   tomoe_canvas_configure_event      (GtkWidget         *widget,
                                                 GdkEventConfigure *event);
static gint   tomoe_canvas_expose_event         (GtkWidget         *widget,
                                                 GdkEventExpose    *event);
static gint   tomoe_canvas_button_press_event   (GtkWidget         *widget,
                                                 GdkEventButton    *event);
static gint   tomoe_canvas_button_release_event (GtkWidget         *widget,
                                                 GdkEventButton    *event);
static gint   tomoe_canvas_motion_notify_event  (GtkWidget         *widget,
                                                 GdkEventMotion    *event);
static void   tomoe_canvas_real_find            (TomoeCanvas       *canvas);
static void   tomoe_canvas_real_clear           (TomoeCanvas       *canvas);
static void   tomoe_canvas_real_normalize       (TomoeCanvas       *canvas);
static void   tomoe_canvas_append_point         (TomoeCanvas       *canvas,
                                                 gint               x,
                                                 gint               y);
static void   tomoe_canvas_draw_line            (TomoeCanvas       *canvas,
                                                 point             *p1,
                                                 point             *p2,
                                                 gboolean           draw);
static void   tomoe_canvas_draw_background      (TomoeCanvas       *canvas,
                                                 gboolean           draw);
static void   tomoe_canvas_draw_axis            (TomoeCanvas       *canvas);
static void   tomoe_canvas_resize               (TomoeCanvas       *canvas,
                                                 gdouble            x_rate,
                                                 gdouble            y_rate);
static void   tomoe_canvas_position             (TomoeCanvas       *canvas,
                                                 gint               dx,
                                                 gint               dy);
static gint   get_distance                      (GList             *first_node,
                                                 GList             *last_node,
                                                 GList            **most_node);
static GList *get_vertex                        (GList             *first_node,
                                                 GList             *last_node);
static void   draw_stroke                       (GList             *stroke,
                                                 TomoeCanvas       *canvas,
                                                 guint             *index);
static void   draw_annotate                     (GList             *stroke,
                                                 TomoeCanvas       *canvas,
                                                 guint              index);

static GList *instance_list = NULL;
static guint canvas_signals[LAST_SIGNAL] = { 0 };
static GtkDrawingAreaClass *parent_class = NULL;

GType
tomoe_canvas_get_type (void)
{
    static GType type = 0;

    if (!type) {
        static const GTypeInfo info = {
            sizeof (TomoeCanvasClass),
            NULL,           /* base_init */
            NULL,           /* base_finalize */
            (GClassInitFunc) tomoe_canvas_class_init,
            NULL,           /* class_finalize */
            NULL,           /* class_data */
            sizeof (TomoeCanvas),
            0,              /* n_preallocs */
            (GInstanceInitFunc) tomoe_canvas_init,
        };

        type = g_type_register_static (GTK_TYPE_DRAWING_AREA,
                                       "TomoeCanvas",
                                       &info, (GTypeFlags) 0);
    }

    return type;
}

static void
tomoe_canvas_class_init (TomoeCanvasClass *klass)
{
    GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class  = GTK_WIDGET_CLASS (klass);

    parent_class = (GtkDrawingAreaClass *) g_type_class_peek_parent (klass);

    canvas_signals[FIND_SIGNAL] =
      g_signal_new ("find",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCanvasClass, find),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);
    canvas_signals[CLEAR_SIGNAL] =
      g_signal_new ("clear",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCanvasClass, clear),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);
    canvas_signals[NORMALIZE_SIGNAL] =
      g_signal_new ("normalize",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCanvasClass, normalize),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);
    canvas_signals[STROKE_ADDED_SIGNAL] =
      g_signal_new ("stroke-added",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCanvasClass, stroke_added),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);
    canvas_signals[STROKE_REVERTED_SIGNAL] =
      g_signal_new ("stroke-reverted",
  		  G_TYPE_FROM_CLASS (klass),
  		  G_SIGNAL_RUN_LAST,
  		  G_STRUCT_OFFSET (TomoeCanvasClass, stroke_reverted),
  		  NULL, NULL,
  		  g_cclosure_marshal_VOID__VOID,
  		  G_TYPE_NONE, 0);

    gobject_class->dispose             = tomoe_canvas_dispose;
    widget_class->configure_event      = tomoe_canvas_configure_event;
    widget_class->expose_event         = tomoe_canvas_expose_event;
    widget_class->button_press_event   = tomoe_canvas_button_press_event;
    widget_class->button_release_event = tomoe_canvas_button_release_event;
    widget_class->motion_notify_event  = tomoe_canvas_motion_notify_event;

    klass->find                        = tomoe_canvas_real_find;
    klass->clear                       = tomoe_canvas_real_clear;
    klass->normalize                   = tomoe_canvas_real_normalize;
    klass->stroke_added                = NULL;
    klass->stroke_reverted             = NULL;

	g_type_class_add_private (gobject_class, sizeof(TomoeCanvasPriv));
}

static void
tomoe_canvas_init (TomoeCanvas *canvas)
{
    GtkWidget *widget = GTK_WIDGET (canvas);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    PangoFontDescription *font_desc;

    gtk_widget_set_events (widget,
                           GDK_EXPOSURE_MASK |
                           GDK_LEAVE_NOTIFY_MASK |
                           GDK_BUTTON_PRESS_MASK |
                           GDK_BUTTON_RELEASE_MASK |
                           GDK_POINTER_MOTION_MASK |
                           GDK_POINTER_MOTION_HINT_MASK);

    font_desc = pango_font_description_from_string ("Sans 12");
    gtk_widget_modify_font (widget, font_desc);

    priv->size              = TOMOE_CANVAS_DEFAULT_SIZE;
    priv->handwrite_line_gc = NULL;
    priv->adjust_line_gc    = NULL;
    priv->annotate_gc       = NULL;
    priv->axis_gc           = NULL;
    priv->pixmap            = NULL;
    priv->stroke            = NULL;
    priv->stroke_list       = NULL;
    priv->candidates        = NULL;
    priv->candidates_len    = 0;
    priv->auto_find_time    = 0;
    priv->auto_find_id      = 0;

    /* FIXME: customize size */
    tomoe_canvas_set_size (canvas, 300);

    gtk_drawing_area_size (GTK_DRAWING_AREA (canvas), priv->size, priv->size);

    if (!instance_list)
        tomoe_init ();
    instance_list = g_list_append (instance_list, (gpointer) canvas);
}

static void
tomoe_canvas_free_stroke_list (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    if (priv->stroke) {
        g_list_foreach (priv->stroke, (GFunc) g_free, NULL);
        g_list_free (priv->stroke);
        priv->stroke = NULL;
    }

    if (priv->stroke_list) {
        GList *node;
        for (node = priv->stroke_list; node; node = g_list_next (node)) {
            GList *stroke = (GList*) node->data;
            g_list_foreach (stroke, (GFunc) g_free, NULL);
            g_list_free (stroke);
        }
        g_list_free (priv->stroke_list);
        priv->stroke_list = NULL;
    }
}

static void
tomoe_canvas_dispose (GObject *object)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (canvas);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    instance_list = g_list_remove (instance_list, (gpointer) canvas);
    if (!instance_list)
        tomoe_term ();

    if (priv->handwrite_line_gc) {
        gdk_gc_unref (priv->handwrite_line_gc);
        priv->handwrite_line_gc = NULL;
    }

    if (priv->adjust_line_gc) {
        gdk_gc_unref (priv->adjust_line_gc);
        priv->adjust_line_gc = NULL;
    }

    if (priv->annotate_gc) {
        gdk_gc_unref (priv->annotate_gc);
        priv->annotate_gc = NULL;
    }

    if (priv->axis_gc) {
        gdk_gc_unref (priv->axis_gc);
        priv->axis_gc = NULL;
    }

    if (priv->pixmap) {
        g_object_unref (priv->pixmap);
        priv->pixmap = NULL;
    }

    if (priv->candidates) {
        tomoe_free_matched (priv->candidates, priv->candidates_len);
        priv->candidates     = NULL;
        priv->candidates_len = 0;
    }

    if (priv->auto_find_id) {
        gtk_timeout_remove (priv->auto_find_id);
        priv->auto_find_id = 0;
    }

    tomoe_canvas_free_stroke_list (canvas);

    if (G_OBJECT_CLASS(parent_class)->dispose)
        G_OBJECT_CLASS(parent_class)->dispose(object);
}

GtkWidget *
tomoe_canvas_new (void)
{
    return GTK_WIDGET(g_object_new (TOMOE_TYPE_CANVAS,
                                    NULL));
}

static gint
tomoe_canvas_configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (widget);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
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

    tomoe_canvas_real_clear (TOMOE_CANVAS (canvas));

    return retval;
}

static gint
tomoe_canvas_expose_event (GtkWidget *widget, GdkEventExpose *event)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (widget);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
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
tomoe_canvas_button_press_event (GtkWidget *widget, GdkEventButton *event)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (widget);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    gboolean retval = FALSE;

    if (priv->auto_find_id) {
        gtk_timeout_remove (priv->auto_find_id);
        priv->auto_find_id = 0;
    }

    /* tomoe_canvas_refresh (canvas); */

    if (event->button == 1 && priv->pixmap != NULL) {
        tomoe_canvas_append_point (canvas, (gint) event->x, (gint) event->y);
    }

    return retval;
}

static gboolean
timeout_auto_find (gpointer user_data)
{
    tomoe_canvas_find (TOMOE_CANVAS (user_data));
    return FALSE;
}

static gint
tomoe_canvas_button_release_event (GtkWidget *widget, GdkEventButton *event)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (widget);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    gboolean retval = FALSE;

    if (event->button != 1)
        return retval;

    if (!priv->stroke)
        return retval;

    draw_annotate (priv->stroke, canvas, g_list_length (priv->stroke_list) + 1);

    priv->stroke_list = g_list_append(priv->stroke_list,
                                      priv->stroke);
    priv->stroke = NULL;

    g_signal_emit (G_OBJECT (widget), canvas_signals[STROKE_ADDED_SIGNAL], 0);

    if (priv->auto_find_id) {
        gtk_timeout_remove (priv->auto_find_id);
        priv->auto_find_id = 0;
    }
    if (priv->auto_find_time > 0) {
        priv->auto_find_id = gtk_timeout_add (priv->auto_find_time,
                                              timeout_auto_find,
                                              (gpointer)canvas);
    } else if (priv->auto_find_time == 0) {
        tomoe_canvas_find (canvas);
    }

    return retval;
}

static gint
tomoe_canvas_motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
    TomoeCanvas *canvas = TOMOE_CANVAS (widget);
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    int x, y;
    GdkModifierType state;
    gboolean retval = FALSE;

    if (!priv->stroke)
        return retval;

    if (event->is_hint) {
        gdk_window_get_pointer(event->window, &x, &y, &state);
    } else {
        x = (int) event->x;
        y = (int) event->y;
        state = (GdkModifierType) event->state;
    }
    
    if (state & GDK_BUTTON1_MASK && priv->pixmap) {
        tomoe_canvas_append_point (canvas, x, y);
    }

    return retval;
}

static void
tomoe_canvas_real_find (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv;
    GtkWidget *widget = GTK_WIDGET (canvas);
    GdkColor   color;
    GList *line;
    glyph g;
    int i, j;

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g.stroke_num = g_list_length (priv->stroke_list);
    g.strokes    = g_new0 (stroke, g.stroke_num);

    for (i = 0, line = priv->stroke_list;
         line;
         i++, line = line->next)
    {
        GList *dots, *dot;
        gint x, y, px, py;

        dots = g_list_prepend (
            get_vertex((GList*)line->data,
                       (GList*)g_list_last((GList*)line->data)),
            ((GList*)(line->data))->data);
        px = py = -1;

        g.strokes[i].point_num = g_list_length (dots);
        g.strokes[i].points = g_new0 (point, g.strokes[i].point_num);

        for (j = 0, dot = dots; dot; j++, dot = dot->next) {
            x = ((point *)(dot->data))->x;
            y = ((point *)(dot->data))->y;

            g.strokes[i].points[j].x = x
                * ((gdouble)TOMOE_CANVAS_DEFAULT_SIZE / priv->size);
            g.strokes[i].points[j].y = y
                * ((gdouble)TOMOE_CANVAS_DEFAULT_SIZE / priv->size);

            if (px != -1) {
                if (!priv->adjust_line_gc) {
                    /* FIXME: customize color */
                    color.red   = 0x8000;
                    color.green = 0x0000;
                    color.blue  = 0x0000;
                    priv->adjust_line_gc = gdk_gc_new (widget->window);
                    tomoe_canvas_set_adjust_line_color (canvas, &color);
                    gdk_gc_set_line_attributes (priv->adjust_line_gc, 1,
                                                GDK_LINE_SOLID,
                                                GDK_CAP_BUTT,
                                                GDK_JOIN_BEVEL);
                }
                gdk_draw_line(priv->pixmap,
                              priv->adjust_line_gc,
                              px, py, x, y);
            }
            px = x;
            py = y;
        }

        g_list_free (dots);
    }

    if (priv->candidates) {
        tomoe_free_matched (priv->candidates, priv->candidates_len);
        priv->candidates     = NULL;
        priv->candidates_len = 0;
    }
    priv->candidates_len = tomoe_get_matched (&g, &priv->candidates);
    for (i = 0; i < g.stroke_num; i++)
        g_free (g.strokes[i].points);
    g_free (g.strokes);

    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      0, 0,
                      0, 0,
                      widget->allocation.width, widget->allocation.height);
}

static void
tomoe_canvas_real_clear (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv;

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    tomoe_canvas_free_stroke_list (canvas);
    tomoe_canvas_draw_background (canvas, TRUE);

    if (priv->candidates) {
        tomoe_free_matched (priv->candidates, priv->candidates_len);
        priv->candidates     = NULL;
        priv->candidates_len = 0;
    }
}

typedef struct _CharSize CharSize;
struct _CharSize
{
    gint x0;
    gint y0;
    gint x1;
    gint y1;
};

static void
get_char_size (TomoeCanvas *canvas, CharSize *char_size)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GList *node0, *node;

    GList *base_stroke = ((GList*)(priv->stroke_list))->data;
    gint base_x = ((point*)(base_stroke->data))->x;
    gint base_y = ((point*)(base_stroke->data))->y;

    char_size->x0 = base_x;
    char_size->y0 = base_y;
    char_size->x1 = base_x;
    char_size->y1 = base_y;

    for (node0 = priv->stroke_list; node0; node0 = g_list_next (node0)) {
        GList *stroke = (GList*) node0->data;

        for (node = stroke; node; node = g_list_next (node)) {
            gint x = ((point*)(node->data))->x;
            gint y = ((point*)(node->data))->y;

            char_size->x0 = MIN (char_size->x0, x);
            char_size->y0 = MIN (char_size->y0, y);
            char_size->x1 = MAX (char_size->x1, x);
            char_size->y1 = MAX (char_size->y1, y);
        }
    }
}

static void
tomoe_canvas_resize (TomoeCanvas *canvas, gdouble x_rate, gdouble y_rate)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GList *node0;

    for (node0 = priv->stroke_list; node0; node0 = g_list_next (node0)) {
        GList *stroke = (GList*) node0->data;

        GList *node1;
        for (node1 = stroke; node1; node1 = g_list_next (node1)) {
            ((point*)(node1->data))->x *= x_rate;
            ((point*)(node1->data))->y *= y_rate;
        }
    }
}

static void
tomoe_canvas_position (TomoeCanvas *canvas, gint dx, gint dy)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GList *node0;

    for (node0 = priv->stroke_list; node0; node0 = g_list_next (node0)) {
        GList *stroke = (GList*) node0->data;

        GList *node1;
        for (node1 = stroke; node1; node1 = g_list_next (node1)) {
            ((point*)(node1->data))->x += dx;
            ((point*)(node1->data))->y += dy;
        }
    }
}

static void
tomoe_canvas_real_normalize (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    CharSize char_size;
    gdouble x_rate, y_rate;
    gint dx, dy;

    get_char_size (canvas, &char_size);
    x_rate = (priv->size * TOMOE_CANVAS_DEFAULT_RATE)
           / (char_size.x1 - char_size.x0);
    y_rate = (priv->size * TOMOE_CANVAS_DEFAULT_RATE)
           / (char_size.y1 - char_size.y0);

    tomoe_canvas_resize (canvas, x_rate, y_rate);

    get_char_size (canvas, &char_size);
    dx = ((priv->size - (char_size.x1 - char_size.x0)) / 2) - char_size.x0;
    dy = ((priv->size - (char_size.y1 - char_size.y0)) / 2) - char_size.y0;

    tomoe_canvas_position (canvas, dx, dy);

    tomoe_canvas_refresh (canvas);
    tomoe_canvas_find (canvas);
}

void
tomoe_canvas_find (TomoeCanvas *canvas)
{
    g_return_if_fail (TOMOE_IS_CANVAS (canvas));
    g_signal_emit (G_OBJECT (canvas), canvas_signals[FIND_SIGNAL], 0);
}

guint
tomoe_canvas_get_number_of_candidates (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv;

    g_return_val_if_fail (TOMOE_IS_CANVAS (canvas), 0);

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    return priv->candidates_len;
}

gchar *
tomoe_canvas_get_nth_candidate (TomoeCanvas *canvas, guint nth)
{
    TomoeCanvasPriv *priv;
    gchar *utf8_str;

    g_return_val_if_fail (TOMOE_IS_CANVAS (canvas), NULL);

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    if (nth < priv->candidates_len) {
        gsize bytes_read, bytes_written;
        utf8_str = g_convert(priv->candidates[nth]->letter, -1,
                             "UTF-8", "EUC-JP",
                             &bytes_read, &bytes_written,
                             NULL);
        return utf8_str;
    } else {
        return g_strdup ("");
    }
}

void
tomoe_canvas_refresh (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv;
    GtkWidget *widget;
    guint index = 1;
    GList *node;

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    widget = GTK_WIDGET (canvas);

    gdk_draw_rectangle (priv->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    tomoe_canvas_draw_axis (canvas);

    for (node = priv->stroke_list; node; node = g_list_next (node)) {
        draw_stroke (node->data, canvas, &index);
    }
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                      priv->pixmap,
                      0, 0,
                      0, 0,
                      widget->allocation.width, widget->allocation.height);
}

void
tomoe_canvas_revert (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv;
    GList *last;

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    /* remove last line */
    last = g_list_last (priv->stroke_list);
    if (last) {
        GList *stroke = (GList*) last->data;

        priv->stroke_list = g_list_remove (priv->stroke_list, stroke);
        g_list_foreach (stroke, (GFunc) g_free, NULL);
        g_list_free (stroke);

        tomoe_canvas_refresh (canvas);

        g_signal_emit (G_OBJECT (canvas),
                       canvas_signals[STROKE_REVERTED_SIGNAL], 0);

        if (!priv->stroke_list)
            g_signal_emit (G_OBJECT (canvas), canvas_signals[CLEAR_SIGNAL], 0);
    }
}

void
tomoe_canvas_clear (TomoeCanvas *canvas)
{
    g_return_if_fail (TOMOE_IS_CANVAS (canvas));
    g_signal_emit (G_OBJECT (canvas), canvas_signals[CLEAR_SIGNAL], 0);
}

void
tomoe_canvas_normalize (TomoeCanvas *canvas)
{
    g_return_if_fail (TOMOE_IS_CANVAS (canvas));
    g_signal_emit (G_OBJECT (canvas), canvas_signals[NORMALIZE_SIGNAL], 0);
}

gboolean
tomoe_canvas_has_stroke (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_val_if_fail (TOMOE_IS_CANVAS (canvas), FALSE);

    if (priv->stroke || priv->stroke_list)
        return TRUE;

    return FALSE;
}

void
tomoe_canvas_set_size (TomoeCanvas *canvas, guint size)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    priv->size = size;
}

void
tomoe_canvas_gc_set_foreground (GdkGC *gc, GdkColor *color)
{
    GdkColor default_color;
    default_color.red   = 0x0000;
    default_color.green = 0x0000;
    default_color.blue  = 0x0000;

    if (color) {
        gdk_colormap_alloc_color (gdk_colormap_get_system (), color,
                                  TRUE, TRUE);
        gdk_gc_set_foreground (gc, color);
    } else {
        gdk_colormap_alloc_color (gdk_colormap_get_system (), &default_color,
                                  TRUE, TRUE);
        gdk_gc_set_foreground (gc, &default_color);
    }
}

void
tomoe_canvas_set_handwrite_line_color (TomoeCanvas *canvas, GdkColor *color)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    tomoe_canvas_gc_set_foreground (priv->handwrite_line_gc, color);
}

void
tomoe_canvas_set_adjust_line_color (TomoeCanvas *canvas, GdkColor *color)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    tomoe_canvas_gc_set_foreground (priv->adjust_line_gc, color);
}

void
tomoe_canvas_set_annotate_color (TomoeCanvas *canvas, GdkColor *color)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    tomoe_canvas_gc_set_foreground (priv->annotate_gc, color);
}

void
tomoe_canvas_set_axis_color (TomoeCanvas *canvas, GdkColor *color)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    tomoe_canvas_gc_set_foreground (priv->axis_gc, color);
}

void
tomoe_canvas_set_auto_find_time (TomoeCanvas *canvas, gint time_msec)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    if (time_msec < 0)
        priv->auto_find_time = -1;
    else
        priv->auto_find_time = time_msec;
}

gint
tomoe_canvas_get_auto_find_time (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    g_return_val_if_fail (TOMOE_IS_CANVAS (canvas), -1);

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);

    return priv->auto_find_time;
}

static void
tomoe_canvas_append_point (TomoeCanvas *canvas, gint x, gint y)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GList *last;
    point *p1, *p2 = g_new0 (point, 1);

    p2->x = x;
    p2->y = y;

    last = g_list_last (priv->stroke);
    priv->stroke = g_list_append (priv->stroke, p2);
    p1 = last ? (point*) last->data : p2;

    tomoe_canvas_draw_line (canvas, p1, p2, TRUE);
}

static void
tomoe_canvas_draw_line (TomoeCanvas *canvas, point *p1, point *p2,
                        gboolean draw)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GtkWidget *widget = GTK_WIDGET (canvas);
    GdkColor   color;
    GdkRectangle update_rect;

    /* FIXME! move to realize method */
    if (!priv->handwrite_line_gc) {
        /* FIXME: customize color */
        color.red   = 0x0000;
        color.green = 0x0000;
        color.blue  = 0x0000;
        priv->handwrite_line_gc = gdk_gc_new (widget->window);
        tomoe_canvas_set_handwrite_line_color (canvas, &color);
        gdk_gc_set_line_attributes (priv->handwrite_line_gc, 4,
                                    GDK_LINE_SOLID,
                                    GDK_CAP_ROUND,
                                    GDK_JOIN_ROUND);
    }

    update_rect.x = MIN (p1->x, p2->x) - 2;
    update_rect.y = MIN (p1->y, p2->y) - 2;
    update_rect.width  = abs (p1->x - p2->x) + (2 * 2);
    update_rect.height = abs (p1->y - p2->y) + (2 * 2);

    gdk_draw_line (priv->pixmap, priv->handwrite_line_gc,
                   p1->x, p1->y, p2->x, p2->y);
    if (draw) {
        gtk_widget_draw (widget, &update_rect);
    }
}

static void
tomoe_canvas_draw_background (TomoeCanvas *canvas, gboolean draw)
{
    TomoeCanvasPriv *priv;
    GtkWidget *widget;

    g_return_if_fail (TOMOE_IS_CANVAS (canvas));

    priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    widget = GTK_WIDGET (canvas);

    gdk_draw_rectangle (priv->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    tomoe_canvas_draw_axis (canvas);

    if (draw) {
        gdk_draw_drawable(widget->window,
                          widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                          priv->pixmap,
                          0, 0,
                          0, 0,
                          widget->allocation.width, widget->allocation.height);
    }
}

static void
tomoe_canvas_draw_axis (TomoeCanvas *canvas)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GtkWidget *widget = GTK_WIDGET (canvas);
    GdkColor color;

    if (!priv->axis_gc) {
        /* FIXME: customize color */
        color.red   = 0x8000;
        color.green = 0x8000;
        color.blue  = 0x8000;
        priv->axis_gc = gdk_gc_new (widget->window);
        tomoe_canvas_set_axis_color (canvas, &color);
        gdk_gc_set_line_attributes (priv->axis_gc, 1,
                                    GDK_LINE_ON_OFF_DASH,
                                    GDK_CAP_BUTT,
                                    GDK_JOIN_ROUND);
    }

    gdk_draw_line (priv->pixmap, priv->axis_gc,
                   (priv->size / 2), 0,
                   (priv->size / 2), priv->size);
    gdk_draw_line (priv->pixmap, priv->axis_gc,
                   0,          (priv->size / 2),
                   priv->size, (priv->size / 2));
}

static gint
get_distance (GList *first_node, GList *last_node, GList **most_node)
{
    /* Getting distance 
     * MAX( |aw - bv + c| )
     * a = x-p : b = y-q : c = py - qx
     * first = (p, q) : last = (x, y) : other = (v, w)
     */

    GList *dot;
    gint a, b, c;
    gint dist = 0;
    gint max  = 0;
    gint denom;
    point *first = (point*) first_node->data;
    point *last  = (point*) last_node->data;
    point *p;

    *most_node = NULL;
    if (first_node == last_node)
    {
        return 0;
    }

    a = last->x - first->x;
    b = last->y - first->y;
    c = last->y * first->x - last->x * first->y;

    for (dot = first_node; dot != last_node; dot = dot->next)
    {
        p = (point*) dot->data;
        dist = abs((a * p->y) - (b * p->x) + c);
        if (dist > max)
        {
            max = dist;
            *most_node = dot;
        }
    }

    denom = a * a + b * b;

    if (denom == 0)
        return 0;
    else
        return max * max / denom;
}

static GList*
get_vertex (GList *first_node, GList *last_node)
{
    GList *rv = NULL;
    GList *most_node;
    gint dist;
    gint error = 300 * 300 / 400; /* 5% */

    dist = get_distance(first_node, last_node, &most_node);

    if (dist > error)
    {
        rv = g_list_concat(get_vertex(first_node, most_node),
                           get_vertex(most_node, last_node));
    }
    else
    {
        rv = g_list_append(rv, last_node->data);
    }
    return rv;
}

static void
draw_stroke (GList *stroke, TomoeCanvas *canvas, guint *index)
{
    GList *node;
    
    for (node = stroke; node; node = g_list_next (node)) {
        GList *next = g_list_next (node);
        if (!next) break;

        point *p1 = (point*) node->data, *p2 = (point*) next->data;
        tomoe_canvas_draw_line (canvas, p1, p2, FALSE);
    }
    draw_annotate (stroke, canvas, *index);
    (*index)++;
}

static void
draw_annotate (GList *stroke, TomoeCanvas *canvas, guint index)
{
    TomoeCanvasPriv *priv = TOMOE_CANVAS_GET_PRIVATE (canvas);
    GtkWidget *widget = GTK_WIDGET (canvas);
    GdkColor   color;

    gchar *buffer; 
    PangoLayout *layout;
    gint width, height;
    gint x, y;
    gdouble r;
    gdouble dx,dy;
    gdouble dl;
    gint sign;

    x = ((point*)((g_list_first (stroke))->data))->x;
    y = ((point*)((g_list_first (stroke))->data))->y;

    if (g_list_length (stroke) == 1) {
        dx = x;
        dy = y;
    } else {
        dx = ((point*)((g_list_last (stroke))->data))->x - x;
        dy = ((point*)((g_list_last (stroke))->data))->y - y;
    }
    dl = sqrt (dx*dx + dy*dy);
    sign = (dy <= dx) ? 1 : -1;

    buffer = g_strdup_printf ("%d", index);
    layout = gtk_widget_create_pango_layout (widget, buffer);
    pango_layout_get_pixel_size (layout, &width, &height);

    r = sqrt (width*width + height*height);

    x += 0.5 + (0.5 * r * dx / dl) + (sign * 0.5 * r * dy / dl) - (width / 2);
    y += 0.5 + (0.5 * r * dy / dl) - (sign * 0.5 * r * dx / dl) - (height / 2);

    if (!priv->annotate_gc) {
        /* FIXME: customize color */
        color.red   = 0x8000;
        color.green = 0x0000;
        color.blue  = 0x0000;
        priv->annotate_gc = gdk_gc_new (widget->window);
        tomoe_canvas_set_annotate_color (canvas, &color);
    }

    gdk_draw_layout (priv->pixmap,
                     priv->annotate_gc,
                     x, y, layout);

    g_free (buffer);
    g_object_unref (layout);
}
