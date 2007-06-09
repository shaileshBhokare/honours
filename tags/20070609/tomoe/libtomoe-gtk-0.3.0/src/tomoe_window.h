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

#ifndef __TOMOE_WINDOW_H__
#define __TOMOE_WINDOW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_WINDOW            (tomoe_window_get_type ())
#define TOMOE_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_WINDOW, TomoeWindow))
#define TOMOE_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_WINDOW, TomoeWindowClass))
#define TOMOE_IS_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_WINDOW))
#define TOMOE_IS_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_WINDOW))
#define TOMOE_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_WINDOW, TomoeWindowClass))


typedef struct _TomoeWindowClass TomoeWindowClass;
typedef struct _TomoeWindow      TomoeWindow;

struct _TomoeWindow
{
    GtkDialog     parent_instance;

    GtkWidget    *notebook;
    GtkWidget    *handwriting_page;
    GtkWidget    *dictionaries_page;
    GtkWidget    *reading_search_page;
    GtkWidget    *character_page;
    GtkWidget    *dictionary_page;

    tomoe_db     *database;
    tomoe_config *config;
};

struct _TomoeWindowClass
{
    GtkDialogClass parent_class;
};


GType       tomoe_window_get_type    (void) G_GNUC_CONST;
GtkWidget * tomoe_window_new         (void);
GtkWidget * tomoe_window_get_canvas  (TomoeWindow *window);
void        tomoe_window_append_page (TomoeWindow *window, GtkWidget *page,
                                      GtkWidget *label, const gchar *label_text);

G_END_DECLS

#endif /* __TOMOE_WINDOW_H__ */
