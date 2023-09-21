#include <stdio.h>
#include <math.h>
#include <gtk/gtk.h>


unsigned long compute_bit_value(int n) {
    return (unsigned long)pow(2, n);
}

static void on_send_command_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *ip_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "ip_entry"));
    GtkWidget *port_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "port_entry"));
    GtkWidget *combobox = GTK_WIDGET(g_object_get_data(G_OBJECT(widget), "combobox"));

    const char *ip = gtk_entry_get_text(GTK_ENTRY(ip_entry));
    const char *port = gtk_entry_get_text(GTK_ENTRY(port_entry));

    const char *selected_number_str = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox));
    int selected_number_int = atoi(selected_number_str);
    unsigned long selected_bit_value = compute_bit_value(selected_number_int);

    g_print("Send command button was clicked! IP: %s, Port: %s, Bit: %lu\n", ip, port, selected_bit_value);

    g_free(selected_number_str);  // Free the dynamically allocated string
}

static void style_window(GtkWidget *window) {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    // CSS to set both the window and the button black
    const char *css = "* { background-color: black; }"
                      "button { background-color: black; color: white; }"; // Ensure button text is readable

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    gtk_css_provider_load_from_data(GTK_CSS_PROVIDER(provider),
        css, -1, NULL);

    g_object_unref(provider);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *send_command_button;
    GtkWidget *ip_entry;
    GtkWidget *port_entry;
    GtkWidget *logo_image;
    GtkWidget *combobox;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Red Pitaya Controller v1.0");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);
    
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a grid
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // NASA LOGO
    logo_image = gtk_image_new_from_file("/Users/matankedar/Downloads/GUI/NASA_Worm_logo.png"); 
    gtk_widget_set_hexpand(logo_image, TRUE);
    gtk_grid_attach(GTK_GRID(grid), logo_image, 2, 0, 1, 1); 

    // Create an "IP" entry field
    ip_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(ip_entry), "Enter IP");
    gtk_grid_attach(GTK_GRID(grid), ip_entry, 0, 0, 2, 1);

    // Create a "Port" entry field
    port_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(port_entry), "Enter Port");
    gtk_grid_attach(GTK_GRID(grid), port_entry, 0, 1, 2, 1);

    // Create a ComboBox (Dropdown) with numbers 1-30
    combobox = gtk_combo_box_text_new();
    for (int i = 1; i <= 30; i++) {
        char num[3];
        sprintf(num, "%d", i);
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox), num);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0);
    gtk_grid_attach(GTK_GRID(grid), combobox, 0, 2, 2, 1); // Position it between the IP and Port entry fields

    // Create a "Send Command" button
    send_command_button = gtk_button_new_with_label("Send Command");
    g_signal_connect(send_command_button, "clicked", G_CALLBACK(on_send_command_button_clicked), NULL);
    g_object_set_data(G_OBJECT(send_command_button), "ip_entry", ip_entry);
    g_object_set_data(G_OBJECT(send_command_button), "port_entry", port_entry);
    g_object_set_data(G_OBJECT(send_command_button), "combobox", combobox);
    gtk_grid_attach(GTK_GRID(grid), send_command_button, 0, 3, 2, 1);

    // Show all widgets
    gtk_widget_show_all(window);

    // Start the main loop
    gtk_main();

    return 0;
}