/*! @file : main.c
 * @author  Luis José Castrillo Fernández
 * @version 1.0.0
 * @date    16/01/2021
 * @brief   Archivo principal
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"


#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MMA851_I2C_DEVICE_ADDRESS	0x1D
const uint16_t kSENSITIVITY_2G = 4096;

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/
void calibracion();

/*******************************************************************************
 * External vars
 ******************************************************************************/


/*******************************************************************************
 * Local vars
 ******************************************************************************/


/*******************************************************************************
 * Private Source Code
 ******************************************************************************/


/*******************************************************************************
 * Public Source Code
 ******************************************************************************/
int main(void) {
	status_t status;
	uint8_t nuevo_byte_uart;
	uint8_t	nuevo_dato_i2c, datox1, datox2, datoy1, datoy2, datoz1, datoz2, nuevo_dato;
	uint16_t xData = 0, yData = 0, zData = 0;
	uint16_t xDatag = 0, yDatag = 0, zDatag = 0;


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps
    //

    printf("Usar teclado para controlar LEDs y mostrar valores del acelerometro MMA8451\r\n");
    printf("R: Encender. r: Apagar. Led Rojo\r\n");
    printf("V: Encender. v: Apagar. Led Verde\r\n");
    printf("A: Encender. a: Apagar. Led Azul\r\n");
    printf("M: buscar acelerometro\r\n");
    printf("c: Calibrar acelerometro\r\n");
    printf("T: Cambiar a modo Activo\r\n");
    printf("t: Cambiar a modo StandBy\r\n");
    printf("s: Verificar en que modo se encuentra el acelerometro\r\n");
    printf("x o X: Mostrar valores del eje x\r\n");
    printf("y o Y: Mostrar valores del eje y\r\n");
    printf("z o Z: Mostrar valores del eje z\r\n");


    while(1) {

    	if(uart0CuantosDatosHayEnBuffer()>0){

    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);

    		if(status==kStatus_Success){

    			printf("dato:%c\r\n",nuevo_byte_uart);

    			switch (nuevo_byte_uart) {

				case 'a':
					gpioPutHigh(KPTB10);
					break;
				case 'A':
					gpioPutLow(KPTB10);
					break;

				case 'v':
					gpioPutHigh(KPTB7);
					break;
				case 'V':
					gpioPutLow(KPTB7);
					break;

				case 'r':
					gpioPutValue(KPTB6,1);
					break;
				case 'R':
					gpioPutValue(KPTB6,0);
					break;

				case 'M':
					i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, WHO_AM_I);
					if(nuevo_dato_i2c==0x1A)
						printf("MMA8451 encontrado!!\r\n");
					else
						printf("MMA8451 error\r\n");

					break;

				case 's':
					i2c0MasterReadByte(&nuevo_dato, MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1);
					if(nuevo_dato==0x01){
						printf("MMA8451 Active\r\n");
					}
					else if (nuevo_dato == 0x00){
						printf("MMA8451 Standby\r\n");
					}
					break;

				//Modo Activo
				case 'T':
					i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1, 0x01);
					break;
				//Modo Standby
				case 't':
					i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1, 0x00);
					break;

				case 'x':
				case 'X':
					i2c0MasterReadByte(&datox1, MMA851_I2C_DEVICE_ADDRESS, OUT_X_MSB);
					i2c0MasterReadByte(&datox2, MMA851_I2C_DEVICE_ADDRESS, OUT_X_LSB);
					xData  = (uint16_t)(datox1 << 6 | datox2 >> 2);
					xDatag = (xData*90)/kSENSITIVITY_2G;
					printf("%i Cuentas",xData);
					printf("\t%i Grados\r\n",xDatag);
					break;

				case 'y':
				case 'Y':
					i2c0MasterReadByte(&datoy1, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_MSB);
					i2c0MasterReadByte(&datoy2, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_LSB);
					yData  = (uint16_t)(datoy1 << 6 | datoy2 >> 2);
					yDatag = (yData*90)/kSENSITIVITY_2G;
					printf("%i Cuentas",yData);
					printf("\t%i Grados\r\n",yDatag);
					break;

				case 'z':
				case 'Z':
					i2c0MasterReadByte(&datoz1, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_MSB);
					i2c0MasterReadByte(&datoz2, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_LSB);
					zData  = (uint16_t)(datoz1 << 6 | datoz2 >> 2);
					zDatag = (zData*90)/kSENSITIVITY_2G;
					printf("%i Cuentas",zData);
					printf("\t%i Grados\r\n",zDatag);
					break;

				case 'c':
					calibracion();
					break;
				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }

    return 0 ;
}

void calibracion(){
	uint8_t datox1, datox2, datoy1, datoy2, datoz1, datoz2;
	uint16_t Xout, Yout, Zout, Xoffset, Yoffset, Zoffset;


	i2c0MasterReadByte(&datox1, MMA851_I2C_DEVICE_ADDRESS, OUT_X_MSB);
	i2c0MasterReadByte(&datox2, MMA851_I2C_DEVICE_ADDRESS, OUT_X_LSB);
	i2c0MasterReadByte(&datoy1, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_MSB);
	i2c0MasterReadByte(&datoy2, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_LSB);
	i2c0MasterReadByte(&datoz1, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_MSB);
	i2c0MasterReadByte(&datoz2, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_LSB);

	Xout = (uint16_t)(datox1 << 6 | datox2 >> 2);           // Compute 14-bit X-axis output value
	Yout = (uint16_t)(datoy1 << 6 | datoy2 >> 2);           // Compute 14-bit Y-axis output value
	Zout = (uint16_t)(datoz1 << 6 | datoz2 >> 2);           // Compute 14-bit Z-axis output value

	Xoffset = (Xout/8)*(-1);        // Compute X-axis offset correction value
	Yoffset = (Yout/8)*(-1);        // Compute Y-axis offset correction value
	Zoffset = (Zout-kSENSITIVITY_2G)/8*(-1);          // Compute Z-axis offset correction value

	// Standby mode to allow writing to the offset registers

	i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1, 0x00);

	i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, OFF_X, Xoffset);
	i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, OFF_Y, Yoffset);
	i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, OFF_Z, Zoffset);

	i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1, 0x01); //Set Active mode
}
