#!/bin/bash

export GDK_BACKEND=x11
export DISPLAY=:99

# Crear carpeta
mkdir -p images

echo "=== Iniciando aplicación ==="
./bin/calculo_punto_fijo &
APP_PID=$!
echo "PID: $APP_PID"

# Esperar suficiente tiempo
sleep 8

echo "=== Buscando ventana con xdotool ==="

# Método 1: Buscar por nombre parcial
WID=$(xdotool search --onlyvisible --name "calculo" 2>/dev/null | head -n1)

# Método 2: Si no encuentra, buscar cualquier ventana GTK
if [ -z "$WID" ]; then
    echo "No se encontró por nombre, buscando cualquier ventana GTK..."
    WID=$(xdotool search --class "Gtk" 2>/dev/null | head -n1)
fi

# Método 3: Buscar por clase de la aplicación
if [ -z "$WID" ]; then
    echo "Buscando por clase de aplicación..."
    WID=$(xdotool search --class "calculo_punto_fijo" 2>/dev/null | head -n1)
fi

# Método 4: Buscar cualquier ventana visible
if [ -z "$WID" ]; then
    echo "Buscando cualquier ventana visible..."
    WID=$(xdotool search --onlyvisible . 2>/dev/null | head -n1)
fi

if [ -z "$WID" ]; then
    echo "✗ No se pudo encontrar ninguna ventana"
    
    # Intentar captura de pantalla completa
    echo "Intentando captura de pantalla completa..."
    timeout 5 xwd -root -out images/fallback.xwd 2>/dev/null || true
    if [ -f images/fallback.xwd ]; then
        convert images/fallback.xwd images/fallback.png 2>/dev/null || true
    fi
    
    kill $APP_PID 2>/dev/null
    exit 1
fi

echo "✓ Ventana encontrada: $WID"

# Intentar activar
xdotool windowactivate $WID 2>/dev/null || true
xdotool windowfocus $WID 2>/dev/null || true
sleep 2

# Tomar captura
xwd -id $WID -out images/window_found.xwd 2>/dev/null && \
    convert images/window_found.xwd images/window_found.png 2>/dev/null || true

echo "=== Interactuando ==="

# Enviar comandos directamente (sin verificar ventana específica)
# Los envíos funcionarán si hay una ventana activa

# Moles
xdotool key Tab Tab
sleep 0.5
xdotool type "2.5"
sleep 0.5
xdotool key Tab Tab
sleep 0.5
xdotool type "300"
sleep 0.5

# Captura
xwd -id $WID -out images/01_moles.xwd 2>/dev/null && \
    convert images/01_moles.xwd images/01_moles.png 2>/dev/null || true

# Botón Test
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key space
sleep 2

# Densidad
xdotool key Tab Tab Tab Tab Tab Tab
sleep 0.5
xdotool type "820000"
sleep 0.5
xdotool key Tab Tab
sleep 0.5
xdotool type "750"
sleep 0.5

# Captura
xwd -id $WID -out images/02_densidad.xwd 2>/dev/null && \
    convert images/02_densidad.xwd images/02_densidad.png 2>/dev/null || true

# Botón Test
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 0.5
xdotool key space
sleep 2

# Energía
xdotool key Tab Tab Tab Tab Tab Tab
sleep 0.5
xdotool type "256"
sleep 0.5
xdotool key Tab Tab
sleep 0.5
xdotool type "8500000"
sleep 0.5

# Captura
xwd -id $WID -out images/03_energia.xwd 2>/dev/null && \
    convert images/03_energia.xwd images/03_energia.png 2>/dev/null || true

# Botón final
xdotool key Shift+Tab Shift+Tab Shift+Tab
sleep 1.5
xdotool key space
sleep 1
xdotool key space
sleep 3

# Captura final
xwd -id $WID -out images/04_final.xwd 2>/dev/null && \
    convert images/04_final.xwd images/04_final.png 2>/dev/null || true

echo "=== Finalizando ==="
kill $APP_PID 2>/dev/null || true

# Verificar capturas
PNG_COUNT=$(ls images/*.png 2>/dev/null | wc -l || echo 0)
echo "Capturas PNG: $PNG_COUNT"

if [ $PNG_COUNT -gt 0 ]; then
    echo "✓ Test completado"
    exit 0
else
    echo "⚠ Test completado sin capturas"
    exit 2
fi
