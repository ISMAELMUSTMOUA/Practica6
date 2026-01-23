#!/usr/bin/env python3

import pytest
import ctypes
import math
import os

so_path = "./build/libpuntofijo.so"
if not os.path.exists(so_path):
    raise FileNotFoundError(f"No se encontró el archivo {so_path}")
lib_fx = ctypes.CDLL(so_path)

lib_fx.calc_moles_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_moles_fx.restype = None
lib_fx.calc_density_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_density_fx.restype = None
lib_fx.calc_kinetic_energy_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
lib_fx.calc_kinetic_energy_fx.restype = None


def read_test_cases(filename):
    cases = []
    if not os.path.exists(filename):
        print(f"Advertencia: No se encuentra {filename}")
        return []
    with open(filename, 'r') as f:
        for line in f:
            if '#' in line or not line.strip():
                continue
            parts = line.split()
            if len(parts) >= 4:
                cases.append({'input1': int(parts[0]),'input2': int(parts[1]),'expected': int(parts[2]),'category': parts[3]})
    return cases


class TestSimple:
    def ejecuta_tests(self, case, function, context):
        resultado = ctypes.c_uint32()
        function(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(resultado))
        self.check_result(resultado.value, case, context)

    @pytest.mark.parametrize("case", read_test_cases("./tests/data/moles_test_cases.txt"))
    def test_moles(self, case):
        self.ejecuta_tests(case, lib_fx.calc_moles_fx, "Moles")

    @pytest.mark.parametrize("case", read_test_cases("./tests/data/density_test_cases.txt"))
    def test_density(self, case):
        self.ejecuta_tests(case, lib_fx.calc_density_fx, "Densidad")

    @pytest.mark.parametrize("case", read_test_cases("./tests/data/energy_test_cases.txt"))
    def test_energy(self, case):
        self.ejecuta_tests(case, lib_fx.calc_kinetic_energy_fx, "Energía")

    def check_result(self, obtained, case, context):
        if case['expected'] == 0:
            assert abs(obtained) < 100 or obtained == 0, \
                f"{context} - {case['category']}: Esperaba 0, obtuvo {obtained}"
        else:
            diferencia = abs(int(obtained) - int(case['expected']))
            porcentaje_error = (diferencia / abs(case['expected'])) * 100.0
            
            assert porcentaje_error <= 6.0, \
                f"{context} - {case['category']}: Error {porcentaje_error:.2f}% > 6% (Exp: {case['expected']}, Obt: {obtained})"


def calcular_mostrar_error(valor_obtenido, valor_esperado, nombre_caso):
    if valor_esperado == 0:
        return 0.0 if valor_obtenido < 100 else 100.0
    
    error_absoluto = abs(valor_obtenido - valor_esperado)
    error_relativo = (error_absoluto / abs(valor_esperado)) * 100.0
    
    if error_relativo > 6.0:
        print(f"\n Fallo en {nombre_caso}:")
        print(f"  Esperado: {valor_esperado} | Obtenido: {valor_obtenido}")
        print(f"  Error relativo: {error_relativo:.2f}%")
    
    return error_relativo


def ejecuta_casos_tests(cases, function, test_type):
    resultados = []
    for case in cases:
        res = ctypes.c_uint32()
        function(ctypes.c_uint32(case['input1']), ctypes.c_uint32(case['input2']), ctypes.byref(res))
        err = calcular_mostrar_error(res.value, case['expected'], f"{test_type}-{case['category']}")
        resultados.append((test_type, case['category'], err <= 6.0))
    return resultados



def test_multiple_cases():
    print("\n" + "="*30)
    print(" Casos de test analizados:") 
    print("="*30)

    moles_cases = read_test_cases("./tests/data/moles_test_cases.txt")
    density_cases = read_test_cases("./tests/data/density_test_cases.txt")
    energy_cases = read_test_cases("./tests/data/energy_test_cases.txt")

    resultados = []
    
    resultados.extend(ejecuta_casos_tests(moles_cases, lib_fx.calc_moles_fx, "Moles"))
    resultados.extend(ejecuta_casos_tests(density_cases, lib_fx.calc_density_fx, "Densidad"))
    resultados.extend(ejecuta_casos_tests(energy_cases, lib_fx.calc_kinetic_energy_fx, "Energía"))
    
    print("\n" + "="*60)
    print(" Reporte de resultados")
    print("="*60)
    
    total = len(resultados)
    exitos = sum(1 for _, _, ok in resultados if ok)
    fallos = total - exitos
    
    print(f"Total casos:      {total}")
    print(f"Casos exitosos:   {exitos}") 
    print(f"Casos fallidos:   {fallos}")
    if total > 0:
        print(f"Porcentaje Éxito: {exitos/total*100:.1f}%")
    else:
        print("No se encontraron casos de prueba")

    fallos = [r for r in resultados if not r[2]]
    if fallos:
        print("\n Lista de fallos:")
        for tipo, cat, _ in fallos:
            print(f"  - {tipo}: {cat}")
    else:
        print("\n Ningun error detectado en los casos testeados")

if __name__ == "__main__":

    test_multiple_cases()
    
    print("\n" + "-"*60)
    print(" Pytest detallado")
    print("-"*60)
    pytest.main([__file__, "-v", "--tb=short"])