// Funciones en punto fijo
// gcc puntofijo.c -o puntofijo -L. -lm
#include <stdio.h>
#include <math.h>
#include "../../include/gestion.h"


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

  GtkWidget *vp_moles, *vt_moles, *resultado_moles;

void on_moles_value_change(GtkWidget *widget, gpointer data) {


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

  GtkWidget *p_densidad, *vt_densidad, *resultado_densidad;

void on_densidad_value_change(GtkWidget *widget, gpointer data) {


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
  
GtkWidget *a_energia, *w_energia, *resultado_energia;

void on_energia_value_change(GtkWidget *widget, gpointer data) {


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
