#!/bin/bash

# 1. Forzar backend X11
export GDK_BACKEND=x11

# 2. Iniciar aplicación en segundo plano
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Bucle de espera manual (Reemplaza a --sync --timeout)
echo "Buscando ventana 'calculo'..."

MAX_INTENTOS=20  # 20 intentos * 0.5 seg = 10 segundos max
CONTADOR=0
WID=""

while [ -z "$WID" ]; do
    # Buscamos la ventana por nombre. Redirigimos errores a null por si acaso.
    # Usamos 'head -n 1' para quedarnos solo con el primer ID si hay varios.
    WID=$(xdotool search --name "calculo" 2>/dev/null | head -n 1)
    
    if [ -n "$WID" ]; then
        break
    fi

    if [ $CONTADOR -ge $MAX_INTENTOS ]; then
        echo "ERROR CRÍTICO: Timeout - La ventana no apareció tras 10 segundos."
        kill $APP_PID 2>/dev/null
        exit 1
    fi

    echo "Esperando ventana... ($CONTADOR/$MAX_INTENTOS)"
    sleep 0.5
    CONTADOR=$((CONTADOR+1))
done

echo "¡Ventana encontrada! ID: $WID"

# 4. Interactuar
# Enfocar la ventana
xdotool windowactivate --sync $WID
sleep 1

echo "Enviando pulsaciones de teclado..."

# --- Pestaña Moles ---
xdotool key --window $WID Tab Tab
xdotool type --window $WID "2.5"

xdotool key --window $WID Tab Tab
xdotool type --window $WID "300"

# --- Cambiar Pestaña ---
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key --window $WID space
sleep 1

# --- Pestaña Densidad ---
xdotool key --window $WID Tab Tab Tab Tab Tab Tab
xdotool type --window $WID "820000"

xdotool key --window $WID Tab Tab
xdotool type --window $WID "750"

echo "Interacción finalizada con éxito."

# 5. Limpieza final
kill $APP_PID 2>/dev/null
echo "Prueba completada"