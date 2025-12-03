/*
 * Project:  Graphical Calculator using C and GTK+
 * File:     main.c
 * Author:   Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:  MIT (see LICENSE file for details)
 *
 * Description:
 *   Application entry point. Creates a GtkApplication and delegates
 *   UI construction to the UI module.
 */

#include <gtk/gtk.h>

#include "ui.h"

static void
on_app_activate(GtkApplication *app, gpointer user_data)
{
    (void) user_data; /* unused */
    create_main_window(app);
}

int
main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("com.mobinyousefi.gtkcalculator",
                                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
