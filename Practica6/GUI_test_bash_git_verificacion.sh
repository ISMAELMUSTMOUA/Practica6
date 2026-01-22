#!/bin/bash

# 1. Configuración
export GDK_BACKEND=x11

# 2. Iniciar App
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Crear carpeta images
mkdir -p images

# 4. Esperar Ventana (Loop de búsqueda)
echo "Buscando ventana 'calculo'..."
MAX_INTENTOS=30 # Damos un poco más de margen
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

# --- ACTIVACIÓN FUERTE (CRÍTICO) ---
# 1. Traer al frente
xdotool windowmap $WID 2>/dev/null
xdotool windowraise $WID 2>/dev/null
# 2. Esperar un segundo
sleep 1
# 3. CLIC FÍSICO para ganar el foco (En coordenadas 200,200 aprox centro-izquierda)
xdotool mousemove --window $WID 200 200 click 1 2>/dev/null
echo "Ventana activada y clic realizado."
sleep 1

# --- TEST 1: MOLES ---
echo "--> Probando Moles..."
# Tab x2 para ir al primer campo
xdotool key --window $WID Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "2.5" 2>/dev/null
sleep 0.5

xdotool key --window $WID Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "300" 2>/dev/null
sleep 0.5

# FOTO 1
scrot "images/evidencia_1_moles.png"

# --- CAMBIO A DENSIDAD ---
# Subimos a las pestañas y movemos a la derecha
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab 2>/dev/null
sleep 0.5
xdotool key --window $WID Right 2>/dev/null
sleep 0.5
xdotool key --window $WID space 2>/dev/null
sleep 1

# IMPORTANTE: Clic de nuevo para asegurar foco en la nueva pestaña
xdotool mousemove --window $WID 200 200 click 1 2>/dev/null

echo "--> Probando Densidad..."
# 6 Tabs para llegar al campo (Tu lógica)
xdotool key --window $WID Tab Tab Tab Tab Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "820000" 2>/dev/null
sleep 0.5

xdotool key --window $WID Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "750" 2>/dev/null
sleep 0.5

# FOTO 2
scrot "images/evidencia_2_densidad.png"

# --- CAMBIO A ENERGÍA ---
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab 2>/dev/null
sleep 0.5
xdotool key --window $WID Right 2>/dev/null
sleep 0.5
xdotool key --window $WID space 2>/dev/null
sleep 1

# Clic de seguridad
xdotool mousemove --window $WID 200 200 click 1 2>/dev/null

echo "--> Probando Energía..."
# 6 Tabs (Tu lógica)
xdotool key --window $WID Tab Tab Tab Tab Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "256" 2>/dev/null
sleep 0.5

xdotool key --window $WID Tab Tab 2>/dev/null
sleep 0.5
xdotool type --window $WID "8500000" 2>/dev/null
sleep 0.5

# Enter final
xdotool key --window $WID Return 2>/dev/null
sleep 1

# FOTO 3
scrot "images/evidencia_3_energia.png"

# Limpieza
kill $APP_PID 2>/dev/null
echo "Prueba completada con éxito."