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

static gboolean
update( GtkWidget * const gauge );

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

    GtkWidget * const window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_assert( window );

    gtk_widget_set_size_request(GTK_WIDGET(window),240,240);
    gtk_widget_add_events(GTK_WIDGET(window),
                          GDK_KEY_PRESS_MASK|GDK_KEY_RELEASE_MASK);
    gtk_window_set_decorated(GTK_WINDOW(window),FALSE);

    GtkWidget *const gauge = mtx_gauge_face_new ();
    g_assert( gauge );

    gtk_container_add (GTK_CONTAINER (window), gauge);
    gtk_widget_realize(gauge);

    gtk_widget_show_all (window);

    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE(gauge),LBOUND, 0.0);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE(gauge),UBOUND, 8000.0);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE(gauge),ROTATION, MTX_ROT_CW);
    mtx_gauge_face_set_value (MTX_GAUGE_FACE (gauge), 0.0);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE(gauge),START_ANGLE, 135.0);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE(gauge),SWEEP_ANGLE, 270.0);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE (gauge), ANTIALIAS, (gfloat)TRUE);
    mtx_gauge_face_set_attribute(MTX_GAUGE_FACE (gauge), PRECISION, (gfloat)1);
    mtx_gauge_face_set_daytime_mode(MTX_GAUGE_FACE(gauge),MTX_DAY);

    guint const id = g_timeout_add( 20, (GSourceFunc)update, gauge );
    g_assert( id );

    mtx_gauge_face_set_show_drag_border (MTX_GAUGE_FACE (gauge), TRUE);

    g_signal_connect (window, "destroy",
                      G_CALLBACK (gtk_main_quit), NULL);

    gtk_main ();
    return 0;
}

static gboolean
update( GtkWidget * const gauge )
{
    g_assert( gauge );

    static gfloat lower = 0.0;
    static gfloat upper = 0.0;
    gfloat cur_val = 0.0;
    gfloat interval = 0.0;
    static gboolean rising = TRUE;

    interval = (upper-lower)/100.0;
    mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), LBOUND, &lower);
    mtx_gauge_face_get_attribute(MTX_GAUGE_FACE(gauge), UBOUND, &upper);

    mtx_gauge_face_get_value(MTX_GAUGE_FACE (gauge), &cur_val);

    if (cur_val >= upper)
        rising = FALSE;
    if (cur_val <= lower)
        rising = TRUE;

    if (rising)
        cur_val+=interval;
    else
        cur_val-=interval;

    mtx_gauge_face_set_value (MTX_GAUGE_FACE (gauge),cur_val);

    return TRUE;
}
