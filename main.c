/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#include <stdio.h>



void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */

    
    aux1[0] = 1;
    aux1[1] = 2;
    aux1[2] = 3;
    aux1[3] = 4;
    aux1[4] = 5;
    aux1[5] = 6;
    
    char temp; //variable to hold values of aux1
    char format = 1;
    buzzer_disable();
    while(1)
    {       
        state = next;
        
        switch(state){
                case STATE_TIME:
                    
                    if(human_code == HR_IR_LEFT)
                        format = FORMAT_24H;
                    else if (human_code == HR_IR_RIGHT)
                        format = FORMAT_12H;
                    get_time(aux1,format);
                    /*put_nums(aux1);
                    __delay_ms(500);
                    point_toggle();
                    //buzzer_toggle();
                    __delay_ms(500);
                    point_toggle(); */
                    break;
            case STATE_HH1:
                
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=2)
                {
                    aux1[3] = human_code; //update number
                    next = STATE_HH2;   //update state
                    human_code = -1;
                }
                break;
            case STATE_HH2:
                
                 if(ir_is_code_number(human_code) && ((aux1[2]<2 && human_code <=9) || human_code <=3))
                 {
                     aux1[2] = human_code; //update number
                     next = STATE_MM1;   //update state
                     human_code = -1;
                 }
                break;
            case STATE_MM1:
                
                if(ir_is_code_number(human_code) && human_code <=5) //check for a valid number
                 {
                     aux1[1] = human_code; //update number
                     next = STATE_MM2;   //update state
                     human_code = -1;
                 }
                break;   
            case STATE_MM2:
                if(ir_is_code_number(human_code) && human_code <=9) //check for a valid number
                 {
                     aux1[0] = human_code; //update number
                     next = STATE_HH1;   //update state
                     human_code = -1;
                 }       
                break;    
                
            case STATE_UP:
		        timer1_counter_10 = UP_INITIAL_COUNTDOWN;	
			get_timer1_counter(aux1);	
			
                break;
	    case STATE_UP_COUNT_DOWN:
			get_timer1_counter(aux1);	
		break;
	    case STATE_UP_COUNTING:
			get_timer1_counter(aux1);
		break;
        case STATE_UP_STOP:
            get_timer1_counter(aux1);
        break;
        case STATE_UP_CFG_MM1:
                
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[3] = human_code; //update numberS
                    next = STATE_UP_CFG_MM2;   //update state
                    human_code = -1;
                }
                break;
        case STATE_UP_CFG_MM2:
                
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[2] = human_code; //update numberS
                    next = STATE_UP_CFG_SS1;   //update state
                    human_code = -1;
                }
                break;
        case STATE_UP_CFG_SS1:
                
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[1] = human_code; //update numberS
                    next = STATE_UP_CFG_SS2;   //update state
                    human_code = -1;
                }
                break;
        case STATE_UP_CFG_SS2:
                
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[0] = human_code; //update numberS
                    next = STATE_UP_CFG_MM1;   //update state
                    human_code = -1;
                }
                break;
            case STATE_DOWN:
                timer1_counter_10 = UP_INITIAL_COUNTDOWN;  
                get_timer1_counter_down(aux1);   
                break;
            case STATE_DOWN_COUNT_DOWN:
                get_timer1_counter_down(aux1);   
                break;
            case STATE_DOWN_COUNTING:
                get_timer1_counter_down(aux1);   
                break;     
            case STATE_DOWN_STOP:
                get_timer1_counter_down(aux1);   
                break;               
            case STATE_DOWN_CFG_MM1:
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[3] = human_code; //update numberS
                    next = STATE_DOWN_CFG_MM2;   //update state
                    human_code = -1;
                }
                break;
            case STATE_DOWN_CFG_MM2:
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[2] = human_code; //update numberS
                    next = STATE_DOWN_CFG_SS1;   //update state
                    human_code = -1;
                }
                break;
            case STATE_DOWN_CFG_SS1:
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[1] = human_code; //update numberS
                    next = STATE_DOWN_CFG_SS2;   //update state
                    human_code = -1;
                }
                break;
            case STATE_DOWN_CFG_SS2:
                //check if code is a valid number
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[0] = human_code; //update numberS
                    next = STATE_DOWN_CFG_MM1;   //update state
                    human_code = -1;
                }
                break;
            case STATE_SPEED:
                timer1_disable();
                timer1_counter_10 = 0;
                timer1_counter_min = 0;
                timer1_counter = 0;
                get_timer1_counter_speed(aux1);
                break;
            case STATE_SPEED_RUN:
                timer1_enable();
                get_timer1_counter_speed(aux1);
                break;
            case STATE_SPEED_PAUSE:
                timer1_disable();
                get_timer1_counter_speed(aux1);
                break;

            case STATE_INTERVAL:
                timer1_disable();
                timer1_counter_10 = 0;
                timer1_counter_min = 0;
                timer1_counter = 0;
                current_program = 0;
                current_type = 'f';
                get_timer1_counter_interval(aux1,state,current_program);
                break;
            case STATE_INTERVAL_FORCE:
                timer1_enable();
                get_timer1_counter_interval(aux1,state,current_program);
                break;
            case STATE_INTERVAL_COLD:
                timer1_enable();
                get_timer1_counter_interval(aux1,state,current_program);
                break;
            case STATE_INTERVAL_STOP:
                timer1_disable();
                get_timer1_counter_interval(aux1,state,current_program);
                break;
            case STATE_INTERVAL_CFG_MM1:
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[3] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_MM2;   //update state
                    human_code = -1;
                }
                break;
            case STATE_INTERVAL_CFG_MM2:
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[2] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_SS1;   //update state
                    human_code = -1;
                }
                break;
            case STATE_INTERVAL_CFG_SS1:
                if(ir_is_code_number(human_code) && human_code <=5)
                {
                    aux1[1] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_SS2;   //update state
                    human_code = -1;
                }
                break;     
            case STATE_INTERVAL_CFG_SS2:
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[0] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_MM1;   //update state
                    human_code = -1;
                }
                break; 
            case STATE_INTERVAL_CFG_PROGRAM_SS1:
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[1] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_PROGRAM_SS2;   //update state
                    human_code = -1;
                }
                break;   
            case STATE_INTERVAL_CFG_PROGRAM_SS2:
                if(ir_is_code_number(human_code) && human_code <=9)
                {
                    aux1[0] = human_code; //update numberS
                    next = STATE_INTERVAL_CFG_PROGRAM_SS1;   //update state
                    human_code = -1;
                }
                break;                              
            default: ;
                
        }              
    }
};



