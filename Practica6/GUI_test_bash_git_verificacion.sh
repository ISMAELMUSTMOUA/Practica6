#!/bin/bash

# 1. Configuración básica
export GDK_BACKEND=x11

# 2. Iniciar aplicación
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 3. Crear carpeta para guardar las fotos (Evidencias)
mkdir -p images

# ---------------------------------------------------------
# CORRECCIÓN PRINCIPAL: CAMBIAMOS wmctrl POR xdotool LOOP
# ---------------------------------------------------------
# En GitHub Actions no hay Gestor de Ventanas, así que wmctrl falla siempre.
# Usamos este bucle que busca directamente la ventana "a la fuerza".

echo "Buscando ventana 'calculo'..."
MAX_INTENTOS=20
CONTADOR=0
WID=""

while [ -z "$WID" ]; do
    # Buscamos cualquier ventana que tenga "calculo" en el nombre
    WID=$(xdotool search --name "calculo" 2>/dev/null | head -n 1)
    
    if [ -n "$WID" ]; then break; fi
    
    if [ $CONTADOR -ge $MAX_INTENTOS ]; then
        echo "ERROR: Timeout esperando ventana."
        kill $APP_PID 2>/dev/null
        exit 1
    fi
    sleep 0.5
    CONTADOR=$((CONTADOR+1))
done

echo "¡Ventana encontrada! ID: $WID"

# --- TRUCO DEL FOCO (Vital para CI/CD) ---
# Hacemos clic en el centro para asegurar que la ventana escucha el teclado
xdotool mousemove --window $WID 100 100 click 1
sleep 1

# ---------------------------------------------------------
# AQUÍ EMPIEZA TU LÓGICA DE NAVEGACIÓN (TUS TABS Y DATOS)
# ---------------------------------------------------------

echo "--> Pestaña 1: Moles..."
# Ir al campo Vp
xdotool key --window $WID Tab Tab
sleep 0.5
xdotool type --window $WID "2.5"
sleep 0.5

# Ir al campo Vt
xdotool key --window $WID Tab Tab
sleep 0.5
xdotool type --window $WID "300"
sleep 0.5

# FOTO 1
scrot "images/evidencia_1_moles.png"

# --- CAMBIO DE PESTAÑA (Tu lógica) ---
# Activar botón Test Específico (Shift+Tab x3 -> Espacio)
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key --window $WID Right  # AÑADIDO: A veces hace falta mover a la derecha en pestañas
xdotool key --window $WID space
sleep 1

# Clic de seguridad para recuperar foco
xdotool mousemove --window $WID 100 100 click 1

echo "--> Pestaña 2: Densidad..."
# Ir al campo p densidad (Tu lógica: 6 Tabs)
xdotool key --window $WID Tab Tab Tab Tab Tab Tab
sleep 0.5
xdotool type --window $WID "820000"
sleep 0.5

# Ir al campo Vt (Tu lógica: 2 Tabs)
xdotool key --window $WID Tab Tab
sleep 0.5
xdotool type --window $WID "750"
sleep 0.5

# FOTO 2
scrot "images/evidencia_2_densidad.png"

# --- CAMBIO DE PESTAÑA ---
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key --window $WID Right # Mover a la siguiente pestaña
xdotool key --window $WID space
sleep 1

# Clic de seguridad
xdotool mousemove --window $WID 100 100 click 1

echo "--> Pestaña 3: Energía (Tus datos)..."
# Ir al campo A energie (Tu lógica: 6 Tabs)
xdotool key --window $WID Tab Tab Tab Tab Tab Tab
sleep 0.5
# TUS DATOS:
xdotool type --window $WID "256"
sleep 0.5

# Ir al campo W
xdotool key --window $WID Tab Tab
sleep 0.5
# TUS DATOS:
xdotool type --window $WID "8500000"
sleep 0.5

# Simular ENTER final
xdotool key --window $WID Return
sleep 1

# FOTO 3
scrot "images/evidencia_3_energia.png"

# Limpiar
kill $APP_PID 2>/dev/null
echo "Prueba completada"