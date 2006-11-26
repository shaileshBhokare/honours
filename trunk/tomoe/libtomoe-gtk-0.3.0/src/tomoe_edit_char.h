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

#ifndef __TOMOE_EDIT_CHAR_H__
#define __TOMOE_EDIT_CHAR_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_EDIT_CHAR            (tomoe_edit_char_get_type ())
#define TOMOE_EDIT_CHAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_EDIT_CHAR, TomoeEditChar))
#define TOMOE_EDIT_CHAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_EDIT_CHAR, TomoeEditCharClass))
#define TOMOE_IS_EDIT_CHAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_EDIT_CHAR))
#define TOMOE_IS_EDIT_CHAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_EDIT_CHAR))
#define TOMOE_EDIT_CHAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_EDIT_CHAR, TomoeEditCharClass))


typedef struct _TomoeEditCharClass TomoeEditCharClass;
typedef struct _TomoeEditChar      TomoeEditChar;

struct _TomoeEditChar
{
    GtkDialog     parent_instance;

    GtkWidget    *ok_button;
    GtkWidget    *input;
    GtkWidget    *label;
    tomoe_char   *character;
    tomoe_dict   *dict;
};

struct _TomoeEditCharClass
{
    GtkDialogClass parent_class;
};


GType       tomoe_edit_char_get_type    (void) G_GNUC_CONST;
GtkWidget * tomoe_edit_char_new         (tomoe_char *chr, tomoe_dict* dict);

G_END_DECLS

#endif /* __TOMOE_EDIT_CHAR_H__ */
 
 
