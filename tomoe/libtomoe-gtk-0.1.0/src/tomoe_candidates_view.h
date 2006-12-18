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

#ifndef __TOMOE_CANDIDATES_VIEW_H__
#define __TOMOE_CANDIDATES_VIEW_H__

#include <gtk/gtk.h>
#include "tomoe_canvas.h"

G_BEGIN_DECLS

#define TOMOE_TYPE_CANDIDATES_VIEW            (tomoe_candidates_view_get_type ())
#define TOMOE_CANDIDATES_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOMOE_TYPE_CANDIDATES_VIEW, TomoeCandidatesView))
#define TOMOE_CANDIDATES_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), TOMOE_TYPE_CANDIDATES_VIEW, TomoeCandidatesViewClass))
#define TOMOE_IS_CANDIDATES_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOMOE_TYPE_CANDIDATES_VIEW))
#define TOMOE_IS_CANDIDATES_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TOMOE_TYPE_CANDIDATES_VIEW))
#define TOMOE_CANDIDATES_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), TOMOE_TYPE_CANDIDATES_VIEW, TomoeCandidatesViewClass))


typedef struct _TomoeCandidatesViewClass TomoeCandidatesViewClass;
typedef struct _TomoeCandidatesView      TomoeCandidatesView;

struct _TomoeCandidatesView
{
    GtkDrawingArea   parent_instance;
};

struct _TomoeCandidatesViewClass
{
    GtkDrawingAreaClass parent_class;

    /* -- signals -- */
    void (*selected) (TomoeCandidatesView *view);
};


GType        tomoe_candidates_view_get_type   (void) G_GNUC_CONST;
GtkWidget   *tomoe_candidates_view_new        (void);
void         tomoe_candidates_view_set_canvas (TomoeCandidatesView *view,
                                               TomoeCanvas         *canvas);
gchar       *tomoe_candidates_view_get_selected_letter
                                              (TomoeCandidatesView *view);

G_END_DECLS

#endif /* __TOMOE_CANDIDATES_VIEW_H__ */
