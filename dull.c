#include <stdio.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

#define PROJECT_NAME "dull"

#include <gtk/gtk.h>

static void load_custom_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void editor_create_menus(GtkWidget **window) {
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(*window), header_bar);

    GtkWidget *menu_btn = gtk_menu_button_new();
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_btn),
                                  "open-menu-symbolic");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu_btn);
    GMenu *menu = g_menu_new();
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menu_btn),
                                   G_MENU_MODEL(menu));

    GMenu *filemenu = g_menu_new();
    g_menu_append(filemenu, "New", "app.new");
    g_menu_append(filemenu, "Save", "app.save");
    g_menu_append(filemenu, "Save as", "app.saveas");
    g_menu_append(filemenu, "Quit", "app.quit");
    g_menu_append_submenu(menu, "File", G_MENU_MODEL(filemenu));

    GMenu *editmenu = g_menu_new();
    g_menu_append(editmenu, "Cut", "app.cut");
    g_menu_append(editmenu, "Copy", "app.copy");
    g_menu_append(editmenu, "Paste", "app.paste");
    g_menu_append_submenu(menu, "Edit", G_MENU_MODEL(editmenu));

    GMenu *toolsmenu = g_menu_new();
    g_menu_append_submenu(menu, "Tools", G_MENU_MODEL(toolsmenu));

    GMenu *aboutmenu = g_menu_new();
    g_menu_append_submenu(menu, "About", G_MENU_MODEL(aboutmenu));

    GMenu *helpmenu = g_menu_new();
    g_menu_append_submenu(menu, "Help", G_MENU_MODEL(helpmenu));
}

static void editor_create_edit_area(GtkWidget **window, const char *syntax,
                                    const char *theme) {
    GtkSourceBuffer *buffer = gtk_source_buffer_new(NULL);
    GtkWidget *view = gtk_source_view_new_with_buffer(buffer);

    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(view), TRUE);
    gtk_source_view_set_auto_indent(GTK_SOURCE_VIEW(view), TRUE);
    gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(view), TRUE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(view), TRUE);

    GtkSourceLanguageManager *lang_manager =
        gtk_source_language_manager_get_default();
    GtkSourceLanguage *lang =
        gtk_source_language_manager_get_language(lang_manager, syntax);
    if (lang) {
        gtk_source_buffer_set_language(buffer, lang);
        gtk_source_buffer_set_highlight_syntax(buffer, TRUE);
        gtk_source_buffer_set_highlight_matching_brackets(buffer, TRUE);
    }

    GtkSourceStyleSchemeManager *style_manager =
        gtk_source_style_scheme_manager_get_default();
    GtkSourceStyleScheme *scheme =
        gtk_source_style_scheme_manager_get_scheme(style_manager, theme);

    if (scheme) {
        gtk_source_buffer_set_style_scheme(buffer, scheme);
    }

    gtk_widget_set_name(view, "editor-area");

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), view);

    gtk_window_set_child(GTK_WINDOW(*window), scrolled_window);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), PROJECT_NAME);
    gtk_window_set_default_size(GTK_WINDOW(window), 860, 640);
    load_custom_css();
    editor_create_menus(&window);
    editor_create_edit_area(&window, "c", "cobalt");

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app =
        gtk_application_new("com.mkamel.dull", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    return status;
}
