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

#ifndef __TOMOE_READING_SEARCH_PAGE_H__
#define __TOMOE_READING_SEARCH_PAGE_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtktable.h>
#include <tomoe/tomoe.h>

G_BEGIN_DECLS

#define TOMOE_TYPE_READING_SEARCH_PAGE            (tomoe_reading_search_page_get_type ())
#define TOMOE_READING_SEARCH_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_READING_SEARCH_PAGE, TomoeReadingSearchPage))
#define TOMOE_READING_SEARCH_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_READING_SEARCH_PAGE, TomoeReadingSearchPageClass))
#define TOMOE_IS_READING_SEARCH_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_READING_SEARCH_PAGE))
#define TOMOE_IS_READING_SEARCH_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_READING_SEARCH_PAGE))
#define TOMOE_READING_SEARCH_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_READING_SEARCH_PAGE, TomoeReadingSearchPageClass))


typedef struct _TomoeReadingSearchPageClass TomoeReadingSearchPageClass;
typedef struct _TomoeReadingSearchPage      TomoeReadingSearchPage;

struct _TomoeReadingSearchPage
{
    GtkTable          parent_instance;

    GtkWidget        *input;
    GtkWidget        *find_button;
    GtkWidget        *result_list;
    GtkListStore     *result_store;
    GtkTreeSelection *result_sel;

    tomoe_db  *database;
};

struct _TomoeReadingSearchPageClass
{
    GtkTableClass parent_class;
};


GType       tomoe_reading_search_page_get_type     (void) G_GNUC_CONST;
GtkWidget * tomoe_reading_search_page_new          (void);
void        tomoe_reading_search_page_set_database (TomoeReadingSearchPage *page,
                                                    tomoe_db               *database);

G_END_DECLS

#endif /* __TOMOE_READING_SEARCH_PAGE_H__ */
 
