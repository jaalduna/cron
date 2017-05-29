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
    INTCON = 0b10001000;	//enable general interrupts and interrupt on port B
    INTCON = 0b0;
    INTCON |= 1<<7; //global interrupt enable
    INTCON |= 1<<6; //enable pheripheral interrupts 
    INTCON |= 1<<3; //RB port change interrupt enable
    INTCON |= 1<<5; //enable interrupts on timer0
    INTCON2 = 0b10000101;
    INTCON2 |= 1<2; //timer0 interrupt bit priority set to high

    T0CON = 0b10000010; //prescaler are on T0CON<2:0> and set to 100. This should triggers an interrupts each 100ms aprox.

    /*Timer 1 configuration register*/
    T1CON = 0x00;
    T1CON |= 1<<7; //RD16, enable 16 bits read/write
    T1CON |= 0<<5; //T1CKPS1 
    T1CON |= 0<<4; //T1CKPS0
    T1CON |= 0<<3; //T1OSCEN: disable T1 oscillator
    T1CON |= 0 << 1; //TMR1CS: internal clock (FOSC/4) 
    T1CON |= 0 << 0; //TMR1ON: START/STOP TMR1 
    /*PIE1 register*/
    PIE1 = 0x00;
    PIE1 |= 1 << 0; //TMR1 overflow interrupt enable bit

    TRISB1 = 0;
    TRISA2 = 0;
    TRISA5 = 0;
    TRISA3 = 0; //define its as input for now.
    /* Initialize peripherals */

    /* Configure the IPEN bit (1=on) in RCON to turn on/off int priorities */
    state = STATE_TIME;
    next  = STATE_TIME;
    /* Enable interrupts */
    
    /*lets set timer1_up_limit_sec and timer1_up_limit_min*/
    timer1_up_limit_sec = 59;
    timer1_up_limit_min = 59;


    /*Initial value for Force/Cold ambiguity in Inverval configuration*/
    current_type = 'f';
    current_program = 0;
    
}

/*put a character on the display on the first position*/
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

/*update all characters from the display with nums array*/
void put_nums(char nums[])
{
    int i;
    for(i=0;i<NUM_DIGITS;i++)
    {
        put_num(get_num(nums[i]));
    }
}
/*update all characters from display with individual arguments for each 7-segment display*/
void put_nums_individual(char num5, char num4, char num3, char num2, char num1, char num0)
{
    put_num(get_num(num0));
    put_num(get_num(num1));
    put_num(get_num(num2));
    put_num(get_num(num3));
    put_num(get_num(num4));
    put_num(get_num(num5));
}

/*Encode a human readable character into a 7 segment display format*/
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

/*Enables double point from display*/
void point_enable(void)
{
    POINT = 1;
}

/*Disable double point from display*/
void point_disable(void)
{
    POINT = 0;
}

/*Toggles double point from display*/
void point_toggle(void)
{
    if(POINT == 0)
        point_enable();
    else
        point_disable();
}

/*Enables buzzer */
void buzzer_enable(void)
{
    buzzer_status= 1;
}
/*Disable buzzer */
void buzzer_disable(void)
{
    buzzer_status= 0;
}
/*Toogle buzzer*/
void buzzer_toggle(void)
{
    if(buzzer_status== 0)
        buzzer_enable();
    else
        buzzer_disable();
}

/*RTC DS1302 RST (CE) enable*/
void ce_enable(void)

{
    RST = 1;
}

/*RTC DS1302 RST (CE) disable*/
void ce_disable(void)
{
    RST = 0;
    SCLK = 0;
}

/*This function reads the byte located in address from the RTC ic DS1302*/
char byte_read(char address)
{
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

/*byte_write writes "value" on "address" on the RTC ic DS1302*/
void byte_write(char address, char value)
{
    ce_enable();
    //send read command
    
    send_command(address);
    send_command(value);
    //ce disable
    ce_disable();
}

/*set_time sets seconds, minuts and hour on the RTC DS1302*/
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

/*get_time update "data" with time values to be displayed. Format = 1 means H1 (24H), Format = 0 means H2 (12H).
Format 1: H1 HH(24H):MM
Format 0: H2 HH(12H):MM*/
void get_time(char data[], unsigned char format)
{
   char aux = READ_RTC_0;
   char seconds =  byte_read(aux);
   aux = READ_RTC_1;
   char minutes =  byte_read(aux);
   aux = READ_RTC_2;
   char hours   =  byte_read(aux);
   
   
   data[0] = minutes & 0x0f;
   data[1] = (minutes & 0xf0)>>4; 
   
   if(format == 1)
   {
   
        data[4] = 1;
        data[2] = hours & 0x0f;
        data[3] = (hours & 0xf0)>>4;  
   
   }
   else
   {   
       data[4] = 2;
       char aux_hour = (hours>>4 & 0x0f)*10 + (hours & 0x0f);
       if(aux_hour<=12)
       {
           data[2] =  hours & 0x0f;
           data[3] = (hours & 0xf0)>>4;
       }
       else
       {
           data[2] = (hours & 0x0f) - 2;
           data[3] = (hours>>4 & 0x0f)-1;
       }
           
   }
   
 
   data[5] = 'H';
   
}

/*get_seconds_reg returns a register according to the second and minute register in DS1302.*/
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

char get_cents_reg(char cents)
{
    char res = 0; 
    res = (cents/10)<<4 | (cents % 10);
    return res;
}

/*get_hour_reg returns a register according to the hour register in DS1302.*/
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

/*This is a low level function to send a command to DS1302*/
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

/*Configure pin IO, in the DS1302 interface, as input.*/
void io_as_input(void)
{
    TRISA3 = 1;
}

/*Configure pin IO, in the DS1302 interface, as output.*/
void io_as_output(void)
{
    TRISA3 = 0;
}


/*get_next_state retuns calculate and returns next state from the current state and the last code received.*/
int get_next_state(int state,int code)
{
     if(state == STATE_TIME && code == IR_EDIT) 
            next = STATE_HH1;
     else if((state == STATE_TIME || state == STATE_DOWN || state == STATE_SPEED || state == STATE_INTERVAL)  && code == IR_UP) 
            next = STATE_UP;
     else if((state == STATE_TIME || state == STATE_UP || state == STATE_SPEED || state == INTERVAL) && code == IR_DOWN) 
            next = STATE_DOWN;
     else if((state == STATE_TIME || state == STATE_UP || state == STATE_DOWN || state == STATE_SPEED) && code == IR_RIGHT)
            next = STATE_INTERVAL;
     else if(state == STATE_HH1 && code == IR_RIGHT) 
            next = STATE_HH2;
     else if(state == STATE_HH2 && code == IR_RIGHT) 
            next = STATE_MM1;
     else if(state == STATE_MM1 && code == IR_RIGHT) 
            next = STATE_MM2;
     else if(state == STATE_MM2 && code == IR_RIGHT) 
            next = STATE_HH1;
     else if((state == STATE_UP || state == STATE_DOWN || state == STATE_SPEED || state = STATE_INTERVAL) && code == IR_ZERO)
            next = STATE_TIME;
     else if((state == STATE_HH1 || state == STATE_HH2 || state == STATE_MM1 || state == STATE_MM2) && code == IR_ESC) 
     {
            next = STATE_TIME;
            char seconds = 0;
            char minutes = aux1[0] + aux1[1]*10;
            char hour = aux1[2] + aux1[3]*10;
            set_time(seconds,minutes,hour);
     }
     else if(state == STATE_UP && code == IR_OK)
	   {
		next = STATE_UP_COUNT_DOWN;
		//lets activate timer1 counter
		timer1_enable();
	   } 
     else if(state == STATE_UP_COUNT_DOWN && timer1_counter_10 == 0)
		next = STATE_UP_COUNTING;	
    else if(state == STATE_UP_COUNTING && timer1_counter_min == timer1_up_limit_min && timer1_counter_10 == timer1_up_limit_sec)
    {
        next = STATE_UP_STOP;
    }
    else if((state == STATE_UP_STOP || state == STATE_UP_COUNTING ) && code == IR_GATO)
        next = STATE_UP;
    else if(state == STATE_UP && code == IR_ASTERISC)
    {
        next = STATE_UP_CFG_MM1;
    }
    else if((state == STATE_UP_CFG_MM1  || state == STATE_UP_CFG_MM2 || state == STATE_UP_CFG_SS1 || state == STATE_UP_CFG_SS2) && code == IR_GATO)
    {
        next = STATE_UP;
        //lets save the limit configuration
        timer1_up_limit_sec = aux1[1]*10+aux1[0];
        timer1_up_limit_min = aux1[3]*10 + aux1[2];
    }
    else if(state == STATE_DOWN && code == IR_OK)
    {
        next = STATE_DOWN_COUNT_DOWN;
        //lets activate timer1 counter
        timer1_enable();
    }
    else if(state == STATE_DOWN_COUNT_DOWN && timer1_counter_10 == 0)
    {
        next = STATE_DOWN_COUNTING;
        //lets load initial countdown on timers1_counter_10 and timer1_counter_min
        timer1_counter_min = timer1_down_init_min;
        timer1_counter_10 = timer1_down_init_sec;
    }
    else if(state == STATE_DOWN_COUNTING && timer1_counter_min == 0 && timer1_counter_10 == 0)
    {
        next = STATE_DOWN_STOP;
    }
    else if(state == STATE_DOWN_STOP && code == IR_GATO)
    {
        next = STATE_DOWN;
    }
    else if(state == STATE_DOWN && code == IR_ASTERISC)
    {
        next = STATE_DOWN_CFG_MM1;
    }
    else if((state == STATE_DOWN_CFG_MM1 || state == STATE_DOWN_CFG_MM2 || state == STATE_DOWN_CFG_SS1 || state == STATE_DOWN_CFG_SS2) && code == IR_GATO)
    {
        next = STATE_DOWN;
        timer1_down_init_min = aux1[3]*10 + aux1[2];
        timer1_down_init_sec = aux1[1]*10 + aux1[0];
    }
    else if((state == STATE_DOWN || state == STATE_UP || state == STATE_TIME) && code == IR_LEFT)
    {
        next = STATE_SPEED;
    }
    else if(state == STATE_SPEED && code == IR_OK)
    {
        next = STATE_SPEED_RUN;
    }
    else if(state == STATE_SPEED_RUN && code == IR_ASTERISC)
    {
        next = STATE_SPEED_PAUSE;
    }
    else if(state == STATE_SPEED_PAUSE && code == IR_OK)
    {
        next = STATE_SPEED_RUN;
    }
    else if((state == STATE_SPEED_RUN || state == STATE_SPEED_PAUSE) && code == IR_GATO)
    {
        next = STATE_SPEED;
    }
    else if(state == STATE_INTERVAL && code == IR_OK)
    {
        next = STATE_INTERVAL_FORCE;
        current_program = 1;
    }
    else if(state == STATE_INTERVAL_FORCE && timer1_counter_min == 0 && timer1_counter_10 == 0)
    {
        next = STATE_INTERVAL_COLD;
    }
    else if(state == STATE_INTERVAL_COLD &&  timer1_counter_min == 0 && timer1_counter_10 == 0)
    {
        if(current_program <= max_program)
        {
            next = STATE_INTERVAL_FORCE;
            current_program +=1;
        }
        else
        {
            next = STATE_INTERVAL_STOP;
        }
    }
    else if(state == STATE_INTERVAL_STOP && code == IR_GATO)
    {
        next = STATE_INTERVAL;
    }
    else if((state == STATE_INTERVAL_CFG_MM1 || state == STATE_INTERVAL_CFG_MM2 || state == STATE_INTERVAL_CFG_SS1 || state == STATE_INTERVAL_CFG_SS2) && code == IR_ASTERISC)
    {
        if(current_program <= NUM_INTERVALS)
        {
            if(current_type == 'f')
            {
                intervals.force[current_program].seconds = aux1[1]*10 + aux1[0]; 
                intervals.force[current_program].minutes = aux1[3]*10 + aux1[2]; 
                current_type = 'c';
            }
            else
            {
                intervals.force[current_program].seconds = aux1[1]*10 + aux1[0]; 
                intervals.force[current_program].minutes = aux1[3]*10 + aux1[2]; 
                current_type = 'f';
                current_program +=1;
            }
           
        }
        else
        {

        }
    }
    else
        next = next;
    return next;
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

/*update_point updates double point in display according to current state and counter*/
void update_point(int state, char counter)
{
	if(counter == 0 && (state == STATE_TIME || state == STATE_UP || state == STATE_UP_COUNTING))
	{
		point_enable();
	}
	else if(counter == 5 && state == STATE_TIME && state != STATE_UP)
	{
		point_disable();	
	}
}

/*update_buzzer updates buzzer status according to current state and counter*/
void update_buzzer(int state, char counter)
{
	if(counter == 0 && (state == STATE_TIME || state == STATE_UP || state == STATE_UP_COUNTING))
	{
		buzzer_enable();
	}
	else if(counter == 5 && state == STATE_TIME && state != STATE_UP)
	{
		buzzer_disable();	
	}
}

void get_timer1_counter( char data[])
{
   data[0] = get_seconds_reg(timer1_counter_10) & 0x0f;
   data[1] = (get_seconds_reg(timer1_counter_10) & 0xf0)>>4;
   data[2] = get_seconds_reg(timer1_counter_min) & 0x0f;
   data[3] = (get_seconds_reg(timer1_counter_min) & 0xf0)>>4;
   data[4] = 'P'; 
   data[5] = 'U';

}

void get_timer1_counter_down( char data[])
{
   data[0] = get_seconds_reg(timer1_counter_10) & 0x0f;
   data[1] = (get_seconds_reg(timer1_counter_10) & 0xf0)>>4;
   data[2] = get_seconds_reg(timer1_counter_min) & 0x0f;
   data[3] = (get_seconds_reg(timer1_counter_min) & 0xf0)>>4;
   data[4] = 'N'; 
   data[5] = 'D';
}

void get_timer1_counter_speed(char data[])
{
   data[0] = get_cents_reg(timer1_counter) & 0x0f;
   data[1] = (get_cents_reg(timer1_counter) & 0xf0)>>4;
   data[2] = get_seconds_reg(timer1_counter_10) & 0x0f;
   data[3] = (get_seconds_reg(timer1_counter_10) & 0xf0)>>4;
   data[4] = get_seconds_reg(timer1_counter_min) & 0x0f;
   data[5] = (get_seconds_reg(timer1_counter_min) & 0xf0)>>4;
}

void get_timer1_counter_interval(char data[],char state, char current_program)
{
   data[0] = get_seconds_reg(timer1_counter_10) & 0x0f;
   data[1] = (get_seconds_reg(timer1_counter_10) & 0xf0)>>4;
   data[2] = get_seconds_reg(timer1_counter_min) & 0x0f;
   data[3] = (get_seconds_reg(timer1_counter_min) & 0xf0)>>4;

   if(state == STATE_INTERVAL_FORCE)
   {
       data[4] = current_program;
       data[5] = 'F';
   }
   else if(state == STATE_INTERVAL_COLD)
   {
       data[4] = current_program;
       data[5] = 'C';    
   }
   else
   {
       data[4] = 0;
       data[5] = 0;
   }

}
 


void timer1_enable(void)
{
	    T1CON |= 1 << 0; //TMR1ON: START/STOP TMR1
}

void timer1_disable(void)
{
	    T1CON &= ~(1 << 0); //TMR1ON: START/STOP TMR1
}

void update_timer1_counter_10(char state, char *counter)
{
	if(state == STATE_UP)
		*counter = UP_INITIAL_COUNTDOWN;
	else if(state == STATE_UP_COUNT_DOWN)
    {
	 	if(*counter>0)
        {
			if (*counter>3)
            {
				*counter-=1;
            }
			else
			{
				*counter-=1;
				//put here logic to beep the buzzer
			}
        }


    }
	else if(state == STATE_UP_COUNTING)
  		if(*counter<59)		
        {
			*counter+=1;
        }
		else
		{
			*counter= 0;
			//put here the logic for increment minutes, maybe create and use other variable
            if(timer1_counter_min < 59)
                timer1_counter_min +=1;
            else
                timer1_counter_min = 0;
		}
    else if(state == STATE_DOWN)
        *counter = DOWN_INITIAL_COUNTDOWN;
    else if(state == STATE_DOWN_COUNT_DOWN)
    {
        if(*counter >0)
        {
            if(*counter>3)
            {
                *counter -=1;
            }
            else
            {
                *counter -=1;
                //put here logic to beep the buzzer
            }
        }
    }
    else if(state == STATE_DOWN_COUNTING)
    {
        if(*counter>0)
        {
            *counter -=1;
        }
        else
        {
            if(timer1_counter_min > 0)
            {
                timer1_counter_min -=1;
                *counter = 59;
            }
            
        }
    }
    else if(state == STATE_SPEED)
    {
        *counter = 0;
        timer1_speed_cents = 0;
        timer1_counter_min = 0;
    }
    else if(state == STATE_SPEED_RUN)
    {
        if(*counter<59)
        {
            *counter +=1;
        }
        else
        {
            *counter = 0;
            if(timer1_counter_min<59)
            {
                timer1_counter_min +=1;
            }
        }
    }
}
