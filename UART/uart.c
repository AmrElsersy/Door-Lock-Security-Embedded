#include "uart.h"
#define MAX_SIZE 4

void Init_UART()
{
	SYSCTL_RCGCUART_R |= 0x0002;
	SYSCTL_RCGCGPIO_R |= 0x0002;
	
	CLEAR_BIT(UART1_CTL_R,0);
	UART1_IBRD_R = 27;
	UART1_FBRD_R = 8;
	
	UART1_LCRH_R = 0x0070;
	UART1_CTL_R  = 0x301;
	
	GPIO_PORTB_AFSEL_R |= 0x03;	
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_AMSEL_R &= ~0x03;	
}

char UART_Read_Char()
{
	while(UART1_FR_R & 0x10 != 0 );
	char x = UART1_DR_R;
	return x;	
}

char* UART_Read_String()
{
  char * str = (char *)malloc(MAX_SIZE);
	int i =0 ;
	while(true)
	{
		str[i] = UART_Read_Char();

		if(str[i] != '\0')
		{
				return str;
		}

		i++;
	}
}





