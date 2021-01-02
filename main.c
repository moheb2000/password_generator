#include <gtk/gtk.h>


typedef struct
{
    GtkWidget *w_sbtn_num;
    GtkWidget *w_ent_password;
    GtkWidget *w_btn_copy;
    GtkWidget *w_lbl_not;
} app_widgets;


void on_btn_copy_clicked(GtkButton *button, app_widgets *widgets)
{
    GtkClipboard *clipboard = gtk_clipboard_get_default(gdk_display_get_default());

    gtk_clipboard_set_text(clipboard, gtk_entry_get_text(GTK_ENTRY(widgets->w_ent_password)), -1);
    gtk_label_set_text(GTK_LABEL(widgets->w_lbl_not), "Password copied to clipboard!");
}


void on_btn_generate_clicked(GtkButton *button, app_widgets *widgets)
{
    gint num = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->w_sbtn_num));
    gchar password[num + 1];
    gint k;
    for(int i = 0; i < num; i++)
    {
        k = g_random_int_range(1, 128);
        if((k>=48&&k<=57)||(k>=65&&k<=90)||(k>=97&&k<=122)||(k>=35&&k<=37)||k==64) password[i] = k;
        else i--;
    }
    password[num] = '\0';

    gtk_entry_set_text(GTK_ENTRY(widgets->w_ent_password), password);
    gtk_label_set_text(GTK_LABEL(widgets->w_lbl_not), "Password was generated!");

    gtk_widget_set_sensitive(widgets->w_btn_copy, TRUE);

}


void on_window_main_destroy()
{
    gtk_main_quit();
}


int main(int argc, char **argv)
{
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;
    app_widgets *widgets = g_slice_new(app_widgets);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "builder.ui", &error) == 0)
    {
        g_printerr("can not open file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    widgets->w_sbtn_num = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_num"));
    widgets->w_ent_password = GTK_WIDGET(gtk_builder_get_object(builder, "ent_password"));
    widgets->w_btn_copy = GTK_WIDGET(gtk_builder_get_object(builder, "btn_copy"));
    widgets->w_lbl_not = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_not"));

    gtk_builder_connect_signals(builder, widgets);
    g_object_unref(builder);

    gtk_widget_show_all(window);

    gtk_main();
    g_slice_free(app_widgets, widgets);

    return 0;
}
