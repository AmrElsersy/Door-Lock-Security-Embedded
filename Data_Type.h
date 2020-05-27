#ifndef DATA_TYPE
#define DATA_TYPE


typedef unsigned char uint8_t ;
typedef char int8_t ;
typedef unsigned short uint16_t ;
typedef short int16_t ;
typedef unsigned long uint32_t ;
typedef long int32_t ;
typedef enum {true=1, false=0} bool;
// Set & Clear 
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))


#endif

