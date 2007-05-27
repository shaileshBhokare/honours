/*review version 0.4
 * 1. added in Math library
 * 2. added in quantilisation.
*/

/*review version 0.3
1. add in smooth feature
2. reduce the traning data amount
*/

/*review version 0.2
1. add in training function
2. add in save function
*/

/*review version 0.1
1. fix line break problem
2. remove starting point drawing
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <math.h>

#define True 1
#define False 0
/* Backing pixmap for drawing area */
static GdkPixmap *pixmap = NULL;
static int xi, xii, yi, yii ;
static int xtemp ; //this is a intermedia temp point, used for smooth feature.
static int ytemp ; 
static int training ;
static char *file = "./data/data.txt";
static char *fullFile = ".data/fullData/data.txt";
static FILE *fp;
static FILE *ffp;
static int firstStroke = True;

/* Create a new backing pixmap of the appropriate size */
static gint
configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
  if (pixmap)
    gdk_pixmap_unref(pixmap);

  pixmap = gdk_pixmap_new(widget->window,
			  widget->allocation.width,
			  widget->allocation.height,
			  -1);
  gdk_draw_rectangle (pixmap,
		      widget->style->white_gc,
		      TRUE,
		      0, 0,
		      widget->allocation.width,
		      widget->allocation.height);//static int x2 ;
//static int y1 ;
//static int y2 ;

  return TRUE;
}

/* Redraw the screen from the backing pixmap */
static gint
expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_pixmap(widget->window,
		  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		  pixmap,
		  event->area.x, event->area.y,
		  event->area.x, event->area.y,
		  event->area.width, event->area.height);

  return FALSE;
}


/* Draw a rectangle on the screen */
static void
draw_brush (GtkWidget *widget, gdouble x, gdouble y)
{
  GdkRectangle update_rect;

  update_rect.x = x ;
  update_rect.y = y ;
  update_rect.width = 1;
  update_rect.height = 1;
  gdk_draw_rectangle (pixmap,
		      widget->style->black_gc,
		      TRUE,
		      update_rect.x, update_rect.y,
		      update_rect.width, update_rect.height);
  gtk_widget_draw (widget, &update_rect);
}


/* draw a line on the screen */
static void
draw_line (GtkWidget *widget, gdouble x1, gdouble y1, gdouble x2, gdouble y2)
{
  /*GdkRectangle update_rect;

  update_rect.x = x1 ;
  update_rect.y = y1 ;
  update_rect.width = 1;
  update_rect.height = 1;*/
  
  gint x, y, width, height;
  x = MIN(x1,x2);
  y = MIN(y1,y2);
  width = 100;
  height = 100;

  gdk_draw_line (pixmap,
		 widget->style->black_gc,
		 x1, y1,
		 x2, y2);
  //gtk_widget_draw (widget, &update_rect);
  gtk_widget_queue_draw_area (widget, x, y, width, height);
}

static gint
button_press_event (GtkWidget *widget, GdkEventButton *event)
{
  if (event->button == 1 /* 1 represent the left button */ && pixmap != NULL)
    {
       draw_brush (widget, event->x, event->y);

	/* flush the memory */
       xi = 0;
       yi = 0;
       xii = event->x;
       yii = event->y;
       
       printf("%d", xii);
       printf("\t\t\t");
       printf("%d", yii);
       printf("\n");
       
       if(fp != NULL && training == True && firstStroke == True)
      	{
        	fprintf(fp, "<s>\n%d,%d", xii, yii);
        	fprintf(ffp, "<s>\n%d,%d", xii, yii);
        	firstStroke = False;
      	}else if(fp != NULL && training == True)
      	{
      		fprintf(fp, "\n</s>\n<s>\n%d,%d", xii, yii);
      		fprintf(ffp, "\n</s>\n<s>\n%d,%d", xii, yii);
      	}
      	
      	xtemp = xii;
      	ytemp = yii;
    }
  return TRUE;
}

static gint
motion_notify_event (GtkWidget *widget, GdkEventMotion *event)
{
  xi = xii;
  yi = yii;

  GdkModifierType state;

  if (event->is_hint)
    gdk_window_get_pointer (event->window, &xii, &yii, &state);
  else
    {
      xii = event->x;
      yii = event->y;
      state = event->state;
    }
    
  if (state & GDK_BUTTON1_MASK && pixmap != NULL)
    {
      draw_line (widget, xi, yi, xii, yii);

      printf("%d", xii);
      printf("\t\t\t");
      printf("%d", yii);
      printf("\n");

      
	 int distance = (xii-xtemp)*(xii-xtemp)+(yii-ytemp)*(yii-ytemp);
	 
 	 if(distance >=25){	
	      if(fp != NULL && training == True)
	      {
	        fprintf(fp, "\n%d,%d",xii, yii);
	      }
	      
	      xtemp=xii;
	      ytemp=yii;
 	 }
     if(fp != NULL && training == True)
      {
        fprintf(ffp, "\n%d,%d",xii, yii);
      }
   }
  
  return TRUE;
}

void
quit ()
{
  gtk_exit (0);
}

/* training function - open the target file to save the training data */
void
train_function ()
{
  training = True;
  fp = fopen(file, "w");
  ffp = fopen(file, "w");
}

void
save_function ()
{
  training = False;
  fprintf(fp, "\n</s>");
  fprintf(ffp, "\n</s>");
  fclose(fp);
  fclose(ffp);
}

int
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkWidget *vbox;

  GtkWidget *button;
  GtkWidget *btnTraining;
  GtkWidget *btnClear;
  GtkWidget *btnSave;

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (window, "Test Input");

  vbox = gtk_vbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_widget_show (vbox);

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (quit), NULL);

  /* Create the drawing area */

  drawing_area = gtk_drawing_area_new ();
  gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);
  gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

  gtk_widget_show (drawing_area);

  /* Signals used to handle backing pixmap */

  gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
		      (GtkSignalFunc) expose_event, NULL);
  gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
		      (GtkSignalFunc) configure_event, NULL);

  /* Event signals */

  gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
		      (GtkSignalFunc) motion_notify_event, NULL);
  gtk_signal_connect (GTK_OBJECT (drawing_area), "button_press_event",
		      (GtkSignalFunc) button_press_event, NULL);

  gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
			 | GDK_LEAVE_NOTIFY_MASK
			 | GDK_BUTTON_PRESS_MASK
			 | GDK_POINTER_MOTION_MASK
			 | GDK_POINTER_MOTION_HINT_MASK);

  /* Add a quit button */
  button = gtk_button_new_with_label ("Quit");
  gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);
  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));
  gtk_widget_show (button);

  /* Add a training button */
  btnTraining = gtk_button_new_with_label ("Train");
  gtk_box_pack_start (GTK_BOX (vbox), btnTraining, FALSE, FALSE, 0); 
  /*gtk_signal_connet_object (GTK_OBJECT (btnTraining), "clicked",
			    GTK_SIGNAL_FUNC (gtk_widget_destroy),
			    GTK_OBJECT (window));*/
  gtk_signal_connect (GTK_OBJECT (btnTraining), "clicked",
		      GTK_SIGNAL_FUNC (train_function), NULL);
  gtk_widget_show (btnTraining);
  
    /* add a Save button */
  btnSave = gtk_button_new_with_label ("Save");
  gtk_box_pack_start (GTK_BOX (vbox), btnSave, FALSE, FALSE, 0);
  /*gtk_signal_connect_object (GTK_OBJECT (btnSave), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));*/
  gtk_signal_connect (GTK_OBJECT (btnSave), "clicked",
		      GTK_SIGNAL_FUNC (save_function), NULL);
  gtk_widget_show (btnSave);
  
  

  /* Add a clear button */
  btnClear = gtk_button_new_with_label ("Clear");
  gtk_box_pack_start (GTK_BOX(vbox), btnClear, FALSE, FALSE, 0);
  /*gtk_signal_connet_object (GTK_OBJECT (btnClear), "clicked",
			    GTK_SIGNAL_FUNC (configure_event),
			    GTK_OBJECT (window));*/
  /*g_signal_connect (G_OBJECT (btnClear), "pressed",
                      G_CALLBACK (clear_screen),
                      NULL);*/
  gtk_widget_show (btnClear);
 

  
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
