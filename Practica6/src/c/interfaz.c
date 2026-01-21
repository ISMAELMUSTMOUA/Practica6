#include <stdlib.h>
#include <stdio.h>

#include "../../include/gestion.h"

extern GtkWidget *vp_moles, *vt_moles, *resultado_moles;
extern GtkWidget *p_densidad, *vt_densidad, *resultado_densidad;
extern GtkWidget *a_energia, *w_energia, *resultado_energia;

// Botones moles
void on_test_clicked(GtkWidget *widget, gpointer data) {
    g_print("Botón presionado: %s\n", (char*)data);
}


GtkWidget *moles_window = NULL;
GtkWidget *vp_entry_moles, *vt_entry_moles, *resultado_entry_moles, *filename_entry_moles;

void on_test_auto_moles_clicked(GtkWidget *widget, gpointer data) {
    g_print("Test automático de Moles iniciado\n");
    GtkWidget *dialogo;
    // Argumentos: Título ventana de dialogo, clase padre, acción del diálogo (abrir, guardar, elegir, mirar FileChooserAction), nombre de botón, ID de respuesta asociada a botón, replicable, acaba en Null
    dialogo = gtk_file_chooser_dialog_new("Selecciona el fichero", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, "Seleccionar", GTK_RESPONSE_ACCEPT, NULL);
    //Ejecutar el diálogo y estudiar la respesta del dialógo  
    if(gtk_dialog_run(GTK_DIALOG(dialogo)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialogo));
        //AQUI GESTIONAR FICHERO
        gchar *command = g_strdup_printf("pytest ./src/python/test_moles.py %s", filename);
        g_print("Fichero seleccionado");
        system(command);
        g_free(command);
        g_free(filename);
       
    } else {
        g_print("Operación cancelada");
    }

    gtk_widget_destroy(dialogo);
}

gchar* formatear_con_punto(double valor) {
                gchar *buffer = g_strdup_printf("%.10f", valor);
                for (int i = 0; buffer[i]; i++) {
                    if (buffer[i] == ',') {
                        buffer[i] = '.';
                    }
                }
                return buffer;
            }


void on_guardar_moles_clicked(GtkWidget *widget, gpointer data) {
    const gchar *vp_text = gtk_entry_get_text(GTK_ENTRY(vp_entry_moles));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(vt_entry_moles));
    const gchar *resultado_text = gtk_entry_get_text(GTK_ENTRY(resultado_entry_moles));
    const gchar *filename_text = gtk_entry_get_text(GTK_ENTRY(filename_entry_moles));
   
    if (vp_text && vt_text && resultado_text && vp_text[0] && vt_text[0] && resultado_text[0]) {
        double vp = atof(vp_text);
        double vt = atof(vt_text);
        double resultado = atof(resultado_text);
       
        gchar *filename;
        if (filename_text && filename_text[0]) {
            filename = g_strdup_printf("%s.txt", filename_text);
        }else {
            filename = g_strdup("test_generado_moles.txt");
        }
        FILE *file = fopen(filename, "a");
        if(file){
            gchar *vp_formateado = formatear_con_punto(vp);
            gchar *vt_formateado = formatear_con_punto(vt);
            gchar *resultado_formateado = formatear_con_punto(resultado);
           
            fseek(file, 0, SEEK_END);
            if (ftell(file) == 0) {
                fprintf(file, "# Vp (V)\tVt (mV)\tResultado (mol)\n");
            }
           
            fprintf(file, "%s\t%s\t%s\n", vp_formateado, vt_formateado, resultado_formateado);
            fclose(file);

            g_free(vp_formateado);
            g_free(vt_formateado);
            g_free(resultado_formateado);

            printf("Fichero '%s' creado\n",filename);

            gtk_entry_set_text(GTK_ENTRY(vp_entry_moles), "");
            gtk_entry_set_text(GTK_ENTRY(vt_entry_moles), "");
            gtk_entry_set_text(GTK_ENTRY(resultado_entry_moles), "");
        } else {
            printf("Error al crear el fichero de test\n");
        }
        g_free(filename);
    }else {
        printf("Ingresa valores válidos para Vp y Vt\n");
    }
}

void on_guardar_cerrar_moles_clicked(GtkWidget *widget, gpointer data) {
   
    if(moles_window){
        gtk_widget_destroy(moles_window);
        moles_window = NULL;
    }        
}


void on_gen_moles_clicked(GtkWidget *widget, gpointer data) {
    moles_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(moles_window), "Generar Fichero de Moles");
    gtk_window_set_default_size(GTK_WINDOW(moles_window), 400, 250);
    gtk_window_set_position(GTK_WINDOW(moles_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(moles_window), 10);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(moles_window), vbox);

    GtkWidget *label_vp = gtk_label_new("Vp (V):");
    vp_entry_moles = gtk_entry_new();
    vp_moles = vp_entry_moles;
    gtk_box_pack_start(GTK_BOX(vbox), label_vp, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), vp_entry_moles, FALSE, FALSE, 0);


    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    vt_entry_moles = gtk_entry_new();
    vt_moles= vt_entry_moles;
    gtk_box_pack_start(GTK_BOX(vbox), label_vt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), vt_entry_moles, FALSE, FALSE, 0);

    GtkWidget *label_resultado = gtk_label_new("Resultado (mol):");
    resultado_entry_moles = gtk_entry_new();
    resultado_moles = resultado_entry_moles;
    gtk_editable_set_editable(GTK_EDITABLE(resultado_entry_moles), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), label_resultado, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), resultado_entry_moles, FALSE, FALSE, 0);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    GtkWidget *label_filename = gtk_label_new("Nombre del fichero (sin extensión):");
    filename_entry_moles = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(filename_entry_moles), "Opcional: test_generado_moles.txt");
    gtk_entry_set_text(GTK_ENTRY(filename_entry_moles), "test_generado_moles");

    gtk_box_pack_start(GTK_BOX(vbox), label_filename, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), filename_entry_moles, FALSE, FALSE, 0);

    g_signal_connect(vp_entry_moles, "changed", G_CALLBACK(on_moles_value_change), NULL);
    g_signal_connect(vt_entry_moles, "changed", G_CALLBACK(on_moles_value_change), NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *btn_guardar = gtk_button_new_with_label("Guardar");
    GtkWidget *btn_guardar_cerrar = gtk_button_new_with_label("Cerrar");

    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar_cerrar, TRUE, TRUE, 0);


    g_signal_connect(btn_guardar, "clicked", G_CALLBACK(on_guardar_moles_clicked), NULL);
    g_signal_connect(btn_guardar_cerrar, "clicked", G_CALLBACK(on_guardar_cerrar_moles_clicked), NULL);

    g_signal_connect(moles_window, "destroy", G_CALLBACK(on_guardar_cerrar_moles_clicked), NULL);

    gtk_widget_show_all(moles_window);

}


//Botones densidad
void on_test_auto_densidad_clicked(GtkWidget *widget, gpointer data) {
    g_print("Test automático de Densidad iniciado\n");
    GtkWidget *dialogo;
    // Argumentos: Título ventana de dialogo, clase padre, acción del diálogo (abrir, guardar, elegir, mirar FileChooserAction), nombre de botón, ID de respuesta asociada a botón, replicable, acaba en Null
    dialogo = gtk_file_chooser_dialog_new("Selecciona el fichero", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, "Seleccionar", GTK_RESPONSE_ACCEPT, NULL);
    //Ejecutar el diálogo y estudiar la respesta del dialógo  
    if(gtk_dialog_run(GTK_DIALOG(dialogo)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialogo));
        //AQUI GESTIONAR FICHERO
        gchar *command = g_strdup_printf("pytest ./src/python/test_dens.py %s", filename);
        g_print("Fichero seleccionado: %s\n", filename);
        system(command);
        g_free(command);
        g_free(filename);
       
    } else {
        g_print("Operación cancelada\n");
    }

    gtk_widget_destroy(dialogo);
}


GtkWidget *densidad_window = NULL;
GtkWidget *p_entry_densidad, *vt_entry_densidad, *resultado_entry_densidad, *filename_entry_densidad;

void on_guardar_densidad_clicked(GtkWidget *widget, gpointer data) {
    const gchar *p_text = gtk_entry_get_text(GTK_ENTRY(p_entry_densidad));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(vt_entry_densidad));
    const gchar *resultado_text = gtk_entry_get_text(GTK_ENTRY(resultado_entry_densidad));
    const gchar *filename_text = gtk_entry_get_text(GTK_ENTRY(filename_entry_densidad));
   
    if (p_text && vt_text && resultado_text && p_text[0] && vt_text[0] && resultado_text[0]) {
        double p = atof(p_text);
        double vt = atof(vt_text);
        double resultado = atof(resultado_text);
       
        gchar *filename;
        if (filename_text && filename_text[0]) {
            filename = g_strdup_printf("%s.txt", filename_text);
        }else {
            filename = g_strdup("test_generado_densidad.txt");
        }
        FILE *file = fopen(filename, "a");
        if(file){
            gchar *p_formateado = formatear_con_punto(p);
            gchar *vt_formateado = formatear_con_punto(vt);
            gchar *resultado_formateado = formatear_con_punto(resultado);
           
            fseek(file, 0, SEEK_END);
            if (ftell(file) == 0) {
                fprintf(file, "# P (#)\tVt (mV)\tDensidad (kg/m³)\n");
            }
           
            fprintf(file, "%s\t%s\t%s\n", p_formateado, vt_formateado, resultado_formateado);
            fclose(file);

            g_free(p_formateado);
            g_free(vt_formateado);
            g_free(resultado_formateado);

            printf("Fichero '%s' creado\n",filename);

            gtk_entry_set_text(GTK_ENTRY(p_entry_densidad), "");
            gtk_entry_set_text(GTK_ENTRY(vt_entry_densidad), "");
            gtk_entry_set_text(GTK_ENTRY(resultado_entry_densidad), "");
        } else {
            printf("Error al crear el fichero de test\n");
        }
        g_free(filename);
    }else {
        printf("Ingresa valores válidos para P y Vt\n");
    }
}

void on_guardar_cerrar_densidad_clicked(GtkWidget *widget, gpointer data) {
   
    if(densidad_window){
        gtk_widget_destroy(densidad_window);
        densidad_window = NULL;
    }        
}


void on_gen_densidad_clicked(GtkWidget *widget, gpointer data) {
    densidad_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(densidad_window), "Generar Fichero de Densidad");
    gtk_window_set_default_size(GTK_WINDOW(densidad_window), 400, 250);
    gtk_window_set_position(GTK_WINDOW(densidad_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(densidad_window), 10);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(densidad_window), vbox);

    GtkWidget *label_p = gtk_label_new("P (#):");
    p_entry_densidad = gtk_entry_new();
    p_densidad = p_entry_densidad;
    gtk_box_pack_start(GTK_BOX(vbox), label_p, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), p_entry_densidad, FALSE, FALSE, 0);


    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    vt_entry_densidad = gtk_entry_new();
    vt_densidad= vt_entry_densidad;
    gtk_box_pack_start(GTK_BOX(vbox), label_vt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), vt_entry_densidad, FALSE, FALSE, 0);

    GtkWidget *label_resultado = gtk_label_new("Resultado (kg/m³):");
    resultado_entry_densidad = gtk_entry_new();
    resultado_densidad = resultado_entry_densidad;
    gtk_editable_set_editable(GTK_EDITABLE(resultado_entry_densidad), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), label_resultado, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), resultado_entry_densidad, FALSE, FALSE, 0);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    GtkWidget *label_filename = gtk_label_new("Nombre del fichero (sin extensión):");
    filename_entry_densidad = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(filename_entry_densidad), "Opcional: test_generado_densidad.txt");
    gtk_entry_set_text(GTK_ENTRY(filename_entry_densidad), "test_generado_densidad");

    gtk_box_pack_start(GTK_BOX(vbox), label_filename, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), filename_entry_densidad, FALSE, FALSE, 0);

    g_signal_connect(p_entry_densidad, "changed", G_CALLBACK(on_densidad_value_change), NULL);
    g_signal_connect(vt_entry_densidad, "changed", G_CALLBACK(on_densidad_value_change), NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *btn_guardar = gtk_button_new_with_label("Guardar");
    GtkWidget *btn_guardar_cerrar = gtk_button_new_with_label("Cerrar");

    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar_cerrar, TRUE, TRUE, 0);


    g_signal_connect(btn_guardar, "clicked", G_CALLBACK(on_guardar_densidad_clicked), NULL);
    g_signal_connect(btn_guardar_cerrar, "clicked", G_CALLBACK(on_guardar_cerrar_densidad_clicked), NULL);

    g_signal_connect(densidad_window, "destroy", G_CALLBACK(on_guardar_cerrar_densidad_clicked), NULL);

    gtk_widget_show_all(densidad_window);

}


//Botones de la energía cinética
void on_test_auto_energia_clicked(GtkWidget *widget, gpointer data) {
    g_print("Test automático de Energía Cinética iniciado\n");
    GtkWidget *dialogo;
    // Argumentos: Título ventana de dialogo, clase padre, acción del diálogo (abrir, guardar, elegir, mirar FileChooserAction), nombre de botón, ID de respuesta asociada a botón, replicable, acaba en Null
    dialogo = gtk_file_chooser_dialog_new("Selecciona el fichero", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, "Seleccionar", GTK_RESPONSE_ACCEPT, NULL);
    //Ejecutar el diálogo y estudiar la respesta del dialógo  
    if(gtk_dialog_run(GTK_DIALOG(dialogo)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialogo));
        //AQUI GESTIONAR FICHERO
        gchar *command = g_strdup_printf("pytest ./src/python/test_ener.py %s", filename);
        g_print("Fichero seleccionado");
        system(command);
        g_free(command);
        g_free(filename);
       
    } else {
        g_print("Operación cancelada");
    }

    gtk_widget_destroy(dialogo);
}

GtkWidget *energia_window = NULL;
GtkWidget *a_entry_energia, *w_entry_energia, *resultado_entry_energia, *filename_entry_energia;

void on_guardar_energia_clicked(GtkWidget *widget, gpointer data) {
    const gchar *a_text = gtk_entry_get_text(GTK_ENTRY(a_entry_energia));
    const gchar *w_text = gtk_entry_get_text(GTK_ENTRY(w_entry_energia));
    const gchar *resultado_text = gtk_entry_get_text(GTK_ENTRY(resultado_entry_energia));
    const gchar *filename_text = gtk_entry_get_text(GTK_ENTRY(filename_entry_energia));
   
    if (a_text && w_text && resultado_text && a_text[0] && w_text[0] && resultado_text[0]) {
        double a = atof(a_text);
        double w = atof(w_text);
        double resultado = atof(resultado_text);
       
        gchar *filename;
        if (filename_text && filename_text[0]) {
            filename = g_strdup_printf("%s.txt", filename_text);
        }else {
            filename = g_strdup("test_generado_energia.txt");
        }
        FILE *file = fopen(filename, "a");
        if(file){
            gchar *a_formateado = formatear_con_punto(a);
            gchar *w_formateado = formatear_con_punto(w);
            gchar *resultado_formateado = formatear_con_punto(resultado);
           
            fseek(file, 0, SEEK_END);
            if (ftell(file) == 0) {
                fprintf(file, "# A (#)\tW (#)\tResultado (J)\n");
            }
           
            fprintf(file, "%s\t%s\t%s\n", a_formateado, w_formateado, resultado_formateado);
            fclose(file);

            g_free(a_formateado);
            g_free(w_formateado);
            g_free(resultado_formateado);

            printf("Fichero '%s' creado\n",filename);

            gtk_entry_set_text(GTK_ENTRY(a_entry_energia), "");
            gtk_entry_set_text(GTK_ENTRY(w_entry_energia), "");
            gtk_entry_set_text(GTK_ENTRY(resultado_entry_energia), "");
        } else {
            printf("Error al crear el fichero de test\n");
        }
        g_free(filename);
    }else {
        printf("Ingresa valores válidos para A y W\n");
    }
}

void on_guardar_cerrar_energia_clicked(GtkWidget *widget, gpointer data) {
   
    if(energia_window){
        gtk_widget_destroy(energia_window);
        energia_window = NULL;
    }        
}


void on_gen_energia_clicked(GtkWidget *widget, gpointer data) {
    energia_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(energia_window), "Generar Fichero de E. Cinetica");
    gtk_window_set_default_size(GTK_WINDOW(energia_window), 400, 250);
    gtk_window_set_position(GTK_WINDOW(energia_window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(energia_window), 10);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(energia_window), vbox);

    GtkWidget *label_a = gtk_label_new("A (#):");
    a_entry_energia = gtk_entry_new();
    a_energia = a_entry_energia;
    gtk_box_pack_start(GTK_BOX(vbox), label_a, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), a_entry_energia, FALSE, FALSE, 0);


    GtkWidget *label_w = gtk_label_new("W (#):");
    w_entry_energia = gtk_entry_new();
    w_energia= w_entry_energia;
    gtk_box_pack_start(GTK_BOX(vbox), label_w, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), w_entry_energia, FALSE, FALSE, 0);

    GtkWidget *label_resultado = gtk_label_new("Resultado (J):");
    resultado_entry_energia = gtk_entry_new();
    resultado_energia = resultado_entry_energia;
    gtk_editable_set_editable(GTK_EDITABLE(resultado_entry_energia), FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), label_resultado, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), resultado_entry_energia, FALSE, FALSE, 0);

    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    GtkWidget *label_filename = gtk_label_new("Nombre del fichero (sin extensión):");
    filename_entry_energia = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(filename_entry_energia), "Opcional: test_generado_energia.txt");
    gtk_entry_set_text(GTK_ENTRY(filename_entry_energia), "test_generado_energia");

    gtk_box_pack_start(GTK_BOX(vbox), label_filename, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), filename_entry_energia, FALSE, FALSE, 0);

    g_signal_connect(a_entry_energia, "changed", G_CALLBACK(on_energia_value_change), NULL);
    g_signal_connect(w_entry_energia, "changed", G_CALLBACK(on_energia_value_change), NULL);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *btn_guardar = gtk_button_new_with_label("Guardar");
    GtkWidget *btn_guardar_cerrar = gtk_button_new_with_label("Cerrar");

    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_guardar_cerrar, TRUE, TRUE, 0);


    g_signal_connect(btn_guardar, "clicked", G_CALLBACK(on_guardar_energia_clicked), NULL);
    g_signal_connect(btn_guardar_cerrar, "clicked", G_CALLBACK(on_guardar_cerrar_energia_clicked), NULL);

    g_signal_connect(energia_window, "destroy", G_CALLBACK(on_guardar_cerrar_energia_clicked), NULL);

    gtk_widget_show_all(energia_window);
}




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
   
    GtkWidget *label_vp = gtk_label_new("Vp (V):");
    GtkWidget *entry_vp = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vp), "Ej: 2.35");
   
    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    GtkWidget *entry_vt = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vt), "Ej: 298.0");

   

   
    GtkWidget *entries_moles[2] = {entry_vp, entry_vt};

    GtkWidget *btn_test_esp = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp, "clicked", G_CALLBACK(on_test_esp_moles_clicked), entries_moles);
    GtkWidget *btn_gen = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen, "clicked", G_CALLBACK(on_gen_moles_clicked), entries_moles);
   


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
   
   
    GtkWidget *label_p = gtk_label_new("P (#):");
    GtkWidget *entry_p = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_p), "Ej: 820000");
   
    GtkWidget *label_vt2 = gtk_label_new("Vt (mV):");
    GtkWidget *entry_vt2 = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_vt2), "Ej: 750.0");

   
    GtkWidget *entries_densidad[2] = {entry_p, entry_vt2};
    GtkWidget *btn_test_esp2 = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp2, "clicked", G_CALLBACK(on_test_esp_densidad_clicked), entries_densidad);
    GtkWidget *btn_gen2 = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen2, "clicked", G_CALLBACK(on_gen_densidad_clicked), entries_densidad);

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
     
    GtkWidget *label_a = gtk_label_new("A (#):");
    GtkWidget *entry_a = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_a), "Ej: 256");
   
    GtkWidget *label_w = gtk_label_new("W (#):");
    GtkWidget *entry_w = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_w), "Ej: 8500000");

   
    GtkWidget *entries_energia[2] = {entry_a, entry_w};
    GtkWidget *btn_test_esp3 = gtk_button_new_with_label("Test Específico");
    g_signal_connect(btn_test_esp3, "clicked", G_CALLBACK(on_test_esp_energia_clicked), entries_energia);
    GtkWidget *btn_gen3 = gtk_button_new_with_label("Generar Fichero");
    g_signal_connect(btn_gen3, "clicked", G_CALLBACK(on_gen_energia_clicked), entries_energia);

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

