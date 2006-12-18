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

#ifndef __TOMOE_CANVAS_H__
#define __TOMOE_CANVAS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_CANVAS            (tomoe_canvas_get_type ())
#define TOMOE_CANVAS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CANVAS, TomoeCanvas))
#define TOMOE_CANVAS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CANVAS, TomoeCanvasClass))
#define TOMOE_IS_CANVAS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CANVAS))
#define TOMOE_IS_CANVAS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CANVAS))
#define TOMOE_CANVAS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_CANVAS, TomoeCanvasClass))


typedef struct _TomoeCanvasClass TomoeCanvasClass;
typedef struct _TomoeCanvas      TomoeCanvas;

struct _TomoeCanvas
{
    GtkDrawingArea   parent_instance;
};

struct _TomoeCanvasClass
{
    GtkDrawingAreaClass parent_class;

    /* -- signals -- */
    void (*find)            (TomoeCanvas *canvas);
    void (*clear)           (TomoeCanvas *canvas);
    void (*normalize)       (TomoeCanvas *canvas);
    void (*stroke_added)    (TomoeCanvas *canvas);
    void (*stroke_reverted) (TomoeCanvas *canvas);
};


GType         tomoe_canvas_get_type           (void) G_GNUC_CONST;
GtkWidget    *tomoe_canvas_new                (void);

void          tomoe_canvas_find               (TomoeCanvas *canvas);
gchar        *tomoe_canvas_get_nth_candidate  (TomoeCanvas *canvas,
                                               guint        nth);
guint         tomoe_canvas_get_number_of_candidates
                                              (TomoeCanvas *canvas);
void          tomoe_canvas_refresh            (TomoeCanvas *canvas);
void          tomoe_canvas_revert             (TomoeCanvas *canvas);
void          tomoe_canvas_clear              (TomoeCanvas *canvas);
void          tomoe_canvas_normalize          (TomoeCanvas *canvas);
gboolean      tomoe_canvas_has_stroke         (TomoeCanvas *canvas);
void          tomoe_canvas_set_size           (TomoeCanvas *canvas,
                                               guint        size);
void          tomoe_canvas_set_handwrite_line_color
                                              (TomoeCanvas *canvas,
                                               GdkColor    *color);
void          tomoe_canvas_set_adjust_line_color
                                              (TomoeCanvas *canvas,
                                               GdkColor    *color);
void          tomoe_canvas_set_annotate_color (TomoeCanvas *canvas,
                                               GdkColor    *color);
void          tomoe_canvas_set_axis_color     (TomoeCanvas *canvas,
                                               GdkColor    *color);
void          tomoe_canvas_set_auto_find_time (TomoeCanvas *canvas,
                                               gint         time_msec);
gint          tomoe_canvas_get_auto_find_time (TomoeCanvas *canvas);

G_END_DECLS

#endif /* __TOMOE_CANVAS_H__ */
