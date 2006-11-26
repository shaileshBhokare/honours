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

#ifndef __TOMOE_STROKE_SEARCH_H__
#define __TOMOE_STROKE_SEARCH_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_STROKE_SEARCH            (tomoe_stroke_search_get_type ())
#define TOMOE_STROKE_SEARCH(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_STROKE_SEARCH, TomoeStrokeSearch))
#define TOMOE_STROKE_SEARCH_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_STROKE_SEARCH, TomoeStrokeSearchClass))
#define TOMOE_IS_STROKE_SEARCH(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_STROKE_SEARCH))
#define TOMOE_IS_STROKE_SEARCH_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_STROKE_SEARCH))
#define TOMOE_STROKE_SEARCH_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_STROKE_SEARCH, TomoeStrokeSearchClass))


typedef struct _TomoeStrokeSearchClass TomoeStrokeSearchClass;
typedef struct _TomoeStrokeSearch      TomoeStrokeSearch;

struct _TomoeStrokeSearch
{
    GtkTable  parent_instance;

    GtkWidget *canvas;

    GtkWidget *button_area;
    GtkWidget *find_button;
    GtkWidget *go_back_button;
    GtkWidget *clear_button;
    GtkWidget *normalize_button;
#if 0
    GtkWidget *details_button;
#endif
    GtkWidget *candidates_view;
};

struct _TomoeStrokeSearchClass
{
    GtkTableClass parent_class;
};


GType       tomoe_stroke_search_get_type    (void) G_GNUC_CONST;
GtkWidget * tomoe_stroke_search_new         (void);
GtkWidget * tomoe_stroke_search_get_canvas  (TomoeStrokeSearch* strokeSearch);

G_END_DECLS

#endif /* __TOMOE_STROKE_SEARCH_H__ */
