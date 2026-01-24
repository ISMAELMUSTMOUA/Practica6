#include <stdlib.h>
#include <stdio.h>

#include "../../include/gestion.h"


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    //GtkWidget *label, *button1, *button2, *entry;
   
    gtk_init(&argc, &argv);

    // Ventana
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculo Punto Fijo");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);
   
    // Grid
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Moles
    GtkWidget *label_moles = gtk_label_new("Moles");
    gtk_widget_set_halign(label_moles, GTK_ALIGN_START);
   
    GtkWidget *btn_test_auto = gtk_button_new_with_label("Test automático");
    g_signal_connect(btn_test_auto, "clicked", G_CALLBACK(on_test_auto_moles_clicked), window);
    gtk_widget_set_tooltip_text(btn_test_auto, "test-moles-auto");
   
    GtkWidget *label_vp = gtk_label_new("Vp (V):");
    GtkWidget *entry_vp = gtk_entry_new();
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_vp), "entry_vp");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vp), "Ej: 2.35");
   
    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    GtkWidget *entry_vt = gtk_entry_new();
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_vt), "entry_vt");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vt), "Ej: 298.0");

   
    GtkWidget *entries_moles[2] = {entry_vp, entry_vt};

    GtkWidget *btn_test_esp = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp, "clicked", G_CALLBACK(on_test_esp_moles_clicked), entries_moles);
    gtk_widget_set_tooltip_text(btn_test_esp, "test-moles-esp");

    GtkWidget *btn_gen = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen, "clicked", G_CALLBACK(on_gen_moles_clicked), entries_moles);
    gtk_widget_set_tooltip_text(btn_gen, "gen-moles");
   
    gtk_grid_attach(GTK_GRID(grid), label_moles,    0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_auto,  1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_esp,   2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_vp,       3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_vp,       4, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_gen,        0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_vt,       3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_vt,       4, 1, 1, 1);

    // Separador 1
    GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), separator1, 0, 2, 5, 1);

    // Densidad
    GtkWidget *label_densidad = gtk_label_new("Densidad");
    gtk_widget_set_halign(label_densidad, GTK_ALIGN_START);
   
    GtkWidget *btn_test_auto2 = gtk_button_new_with_label("Test automático");
    g_signal_connect(btn_test_auto2, "clicked", G_CALLBACK(on_test_auto_densidad_clicked), window);
    gtk_widget_set_tooltip_text(btn_test_auto2, "test-densidad-auto");
   
   
    GtkWidget *label_p = gtk_label_new("P (#):");
    GtkWidget *entry_p = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_p), "Ej: 820000");
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_p), "entry_p");
   
    GtkWidget *label_vt2 = gtk_label_new("Vt (mV):");
    GtkWidget *entry_vt2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vt2), "Ej: 750.0");
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_vt2), "entry_vt2");
   
    GtkWidget *entries_densidad[2] = {entry_p, entry_vt2};
    GtkWidget *btn_test_esp2 = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp2, "clicked", G_CALLBACK(on_test_esp_densidad_clicked), entries_densidad);
    gtk_widget_set_tooltip_text(btn_test_esp2, "test-densidad-esp");

    GtkWidget *btn_gen2 = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen2, "clicked", G_CALLBACK(on_gen_densidad_clicked), entries_densidad);
    gtk_widget_set_tooltip_text(btn_gen2, "gen-densidad");

    gtk_grid_attach(GTK_GRID(grid), label_densidad,    0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_auto2,  1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_esp2,   2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_p,       3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_p,       4, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_gen2,        0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_vt2,       3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_vt2,       4, 4, 1, 1);

    // Separador 2
    GtkWidget *separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), separator2, 0, 5, 5, 1);

    // Energía cinética
    GtkWidget *label_energia = gtk_label_new("E. Cinética");
    gtk_widget_set_halign(label_energia, GTK_ALIGN_START);
   
    GtkWidget *btn_test_auto3 = gtk_button_new_with_label("Test automático");
    g_signal_connect(btn_test_auto3, "clicked", G_CALLBACK(on_test_auto_energia_clicked), window);
    gtk_widget_set_tooltip_text(btn_test_auto3, "test-energia-auto");
     
    GtkWidget *label_a = gtk_label_new("A (#):");
    GtkWidget *entry_a = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_a), "Ej: 256");
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_a), "entry_a");
   
    GtkWidget *label_w = gtk_label_new("W (#):");
    GtkWidget *entry_w = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_w), "Ej: 8500000");
    gtk_widget_set_tooltip_text(GTK_WIDGET(entry_w), "entry_w");
   
    GtkWidget *entries_energia[2] = {entry_a, entry_w};
    GtkWidget *btn_test_esp3 = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp3, "clicked", G_CALLBACK(on_test_esp_energia_clicked), entries_energia);
    gtk_widget_set_tooltip_text(btn_test_esp3, "test-energia-esp");

    GtkWidget *btn_gen3 = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen3, "clicked", G_CALLBACK(on_gen_energia_clicked), entries_energia);
    gtk_widget_set_tooltip_text(btn_gen3, "gen-energ");

    gtk_grid_attach(GTK_GRID(grid), label_energia,    0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_auto3,  1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_test_esp3,   2, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_a,       3, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_a,       4, 6, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_gen3,        0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_w,       3, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_w,       4, 7, 1, 1);

    gtk_widget_show_all(window);

    g_print("Interfaz iniciada\n");
   
    gtk_main();
    return 0;
}
