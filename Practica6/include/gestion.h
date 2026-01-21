#ifndef GESTION
#define GESTION

#include <gtk/gtk.h>
#include <stdint.h>
#include "puntofijo.h"


void on_test_esp_moles_clicked(GtkWidget *widget, gpointer data);
void on_moles_value_change(GtkWidget *widget, gpointer data);

void on_test_esp_densidad_clicked(GtkWidget *widget, gpointer data);
void on_densidad_value_change(GtkWidget *widget, gpointer data);

void on_test_esp_energia_clicked(GtkWidget *widget, gpointer data);
void on_energia_value_change(GtkWidget *widget, gpointer data);

#endif
