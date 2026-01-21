#!/bin/bash

# 1. Forzar X11
export GDK_BACKEND=x11

# 2. Iniciar aplicación en segundo plano
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Esperar a que la ventana aparezca usando xdotool
echo "Buscando ventana..."

# xdotool search:
# --sync: Espera a que la ventana aparezca
# --timeout 10000: Espera máximo 10 segundos
# --name: Busca por parte del título o nombre del ejecutable
# "calculo": Texto a buscar
WID=$(xdotool search --sync --timeout 10000 --name "calculo" | head -n 1)

if [ -n "$WID" ]; then
    echo "Ventana encontrada ID: $WID"
    
    # Enfocar la ventana
    xdotool windowactivate --sync $WID
    sleep 1
    
    echo "Enviando teclas..."
    
    # --- Interacción con la pestaña Moles ---
    # Tab x2 para ir al campo
    xdotool key --window $WID Tab Tab
    xdotool type --window $WID "2.5"
    
    xdotool key --window $WID Tab Tab
    xdotool type --window $WID "300"
    
    # --- Cambiar de Pestaña (Shift+Tab x3 -> Espacio) ---
    xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
    sleep 0.5
    xdotool key --window $WID space
    sleep 1

    # --- Interacción con la pestaña Densidad ---
    # Tabs para llegar al campo
    xdotool key --window $WID Tab Tab Tab Tab Tab Tab
    xdotool type --window $WID "820000"
    
    xdotool key --window $WID Tab Tab
    xdotool type --window $WID "750"
    
    echo "Interacción finalizada con éxito."
else
    echo "ERROR CRÍTICO: xdotool no encontró la ventana después de 10 segundos."
    # Listar ventanas visibles para depuración
    xdotool search --name ""
    kill $APP_PID
    exit 1
fi

# 4. Limpieza
kill $APP_PID 2>/dev/null
echo "Prueba completada"