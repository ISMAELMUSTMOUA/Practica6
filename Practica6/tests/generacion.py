#!/usr/bin/env python3

import ctypes
import math
import random
import os
import sys


so_path = "./build/libpuntofijo.so"
if not os.path.exists(so_path):
    raise FileNotFoundError(f"No se encontró el archivo {so_path}")

lib_fx = ctypes.CDLL(so_path)

lib_fx.tam_moles.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_dens.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_ener.argtypes = [ctypes.POINTER(ctypes.c_uint8)]

so_path_2 = "./build/libcomaflotante.so"
if not os.path.exists(so_path_2):
    raise FileNotFoundError(f"No se encontró el archivo {so_path_2}")

lib_float = ctypes.CDLL(so_path_2)

lib_float.calc_moles.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_moles.restype = ctypes.c_float
lib_float.calc_density.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_density.restype = ctypes.c_float
lib_float.calc_kinetic_energy.argtypes = [ctypes.c_float, ctypes.c_float]
lib_float.calc_kinetic_energy.restype = ctypes.c_float



def get_scalings(func_name):
    tams_array = (ctypes.c_uint8 * 3)()
    if func_name == 'moles':
        lib_fx.tam_moles(tams_array)
    elif func_name == 'density':
        lib_fx.tam_dens(tams_array)
    elif func_name == 'energy':
        lib_fx.tam_ener(tams_array)
    return (tams_array[0], tams_array[1], tams_array[2])

def float_to_fixed(value, fractional_bits):
    if math.isnan(value) or math.isinf(value):
        return 0
    
    max_int = 0xFFFFFFFF  
    min_int = 0
    
    scaled = value * (1 << fractional_bits)
    
    if scaled > max_int:
        return max_int
    elif scaled < min_int:
        return min_int
    return int(scaled)

RANGOS = {
    'moles': {'Vp_min': 0.2, 'Vp_max': 4.7, 'Vt_min': 0.0, 'Vt_max': 1500.0},
    'density': {'P_min': 50000.0, 'P_max': 110000.0, 'Vt_min': 100.0, 'Vt_max': 1750.0},
    'energy': {'A_min': 0, 'A_max': 512.0, 'W_min': 8388608.0, 'W_max': 16777216}
}

def in_range(func_name, val1, val2):
    r = RANGOS[func_name]
    if func_name == 'moles':
        return (r['Vp_min'] <= val1 <= r['Vp_max']) and (r['Vt_min'] <= val2 <= r['Vt_max'])
    elif func_name == 'density':
        return (r['P_min'] <= val1 <= r['P_max']) and (r['Vt_min'] <= val2 <= r['Vt_max'])
    elif func_name == 'energy':
        return (r['A_min'] <= val1 <= r['A_max']) and (r['W_min'] <= val2 <= r['W_max'])
    return False

esc_Vp, esc_Vt, esc_res = get_scalings('moles')
moles_cases = []

tabla_moles = [
    (0.2, 0.2, "min"),
    (1, 200.0, "caso_normal"),
    (2.35, 298.0, "caso_normal_2"),
    (4.7, 1500.0, "max")
    
]

limites_moles = [
    (0.19, 100.0, "limite_Vp_low"),
    (5.0, 100.0, "limite_Vp_high"),
    (2.0, 1600.0, "limite_Vt_high"),
    (2.5, 0.0, "limite_Vt_min")
]


especiales_moles = [
    (sys.float_info.max, sys.float_info.max, "MAX_float"),
    (float('inf'), float('inf'), "infinito"),
    (float('-inf'), float('-inf'), "menos_infinito"),
    (float('nan'), 100.0, "NAN_val"),
    ((0xFFFFFFFF+1) , (0xFFFFFFFF+1), "INT_overflow"),
    (0.0,0.0, "cero"),
    (1.0,1.0, "uno")
]

random_moles = []
for i in range(300):
    Vp = random.uniform(0.2, 4.5)
    Vt = random.uniform(0.0, 1500.0)
    random_moles.append((Vp, Vt, f"rand_wide_{i}"))

random_moles2 = []
for j in range(300):
    Vp = random.uniform(1.5, 3)
    Vt = random.uniform(500, 1000.0)
    random_moles2.append((Vp, Vt, f"rand_narrow_{j}"))

all_moles_inputs = tabla_moles + limites_moles + especiales_moles + random_moles + random_moles2

for Vp, Vt, name in all_moles_inputs:
    if in_range('moles', Vp, Vt):
        res_float = lib_float.calc_moles(ctypes.c_float(Vp), ctypes.c_float(Vt))
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
    moles_cases.append((float_to_fixed(Vp, esc_Vp), float_to_fixed(Vt, esc_Vt), exp_fx, name))

with open("tests/data/moles_test_cases.txt", "w") as f:
    f.write(f"{'#Vp':^10} {'Vt':^10} {'Resultado':^12} {'Tipo':^16}\n")
    
    for vp, vt, res, name in moles_cases:
        f.write(f"{vp:>10} {vt:>10} {res:>12} {name:<20}\n")
print(f"\nSe han generado {len(moles_cases)} casos de moles")

esc_P, esc_Vt, esc_res = get_scalings('density')
density_cases = []

tabla_density = [
    (50000.0, 750.0, "limite_P_min"),  
    (82000.0, 100.0, "limite_Vt_min"),  
    (82000.0, 750.0, "caso_normal_1"),
    (41000.0, 750.0, "caso_normal_2"),    
    (51000.0, 780.0, "caso_normal_3"),   
    (110000.0, 750.0, "limite_P_max"),     
    (82000.0, 1750.0, "limite_Vt_max"), 
]

limites_density = [
   
    (49999.0, 750.0, "limite_P_below"),
    (82000.0, 99.0, "limite_Vt_below"),
    (110001.0, 750.0, "limite_P_above"),
    (82000.0, 1751.0, "limite_Vt_above")

]

especiales_dens = [
    (sys.float_info.max, sys.float_info.max, "MAX_float"),
    (float('inf'), float('inf'), "infinito"),
    (float('-inf'), float('-inf'), "menos_infinito"),
    (float('nan'), 100.0, "NAN_val"),
    ((0xFFFFFFFF+1) , (0xFFFFFFFF+1), "INT_overflow"),
    (0.0,0.0, "cero"),
    (1.0,1.0, "uno")
]

random_density = []
for i in range(300):
    P = random.uniform(50000.0, 110000.0)
    Vt = random.uniform(250.0, 1750.0)     
    random_density.append((P, Vt, f"rand_wide_{i}"))

random_density2 = []
for i in range(300):
    P = random.uniform(75000.0, 100000.0)
    Vt = random.uniform(500.0, 1500.0)     
    random_density.append((P, Vt, f"rand_narrow_{i}"))

all_density_inputs = tabla_density + limites_density + especiales_dens + random_density + random_density2

for P, Vt, name in all_density_inputs:
    if in_range('density', P, Vt):
        res_float = lib_float.calc_density(ctypes.c_float(P), ctypes.c_float(Vt))
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
    density_cases.append((float_to_fixed(P, esc_P), float_to_fixed(Vt, esc_Vt), exp_fx, name))

with open("tests/data/density_test_cases.txt", "w") as f:
    f.write(f"{'#P':^11} {'Vt':^10} {'Resultado':^12} {'Tipo':^15}\n")
    
    for P_fixed, Vt_fixed, res, name in density_cases:
        f.write(f"{P_fixed:>7} {Vt_fixed:>10} {res:>12} {name:<20}\n")
print(f" Se han generado {len(density_cases)} casos de densidad")

esc_A, esc_W, esc_res = get_scalings('energy')
energy_cases = []

tabla_energy = [
    (0.1, 9000000.0, "limite_A_min"),        
    (128.0, 8388608.0, "limite_W_min"),      
    (256.0, 8500000.0, "caso_normal_1"),
    (128.0, 10000000.0, "caso_normal_2"),   
    (256.0, 11000000.0, "caso_normal_3"), 
    (512.0, 9000000.0, "limite_A_max"), 
    (250.0, 16777216.0, "limite_W_max"), 
]

limites_energy = [
    (0.0, 9000000.0, "limite_A_below"),
    (513.0, 9000000.0, "limite_A_over"),
    (256, 8388607.0, "limite_W_below"),
    (513.0, 16777217.0, "limite_W_over")

]

especiales_ener= [
    (sys.float_info.max, sys.float_info.max, "MAX_float"),
    (float('inf'), float('inf'), "infinito"),
    (float('-inf'), float('-inf'), "menos_infinito"),
    (float('nan'), 100.0, "NAN_val"),
    ((0xFFFFFFFF+1) , (0xFFFFFFFF+1), "INT_overflow"),
    (0.0,0.0, "cero"),
    (1.0,1.0, "uno")
]


random_energy = []
for i in range(300):
    A = random.uniform(0, 512) 
    W = random.uniform(8388608.0, 16777216.0)
    random_energy.append((A, W, f"rand_wide_{i}"))

random_energy2 = []
for i in range(300):
    A = random.uniform(128, 256) 
    W = random.uniform(8500000.0, 12000000.0)
    random_energy.append((A, W, f"rand_narrow_{i}"))

all_energy_inputs = tabla_energy + limites_energy + especiales_ener + random_energy + random_energy2

for A, W, name in all_energy_inputs:
    if in_range('energy', A, W):
        res_float = lib_float.calc_kinetic_energy(ctypes.c_float(A), ctypes.c_float(W))
        exp_fx = float_to_fixed(res_float, esc_res)
    else:
        exp_fx = 0
    energy_cases.append((float_to_fixed(A, esc_A), float_to_fixed(W, esc_W), exp_fx, name))

with open("tests/data/energy_test_cases.txt", "w") as f:
    f.write(f"{'#A':^10} {'W':^12} {'Resultado':^12} {'Tipo':^12}\n")
    
    for A_fixed, W_fixed, res, name in energy_cases:
        f.write(f"{A_fixed:>7} {W_fixed:>14} {res:>11} {name:<20}\n")
print(f"Se han generado {len(energy_cases)} casos de energia cinetica\n")