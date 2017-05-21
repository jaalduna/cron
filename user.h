    /******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/
/*Pins for display (7segments display controller)*/
#define DS RB3 //serial data input 74HC595
#define SHCP RB4 // shift register clock input, (this pin is also connected to STCP on hardware) 74HC595
#define NUM_DIGITS 6 //number of digits on the display.
#define POINT RB1 //chrono point

#define BUZZER RB0 //buzzer

#define SCLK RA2  //output
#define IO   RA3  //input/output
#define RST  RA5  //output

#define IR   RB5 //IR sensor

/* TODO Application specific user parameters used in user.c may go here */

#define _XTAL_FREQ      20000000L

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */
//Defs for 7 segments numbers
#define SEVEN_SEG_ONE     0b00110000
#define SEVEN_SEG_TWO     0b01101101
#define SEVEN_SEG_THREE   0b01111001
#define SEVEN_SEG_FOUR    0b00110011
#define SEVEN_SEG_FIVE    0b01011011
#define SEVEN_SEG_SIX     0b01011111
#define SEVEN_SEG_SEVEN   0b01110000
#define SEVEN_SEG_EIGHT   0b01111111
#define SEVEN_SEG_NINE    0b01110011
#define SEVEN_SEG_ZERO    0b01111110
#define SEVEN_SEG_VOID    0b00000000

//defines for 7 segments letters
//UP letters
#define SEVEN_SEG_U      0b00111110
#define SEVEN_SEG_P      0b01100111
//DN letters
#define SEVEN_SEG_D      0b00111101
#define SEVEN_SEG_N      0b00010101
//H for time
#define SEVEN_SEG_H      0b00110111

/*Defines for IR control buttons*/

#define IR_UP           0xABB9 //code 10
#define IR_OK           0xEAA9 //code 11
#define IR_DOWN         0xEBAD //code 12
#define IR_LEFT         0xAAE9 //code 13
#define IR_RIGHT        0xAAEE //revised code 14
#define IR_ONE          0x6BB9 //code 1
#define IR_TWO          0x6EAD //code 2
#define IR_THREE        0x3BAD //code 3
#define IR_FOUR         0xAEE9 //code 4
#define IR_FIVE         0xBBA9 //code 5
#define IR_SIX          0xBBB9 //code 6
#define IR_SEVEN        0xABA9 //code 7
#define IR_EIGHT        0x6EE9 //code 8
#define IR_NINE         0x6EB9 //code 9
#define IR_ZERO         0xBAB9 //code 0
#define IR_ASTERISC     0xAABA //revised code 15
#define IR_GATO         0xAEBA //revised code 16

#define IR_EDIT         IR_ASTERISC
#define IR_ESC          IR_GATO



#define WRITE_RTC_0 0x80
#define READ_RTC_0  0x81
#define WRITE_RTC_1 0x82
#define READ_RTC_1  0x83
#define WRITE_RTC_2 0x84
#define READ_RTC_2  0x85

#define CODE_NUM_BITS 47

//Defines for STATES
#define STATE_TIME       0
#define STATE_HH1        1
#define STATE_HH2        2
#define STATE_MM1        3
#define STATE_MM2        4




int state, next;
char human_code; //code contains the last received code

void put_num(char num); //put a single number on the display
void put_nums(char nums[]); //update all the numbers from the display
void put_nums_individual(char num5, char num4, char num3, char num2, char num1, char num0); //update all the numbers from the display
char get_num(char num);     //gets 7segment code for number between 0 and 9
void point_enable(void);       //enable display point
void point_disable(void);     //disable display point
void point_toggle(void);      //toggle display point

void buzzer_enable(void);
void buzzer_disable(void);
void buzzer_toggle(void);

void ce_enable(void);
void ce_disable(void);
char byte_read(char address);
void byte_write(char address, char value);
char get_seconds_reg(char seconds);
char get_hour_reg(char hour);
void set_time(char seconds, char minutes, char hour);
void get_time(char data[]);

void send_command(char command);
void io_as_input(void);
void io_as_output(void);

/*is_code_number return 1 if human_code is a number or 0 if not*/
char ir_is_code_number(char human_code); //this function check code is a number (0,1,2,3,4,5,6,7,8,9)
/*ir_get_human_code returns a human readable code for each pressed button. if a non registered code is press, then -1 is returned.*/
char ir_get_human_code(int code); 


int get_next_state(int state,int code); 
//only update code when there is a valid code
//then evaluate the next state outside the interrupt routine
//generate also a global variable for the recieved_code and take

void InitApp(void);         /* I/O and Peripheral Initialization */
