#!/usr/bin/env python3
from dogtail.tree import root
import subprocess
import time
import pytest
import os
import ctypes

num_casos = 2

# Abrir librería para usar los escalados
so_path2 = "./build/libpuntofijo.so"
if not os.path.exists(so_path2):
    raise FileNotFoundError(f"No se encontró el archivo {so_path2}")

lib_fx = ctypes.CDLL(so_path2)
lib_fx.tam_moles.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_dens.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
lib_fx.tam_ener.argtypes = [ctypes.POINTER(ctypes.c_uint8)]



# Para iniciar la GUI 
@pytest.fixture(scope="session")
def iniciar_GUI():
    so_path = "./bin/calculo_punto_fijo"
    if not os.path.exists(so_path):
        raise FileNotFoundError(f"No se encontró el archivo {so_path}")

    print("*** Comenzando Test de GUI con dogtail ***")

    # Iniciar la aplicación como subproceso
    try:
        process = subprocess.Popen(so_path)
    except Exception:
        print("NO PASS: No se pudo iniciar la aplicación")
        exit(1)

    app = root.application("calculo_punto_fijo")

    yield app

    process.terminate()
    process.wait()

    print("*** Test de GUI con dogtail finalizado ***")



# Leer datos desde los ficheros de data
def datos(filename):
    if not os.path.exists(filename):
        raise FileNotFoundError(f"No se encontró el archivo {filename}")
    else:
        with open(filename, 'r') as f:
            lineas = []
            for i in range(num_casos):
                linea = f.readline()
                if '#' in linea or not linea.strip():
                    continue
                linea = linea.split()
                lineas.append(linea)
            return lineas



# Test para las funciones de moles
@pytest.mark.parametrize("linea", datos("./tests/data/moles_test_cases.txt"))
def test_moles_esp(linea, iniciar_GUI):
    app = iniciar_GUI
    
    tams_array = (ctypes.c_uint8 * 3)()
    lib_fx.tam_moles(tams_array)

    app.child(description='entry_vp').text = str(float(linea[0]) / 2**tams_array[0])
    app.child(description='entry_vt').text = str(float(linea[1]) / 2**tams_array[1])
    app.child(description='test-moles-esp').click()


def test_moles_auto(iniciar_GUI):
    app = iniciar_GUI
    app.child(description='test-moles-auto').click()
    time.sleep(1)

    selector = app.childNamed('Selecciona el fichero')
    selector.childNamed('src').doubleClick()
    selector.childNamed('python').doubleClick()
    selector.childNamed('data').doubleClick()
    selector.childNamed('testeo1.txt').doubleClick()


def test_gen_moles(iniciar_GUI):
    app = iniciar_GUI
    app.child(description='gen-moles').click()
    ventana = app.childNamed('Guardar Fichero')
    


# Test para las funciones de densidad
@pytest.mark.parametrize("linea", datos("./tests/data/density_test_cases.txt"))
def test_densidad_esp(linea, iniciar_GUI):
    app = iniciar_GUI
    
    tams_array = (ctypes.c_uint8 * 3)()
    lib_fx.tam_dens(tams_array)

    app.child(description='entry_p').text = str(float(linea[0]) / 2**tams_array[0])
    app.child(description='entry_vt2').text = str(float(linea[1]) / 2**tams_array[1])
    app.child(description='test-densidad-esp').click()


def test_densidad_auto(iniciar_GUI):
    app = iniciar_GUI
    app.child(description='test-densidad-auto').click()
    time.sleep(1)

    selector = app.childNamed('Selecciona el fichero')
    selector.childNamed('src').doubleClick()
    selector.childNamed('python').doubleClick()
    selector.childNamed('data').doubleClick()
    selector.childNamed('testeo2.txt').doubleClick()



# Test para las funciones de densidad
@pytest.mark.parametrize("linea", datos("./tests/data/energy_test_cases.txt"))
def test_energia_esp(linea, iniciar_GUI):
    app = iniciar_GUI
    
    tams_array = (ctypes.c_uint8 * 3)()
    lib_fx.tam_ener(tams_array)

    app.child(description='entry_a').text = str(float(linea[0]) / 2**tams_array[0])
    app.child(description='entry_w').text = str(float(linea[1]) / 2**tams_array[1])
    app.child(description='test-energia-esp').click()


def test_energia_auto(iniciar_GUI):
    app = iniciar_GUI
    app.child(description='test-energia-auto').click()
    time.sleep(1)

    selector = app.childNamed('Selecciona el fichero')
    selector.childNamed('src').doubleClick()
    selector.childNamed('python').doubleClick()
    selector.childNamed('data').doubleClick()
    selector.childNamed('testeo3.txt').doubleClick()