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

#ifndef __TOMOE_DICTIONARIES_PAGE_H__
#define __TOMOE_DICTIONARIES_PAGE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_DICTIONARIES_PAGE            (tomoe_dictionaries_page_get_type ())
#define TOMOE_DICTIONARIES_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICTIONARIES_PAGE, TomoeDictionariesPage))
#define TOMOE_DICTIONARIES_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICTIONARIES_PAGE, TomoeDictionariesPageClass))
#define TOMOE_IS_DICTIONARIES_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICTIONARIES_PAGE))
#define TOMOE_IS_DICTIONARIES_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICTIONARIES_PAGE))
#define TOMOE_DICTIONARIES_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_DICTIONARIES_PAGE, TomoeDictionariesPageClass))


typedef struct _TomoeDictionariesPageClass TomoeDictionariesPageClass;
typedef struct _TomoeDictionariesPage      TomoeDictionariesPage;

struct _TomoeDictionariesPage
{
    GtkTable      parent_instance;

    GtkWidget    *dict_list;
    GtkListStore *store;
    GtkWidget    *remove_button;

    tomoe_db     *database;
    tomoe_config *config;
};

struct _TomoeDictionariesPageClass
{
    GtkTableClass parent_class;
};


GType       tomoe_dictionaries_page_get_type     (void) G_GNUC_CONST;
GtkWidget * tomoe_dictionaries_page_new          (tomoe_db              *db,
                                                  tomoe_config          *config);

G_END_DECLS

#endif /* __TOMOE_DICTIONARIES_PAGE_H__ */
 
