#!/bin/bash

# Configuración para entorno headless
export GDK_BACKEND=x11
# NOTA: xvfb-run ya establecerá DISPLAY automáticamente

echo "=== Iniciando aplicación ==="
./bin/calculo_punto_fijo &
APP_PID=$!
echo "PID: $APP_PID"

# Dar tiempo para que la aplicación se inicie
sleep 5

echo "=== Buscando ventana ==="
# Listar ventanas para debug
wmctrl -l

# Intentar encontrar la ventana
MAX_ATTEMPTS=20
WID_HEX=""
for i in $(seq 1 $MAX_ATTEMPTS); do
  WID_HEX=$(wmctrl -l | grep -i "calculo" | awk '{print $1}')
  if [ -n "$WID_HEX" ]; then
    echo "✓ Ventana encontrada en intento $i: $WID_HEX"
    break
  fi
  echo "Intento $i/$MAX_ATTEMPTS: ventana no encontrada, esperando..."
  sleep 1
done

if [ -z "$WID_HEX" ]; then
  echo "✗ ERROR: No se pudo encontrar la ventana después de $MAX_ATTEMPTS intentos"
  echo "Ventanas disponibles:"
  wmctrl -l
  kill $APP_PID 2>/dev/null
  exit 1
fi

# Activar ventana
echo "Activando ventana: $WID_HEX"
wmctrl -i -a "$WID_HEX"
sleep 2

# Tomar captura inicial
echo "Tomando captura inicial..."
xwd -id $WID_HEX -out images/00_inicio.xwd 2>/dev/null
if [ -f images/00_inicio.xwd ]; then
  convert images/00_inicio.xwd images/00_inicio.png 2>/dev/null
  echo "✓ Captura inicial tomada"
else
  echo "⚠ No se pudo tomar captura inicial"
fi

echo "=== Realizando pruebas de interacción ==="

# Prueba 1: Campo Vp (Moles)
xdotool windowactivate $WID_HEX
sleep 0.5
xdotool key Tab Tab
sleep 0.5
xdotool type "2.5"
sleep 0.5

# Prueba 2: Campo Vt (Moles)
xdotool key Tab Tab
sleep 0.5
xdotool type "300"
sleep 0.5

# Captura después de moles
xwd -id $WID_HEX -out images/01_moles.xwd 2>/dev/null && \
  convert images/01_moles.xwd images/01_moles.png 2>/dev/null

# Botón Test Específico
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key space
sleep 2

# Prueba 3: Campo p (Densidad)
xdotool key Tab Tab Tab Tab Tab Tab
sleep 0.5
xdotool type "820000"
sleep 0.5

# Prueba 4: Campo Vt (Densidad)
xdotool key Tab Tab
sleep 0.5
xdotool type "750"
sleep 0.5

# Captura después de densidad
xwd -id $WID_HEX -out images/02_densidad.xwd 2>/dev/null && \
  convert images/02_densidad.xwd images/02_densidad.png 2>/dev/null

# Botón Test Específico
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key space
sleep 2

# Prueba 5: Campo A (Energía)
xdotool key Tab Tab Tab Tab Tab Tab
sleep 0.5
xdotool type "256"
sleep 0.5

# Prueba 6: Campo W (Energía)
xdotool key Tab Tab
sleep 0.5
xdotool type "8500000"
sleep 0.5

# Captura después de energía
xwd -id $WID_HEX -out images/03_energia.xwd 2>/dev/null && \
  convert images/03_energia.xwd images/03_energia.png 2>/dev/null

# Botón Test Específico final
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key space
sleep 3

# Captura final
xwd -id $WID_HEX -out images/04_final.xwd 2>/dev/null && \
  convert images/04_final.xwd images/04_final.png 2>/dev/null

echo "=== Finalizando ==="
kill $APP_PID 2>/dev/null
wait $APP_PID 2>/dev/null

# Verificar resultados
echo "=== Resumen ==="
if [ -d "images" ]; then
  PNG_COUNT=$(find images -name "*.png" | wc -l)
  echo "Capturas PNG generadas: $PNG_COUNT"
  if [ $PNG_COUNT -gt 0 ]; then
    echo "✓ Test completado con éxito"
    exit 0
  else
    echo "⚠ Test completado pero sin capturas PNG"
    exit 2
  fi
else
  echo "✗ No se creó la carpeta images"
  exit 1
fi
