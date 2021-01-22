# luiscastrillo97-FRDMKL02Z_mma8451
Este repositorio contiene el programa para encender los LEDs RGB de la tarjeta FRDMKL02Z utilizando el puerto UART para obtener datos del teclado. Además, se utiliza el acelerómetro MMA8451 que contiene la tarjeta de desarrollo. De esta forma, presionando algunas letras del teclado, se pueden obtener datos de los ejes x, y, z del acelerómetro, y se pueden escribir valores en los registros de este.

 Se copiaron las librerias que generó el Ingeniero Ernerto Rincón para utilizar el puerto UART (sdk_hal_uart0), los puertos GPIO (sdk_hal_gpio) y el bus I2C (sdk_hal_i2c0).
 
 Se creó una función de calibración para ajustar el offset del acelerómetro. Por último se desarrolló el código con éxito.

 Cualquier tecla del teclado que se oprima se verá reflejada en la terminal serial.

 * Oprimiendo A encendemos el LED Azul.
 * Oprimiendo a apagamos el LED Azul.
 * Oprimiendo V encendemos el LED Verde.
 * Oprimiendo v apagamos el LED Verde.
 * Oprimiendo R encendemos el LED Rojo.
 * Oprimiendo r apagamos el LED Rojo.
 * Oprimiendo M encendemos la combinación de LEDs que resulta en el color Magenta.
 * Oprimiendo m apagamos la combinación Magenta.
 * Oprimiendo C encendemos la combinación de LEDs que resulta en el color Cyan.
 * Oprimiendo c apagamos la combinación Cyan.
 * Oprimiendo L encendemos la combinación de LEDs que resulta en el color Amarillo.
 * Oprimiendo l apagamos la combinación Amarillo.

 * b: Verificar si hay conexión con el acelerometro.
 * n: Calibrar acelerometro.
 * o: Cambiar acelerómetro a modo Activo.
 * f: Cambiar acelerómetro a modo StandBy.
 * e: Verificar en que modo se encuentra el acelerometro.
 * x o X: Mostrar valores del eje x.
 * y o Y: Mostrar valores del eje y.
 * z o Z: Mostrar valores del eje z.

 * Oprimiendo cualquier otra tecla, solo se muestra la tecla que se oprime en la terminal, pero, no realiza otra acción.


