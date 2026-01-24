# Practica6 – Descripción de carpetas

Este proyecto está organizado en varios directorios, cada uno con una función específica dentro del desarrollo, compilación, pruebas y documentación.

## Estructura de Directorios

- **document/**
  Incluye documentación relacionada con la práctica: informes, especificaciones, apuntes o cualquier archivo explicativo necesario para comprender el proyecto y imágenes utilizadas en la documentación.

- **bin/**
  Directorio donde se almacenan los ejecutables generados tras la compilación del proyecto.

- **build/**
  Carpeta utilizada para almacenar archivos intermedios de compilación, como objetos (`.o`) o dependencias generadas por el compilador.

- **include/**
  Almacena los archivos de cabecera (`.h`) del proyecto. Aquí se definen funciones, estructuras, constantes y prototipos utilizados en el código fuente.

- **src/**
  Incluye el código fuente principal del proyecto (`.c`, `.py`). Es el núcleo funcional de la práctica.

- **tests/**
  Contiene los scripts y archivos necesarios para ejecutar pruebas automáticas del proyecto, como casos de prueba o configuraciones de test.

- **makefile**
  Archivo para compilación, limpieza y ejecución del proyecto mediante el comando `make`.
