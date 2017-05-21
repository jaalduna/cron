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
        __delay_ms(40.6);
        for(i = 0; i < CODE_NUM_BITS ; i ++)
        {
            code[i] = IR;
            __delay_us(590);
        }
        int final_code = 0;
        for(i = 0; i<16;i++)
        {
            final_code |= code[i]<<i;
        }
        //lets get the button press from final code.
        //lets calculate the next state from the current state and the pressed button.
        
        //lets decode the final_code into a human_read format
        if(final_code !=0xff) //if we received a valid code
        {
            human_code = ir_get_human_code(final_code);
            next = get_next_state(state,final_code);
        }
        //lets clear the interrupt flag and enable the interrupts.
        RBIF = 0;
        INTCONbits.GIE = 1;
      }
   

}

/*
/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{

      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

#if 0

      /* TODO Add Low Priority interrupt routine code here. */

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

}
*/
