#ifndef GESTION
#define GESTION

#include <gtk/gtk.h>
#include <stdint.h>
#include "puntofijo.h"

/**** Public functions declarations ****/
extern void on_test_esp_moles_clicked(GtkWidget *widget, gpointer data);
extern void on_test_auto_moles_clicked(GtkWidget *widget, gpointer data);
extern void on_gen_moles_clicked(GtkWidget *widget, gpointer data);

extern void on_test_esp_densidad_clicked(GtkWidget *widget, gpointer data);
extern void on_test_auto_densidad_clicked(GtkWidget *widget, gpointer data);
extern void on_gen_densidad_clicked(GtkWidget *widget, gpointer data);

extern void on_test_esp_energia_clicked(GtkWidget *widget, gpointer data);
extern void on_test_auto_energia_clicked(GtkWidget *widget, gpointer data);
extern void on_gen_energia_clicked(GtkWidget *widget, gpointer data);

#endif
