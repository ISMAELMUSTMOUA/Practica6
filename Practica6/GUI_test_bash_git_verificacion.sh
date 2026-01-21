#!/bin/bash

# 1. Configuración
export GDK_BACKEND=x11

# 2. Iniciar App
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Esperar Ventana
echo "Buscando ventana 'calculo'..."
MAX_INTENTOS=20
CONTADOR=0
WID=""

while [ -z "$WID" ]; do
    WID=$(xdotool search --name "calculo" 2>/dev/null | head -n 1)
    if [ -n "$WID" ]; then break; fi
    if [ $CONTADOR -ge $MAX_INTENTOS ]; then
        echo "Timeout esperando ventana."
        kill $APP_PID 2>/dev/null
        exit 1
    fi
    sleep 0.5
    CONTADOR=$((CONTADOR+1))
done

echo "¡Ventana encontrada! ID: $WID"
sleep 1

# --- TEST 1: MOLES ---
echo "--> Probando Moles..."
xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "2.5" 2>/dev/null
xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "300" 2>/dev/null
sleep 0.5
# Simulamos Calcular (si tienes botón haz Enter, si es automático solo foto)
xdotool key --window $WID Return 2>/dev/null 
sleep 1
# FOTO 1
scrot "evidencia_1_moles.png"
echo "Captura tomada: evidencia_1_moles.png"

# --- TEST 2: DENSIDAD ---
echo "--> Cambiando a Densidad..."
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab 2>/dev/null
sleep 0.2
xdotool key --window $WID Right 2>/dev/null
sleep 0.2
xdotool key --window $WID space 2>/dev/null
sleep 1

xdotool key --window $WID Tab Tab Tab Tab Tab Tab 2>/dev/null
xdotool type --window $WID "820000" 2>/dev/null
xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "750" 2>/dev/null
sleep 0.5
xdotool key --window $WID Return 2>/dev/null
sleep 1
# FOTO 2
scrot "evidencia_2_densidad.png"
echo "Captura tomada: evidencia_2_densidad.png"

# --- TEST 3: ENERGÍA ---
echo "--> Cambiando a Energía..."
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab 2>/dev/null
sleep 0.2
xdotool key --window $WID Right 2>/dev/null
sleep 0.2
xdotool key --window $WID space 2>/dev/null
sleep 1

echo "--> Escribiendo datos Energía..."
xdotool key --window $WID Tab Tab Tab Tab Tab Tab 2>/dev/null
xdotool type --window $WID "256" 2>/dev/null
sleep 0.2
xdotool key --window $WID Tab Tab 2>/dev/null
xdotool type --window $WID "8500000" 2>/dev/null
sleep 0.5
xdotool key --window $WID Return 2>/dev/null
sleep 1
# FOTO 3
scrot "evidencia_3_energia.png"
echo "Captura tomada: evidencia_3_energia.png"

# 5. Limpieza
kill $APP_PID 2>/dev/null
echo "Prueba completada con fotos"