// Funciones en punto fijo
// gcc puntofijo.c -o puntofijo -L. -lm
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "../../include/puntofijo.h"

#define FX_MUL(a,b,c) (((uint64_t)(a) * (uint64_t)(b)) >> c)
#define FX_DIV(a,b,c) (((uint64_t)(a) << c) / (uint64_t)(b))
#define FX_SQRT(x,c) punto_fijo_sqrt(x,c)

uint32_t punto_fijo_sqrt(uint32_t valor,uint8_t escalado) {
    if (valor <= 0) return 0;  
    uint32_t resultado = valor / 2;
    uint32_t anterior = 0;    
    while (resultado != anterior) {
        anterior = resultado;
        resultado = (anterior + FX_DIV(valor, anterior,escalado)) / 2;
    }    
    return resultado;
}

void tam_moles(uint8_t * tams){
  tams[0]=28;
  tams[1]=20;
  tams[2]=26;
}

void tam_dens(uint8_t * tams){
  tams[0]=8;
  tams[1]=8;
  tams[2]=20;
}

void tam_ener(uint8_t * tams){
  tams[0]=8;
  tams[1]=8;
  tams[2]=26;
}

/* Número de Moles con Ley de Gases Ideales */
void calc_moles_fx(uint32_t Vp_fx,uint32_t Vt_fx,uint32_t *resultado) {

    uint8_t tams[3];
    tam_moles(tams);
    uint32_t esc_Vp=tams[0];
    uint32_t esc_Vt=tams[1];

    //Generamos las variables de entrada para los cálculos en coma flotante  
    float Vp=(float)Vp_fx/(1<<esc_Vp);
    float Vt=(float)Vt_fx/(1<<esc_Vt);
    

    //Cálculos en coma flotante (copiar desde comaflotante.c)
    const float R = 8.314; 
    const float V = 0.0025;
    float Vp_offset = (Vp - 0.2);
    float ratio = 700.0 / 4.5;
    float P_kPa = Vp_offset * ratio;
    float P = P_kPa*1000;
    float Tc = Vt/10;
    float T = Tc + 273.15;
    float n = (P*V)/(R*T);

    //Cálculos en punto fijo 
    const uint32_t R_fx = round(8.314 * (1<<8));
    const uint32_t V_fx = round(0.0025 * ((uint64_t)1<<(uint64_t)(tams[2])));   
    const uint32_t K_ADJ = round(273.15 * (1<<8)); 
    
    uint32_t Vp_offset_fx = (Vp_fx - 53687091) >> 20;
    uint32_t ratio_fx = 39822;
    uint32_t P_kPa_fx = FX_MUL(Vp_offset_fx, ratio_fx, 8);
    uint32_t P_fx = FX_MUL(P_kPa_fx, 256000, 8); 
    uint32_t Tc_fx = FX_DIV(Vt_fx, 40960, 0);
    uint32_t T_fx = Tc_fx + K_ADJ;
    uint32_t n_fx = FX_DIV(FX_MUL(P_fx, V_fx, tams[2]), FX_MUL(R_fx, T_fx, 8), tams[2]);
  
    printf("Escalado Vp: %d     Escalado Vt: %d\n",esc_Vp,esc_Vt);
    printf("Vp:        %f    (guarda %.2f bits)\n",Vp,32-log2(Vp_fx));
    printf("Vt:        %f    (guarda %.2f bits)\n",Vt,32-log2(Vt_fx));    
    printf("Vp_offset: %f,%f (guarda %.2f bits)\n",Vp_offset,(float)Vp_offset_fx/(1<<8),32-log2(Vp_offset_fx));    
    printf("ratio:     %f,%f (guarda %.2f bits)\n",ratio,(float)ratio_fx/(1<<8),32-log2(ratio_fx));    
    printf("P_kPa:     %f,%f (guarda %.2f bits)\n",P_kPa,(float)P_kPa_fx/(1<<8),32-log2(P_kPa_fx));    
    printf("P:         %f,%f (guarda %.2f bits)\n",P,(float)P_fx/(1<<8),32-log2(P_fx));
    printf("Tc:        %f,%f (guarda %.2f bits)\n",Tc,(float)Tc_fx/(1<<8),32-log2(Tc_fx));
    printf("T:         %f,%f (guarda %.2f bits)\n",T,(float)T_fx/(1<<8),32-log2(T_fx));    
    printf("n:         %f,%f (guarda %.2f bits)\n",n,(float)n_fx/(1<<tams[2]),32-log2(n_fx));
        
    *resultado = n_fx;     
    
  
}

/* Densidad del Aire con Presión y Temperatura */
void calc_density_fx(uint32_t dato_P, uint32_t datoVt,uint32_t *resultado) {




    uint8_t tams[3];
    tam_dens(tams);

  //punto flotante 
    const float C0 = 50000.0;
    const float C1 = 7500.0;
    const float R = 287.0;    // J/(kg·K)

    float P_float = dato_P/(float)(1<<tams[0]); // Convertir a flotante
    float P = (P_float - C0)/C1;

    float Vt_float = datoVt/(float)(1<<tams[1]); // Convertir a flotante
    float Tc = (Vt_float - 500)/10;
    float T = Tc + 273.15;
    float B = R*T;
    float rho = P/(R*T);
    
    //punto fijo  
    const uint64_t C0_fx = 50000.0*(1<<8);
    const uint32_t C1_fx = 7500.0*(1<<8);
    const uint32_t R_fx  = 287.0 *(1<<4);
  
    uint32_t P_fx = FX_DIV(dato_P, C1_fx, 8) - FX_DIV(C0_fx, C1_fx, 8); 
    uint32_t Tc_fx = FX_DIV((datoVt - (500*(1<<8))), (10*(1<<8)), 8); 
    uint32_t T_fx = Tc_fx + 273.15*(1<<8);
    uint32_t T_fx_esc = T_fx >> 4;

    uint32_t B_fx = FX_MUL(T_fx_esc, R_fx, 0);
    uint32_t rho_fx =  FX_DIV(P_fx, B_fx, tams[2]);

    // --- Resultados y comparación ---
    printf("Escalado P: %d     Escalado Vt: %d\n",tams[0],tams[1]);
    printf("Vt:       %f (guarda %.2f bits)\n",Vt_float,32-log2(Vt_float));
    printf("P_entra:  %f (guarda %.2f bits)\n",P_float,32-log2(P_float)); 
    printf("P:     %f,%f (guarda %.2f bits)\n",P,(float)P_fx/(1<<8),32-log2(P_fx));    
    printf("Tc:    %f,%f (guarda %.2f bits)\n",Tc,(float)Tc_fx/(1<<8),32-log2(Tc_fx)); 
    printf("T:     %f,%f (guarda %.2f bits)\n",T,(float)T_fx/(1<<8),32-log2(T_fx)); 
    printf("B:     %f,%f (guarda %.2f bits)\n",B,(float)B_fx/(1<<8),32-log2(B_fx));     
    printf("rho:   %f,%f (guarda %.2f bits)\n",rho,(float)rho_fx/(1<<tams[2]),32-log2(rho_fx));
    *resultado = rho_fx;


}

/* Energía Cinética con Aceleración y Peso */
void calc_kinetic_energy_fx(uint32_t dato_A, uint32_t dato_W,uint32_t *resultado) {
  uint8_t tams[3];
  tam_ener(tams);
  //punto flotante
    const float t = 1.0;      // Tiempo fijo en s

    float A_float = dato_A/(float)(1<<8);
    float A_g = (A_float*4)/1024;
    float A = A_g*9.81;
    float v = A * t;          // Velocidad en m/s

    float W_float = dato_W/(float)(1<<8);
    float W_g = (W_float-8388608)/16777;
    float m = W_g/1000;
    float v2=v*v;
    float Ek = (0.5)*m*(v*v);

  //punto fijo
  const uint32_t T_fx = 1 << 8;                    // t = 1.0 [8:8]
  const uint32_t CONST_4_fx = 4 *(1 << 8);              // 4.0 [8:8]
  const uint32_t CONST_9_81_fx = round(9.81 * (1<<8));
  const uint32_t CONST_1024_fx = 1024 *(1 << 8);
  const uint32_t CONST_16777_fx = 16777 *(1 << 8);
  const uint32_t CONST_fx = 8388608;
  const uint32_t CONST_1000_fx = 1000 << 8;
  const uint32_t HALF_fx = (1 << 7);               // 0.5 en [8:8]

  // A_g = (dato_A * 4) / 1024
  uint32_t A_g_fx = FX_DIV(FX_MUL(dato_A, CONST_4_fx, 8), CONST_1024_fx, 16);
  // A = A_g * 9.81
  uint32_t A_fx = FX_MUL(A_g_fx, CONST_9_81_fx, 8);
  // v = A * t
  uint32_t v_fx = FX_MUL(A_fx, T_fx, 8);

  // m = w_g/1000
  uint32_t diff_W_fx = dato_W - CONST_fx*(1<<8);
  uint32_t w_tmp_fx = FX_DIV(diff_W_fx, CONST_16777_fx, 16);
  uint32_t m_fx = FX_DIV(w_tmp_fx, CONST_1000_fx, 8);

  // Ek = 0.5 * m * v^2
  uint32_t v2_fx = FX_MUL(v_fx, v_fx, 12);
  uint32_t Ek_fx = FX_MUL(FX_MUL(HALF_fx, m_fx, 8), v2_fx, 10);

  // --- Resultados y comparación ---
  printf("Escalado A:   %d     Escalado W: %d\n",tams[0],tams[1]);
  printf("A_entr:       %f (guarda %.2f bits)\n",A_float,32-log2(A_float));
  printf("W_entra:      %f (guarda %.2f bits)\n",W_float,32-log2(W_float)); 
  printf("A:  %f,%f (guarda %.2f bits)\n", A, (float)A_fx/(1<<16), 32-log2(A_fx));
  printf("v:  %f,%f (guarda %.2f bits)\n", v, (float)v_fx/(1<<16), 32-log2(v_fx));
  printf("W_g:  %f,%f (guarda %.2f bits)\n", W_g, (float)w_tmp_fx/(1<<16), 32-log2(w_tmp_fx));
  printf("m:  %f,%f (guarda %.2f bits)\n", m, (float)m_fx/(1<<16), 32-log2(m_fx));
  printf("V^2:  %f,%f (guarda %.2f bits)\n", v2, (float)v2_fx/(1<<20), 32-log2(v2_fx));
  printf("Ek: %f,%f (guarda %.2f bits)\n", Ek, (float)Ek_fx/(1<<tams[2]), 32-log2(Ek_fx));

  *resultado = Ek_fx;
  
}
