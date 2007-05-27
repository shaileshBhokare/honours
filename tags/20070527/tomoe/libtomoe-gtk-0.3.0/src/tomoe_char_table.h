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

#ifndef __TOMOE_CHAR_TABLE_H__
#define __TOMOE_CHAR_TABLE_H__

#include <gtk/gtk.h>
#include "tomoe_canvas.h"

G_BEGIN_DECLS

#define TOMOE_TYPE_CHAR_TABLE            (tomoe_char_table_get_type ())
#define TOMOE_CHAR_TABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CHAR_TABLE, TomoeCharTable))
#define TOMOE_CHAR_TABLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CHAR_TABLE, TomoeCharTableClass))
#define TOMOE_IS_CHAR_TABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CHAR_TABLE))
#define TOMOE_IS_CHAR_TABLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CHAR_TABLE))
#define TOMOE_CHAR_TABLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_CHAR_TABLE, TomoeCharTableClass))


typedef struct _TomoeCharTableClass TomoeCharTableClass;
typedef struct _TomoeCharTable      TomoeCharTable;

typedef enum
{
    TOMOE_CHAR_TABLE_LAYOUT_SINGLE_HORIZONTAL,
    TOMOE_CHAR_TABLE_LAYOUT_SINGLE_VERTICAL,
    TOMOE_CHAR_TABLE_LAYOUT_HORIZONTAL,
    TOMOE_CHAR_TABLE_LAYOUT_VERTICAL
} TomoeCharTableLayout;

struct _TomoeCharTable
{
    GtkDrawingArea   parent_instance;
};

struct _TomoeCharTableClass
{
    GtkDrawingAreaClass parent_class;

    /* -- signals -- */
    void (*selected) (TomoeCharTable *view);
};


GType        tomoe_char_table_get_type   (void) G_GNUC_CONST;
GtkWidget   *tomoe_char_table_new        (void);
tomoe_char  *tomoe_char_table_get_selected
                                         (TomoeCharTable *view);
TomoeCharTableLayout
             tomoe_char_table_get_layout (TomoeCharTable *view);
void         tomoe_char_table_set_layout (TomoeCharTable *view,
                                          TomoeCharTableLayout layout);

/* Will be removed */
void         tomoe_char_table_set_canvas (TomoeCharTable *view,
                                          TomoeCanvas         *canvas);

G_END_DECLS

#endif /* __TOMOE_CHAR_TABLE_H__ */
