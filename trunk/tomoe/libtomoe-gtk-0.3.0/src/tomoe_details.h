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

#ifndef __TOMOE_DETAILS_H__
#define __TOMOE_DETAILS_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_DETAILS            (tomoe_details_get_type ())
#define TOMOE_DETAILS(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DETAILS, TomoeDetails))
#define TOMOE_DETAILS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DETAILS, TomoeDetailsClass))
#define TOMOE_IS_DETAILS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DETAILS))
#define TOMOE_IS_DETAILS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DETAILS))
#define TOMOE_DETAILS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_DETAILS, TomoeDetailsClass))


typedef struct _TomoeDetailsClass TomoeDetailsClass;
typedef struct _TomoeDetails      TomoeDetails;

struct _TomoeDetails
{
    GtkDialog     parent_instance;

    GtkListStore *basic_prop;
    GtkListStore *sub_prop;
    GtkWidget    *meta_view;
    GtkWidget    *canvas;
    GtkWidget    *edit_char_button;
    GtkWidget    *edit_strokes_button;
    GtkWidget    *edit_meta_button;
    tomoe_char   *character;
};

struct _TomoeDetailsClass
{
    GtkDialogClass parent_class;
};


GType       tomoe_details_get_type    (void) G_GNUC_CONST;
GtkWidget * tomoe_details_new         (tomoe_char *chr);

G_END_DECLS

#endif /* __TOMOE_DETAILS_H__ */
 
