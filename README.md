# reimagined-broccoli-frequency-counter
A frequency counter driver for a stm32f429ZIT6

# Como funciona
El driver emplea 2 timers para medir la frecuencia entrante: El timer 9 cuenta en el pin E6 la cantidad de estados altos que se detectan en un segundo mientras que el timer 10 genera una base de tiempo cercana a un segundo para obtener la frecuencia.

# Requisitos
No es necesario tener incluidos los driver HAL para que este funcione. Para incluirlo es necesario agregar el archivo Freq.h a la carpeta del proyecto donde lo esten usando y agregar un define:
#define XTAL_FREQ (la frecuencia que usan en el micro)

# Como utilizarlo
Para instalarlo solo es necesario llamar la función, esta retornará el valor medido en forma uint32_t.

# Descripción del funcionamiento
El dirver cuenta con 2 funciones principales y 2 funciones basadas en interrupciones
- Las interrupciones manejan los momentos en que los timers realizan overflows o indican que se termino el tiempo de cuenta
- Una función de inicialización para configurar los periféricos necesarios (en caso de no ejecutarse se auto realiza cuando se trata de realizar una medicion)
- Una funcion con la cual se realiza el calculo y el regreso del valor medido

