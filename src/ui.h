/*
 * Project:  Graphical Calculator using C and GTK+
 * File:     ui.h
 * Author:   Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:  MIT (see LICENSE file for details)
 *
 * Description:
 *   Public interface of the UI layer. Responsible for constructing
 *   the main window and exposing the AppWidgets handle used by
 *   callbacks inside the UI module.
 */

#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

/*
 * AppWidgets:
 *   Convenience structure that groups together all top-level widgets
 *   the callbacks need to access. This avoids the use of globals and
 *   keeps the code modular.
 */

typedef struct _AppWidgets {
    GtkWidget *window;        /* main application window            */
    GtkWidget *display_entry; /* calculator display (GtkEntry)      */
    GtkWidget *status_bar;    /* status messages (GtkStatusbar)     */
    guint      status_ctx_id; /* statusbar context ID               */
} AppWidgets;

/*
 * create_main_window:
 *   Builds and shows the main calculator window for the given
 *   GtkApplication instance.
 */
void create_main_window(GtkApplication *app);

#endif /* UI_H */
