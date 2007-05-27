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

#ifndef __TOMOE_EDIT_STROKES_H__
#define __TOMOE_EDIT_STROKES_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_EDIT_STROKES            (tomoe_edit_strokes_get_type ())
#define TOMOE_EDIT_STROKES(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_EDIT_STROKES, TomoeEditStrokes))
#define TOMOE_EDIT_STROKES_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_EDIT_STROKES, TomoeEditStrokesClass))
#define TOMOE_IS_EDIT_STROKES(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_EDIT_STROKES))
#define TOMOE_IS_EDIT_STROKES_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_EDIT_STROKES))
#define TOMOE_EDIT_STROKES_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_EDIT_STROKES, TomoeEditStrokesClass))


typedef struct _TomoeEditStrokesClass TomoeEditStrokesClass;
typedef struct _TomoeEditStrokes      TomoeEditStrokes;

struct _TomoeEditStrokes
{
    GtkDialog    parent_instance;

    GtkWidget    *canvas;
    GtkWidget    *go_back_button;
    GtkWidget    *clear_button;
    GtkWidget    *normalize_button;

    tomoe_char   *character;
};

struct _TomoeEditStrokesClass
{
    GtkDialogClass parent_class;
};


GType       tomoe_edit_strokes_get_type     (void) G_GNUC_CONST;
GtkWidget * tomoe_edit_strokes_new          (tomoe_char *chr);

G_END_DECLS

#endif /* __TOMOE_EDIT_STROKES_H__ */
 
 
