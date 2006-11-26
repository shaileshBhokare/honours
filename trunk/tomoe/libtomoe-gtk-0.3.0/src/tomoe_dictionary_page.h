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

#ifndef __TOMOE_DICTIONARY_PAGE_H__
#define __TOMOE_DICTIONARY_PAGE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_DICTIONARY_PAGE            (tomoe_dictionary_page_get_type ())
#define TOMOE_DICTIONARY_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_DICTIONARY_PAGE, TomoeDictionaryPage))
#define TOMOE_DICTIONARY_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_DICTIONARY_PAGE, TomoeDictionaryPageClass))
#define TOMOE_IS_DICTIONARY_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_DICTIONARY_PAGE))
#define TOMOE_IS_DICTIONARY_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_DICTIONARY_PAGE))
#define TOMOE_DICTIONARY_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_DICTIONARY_PAGE, TomoeDictionaryPageClass))


typedef struct _TomoeDictionaryPageClass TomoeDictionaryPageClass;
typedef struct _TomoeDictionaryPage      TomoeDictionaryPage;

struct _TomoeDictionaryPage
{
    GtkTable          parent_instance;

    GtkWidget        *char_list;
    GtkListStore     *store;
    GtkTreeSelection *char_sel;
    GtkWidget        *dict_list;
    GtkWidget        *add_button;
    GtkWidget        *delete_button;
    GtkWidget        *details_button;

    tomoe_db         *database;
    tomoe_config     *config;
};

struct _TomoeDictionaryPageClass
{
    GtkTableClass parent_class;
};


GType       tomoe_dictionary_page_get_type     (void) G_GNUC_CONST;
GtkWidget * tomoe_dictionary_page_new          (tomoe_db              *db,
                                                tomoe_config          *config);

G_END_DECLS

#endif /* __TOMOE_DICTIONARY_PAGE_H__ */
 
