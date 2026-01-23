#!/usr/bin/env python3
from dogtail.tree import root
import subprocess
import time
import pytest
import os

so_path = "./bin/calculo_punto_fijo"
if not os.path.exists(so_path):
    raise FileNotFoundError(f"No se encontró el archivo {so_path}")

print(f"*** Comenzando Test de GUI con dogtail ***")

# Iniciar la aplicación como subproceso
try:
    process = subprocess.Popen(so_path)
except Exception:
    print("NO PASS: No se pudo iniciar la aplicación")
    exit(1)

time.sleep(1)

app = root.application("calculo_punto_fijo")

button_moles = app.child(name='btn_test_esp_moles').click()
time.sleep(10)

app.child(name="btn_test_esp_moles").click()
time.sleep(10)

app.child(name="btn_test_auto_energia").click()
time.sleep(10)