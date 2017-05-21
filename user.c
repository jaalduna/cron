/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* TODO Initialize User Ports/Peripherals/Project here */
    /*Pins for 74HC595 (7segments display controller)*/
    TRISB3 = 0; //makes 3th bit of PORTB Output;
    TRISB4 = 0; //makes 4th bit of PORTB Output;
    /* Setup analog functionality and port direction */
    CCP2CON  = 0x00;
    SSPCON1 = 0X00; //disable SPI on PORTA.
    ADCON1 = 0X0F; //set all pins related with A/D as digital I/O
    INTCON = 0b10001000;
    TRISB1 = 0;
    TRISA2 = 0;
    TRISA5 = 0;
    TRISA3 = 0; //define its as input for now.
    /* Initialize peripherals */

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */
    state = STATE_TIME;
    next  = STATE_TIME;
    /* Enable interrupts */
}

void put_num(char num)
{
    char mask = 0x01;
        int i;
        for(i= 0; i<8; i++)
        {
            DS = num>>i & mask;
            SHCP = 0;
            SHCP = 1;
        }
}

void put_nums(char nums[])
{
    int i;
    for(i=0;i<NUM_DIGITS;i++)
    {
        put_num(get_num(nums[i]));
    }
}
/*update all the numbers from the display*/
void put_nums_individual(char num5, char num4, char num3, char num2, char num1, char num0)
{
    put_num(get_num(num0));
    put_num(get_num(num1));
    put_num(get_num(num2));
    put_num(get_num(num3));
    put_num(get_num(num4));
    put_num(get_num(num5));
}

char get_num(char num)
{
    switch(num)
    {
        case 1: return SEVEN_SEG_ONE;
        case 2: return SEVEN_SEG_TWO;
        case 3: return SEVEN_SEG_THREE;
        case 4: return SEVEN_SEG_FOUR;
        case 5: return SEVEN_SEG_FIVE;
        case 6: return SEVEN_SEG_SIX;
        case 7: return SEVEN_SEG_SEVEN;
        case 8: return SEVEN_SEG_EIGHT;
        case 9: return SEVEN_SEG_NINE;
        case 0: return SEVEN_SEG_ZERO;
        case 'U': return SEVEN_SEG_U;
        case 'P': return SEVEN_SEG_P;
        case 'H': return SEVEN_SEG_H;
        case 'D': return SEVEN_SEG_D;
        case 'N': return SEVEN_SEG_N;
        case 'V': return SEVEN_SEG_VOID;
        default: return SEVEN_SEG_EIGHT;  
    }
}

void point_enable(void)
{
    POINT = 1;
}

void point_disable(void)
{
    POINT = 0;
}

void point_toggle(void)
{
    if(POINT == 0)
        point_enable();
    else
        point_disable();
}

void buzzer_enable(void)
{
    BUZZER = 1;
}

void buzzer_disable(void)
{
    BUZZER = 0;
}

void buzzer_toggle(void)
{
    if(BUZZER == 0)
        buzzer_enable();
    else
        buzzer_disable();
}

void ce_enable(void){
    RST = 1;
}
void ce_disable(void){
    RST = 0;
    SCLK = 0;
}

char byte_read(char address){
    char res = 0;
    
    //ce enable
    ce_enable();
    //send read command
    send_command(address);
    io_as_input();
    int i;
    for(i=0;i<8;i++)
    {
        SCLK = 0;
        __delay_us(4);
        res |= IO<<i;
        SCLK = 1;
        __delay_us(10);
        
    }
    //ce disable
    ce_disable();
    
    //return res
    return res; 
}

void byte_write(char address, char value)
{
    ce_enable();
    //send read command
    
    send_command(address);
    send_command(value);
    //ce disable
    ce_disable();
}

void set_time(char seconds, char minutes, char hour)
{
    //calculate seconds register
    char sec_reg = get_seconds_reg(seconds);
    char min_reg = get_seconds_reg(minutes);
    char hour_reg = get_hour_reg(hour);
    
    byte_write(WRITE_RTC_0,sec_reg);
    byte_write(WRITE_RTC_1,min_reg);
    byte_write(WRITE_RTC_2,hour_reg);
    
   
}
void get_time(char data[])
{
   char aux = READ_RTC_0;
   char seconds =  byte_read(aux);
   aux = READ_RTC_1;
   char minutes =  byte_read(aux);
   aux = READ_RTC_2;
   char hours   =  byte_read(aux);
   
   data[0] = minutes & 0x0f;
   data[1] = (minutes & 0xf0)>>4; 
   data[2] = hours & 0x0f;
   data[3] = (hours & 0xf0)>>4;   
   data[4] = 1;
   data[5] = 'H';
}

char get_seconds_reg(char seconds)
{
    char res = 0;
    if(seconds<10)
    {
        res = seconds;
    }
    else if(seconds <20)
    {
        res = 1<<4 | (seconds - 10);
    }
    else if(seconds <30)
    {   
        res = 2<<4 | (seconds -20);
    }
    else if(seconds <40)
    {
        res = 3<<4 | (seconds - 30);
    }
    else if(seconds <50)
    {
        res = 4<<4 | (seconds - 40);
    }
    else 
    {
        res = 5<<4 | (seconds -50);
    }
    
    return res;
}
char get_hour_reg(char hour)
{   
    char res = 0;
    if(hour < 10)
    {
        res = hour;
    }
    else if(hour < 20)
    {
        res = 1<<4  | (hour -10);
    }
    else 
    {
        res = 2<<4 | (hour-20);
    }
    
    return res;

}
void send_command(char command)
{
    int i;
    io_as_output();
    for(i=0; i<8; i++)
    {
        IO = command>>i & 0x01;
        SCLK = 1;
        __delay_us(10);
        SCLK = 0;
        __delay_us(4);
        
    }
}

void io_as_input(void)
{
    TRISA3 = 1;
}

void io_as_output(void)
{
    TRISA3 = 0;
}

void get_code(char code[])
{
    
}

int get_next_state(int state,int code)
{
     if(state == STATE_TIME && code == IR_EDIT) 
            next = STATE_HH1;
     else if(state == STATE_HH1 && code == IR_RIGHT) 
            next = STATE_HH2;
     else if(state == STATE_HH2 && code == IR_RIGHT) 
            next = STATE_MM1;
     else if(state == STATE_MM1 && code == IR_RIGHT) 
            next = STATE_MM2;
     else if(state == STATE_MM2 && code == IR_RIGHT) 
            next = STATE_HH1;
     else if((state == STATE_HH1 || state == STATE_HH2 || state == STATE_MM1 || state == STATE_MM2) && code == IR_ESC) 
            next = STATE_TIME;
        else
            next = next;
}

/*is_code_number return 1 if human_code is a number or 0 if not*/
char ir_is_code_number(char human_code)
{
    if(human_code == 0 || human_code == 1 || human_code == 2 || human_code == 3 || human_code == 4
            || human_code == 5 || human_code == 6 || human_code == 7 || human_code == 8 || human_code == 9)
        return 1;
    else
        return 0;
}

/*ir_get_human_code returns a human readable code for each pressed button. if a non registered code is press, then -1 is returned.*/
char ir_get_human_code(int code)
{
    char human_read_code;
        switch(code)
        {
                case IR_UP: 
                    human_read_code =  10;  
                    break;
                case IR_OK:
                    human_read_code = 11;
                    break;
                case IR_DOWN:
                    human_read_code = 12;
                    break;
                case IR_LEFT:
                    human_read_code = 13;
                    break;
                case IR_RIGHT:
                    human_read_code =  14;
                    break;
                case IR_ONE:
                    human_read_code =  1;
                    break;
                case IR_TWO:
                    human_read_code =  2;
                    break;
                case IR_THREE:
                    human_read_code = 3;
                    break;
                case IR_FOUR:
                    human_read_code =  4;
                    break;
                case IR_FIVE:
                    human_read_code =  5;
                    break;
                case IR_SIX:
                    human_read_code = 6;
                    break;
                case IR_SEVEN:
                    human_read_code = 7;
                    break;
                case IR_EIGHT:
                    human_read_code = 8;
                    break;
                case IR_NINE:
                    human_read_code = 9;
                    break;
                case IR_ZERO:
                    human_read_code = 0;
                    break;
                case IR_ASTERISC: 
                    human_read_code = 15;
                    break;
                case IR_GATO:
                    human_read_code = 16;
                    break;
                default:
                    return -1;
        }
                            
                
}