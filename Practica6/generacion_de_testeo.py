#!/usr/bin/env python3

import ctypes
import math
import random
import os

print("=" * 70)
print("   GENERADOR DE TESTS PARA PUNTO FIJO CON TOLERANCIA DEL 6%")
print("=" * 70)

# 1. Compilar las librerías automáticamente
#    Asegura que el script use tu último código C (puntofijo.c)
print("Compilando librerías...")
if os.path.exists("./build/libpuntofijo.so"):
    os.remove("./build/libpuntofijo.so") # Borrar anterior para forzar recompilación

os.system("gcc -shared -fPIC -o ./build/libpuntofijo.so ./src/c/puntofijo.c -lm")

if not os.path.exists("./build/libcomaflotante.so"):
    os.system("gcc -shared -fPIC -o ./build/libcomaflotante.so ./src/c/comaflotante.c -lm")

# 2. Cargar librerías
lib_float = ctypes.CDLL("./build/libcomaflotante.so")
lib_fx = ctypes.CDLL("./build/libpuntofijo.so")    
# Configurar firmas de funciones (Tipos de argumentos y retorno)
lib_float.calc_moles.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_moles.restype = ctypes.c_float
lib_float.calc_density.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_density.restype = ctypes.c_float
lib_float.calc_kinetic_energy.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_kinetic_energy.restype = ctypes.c_float

lib_fx.tam_moles.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_dens.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_ener.argtypes = [ctypes.POINTER(ctypes.c_uint8)]

# 3. Funciones auxiliares
def get_scalings(func_name):
    tams_array = (ctypes.c_uint8 * 3)()
    if func_name == 'moles':
        lib_fx.tam_moles(tams_array)
    elif func_name == 'density':
        lib_fx.tam_dens(tams_array)
    elif func_name == 'energy':
        lib_fx.tam_ener(tams_array)
    # Devuelve (Escala Entrada 1, Escala Entrada 2, Escala Salida)
    return (tams_array[0], tams_array[1], tams_array[2])

def float_to_fixed(value, fractional_bits):
    if math.isnan(value) or math.isinf(value):
        return 0
    # Multiplicar por 2^n
    scaled = value * (1 << fractional_bits)
    
    # Saturación simple para evitar overflow de 32 bits
    if scaled > 4294967295: # Max uint32
        return 4294967295
    elif scaled < 0:
        return 0
    return int(scaled)

# 4. DEFINICIÓN DE RANGOS DE VALIDEZ 
RANGES = {
    'moles': {
        'Vp_min': 0.2, 'Vp_max': 4.7, 
        'Vt_min': 0.0, 'Vt_max': 1500.0
    },
    'density': {
        'P_min': 50000.0, 'P_max': 1100000.0, 
        'Vt_min': 500.0,  'Vt_max': 1750.0     
    },
    'energy': {
        'A_min': 0.0,      'A_max': 500.0, 
        'W_min': 8388608.0,'W_max': 12000000.0
    }
}

def in_range(func_name, val1, val2):
    r = RANGES[func_name]
    if func_name == 'moles':
        return (r['Vp_min'] <= val1 <= r['Vp_max']) and (r['Vt_min'] <= val2 <= r['Vt_max'])
    elif func_name == 'density':
        return (r['P_min'] <= val1 <= r['P_max']) and (r['Vt_min'] <= val2 <= r['Vt_max'])
    elif func_name == 'energy':
        # Nota: A > 0.0 en estricto, pero >= se acepta si el código C lo maneja
        return (r['A_min'] <= val1 <= r['A_max']) and (r['W_min'] <= val2 <= r['W_max'])
    return False

# =========================================================================
# GENERACIÓN DE CASOS DE PRUEBA
# =========================================================================

# --- 1. MOLES ---
print("\n1. Generando casos para MOLES...")
esc_Vp, esc_Vt, esc_res = get_scalings('moles')
print(f"    -> Detectada escala de salida: Q{esc_res}")
moles_cases = []

# Casos fijos y frontera (ya existentes)
base_moles = [
    (0.2, 0.2, "table_min"), (4.7, 1500.0, "table_max"),
    (2.35, 298.0, "normal_case"), (0.19, 100.0, "err_Vp_low"),
    (5.0, 100.0, "err_Vp_high"), (2.0, 1600.0, "err_Vt_high")
]
# Añadir aleatorios para completar 20
for i in range(14):
    Vp = random.uniform(0.2, 4.7)
    Vt = random.uniform(0.0, 1500.0)
    base_moles.append((Vp, Vt, f"random_{i}"))

for Vp, Vt, name in base_moles:
    if in_range('moles', Vp, Vt):
        res_float = lib_float.calc_moles(ctypes.c_float(Vp), ctypes.c_float(Vt))
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
    moles_cases.append(f"{float_to_fixed(Vp, esc_Vp)} {float_to_fixed(Vt, esc_Vt)} {exp_fx} {name}")

with open("tests/data/moles_test_cases.txt", "w") as f:
    f.write(f"# Moles scale: {esc_res}\n")
    for c in moles_cases: f.write(c + "\n")
print(f"  ✓ {len(moles_cases)} casos generados.")


# --- 2. DENSIDAD (20 Casos) ---
print("\n2. Generando casos para DENSIDAD...")
esc_P, esc_Vt, esc_res = get_scalings('density')
print(f"    -> Detectada escala de salida: Q{esc_res}")
density_cases = []

# A. Tabla ORIGINAL (4 casos)
tabla_density = [
    (820000.0, 750.0, "table_820k_750"),
    (4100000.0, 75000.0, "table_4.1M_inv"),    
    (5100000.0, 79800.0, "table_5.1M_inv"),   
    (10000000.0, 159600.0, "table_10M_inv"), 
]

# B. Valores Frontera (5 casos)
boundary_density = [
    (50000.0, 750.0, "bound_P_min"),       
    (1100000.0, 750.0, "bound_P_max"),     
    (820000.0, 500.0, "bound_Vt_min"),      
    (820000.0, 1750.0, "bound_Vt_max"),    
    (49999.0, 750.0, "bound_P_below") 
]

# C. Aleatorios Válidos (11 casos para llegar a 20)
random_density = []
for i in range(11):
    P = random.uniform(50000.0, 1100000.0)
    Vt = random.uniform(500.0, 1750.0)     
    random_density.append((P, Vt, f"rand_valid_{i}"))

# Procesar todos
all_density_inputs = tabla_density + boundary_density + random_density

for P, Vt, name in all_density_inputs:
    if in_range('density', P, Vt):
        res_float = lib_float.calc_density(ctypes.c_float(P), ctypes.c_float(Vt))
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
    
    density_cases.append(f"{float_to_fixed(P, esc_P)} {float_to_fixed(Vt, esc_Vt)} {exp_fx} {name}")

with open("tests/data/density_test_cases.txt", "w") as f:
    f.write(f"# Density scale: {esc_res}\n")
    for c in density_cases: f.write(c + "\n")
print(f"  ✓ {len(density_cases)} casos generados.")


# --- 3. ENERGÍA (20 Casos - ESCALA Q26 AUTOMÁTICA) ---
print("\n3. Generando casos para ENERGÍA...")
# get_scalings leerá '26' de tu librería C compilada
esc_A, esc_W, esc_res = get_scalings('energy') 
print(f"    -> Detectada escala de salida: Q{esc_res}")

energy_cases = []

# A. Tabla ORIGINAL (4 casos)
tabla_energy = [
    (256.0, 8500000.0, "table_256_8.5M"),
    (768.0, 10000000.0, "table_768_inv"),   
    (1024.0, 11000000.0, "table_1024_inv"), 
    (1280.0, 12000000.0, "table_1280_inv"), 
]

# B. Valores Frontera (5 casos)
boundary_energy = [
    (0.1, 9000000.0, "bound_A_min"),        
    (500.0, 9000000.0, "bound_A_max"),      
    (250.0, 8388608.0, "bound_W_min"),      
    (250.0, 12000000.0, "bound_W_max"),     
    (500.1, 9000000.0, "bound_A_over") 
]

# C. Aleatorios Válidos (11 casos para llegar a 20)
random_energy = []
for i in range(11):
    A = random.uniform(1.0, 500.0) 
    W = random.uniform(8388608.0, 12000000.0)
    random_energy.append((A, W, f"rand_valid_{i}"))

# Procesar todos
all_energy_inputs = tabla_energy + boundary_energy + random_energy

for A, W, name in all_energy_inputs:
    if in_range('energy', A, W):
        res_float = lib_float.calc_kinetic_energy(ctypes.c_float(A), ctypes.c_float(W))
        # Aquí se usa esc_res (que será 26) para convertir el float esperado a fijo
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
        
    energy_cases.append(f"{float_to_fixed(A, esc_A)} {float_to_fixed(W, esc_W)} {exp_fx} {name}")

with open("tests/data/energy_test_cases.txt", "w") as f:
    f.write(f"# Energy scale: {esc_res}\n")
    for c in energy_cases: f.write(c + "\n")
print(f"  ✓ {len(energy_cases)} casos generados.")

print("\n" + "="*70)
print("¡GENERACIÓN COMPLETADA!")
print("Ejecuta 'pytest pytest_cap1.py -v' para validar tu código C.")
print("="*70)