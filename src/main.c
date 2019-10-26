/*An application that has two tasks, task1 and task2.
 * task1 priority 1
 * task2 priority 2
 * task3 priority 3
 * Task 3 should toggle the LED every 1 seconds and should delete itself when the user button is pressed by the user
 * Task 2 should toggle the led for every 500 ms and should delete itself when the user button is pressed by the user
 * Task 1 should toggle the led for every 200 ms
 */




#include<string.h>
#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"

#define TRUE 1
#define FALSE 0

#define NOT_PRESSED FALSE
#define PRESSED TRUE

//function prototypes
static void prvSetupHardware(void);
void printmsg(char *msg);
static void prvSetupUart(void);
void prvSetupGpio(void);
void rtos_delay(uint32_t delay_in_ms);

//tasks prototypes
void vTask1_handler(void *params);
void vTask2_handler(void *params);
void vTask3_handler(void *params);

//global space for some variable
char usr_msg[250]={0};
TaskHandle_t xTaskHandle1=NULL;
TaskHandle_t xTaskHandle2=NULL;
TaskHandle_t xTaskHandle3=NULL;

int main(void)
{
	DWT->CTRL |= (1 << 0);//Enable CYCCNT in DWT_CTRL.

	//1.  Reset the RCC clock configuration to the default reset state.
	//HSI ON, PLL OFF, HSE OFF, system clock = 16MHz, cpu_clock = 16MHz
	RCC_DeInit();

	//2. update the SystemCoreClock variable
	SystemCoreClockUpdate();

	prvSetupHardware();

	sprintf(usr_msg,"This is Task Delete Demo\r\n");
	printmsg(usr_msg);


	//task-1
    xTaskCreate(vTask1_handler,"TASK-1",500,NULL,1,&xTaskHandle1);

    //task-2
    xTaskCreate(vTask2_handler,"TASK-2",500,NULL,2,&xTaskHandle2);

    //Task3
    xTaskCreate(vTask3_handler,"TASK-3",500,NULL,3,&xTaskHandle3);

    //Starting the scheduler
    vTaskStartScheduler();

	for(;;);
}


void vTask1_handler(void *params)
{

	sprintf(usr_msg,"Task-1 is running\r\n");
	printmsg(usr_msg);

	while(1)
	{
		//rtos_delay(200);
		vTaskDelay(100);
		GPIO_ToggleBits(GPIOA,GPIO_Pin_5);

	}
}



void vTask2_handler(void *params)
{
	sprintf(usr_msg,"Task-2 is running\r\n");
	printmsg(usr_msg);

	while(1)
	{
		if( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) )
				{
					//button is not pressed on the nucleo board
					//lets toggle the led for every 1 sec
		             rtos_delay(500);
					 GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
				}else
				{
					//button is pressed on the nucleo board
					//Task2 deletes itself
					sprintf(usr_msg,"Task2 is getting deleted\r\n");
					printmsg(usr_msg);
					vTaskDelete(NULL);
				}
	}


}


void vTask3_handler(void *params)
{


	sprintf(usr_msg,"Task-3 is running\r\n");
	printmsg(usr_msg);

	while(1)
	{
		if( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) )
		{
			//button is not pressed on the nucleo board
			//lets toggle the led for every 1 sec
             rtos_delay(1000);
			 GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
		}else
		{
			//button is pressed on the nucleo board
			//Task3 deletes itself
			sprintf(usr_msg,"Task3 is getting deleted\r\n");
			printmsg(usr_msg);
			vTaskDelete(NULL);
			rtos_delay(1000);
		}

	}
}




static void prvSetupHardware(void)
{
	//Setup Button and LED
	prvSetupGpio();

	//setup UART2
	prvSetupUart();
}

void printmsg(char *msg)
{
	for(uint32_t i=0; i < strlen(msg); i++)
	{
		while ( USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);
		USART_SendData(USART2,msg[i]);
	}

	while ( USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);

}


static void prvSetupUart(void)
{
	GPIO_InitTypeDef gpio_uart_pins;
	USART_InitTypeDef uart2_init;

	//1. Enable the UART2  and GPIOA Peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	//PA2 is UART2_TX, PA3 is UART2_RX

	//2. Alternate function configuration of MCU pins to behave as UART2 TX and RX

	//zeroing each and every member element of the structure
	memset(&gpio_uart_pins,0,sizeof(gpio_uart_pins));

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_uart_pins.GPIO_OType= GPIO_OType_PP;
	gpio_uart_pins.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA, &gpio_uart_pins);


	//3. AF mode settings for the pins
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //PA2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //PA3

	//4. UART parameter initializations
	//zeroing each and every member element of the structure
	memset(&uart2_init,0,sizeof(uart2_init));

	uart2_init.USART_BaudRate = 115200;
	uart2_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart2_init.USART_Mode =  USART_Mode_Tx | USART_Mode_Rx;
	uart2_init.USART_Parity = USART_Parity_No;
	uart2_init.USART_StopBits = USART_StopBits_1;
	uart2_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&uart2_init);


	//5. Enable the UART2 peripheral
	USART_Cmd(USART2,ENABLE);

}


void prvSetupGpio(void)
{
// this function is board specific

	//Peripheral clock enable for GPIOA and GPIOC
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);

	GPIO_InitTypeDef led_init, button_init;
	led_init.GPIO_Mode = GPIO_Mode_OUT;
	led_init.GPIO_OType = GPIO_OType_PP;
	led_init.GPIO_Pin = GPIO_Pin_5;
	led_init.GPIO_Speed = GPIO_Low_Speed;
	led_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&led_init);

	button_init.GPIO_Mode = GPIO_Mode_IN;
	button_init.GPIO_OType = GPIO_OType_PP;
	button_init.GPIO_Pin = GPIO_Pin_13;
	button_init.GPIO_Speed = GPIO_Low_Speed;
	button_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&button_init);


}

void rtos_delay(uint32_t delay_in_ms)
{
	uint32_t current_tick_count = xTaskGetTickCount();

	uint32_t delay_in_ticks = (delay_in_ms * configTICK_RATE_HZ ) /1000 ;

	while(xTaskGetTickCount() <  (current_tick_count + delay_in_ticks));

}
