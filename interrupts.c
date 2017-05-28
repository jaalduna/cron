/******************************************************************************/
/*Files to Include                                                            */
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

char counter;
char masked_digits[6];


/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0
    
      /* TODO Add High Priority interrupt routine code here. */
    
      /* Determine which flag generated the interrupt */
      if(<Interrupt Flag 1>)
      {
          <Interrupt Flag 1=0>; /* Clear Interrupt Flag 1 */
      }
      else if (<Interrupt Flag 2>)
      {
          <Interrupt Flag 2=0>; /* Clear Interrupt Flag 2 */
      }
      else
      {
          /* Unhandled interrupts */
      }

#endif
      
      if (RBIF == 1)
      {
        //lets disable interrupts
        INTCONbits.GIE = 0;
        char aux = 0;
        //lets check the received code
        int i;
        char code[CODE_NUM_BITS];
        //lets wait 40.5 ms, to skip the common code
        __delay_ms(40.7);
        for(i = 0; i < CODE_NUM_BITS ; i ++)
        {
            code[i] = IR;
            __delay_us(565);
        }
        int final_code = 0;
        for(i = 0; i<16;i++)
        {
            final_code |= code[i]<<i;
        }
        //lets get the button press from final code.
        //lets calculate the next state from the current state and the pressed button.
        
        //lets decode the final_code into a human_read format
        if(final_code !=0xffff) //if we received a valid code
        {
            human_code = ir_get_human_code(final_code);
            next = get_next_state(state,final_code);
        }
        //lets clear the interrupt flag and enable the interrupts.
        RBIF = 0;
        INTCONbits.GIE = 1;
      }
      else if (TMR0IF == 1)
	{
	  /*Lets assign to masked_digits the characters stored in aux1*/
          int i;
          for(i=0; i<NUM_DIGITS;i++)
          {
              masked_digits[i] = aux1[i];
          }

        //lets update time interval
		if(counter < NUM_TIME_STEPS - 1)
		{
			counter = counter + 1;	
		}
		else
		{
			counter = 0;
		}	
          
		char digits[6];
		int i;
		
		for (i=0; i<6;i++)
			digits[6] = i;
        
	//update point according to state and counter
	update_point(state,counter);
	update_buzzer(state,counter);
	
        if(counter == 0 )
        {
            point_enable();
        }
        else if(counter == 5 && state == STATE_TIME)
        {
            point_disable();
        }
        

	if(buzzer_status)
	{
		if(BUZZER == 1)
			BUZZER = 0;
		else
		   BUZZER = 1;
	}
        if(counter >= 5)
        {
            switch(state)
            {
                case STATE_HH1:
                    masked_digits[3] =  'V';
                    break;
                case STATE_HH2:
                    masked_digits[2] =  'V';
                    break;
                case STATE_MM1:
                    masked_digits[1] =  'V';
                    break;
                case STATE_MM2:
                    masked_digits[0] =  'V';
                    break;
            }
        }
        
		//update_display(masked_digits, &counter);
		put_nums(masked_digits);
		TMR0IF = 0;
	}

     else if (TMR1IF == 1)
	{
		//lets update a counter and then write it to aux1 display varible			
		if(timer1_counter <9)	
			timer1_counter += 1;		
		else
		{
			timer1_counter = 0;
			update_timer1_counter_10(state,&timer1_counter_10);
		}
	
		TMR1IF = 0;	
	}
}
