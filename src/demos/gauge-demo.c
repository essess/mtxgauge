/*
 * Copyright (C) 2006 by Dave J. Andruczyk <djandruczyk at yahoo dot com>
 * and Chris Mire (czb)
 *
 * MegaTunix gauge widget
 *
 *
 * This software comes under the GPL (GNU Public License)
 * You may freely copy,distribute etc. this as long as the source code
 * is made available for FREE.
 *
 * No warranty is made or implied. You use this program at your own risk.
 */

#include <gauge.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>

static gboolean
  update( gpointer );

/**
 * notes:
 * 'r' resets a tattletail, which is enabled via 't'
 *      pressing either one of those 'feels' like they block 'painting' the gauge so things
 *      appear to slow down ... hold down the 't' key while demo running and you'll see
 *      what I mean. When the gauge is small, the effect is not noticed - enlarge it to exaggerate
 */

int
    main( int argc, char **argv )
{
    gtk_init (&argc, &argv);

    GtkWidget *const w = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    g_assert( w );

    gtk_widget_set_size_request( w, 240, 240);
    gtk_widget_add_events( w, GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK );
    gtk_window_set_decorated( GTK_WINDOW(w), FALSE );

    MtxGaugeFace *const gf = mtx_gauge_face_new();
    g_assert( gf );

    gtk_container_add( GTK_CONTAINER(w), GTK_WIDGET(gf) );
    gtk_widget_realize( GTK_WIDGET(gf) );

    mtx_gauge_face_set_attribute( gf, LBOUND, 0.0 );
    mtx_gauge_face_set_attribute( gf,UBOUND, 8000.0 );
    mtx_gauge_face_set_attribute( gf,ROTATION, MTX_ROT_CW );
    mtx_gauge_face_set_value ( gf, 0.0 );
    mtx_gauge_face_set_attribute( gf,START_ANGLE, 135.0 );
    mtx_gauge_face_set_attribute( gf,SWEEP_ANGLE, 270.0 );
    mtx_gauge_face_set_attribute( gf, ANTIALIAS, (gfloat)TRUE );
    mtx_gauge_face_set_attribute( gf, PRECISION, (gfloat)1 );
    mtx_gauge_face_set_daytime_mode( gf, MTX_DAY );

    guint const id = g_timeout_add( 20, update, gf );
    g_assert( id );

    mtx_gauge_face_set_show_drag_border ( gf, TRUE);
    g_signal_connect( w, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all( w );
    gtk_main ();
    return EXIT_SUCCESS;
}

static gboolean
  update( gpointer p )
{
    MtxGaugeFace *const gf = MTX_GAUGE_FACE( p );

    static gfloat lower = 0.0;
    static gfloat upper = 0.0;
    gfloat cur_val = 0.0;
    gfloat interval = 0.0;
    static gboolean rising = TRUE;

    interval = (upper-lower)/100.0;
    mtx_gauge_face_get_attribute( gf, LBOUND, &lower);
    mtx_gauge_face_get_attribute( gf, UBOUND, &upper);

    mtx_gauge_face_get_value( gf, &cur_val);

    if (cur_val >= upper)
        rising = FALSE;
    if (cur_val <= lower)
        rising = TRUE;

    if (rising)
        cur_val+=interval;
    else
        cur_val-=interval;

    mtx_gauge_face_set_value ( gf, cur_val);

    return TRUE;
}
