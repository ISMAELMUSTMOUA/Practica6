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
    
    # Características de calc_density_fx
    lib.calc_density_fx.argtypes = [ctypes.c_uint32, ctypes.c_uint32, ctypes.POINTER(ctypes.c_uint32)]
    lib.calc_density_fx.restype = None  # No devuelve valor directamente, usa paso por referencia
    
    # Características de tam_dens
    lib.tam_dens.argtypes = [ctypes.POINTER(ctypes.c_uint8)]
    lib.tam_dens.restype = None
    
    # Obtener los tamaños de escalado
    tams_array = (ctypes.c_uint8 * 3)()
    lib.tam_dens(tams_array)
    tams = [tams_array[i] for i in range(3)]
else:
    so_path = "./build/libcomaflotante.so"
    if not os.path.exists(so_path):
        raise FileNotFoundError(f"No se encontró el archivo {so_path}")
    lib = ctypes.CDLL(so_path)
    lib.calc_density.argtypes = [ctypes.c_float, ctypes.c_float]
    lib.calc_density.restype = ctypes.c_float

def leer_datos():
    data = []
    with open("./src/python/data/testeo2.txt", "r") as f:
        for line in f:
            if line.strip() and not line.startswith("#"):  # Ignorar líneas vacías o comentarios
                P_raw, Vt, result = map(float, line.split())
                if punto_fijo:
                    P_fx = int((1 << tams[0]) * P_raw)
                    Vt_fx = int((1 << tams[1]) * Vt)
                    result_fx = int((1 << tams[2]) * result)
                    data.append((P_fx, Vt_fx, result_fx, result))
                else:
                    data.append((P_raw, Vt, result, result))
    return data


def probar_calc_density():
    datos = leer_datos()
    for P_fx, Vt_fx, expected_fx, expected_raw in datos:
        if punto_fijo:
            dato_P = ctypes.c_uint32(P_fx)
            dato_Vt = ctypes.c_uint32(Vt_fx)
            resultado = ctypes.c_uint32()  # Crear variable para recibir el resultado
            

            lib.calc_density_fx(dato_P, dato_Vt, ctypes.byref(resultado))
            result_fx = resultado.value  # Usamos .value para obtener el valor del c_uint32        
            result_float = result_fx / (1 << tams[2])    
            #expected_float = expected_fx / (1 << tams[2])  # Algo de esta conversion es lo que falla
            expected_float = expected_raw
        else:
            result_float = lib.calc_density(P_fx, Vt_fx)
            expected_float = expected_fx
        
        if abs(expected_float) > 0:
            relativo = abs(result_float - expected_float) / expected_float
        else:
            relativo = 0

        tolerancia = 0.05
        if relativo < tolerancia:
            print(f"Éxito: P={P_fx}, Vt={Vt_fx}, Esperado={expected_float}, Obtenido={result_float}, Error relativo={relativo:.10f}")
        else:
            print(f"Fallo: P={P_fx}, Vt={Vt_fx}, Esperado={expected_float}, Obtenido={result_float}, Error relativo={relativo:.10f}")
        

            
# Ejecutar el script
if __name__ == "__main__":
    if punto_fijo:
        print(f"Escalados: P={tams[0]}, Vt={tams[1]}, Resultado={tams[2]}")
    probar_calc_density()
