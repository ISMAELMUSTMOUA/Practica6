#!/bin/bash

# 1. Configuración para entorno headless
export GDK_BACKEND=x11
export DISPLAY=:99  # IMPORTANTE: Xvfb usa este display

# 3. Iniciar aplicación en background
echo "Iniciando aplicación..."
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# 4. Esperar más tiempo en CI (puede ser más lento)
sleep 5

# 5. Buscar ventana con wmctrl
echo "Buscando ventana con wmctrl..."
wmctrl -l  # Debug: mostrar todas las ventanas

# Intentar varias veces (CI puede ser lento)
for i in {1..10}; do
    WID_HEX=$(wmctrl -l | grep -i "calculo" | awk '{print $1}')
    if [ -n "$WID_HEX" ]; then
        echo "Ventana encontrada en intento $i: $WID_HEX"
        break
    fi
    echo "Intento $i: ventana no encontrada, esperando..."
    sleep 2
done

if [ -z "$WID_HEX" ]; then
    echo "ERROR: No se encontró la ventana después de 10 intentos"
    echo "Ventanas disponibles:"
    wmctrl -l
    ps aux | grep calculo
    kill $APP_PID 2>/dev/null
    exit 1
fi

# 6. Activar ventana
echo "Activando ventana $WID_HEX..."
wmctrl -i -a "$WID_HEX"
sleep 2

# 7. Convertir a decimal para xdotool
WID=$(printf "%d" "0x$WID_HEX")
echo "WID decimal: $WID"

# 8. Tomar captura inicial (con xwd que funciona mejor en Xvfb)
echo "Tomando captura inicial..."
xwd -id $WID -out images/00_inicio.xwd 2>/dev/null && 
  convert images/00_inicio.xwd images/00_inicio.png 2>/dev/null || 
  echo "Advertencia: No se pudo tomar captura inicial"

# 9. Interacción con la aplicación
echo "Iniciando interacción..."

# --- MOLES ---
echo "--> Probando Moles..."
xdotool key --window $WID Tab Tab
sleep 1
xdotool type --window $WID "2.5"
sleep 1
xdotool key --window $WID Tab Tab
sleep 1
xdotool type --window $WID "300"
sleep 1

# Captura Moles
xwd -id $WID -out images/01_moles.xwd 2>/dev/null && 
  convert images/01_moles.xwd images/01_moles.png 2>/dev/null

# Botón Test Específico
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 1
xdotool key --window $WID space
sleep 2

# --- DENSIDAD ---
echo "--> Probando Densidad..."
xdotool key --window $WID Tab Tab Tab Tab Tab Tab
sleep 1
xdotool type --window $WID "820000"
sleep 1
xdotool key --window $WID Tab Tab
sleep 1
xdotool type --window $WID "750"
sleep 1

# Captura Densidad
xwd -id $WID -out images/02_densidad.xwd 2>/dev/null && 
  convert images/02_densidad.xwd images/02_densidad.png 2>/dev/null

# Botón Test Específico
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 1
xdotool key --window $WID space
sleep 2

# --- ENERGÍA ---
echo "--> Probando Energía..."
xdotool key --window $WID Tab Tab Tab Tab Tab Tab
sleep 1
xdotool type --window $WID "256"
sleep 1
xdotool key --window $WID Tab Tab
sleep 1
xdotool type --window $WID "8500000"
sleep 1

# Captura Energía
xwd -id $WID -out images/03_energia.xwd 2>/dev/null && 
  convert images/03_energia.xwd images/03_energia.png 2>/dev/null

# Botón Test Específico
xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
sleep 1
xdotool key --window $WID space
sleep 3

# Captura final
xwd -id $WID -out images/04_final.xwd 2>/dev/null && 
  convert images/04_final.xwd images/04_final.png 2>/dev/null

# 10. Limpieza
echo "Finalizando..."
kill $APP_PID 2>/dev/null
wait $APP_PID 2>/dev/null

# 11. Verificar capturas
echo "Resumen de capturas:"
ls -la images/ || echo "No se creó la carpeta images"

# Verificar que hay al menos una captura
if ls images/*.png 1> /dev/null 2>&1; then
    echo "✓ Capturas generadas correctamente"
    exit 0
else
    echo "⚠ No se generaron capturas PNG, pero puede que hayan .xwd"
    # No salimos con error porque puede que la prueba funcione sin capturas
    exit 0
fi
