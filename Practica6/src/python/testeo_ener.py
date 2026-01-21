import ctypes
import os
import platform

punto_fijo = True

class Dato(ctypes.Structure):
    _fields_ = [
        ("coma_flotante", ctypes.c_float),
        ("punto_fijo", ctypes.c_uint32),
        ("num_bits_escala", ctypes.c_uint8)
    ]


if punto_fijo:
    so_path = "./build/libpuntofijo.so"
    if not os.path.exists(so_path):
        raise FileNotFoundError(f"No se encontró el archivo {so_path}")
    lib = ctypes.CDLL(so_path)
    
    # Características de calc_kinetic_energy_fx
    lib.calc_kinetic_energy_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
    lib.calc_kinetic_energy_fx.restype = None  # No devuelve valor directamente, usa paso por referencia
    
    # Características de tam_ener
    lib.tam_ener.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    lib.tam_ener.restype = None
    
    # Obtener los tamaños de escalado
    tams_array = (ctypes.c_uint8 * 3)()
    lib.tam_ener(tams_array)
    tams = [tams_array[i] for i in range(3)]
else:
    so_path = "./build/libcomaflotante.so"
    if not os.path.exists(so_path):
        raise FileNotFoundError(f"No se encontró el archivo {so_path}")
    lib = ctypes.CDLL(so_path)
    lib.calc_kinetic_energy.argtypes = [ctypes.c_float, ctypes.c_float]
    lib.calc_kinetic_energy.restype = ctypes.c_float

def leer_datos():
    data = []
    with open("./src/python/data/testeo3.txt", "r") as f:
        for line in f:
            if line.strip() and not line.startswith("#"):  # Ignorar líneas vacías o comentarios
                A_raw, W_raw, result = map(float, line.split())
                if punto_fijo:
                    A_fx = int((1 << tams[0]) * A_raw)
                    W_fx = int((1 << tams[1]) * W_raw)
                    result_fx = int((1 << tams[2]) * result)
                    data.append((A_fx, W_fx, result_fx))
                else:
                    data.append((A_raw, W_raw, result))
    return data


def probar_calculo_moles():
    datos = leer_datos()
    for A_fx, W_fx, expected_fx in datos:
        if punto_fijo:
            dato_A = ctypes.c_uint32(A_fx)
            dato_W = ctypes.c_uint32(W_fx)
            resultado = ctypes.c_uint32()  # Crear variable para recibir el resultado
            
            lib.calc_kinetic_energy_fx(dato_A, dato_W, ctypes.byref(resultado))
        
            result_fx = resultado.value  # Usamos .value para obtener el valor del c_uint32        
            result_float = result_fx / (1 << tams[2])
            expected_float = expected_fx / (1 << tams[2])
        else:
            result_float = lib.calc_kinetic_energy(A_fx, W_fx)
            expected_float = expected_fx
        
        if abs(expected_float) > 0:
            relativo = abs(result_float - expected_float) / expected_float
        else:
            relativo = 0

        tolerancia = 0.05
        if relativo < tolerancia:
            print(f"Éxito: A={A_fx}, W={W_fx}, Esperado={expected_float}, Obtenido={result_float}, Error relativo={relativo:.4f}")
        else:
            print(f"Fallo: A={A_fx}, W={W_fx}, Esperado={expected_float}, Obtenido={result_float}, Error relativo={relativo:.4f}")
        

            
# Ejecutar el script
if __name__ == "__main__":
    if punto_fijo:
        print(f"Escalados: A={tams[0]}, W={tams[1]}, Resultado={tams[2]}")
    probar_calculo_moles()
