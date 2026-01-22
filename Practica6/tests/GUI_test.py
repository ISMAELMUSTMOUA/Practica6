import subprocess
import time
import os

# Veridicar la ruta de la aplicación
so_path = "./bin/calculo_punto_fijo"
if not os.path.exists(so_path):
    raise FileNotFoundError(f"No se encontró el archivo {so_path}")

print(f"*** Comenzando Test de GUI ***")

# Iniciar la aplicación como subproceso
try:
    process = subprocess.Popen(so_path)
except Exception:
    print("NO PASS: No se pudo iniciar la aplicación")
    exit(1)

time.sleep(1)

# Verificar si la aplicación sigue en marcha
if process.poll() is None:
    print("PASS: Test de GUI superado, aplicación estable")
    process.terminate()
else:
    print("NO PASS: La aplicación se cerró durante el test")