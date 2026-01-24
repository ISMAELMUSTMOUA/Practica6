// Funciones en punto fijo
// gcc puntofijo.c -o puntofijo -L. -lm
#include <stdio.h>
#include <math.h>
#include "../../include/gestion.h"

/**** Private variables ****/
static GtkWidget *moles_window = NULL;
static GtkWidget *vp_entry_moles, *vt_entry_moles, *resultado_entry_moles, *filename_entry_moles;
static GtkWidget *vp_moles, *vt_moles, *resultado_moles;

static GtkWidget *densidad_window = NULL;
static GtkWidget *p_entry_densidad, *vt_entry_densidad, *resultado_entry_densidad, *filename_entry_densidad;
static GtkWidget *p_densidad, *vt_densidad, *resultado_densidad;

static GtkWidget *energia_window = NULL;
static GtkWidget *a_entry_energia, *w_entry_energia, *resultado_entry_energia, *filename_entry_energia;
static GtkWidget *a_energia, *w_energia, *resultado_energia;


/**** Private functions declarations ****/
static void on_guardar_moles_clicked(GtkWidget *widget, gpointer data);
static void on_guardar_cerrar_moles_clicked(GtkWidget *widget, gpointer data);
static void on_moles_value_change(GtkWidget *widget, gpointer data);

static void on_guardar_densidad_clicked(GtkWidget *widget, gpointer data);
static void on_guardar_cerrar_densidad_clicked(GtkWidget *widget, gpointer data);
static void on_densidad_value_change(GtkWidget *widget, gpointer data);

static void on_guardar_energia_clicked(GtkWidget *widget, gpointer data);
static void on_guardar_cerrar_energia_clicked(GtkWidget *widget, gpointer data);
static void on_energia_value_change(GtkWidget *widget, gpointer data);

static gchar* formatear_con_punto(double valor);


/**** Private functions definitions ****/
static void on_guardar_moles_clicked(GtkWidget *widget, gpointer data) {
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

static void on_guardar_cerrar_moles_clicked(GtkWidget *widget, gpointer data) {
   
    if(moles_window){
        gtk_widget_destroy(moles_window);
        moles_window = NULL;
    }        
}

static void on_moles_value_change(GtkWidget *widget, gpointer data) {


    const gchar *vp_text = gtk_entry_get_text(GTK_ENTRY(vp_moles));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(vt_moles));
    
    if (vp_text && vt_text && vp_text[0] && vt_text[0]) {
        double vp = atof(vp_text);
        double vt = atof(vt_text);
        const float R = 8.314; 
        const float V = 0.0025;
        float Vp_offset = (vp - 0.2);
        float ratio = 700.0 / 4.5;
        float P_kPa = Vp_offset * ratio;
        float P = P_kPa*1000;
        float Tc = vt/10;
        float T = Tc + 273.15;
        float n = (P*V)/(R*T);
        
        gchar *resultado_str = g_strdup_printf("%.10f", n);
        gtk_entry_set_text(GTK_ENTRY(resultado_moles), resultado_str);
        g_free(resultado_str);
    } else {
        gtk_entry_set_text(GTK_ENTRY(resultado_moles), "");
    }
}


static void on_guardar_densidad_clicked(GtkWidget *widget, gpointer data) {
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

static void on_guardar_cerrar_densidad_clicked(GtkWidget *widget, gpointer data) {
   
    if(densidad_window){
        gtk_widget_destroy(densidad_window);
        densidad_window = NULL;
    }        
}

static void on_densidad_value_change(GtkWidget *widget, gpointer data) {


    const gchar *p_text = gtk_entry_get_text(GTK_ENTRY(p_densidad));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(vt_densidad));
    
    if (p_text && vt_text && p_text[0] && vt_text[0]) {
        double dato_P = atof(p_text);
        double datoVt = atof(vt_text);
        const float C0 = 50000.0;
        const float C1 = 7500.0;
        const float R = 287.0;    // J/(kg·K)
        float P = (dato_P - C0)/C1;
        float Tc = (datoVt - 500)/10;
        float T = Tc + 273.15;
        float rho = P/(R*T);
        
        gchar *resultado_str = g_strdup_printf("%.10f", rho);
        gtk_entry_set_text(GTK_ENTRY(resultado_densidad), resultado_str);
        g_free(resultado_str);
    } else {
        gtk_entry_set_text(GTK_ENTRY(resultado_densidad), "");
    }
}


static void on_guardar_energia_clicked(GtkWidget *widget, gpointer data) {
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

static void on_guardar_cerrar_energia_clicked(GtkWidget *widget, gpointer data) {
   
    if(energia_window){
        gtk_widget_destroy(energia_window);
        energia_window = NULL;
    }        
}

static void on_energia_value_change(GtkWidget *widget, gpointer data) {


    const gchar *a_text = gtk_entry_get_text(GTK_ENTRY(a_energia));
    const gchar *w_text = gtk_entry_get_text(GTK_ENTRY(w_energia));
    
    if (a_text && w_text && a_text[0] && w_text[0]) {
        double dato_A = atof(a_text);
        double dato_W = atof(w_text);
        const float t = 1.0;      // Tiempo fijo en s
        float A_g = (dato_A*4)/1024;
        float A = A_g*9.81;
        float v = A * t;          // Velocidad en m/s
        float W_g = (dato_W-8388608)/16777;
        float m = W_g/1000;
        float Ek = (0.5)*m*(v*v);
        
        gchar *resultado_str = g_strdup_printf("%.10f", Ek);
        gtk_entry_set_text(GTK_ENTRY(resultado_energia), resultado_str);
        g_free(resultado_str);
    } else {
        gtk_entry_set_text(GTK_ENTRY(resultado_energia), "");
    }
}


static gchar* formatear_con_punto(double valor) {
    gchar *buffer = g_strdup_printf("%.10f", valor);
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == ',') {
            buffer[i] = '.';
        }
    }
    return buffer;
}


/**** Public functions definitions ****/
void on_test_esp_moles_clicked(GtkWidget *widget, gpointer data) {
    g_print("\nTest específico de Moles iniciado\n");
    GtkWidget **entries = (GtkWidget **)data;
    GtkWidget *entry_vp = entries[0];
    GtkWidget *entry_vt = entries[1];

    const gchar *vp_text = gtk_entry_get_text(GTK_ENTRY(entry_vp));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(entry_vt));
    
    if (vp_text && vt_text && vp_text[0] && vt_text[0]) {
        double vp = atof(vp_text);
        double vt = atof(vt_text);
        uint32_t resultado;
        // Límites realistas de entrada
        if (vp <0.2  || vp > 4.7) {
            fprintf(stderr, "Error: Vp fuera de rango [0.2, 4.7] V\n");
            resultado = 0;
            return;
        }
        if (vt < 0 || vt > 1500.0) {
            fprintf(stderr, "Error: Vt fuera de rango [0.0, 1500.0] mV\n");
            resultado = 0;
            return;
        }

        uint8_t tams[3];
        tam_moles(tams);

        double dato1 = (vp*(1<<tams[0]));
        double dato2 = (vt*(1<<tams[1]));
        calc_moles_fx(dato1, dato2, &resultado);
        printf("\nResultado de prueba específica: %f\n",(float)resultado/(1<<tams[2]));
    }
}

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
    gtk_widget_set_tooltip_text(GTK_WIDGET(vp_entry_moles), "entry_vp_moles");
    vp_moles = vp_entry_moles;
    gtk_box_pack_start(GTK_BOX(vbox), label_vp, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), vp_entry_moles, FALSE, FALSE, 0);


    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    vt_entry_moles = gtk_entry_new();
    gtk_widget_set_tooltip_text(GTK_WIDGET(vt_entry_moles), "entry_vt_moles");
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


void on_test_esp_densidad_clicked(GtkWidget *widget, gpointer data) {
    g_print("Test específico de Densidad iniciado\n");
    GtkWidget **entries = (GtkWidget **)data;
    GtkWidget *entry_p = entries[0];
    GtkWidget *entry_vt = entries[1];

    const gchar *p_text = gtk_entry_get_text(GTK_ENTRY(entry_p));
    const gchar *vt_text = gtk_entry_get_text(GTK_ENTRY(entry_vt));
    
    if (p_text && vt_text && p_text[0] && vt_text[0]) {
        double p = atof(p_text);
        double vt = atof(vt_text);
        uint32_t resultado;
        
        // Rango realista de presión y voltaje de temperatura
        if (p < 50e3 || p > 1e6) {
            fprintf(stderr, "Error: Presión fuera de rango [ 50e3, 1e6]\n");
            resultado = 0;
            return;
        }
        if (vt < 500.0 || vt > 1750.0) {
            fprintf(stderr, "Error: Vt fuera de rango [500.0, 1750.0]\n");
            resultado = 0;
            return;
        }

        uint8_t tams[3];
        tam_dens(tams);
        double dato1 = (p*(1<<tams[0]));
        double dato2 = (vt*(1<<tams[1]));

        calc_density_fx(dato1, dato2, &resultado);
        printf("\nResultado de prueba específica: %f\n",(float)resultado/(1<<tams[2]));
    }

}

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
    gtk_widget_set_tooltip_text(GTK_WIDGET(p_entry_densidad), "entry_p_densidad");
    p_densidad = p_entry_densidad;
    gtk_box_pack_start(GTK_BOX(vbox), label_p, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), p_entry_densidad, FALSE, FALSE, 0);


    GtkWidget *label_vt = gtk_label_new("Vt (mV):");
    vt_entry_densidad = gtk_entry_new();
    gtk_widget_set_tooltip_text(GTK_WIDGET(vt_entry_densidad), "entry_vt_densidad");
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


void on_test_esp_energia_clicked(GtkWidget *widget, gpointer data) {
    g_print("Test específico de Energía Cinética iniciado\n");
    GtkWidget **entries = (GtkWidget **)data;
    GtkWidget *entry_a = entries[0];
    GtkWidget *entry_w = entries[1];

    const gchar *a_text = gtk_entry_get_text(GTK_ENTRY(entry_a));
    const gchar *w_text = gtk_entry_get_text(GTK_ENTRY(entry_w));
    
    if (a_text && w_text && a_text[0] && w_text[0]) {
        double a = atof(a_text);
        double w = atof(w_text);
        uint32_t resultado;

        // Límites razonables
        if (a < 0.0 || a > 500.0) {
            fprintf(stderr, "Error: Aceleración fuera de rango [0, 500]\n");
            resultado = 0;
            return;
        }
        if (w < 8388608 || w > 12e6) {
            fprintf(stderr, "Error: Peso fuera de rango [8388608, 12e6]\n");
            resultado = 0;
            return;
        }

    
        uint8_t tams[3];
        tam_ener(tams);
        double dato1 = (a*(1<<tams[0]));
        double dato2 = (w*(1<<tams[1]));

        calc_kinetic_energy_fx(dato1, dato2, &resultado);
        printf("\nResultado de prueba específica: %f\n",(float)resultado/(1<<tams[2]));
    }

}

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
    gtk_widget_set_tooltip_text(GTK_WIDGET(a_entry_energia), "entry_a_energia");
    a_energia = a_entry_energia;
    gtk_box_pack_start(GTK_BOX(vbox), label_a, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), a_entry_energia, FALSE, FALSE, 0);


    GtkWidget *label_w = gtk_label_new("W (#):");
    w_entry_energia = gtk_entry_new();
    gtk_widget_set_tooltip_text(GTK_WIDGET(w_entry_energia), "entry_w_energia");
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