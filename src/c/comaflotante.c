//Implementación en coma flotante
//gcc comaflotante.c -o comaflotante -lm
#include <stdio.h>
#include <math.h>
#include "../../include/comaflotante.h"

/* Número de Moles con Ley de Gases Ideales */
float calc_moles(float Vp, float Vt) {

    const float R = 8.314;    // J/(mol·K)
    const float V = 0.0025;   // m^3 (2.5 L)
    

    float Vp_offset = (Vp - 0.2);
    float ratio = 700.0 / 4.5;
    float P_kPa = Vp_offset * ratio;
    float P = P_kPa*1000; 

    float Tc = Vt/10;
    float T = Tc + 273.15;

    float n = (P*V)/(R*T);
    return n;
}

/* Densidad del Aire con Presión y Temperatura */
float calc_density(float P_raw, float Vt) {
    const float C0 = 50000.0;
    const float C1 = 7500.0;
    const float R = 287.0;    // J/(kg·K)

    float P = (P_raw-C0)/C1;

    float Tc = (Vt-500)/10 ;
    float T = Tc + 273.15;

    float rho = P/(R*T);
    return rho;
}

/* Energía Cinética con Aceleración y Peso */
float calc_kinetic_energy(float A_raw, float W_raw) {
    const float t = 1.0;      // Tiempo fijo en s

    float A_g = (A_raw*4)/1024;
    float A = A_g*9.81;
    float v = A * t;          // Velocidad en m/s

    float W_g = (W_raw-8388608)/16777;
    float m = W_g/1000;
    
    float Ek = (0.5)*m*(v*v);
    return Ek;
}

/* Probar las funciones con datos típicos*/
int main() {
    float Vp = 2.35;        // V
    float Vt_moles = 298.0; // mV
    float n = calc_moles(Vp, Vt_moles);
    printf("Numero de moles: %.6f mol\n", n);

    float P_raw = 820000.0; // Cuentas
    float Vt_density = 750.0; // mV
    float rho = calc_density(P_raw, Vt_density);
    printf("Densidad del aire: %.6f kg/m^3\n", rho);

    float A_raw = 256.0;    // Cuentas
    float W_raw = 8500000.0;// Cuentas
    float Ek = calc_kinetic_energy(A_raw, W_raw);
    printf("Energia cinetica: %.6f J\n", Ek);

    return 0;
}
