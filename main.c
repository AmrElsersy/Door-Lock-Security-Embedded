//#include <stdint.h>
#include "Keypad/keypad.h"
#include "UART/uart.h" 
#include "LCD/lcd.h"
#include <stdlib.h>

#define PASSWORD_SIZE 4

/*************************** FUNCTION DECLRATIONS *************************/ 
void getPassword(char * pw_arr);
bool Login_Validation(char * pw_arr , char* pw_true);
/************************** MAIN FUNCTION ************************************/

// normal state that client can write the password to open the door
#define PASSWORD_STATE 0
// cleaning state that the door is always open
#define CLEANING_STATE 1
// closing state where the room is unbooked and always close
#define CLOSING_STATE 2
// for door
#define OPEN 1
#define CLOSED 0


struct {
	int Room_Number;
	int Room_State;
	char *Password;
	int Door_State;
} Door;

int main()
{  	
	keypad_Init();
	LCD_Init();
	Init_UART();

	struct Door;
	Door.Room_Number = 4;
	Door.Room_State = CLOSING_STATE;
	Door.Password = "";
	
	char* roomString  = UART_Read_String();
	char* stateString = UART_Read_String();

	int state = atoi(stateString);
	int room = atoi(roomString);

	// set password state
	if(state == PASSWORD_STATE)
	{
			if (Door.Room_Number == room)
			{
				// set the new password that get sent by UART from the PC
				char *newPassword = UART_Read_String();
				Door.Password = newPassword;
				
				// wait for the client to read the password by the Keybad
				char* keybadPassword;
				getPassword(keybadPassword);

				// check the equality between the 2 passwords
				if (Login_Validation(Door.Password,keybadPassword))
				{
					Door.Door_State = OPEN;
				}
				else
				{
					Door.Door_State = CLOSED;
				}
			}
	}

	// room cleaning state
	else if (state == CLEANING_STATE)
	{
			if (Door.Room_Number == room)
			{
					Door.Room_State = CLEANING_STATE;
					Door.Door_State = OPEN;
			}		
	}
	else if (state == CLOSING_STATE)
	{
			if (Door.Room_Number == room)
			{
					Door.Room_State = CLOSING_STATE;
					Door.Door_State = CLOSED;				
			}
	}
}

/*************************** FUNCTION DEFINITIONS *************************/ 

bool Login_Validation(char * pw_arr , char* pw_true)
{
	
	for ( uint8_t j=0; j<PASSWORD_SIZE; j++){
				// wrong password if 1 character dosn't match
				if(pw_arr[j] != pw_true[j]){
					
					SET_BIT(GPIO_PORTD_AHB_DATA_R ,3);
					return 0;
				}
		}
	return 1;
}

void getPassword(char * pw_arr)
{	
	uint16_t i=0;
	char space_counter = 0; 
	while(1)
	{			
			char x=KeyPad_getPressedKey();
			if (x == '+' && i == PASSWORD_SIZE )
			{
				if(space_counter < PASSWORD_SIZE )
				{
					LCD_Clear();
					LCD_Write_String("Invalid Password");
					LCD_Set_Cursor_Position(1,6);					
					i=0;
					space_counter = 0;
				}
				else
					return;
			}
			
			// move the cursor on the LCD to the left 1 point
			else if (x =='.'&& i > 0 )
			{
				LCD_Shift_Cursor_Left();
				i--;
			}
			// move the cursor on the LCD to the right 1 point
			else if ( x== '=' &&  i < PASSWORD_SIZE )
			{
				LCD_Shift_Cursor_Right();
				i ++;
			}
			// delete the last character
			else if (x=='/' && i > 0 &&  i <= PASSWORD_SIZE)
			{
				LCD_Back(); 
				--i ;
				space_counter --;
			}
			// if the user enter a number, add it to the password
			else if( ( x >= '0' && x <= '9' ) && i < PASSWORD_SIZE  ) 
			{
				LCD_Write_Char(x);
				pw_arr[i]=x;
				i++; 
				space_counter++;
			}

			delay_m(1000);
	}
}

