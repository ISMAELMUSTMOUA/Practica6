#!/bin/bash

# 1. Forzar backend X11
export GDK_BACKEND=x11

# 2. Iniciar aplicación en segundo plano
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Bucle de espera manual
echo "Buscando ventana 'calculo'..."

MAX_INTENTOS=20
CONTADOR=0
WID=""

while [ -z "$WID" ]; do
    WID=$(xdotool search --name "calculo" 2>/dev/null | head -n 1)
    
    if [ -n "$WID" ]; then
        break
    fi

    if [ $CONTADOR -ge $MAX_INTENTOS ]; then
        echo "ERROR CRÍTICO: Timeout esperando ventana."
        kill $APP_PID 2>/dev/null
        exit 1
    fi

    sleep 0.5
    CONTADOR=$((CONTADOR+1))
done

echo "¡Ventana encontrada! ID: $WID"

# 4. Interactuar
# NOTA: En CI/CD sin gestor de ventanas, windowactivate falla. 
# Lo quitamos y añadimos un sleep para asegurar estabilidad.
sleep 1

echo "Enviando pulsaciones de teclado..."

# Usamos 2>/dev/null para ocultar los warnings del servidor X11 (XGetInputFocus)
# que son normales en xvfb y ensucian el log.

# --- Pestaña Moles ---
xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "2.5" 2>/dev/null

xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "300" 2>/dev/null

# --- Cambiar Pestaña ---
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab 2>/dev/null
sleep 0.5
xdotool key --window $WID space 2>/dev/null
sleep 1

# --- Pestaña Densidad ---
xdotool key --window $WID Tab Tab Tab Tab Tab Tab 2>/dev/null
xdotool type --window $WID "820000" 2>/dev/null

xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "750" 2>/dev/null

echo "Interacción finalizada con éxito."

# 5. Limpieza final
kill $APP_PID 2>/dev/null
echo "Prueba completada"