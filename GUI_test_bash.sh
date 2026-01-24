#!/bin/bash

# 1. Primero, forzar X11 para la aplicación
export GDK_BACKEND=x11

# Instalar wmctrl si no está
if ! command -v wmctrl &> /dev/null; then
    echo "Instalando wmctrl..."
    sudo apt-get install -y wmctrl
fi

# 2. si hace falta activada el robo de foco en Cinnamon
if gsettings list-schemas | grep -q "org.cinnamon.desktop.wm.preferences"; then
    FOCUS_LEVEL=$(gsettings get org.cinnamon.desktop.wm.preferences focus-stealing-prevention-level)
    
    if [ "$FOCUS_LEVEL" != "0" ]; then
        echo "Activando permiso para que las ventanas roben el foco"
        gsettings set org.cinnamon.desktop.wm.preferences focus-stealing-prevention-level 0
    else
        echo "El foco ya está permitido en Cinnamon."
    fi
else
    echo "No estás en Cinnamon. Se omite la configuración del foco."
fi


# git para validar el .yml
# Iniciar aplicación
./bin/calculo_punto_fijo &
APP_PID=$!
echo "App started with PID $APP_PID"

# Esperar
sleep 10

# Buscar con wmctrl (más confiable para GTK)
echo "Buscando ventana con wmctrl..."
wmctrl -l  # Mostrar todas las ventanas


# Esperar
sleep 10

# Buscar por nombre
WID_HEX=$(wmctrl -l | grep -i "calculo punto fijo" | awk '{print $1}')
echo "WID_HEX encontrado: $WID_HEX"

if [ -n "$WID_HEX" ]; then
    # wmctrl devuelve IDs hexadecimales, convertir a decimal
    WID=$(printf "%d" "0x$WID_HEX")
    echo "WID decimal: $WID"
    
    # Activar con wmctrl (funciona mejor para GTK)
    wmctrl -i -a "$WID_HEX"
    sleep 1
    
    # Ahora usar xdotool para enviar teclas
    echo "Ventana activada. Enviando teclas..."
    
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
    
    # Activar botón Test Específico
    xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
    sleep 0.5
    xdotool key --window $WID space
    sleep 1

    # Ir al campo p densidad 
    xdotool key --window $WID Tab Tab Tab Tab Tab Tab
    sleep 0.5
    xdotool type --window $WID "820000"
    sleep 0.5
    
    # Ir al campo Vt
    xdotool key --window $WID Tab Tab
    sleep 0.5
    xdotool type --window $WID "750"
    sleep 0.5
    
    # Activar botón Test Específico
    xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
    sleep 0.5
    xdotool key --window $WID space
    sleep 1

    # Ir al campo A energie  
    xdotool key --window $WID Tab Tab Tab Tab Tab Tab
    sleep 0.5
    xdotool type --window $WID "256"
    sleep 0.5
    
    # Ir al campo W
    xdotool key --window $WID Tab Tab
    sleep 0.5
    xdotool type --window $WID "8500000"
    sleep 0.5
    
    # Activar botón Test Específico
    xdotool key --window $WID Shift+Tab Shift+Tab Shift+Tab
    sleep 0.5
    xdotool key --window $WID space
    
    
    sleep 2
else
    echo "ERROR: No se encontró la ventana con wmctrl"
fi

# Limpiar
kill $APP_PID 2>/dev/null
echo "Prueba completada"
