#include "UART.h"
#include "debug.h"

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

void printn(char *msg)
{
	printf("%s%s",msg,newline);
}

void UART_init()
{
	UartHandle.Instance				= USARTx;
	UartHandle.Init.BaudRate		= USARTx_BUAD;
	UartHandle.Init.WordLength		= UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits		= UART_STOPBITS_1;
	UartHandle.Init.Parity			= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl		= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode			= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling	= UART_OVERSAMPLING_8;
	if (HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		/* Initialization Error */
		error("HAL_UART_Init() failed!");
	}
	else
	{
		printf(newline);
		printn("XPhone boot!");
	}
}
