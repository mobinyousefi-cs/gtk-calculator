/*
 * Project:  Graphical Calculator using C and GTK+
 * File:     ui.c
 * Author:   Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
 * License:  MIT (see LICENSE file for details)
 *
 * Description:
 *   Implements the graphical user interface using GTK+. This module
 *   wires the button callbacks to the core calculator engine.
 */

#include "ui.h"
#include "calculator.h"

#include <string.h>

/* --- Internal helpers -------------------------------------------------- */

static GtkWidget *create_button(const char *label, AppWidgets *app_wdgts);
static void       append_to_display(AppWidgets *app_wdgts, const char *text);
static void       clear_display(AppWidgets *app_wdgts);
static void       backspace_display(AppWidgets *app_wdgts);
static void       set_status(AppWidgets *app_wdgts, const char *msg);
static void       on_button_clicked(GtkButton *button, gpointer user_data);

/* --- Public API --------------------------------------------------------- */

void
create_main_window(GtkApplication *app)
{
    AppWidgets *app_wdgts = g_malloc0(sizeof(AppWidgets));

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ Graphical Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 400);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    app_wdgts->window = window;

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    /* Display entry ------------------------------------------------------ */
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_alignment(GTK_ENTRY(entry), 1.0);  /* right aligned */
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);
    app_wdgts->display_entry = entry;

    /* Status bar --------------------------------------------------------- */
    GtkWidget *status_bar = gtk_statusbar_new();
    app_wdgts->status_bar = status_bar;
    app_wdgts->status_ctx_id =
        gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar), "main");
    gtk_box_pack_start(GTK_BOX(vbox), status_bar, FALSE, FALSE, 0);
    set_status(app_wdgts, "Ready");

    /* Button grid -------------------------------------------------------- */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    const char *buttons[][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", ".", "=", "+"},
        {"C", "CE", "\xE2\x86\x90", "^"} /* \xE2\x86\x90 = Unicode left-arrow */
    };

    for (gint row = 0; row < 5; ++row) {
        for (gint col = 0; col < 4; ++col) {
            GtkWidget *btn = create_button(buttons[row][col], app_wdgts);
            gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
        }
    }

    gtk_widget_show_all(window);
}

/* --- Static helpers ---------------------------------------------------- */

static GtkWidget *
create_button(const char *label, AppWidgets *app_wdgts)
{
    GtkWidget *button = gtk_button_new_with_label(label);
    gtk_widget_set_hexpand(button, TRUE);
    gtk_widget_set_vexpand(button, TRUE);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), app_wdgts);
    return button;
}

static void
append_to_display(AppWidgets *app_wdgts, const char *text)
{
    const gchar *current = gtk_entry_get_text(GTK_ENTRY(app_wdgts->display_entry));
    gchar *new_text = g_strconcat(current, text, NULL);
    gtk_entry_set_text(GTK_ENTRY(app_wdgts->display_entry), new_text);
    g_free(new_text);
}

static void
clear_display(AppWidgets *app_wdgts)
{
    gtk_entry_set_text(GTK_ENTRY(app_wdgts->display_entry), "");
}

static void
backspace_display(AppWidgets *app_wdgts)
{
    const gchar *current = gtk_entry_get_text(GTK_ENTRY(app_wdgts->display_entry));
    gsize len = strlen(current);
    if (len == 0) {
        return;
    }

    gchar *new_text = g_strndup(current, len - 1);
    gtk_entry_set_text(GTK_ENTRY(app_wdgts->display_entry), new_text);
    g_free(new_text);
}

static void
set_status(AppWidgets *app_wdgts, const char *msg)
{
    gtk_statusbar_pop(GTK_STATUSBAR(app_wdgts->status_bar),
                      app_wdgts->status_ctx_id);
    gtk_statusbar_push(GTK_STATUSBAR(app_wdgts->status_bar),
                       app_wdgts->status_ctx_id,
                       msg);
}

static void
on_button_clicked(GtkButton *button, gpointer user_data)
{
    AppWidgets *app_wdgts = (AppWidgets *) user_data;
    const gchar *label = gtk_button_get_label(button);

    if (g_strcmp0(label, "C") == 0) {
        clear_display(app_wdgts);
        set_status(app_wdgts, "Cleared");
        return;
    }

    if (g_strcmp0(label, "CE") == 0) {
        clear_display(app_wdgts);
        set_status(app_wdgts, "Entry cleared");
        return;
    }

    if (g_strcmp0(label, "\xE2\x86\x90") == 0) { /* backspace */
        backspace_display(app_wdgts);
        return;
    }

    if (g_strcmp0(label, "=") == 0) {
        const gchar *expr = gtk_entry_get_text(GTK_ENTRY(app_wdgts->display_entry));
        if (expr == NULL || *expr == '\0') {
            set_status(app_wdgts, "Nothing to evaluate");
            return;
        }

        double result = 0.0;
        CalcError error;
        if (calc_evaluate(expr, &result, &error)) {
            gchar buf[64];
            g_snprintf(buf, sizeof(buf), "%.12g", result);
            gtk_entry_set_text(GTK_ENTRY(app_wdgts->display_entry), buf);
            set_status(app_wdgts, "OK");
        } else {
            set_status(app_wdgts, error.message);
        }
        return;
    }

    /* Default: treat the label as part of the expression */
    append_to_display(app_wdgts, label);
}
