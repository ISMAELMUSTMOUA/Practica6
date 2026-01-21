#!/usr/bin/env python3
"""
Pytest FINAL simplificado y corregido (Tolerancia ajustada al 6%)
"""
import pytest
import ctypes
import math
import os

# Cargar librerías
lib_fx = ctypes.CDLL("../../../Practica6/build/libpuntofijo.so")

# Configurar funciones (void en tu código)
lib_fx.calc_moles_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_moles_fx.restype = None
lib_fx.calc_density_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_density_fx.restype = None
lib_fx.calc_kinetic_energy_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_kinetic_energy_fx.restype = None

# Leer casos de prueba
def read_test_cases(filename):
    cases = []
    if not os.path.exists(filename):
        print(f"Advertencia: No se encuentra {filename}")
        return []
    with open(filename, 'r') as f:
        for line in f:
            if line.startswith('#') or not line.strip():
                continue
            parts = line.split()
            if len(parts) >= 4:
                cases.append({
                    'input1': int(parts[0]),
                    'input2': int(parts[1]),
                    'expected': int(parts[2]),
                    'category': parts[3]
                })
    return cases

# Función de comparación con 6% de tolerancia
def within_tolerance(actual, expected, tolerance_percent=6.0): 
    if expected == 0:
        return abs(actual) < 100  # Tolerancia para cero en punto fijo
    
    error_percent = abs(actual - expected) / abs(expected) * 100.0
    return error_percent <= tolerance_percent

# --- CLASE DE TEST PARA PYTEST ---
class TestSimple:
    @pytest.mark.parametrize("case", read_test_cases("../data/moles_test_cases.txt"))
    def test_moles(self, case):
        resultado = ctypes.c_uint32()
        lib_fx.calc_moles_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(resultado))
        self._check_result(resultado.value, case, "Moles")

    @pytest.mark.parametrize("case", read_test_cases("../data/density_test_cases.txt"))
    def test_density(self, case):
        resultado = ctypes.c_uint32()
        lib_fx.calc_density_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(resultado))
        self._check_result(resultado.value, case, "Densidad")

    @pytest.mark.parametrize("case", read_test_cases("../data/energy_test_cases.txt"))
    def test_energy(self, case):
        resultado = ctypes.c_uint32()
        lib_fx.calc_kinetic_energy_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(resultado))
        self._check_result(resultado.value, case, "Energía")

    def _check_result(self, obtained, case, context):
        if case['expected'] == 0:
            assert abs(obtained) < 100 or obtained == 0, \
                f"{context} - {case['category']}: Esperaba 0, obtuvo {obtained}"
        else:
            diferencia = abs(int(obtained) - int(case['expected']))
            porcentaje_error = (diferencia / abs(case['expected'])) * 100.0
            
        
            assert porcentaje_error <= 6.0, \
                f"{context} - {case['category']}: Error {porcentaje_error:.2f}% > 6% (Exp: {case['expected']}, Obt: {obtained})"

# --- FUNCIÓN DE REPORTE MANUAL ---
def calcular_y_mostrar_error(valor_obtenido, valor_esperado, nombre_caso):
    if valor_esperado == 0:
        return 0.0 if valor_obtenido < 100 else 100.0
    
    error_absoluto = abs(valor_obtenido - valor_esperado)
    error_relativo = (error_absoluto / abs(valor_esperado)) * 100.0
    
    if error_relativo > 6.0:
        print(f"\nFALLO en {nombre_caso}:")
        print(f"  Esperado: {valor_esperado} | Obtenido: {valor_obtenido}")
        print(f"  Error relativo: {error_relativo:.2f}%")
    
    return error_relativo

def test_multiple_cases():
    """Prueba todos los casos y muestra estadísticas completas"""
    print("\n" + "="*60)
    print(" EJECUTANDO SUITE COMPLETA DE PRUEBAS (Tolerancia 6%)") 
    print("="*60)

    moles_cases = read_test_cases("../data/moles_test_cases.txt")
    density_cases = read_test_cases("../data/density_test_cases.txt")
    energy_cases = read_test_cases("../data/energy_test_cases.txt")

    resultados = []
    
    # 1. Probar MOLES
    for case in moles_cases:
        res = ctypes.c_uint32()
        lib_fx.calc_moles_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(res))
        err = calcular_y_mostrar_error(res.value, case['expected'], f"Moles-{case['category']}")
        resultados.append(("Moles", case['category'], err <= 6.0)) 

    # 2. Probar DENSIDAD
    for case in density_cases:
        res = ctypes.c_uint32()
        lib_fx.calc_density_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(res))
        err = calcular_y_mostrar_error(res.value, case['expected'], f"Densidad-{case['category']}")
        resultados.append(("Densidad", case['category'], err <= 6.0)) 

    # 3. Probar ENERGÍA
    for case in energy_cases:
        res = ctypes.c_uint32()
        lib_fx.calc_kinetic_energy_fx(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(res))
        err = calcular_y_mostrar_error(res.value, case['expected'], f"Energía-{case['category']}")
        resultados.append(("Energía", case['category'], err <= 6.0))
    
    # --- RESUMEN ---
    print("\n" + "="*60)
    print(" RESUMEN FINAL")
    print("="*60)
    
    total = len(resultados)
    exitos = sum(1 for _, _, ok in resultados if ok)
    
    print(f"Total casos:      {total}")
    print(f"Pasaron (<=6%):   {exitos}") 
    if total > 0:
        print(f"Porcentaje Éxito: {exitos/total*100:.1f}%")
    else:
        print("No se encontraron casos de prueba.")

    # Listar fallos si los hay
    fallos = [r for r in resultados if not r[2]]
    if fallos:
        print("\n LISTA DE FALLOS:")
        for tipo, cat, _ in fallos:
            print(f"  - {tipo}: {cat}")
    else:
        print("\n ¡TODOS LOS TESTS PASARON CORRECTAMENTE!")

if __name__ == "__main__":
    # Ejecutar reporte manual primero
    test_multiple_cases()
    
    # Luego ejecutar pytest formalmente
    print("\n" + "-"*60)
    print(" Ejecutando Pytest detallado...")
    print("-"*60)
    pytest.main([__file__, "-v", "--tb=short"])