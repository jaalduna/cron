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

#define IR_UP           0xABBa //revised code 10
#define IR_OK           0xEAAA //revised code 11
#define IR_DOWN         0xEBAE //revised code 12
#define IR_LEFT         0xAAEA //revised code 13
#define IR_RIGHT        0xAAEE //revised code 14
#define IR_ONE          0xEBBA //revised code 1
#define IR_TWO          0xEEAE //revised code 2
#define IR_THREE        0xBBAE //revised code 3
#define IR_FOUR         0xAEEA //revised code 4
#define IR_FIVE         0xBBAA //revised code 5
#define IR_SIX          0xBBBA //revised code 6
#define IR_SEVEN        0xABAA //revised code 7
#define IR_EIGHT        0xEEEA //revised code 8
#define IR_NINE         0xEEBA //revised code 9
#define IR_ZERO         0xBABA //revised code 0
#define IR_ASTERISC     0xAABA //revised code 15
#define IR_GATO         0xAEBA //revised code 16

#define IR_EDIT         IR_ASTERISC
#define IR_ESC          IR_GATO

/*Human read codes*/
#define HR_IR_UP   10
#define HR_IR_OK	11
#define HR_IR_DOWN	12
#define HR_IR_LEFT 	13
#define HR_IR_RIGHT	14
#define HR_IR_ONE	1
#define HR_IR_TWO	2
#define HR_IR_THREE	3
#define HR_IR_FOUR	4
#define HR_IR_FIVE	5
#define HR_IR_SIX	6
#define HR_IR_SEVEN	7
#define HR_IR_EIGHT	8
#define HR_IR_NINE	9
#define HR_IR_ZERO	0
#define HR_IR_ASTERICS	15
#define HR_IR_GATO	16


/*Defines for DS1302*/
#define FORMAT_24H	1 
#define FORMAT_12H	0

#define WRITE_RTC_0 0x80
#define READ_RTC_0  0x81
#define WRITE_RTC_1 0x82
#define READ_RTC_1  0x83
#define WRITE_RTC_2 0x84
#define READ_RTC_2  0x85

#define CODE_NUM_BITS 47

/* Defines for STATES */
#define STATE_TIME       0
#define STATE_HH1        1
#define STATE_HH2        2
#define STATE_MM1        3
#define STATE_MM2        4

#define STATE_UP         5

#define STATE_UP_CFG_MM1	7
#define STATE_UP_CFG_MM2	8
#define STATE_UP_CFG_SS1	9
#define STATE_UP_CFG_SS2	10
#define STATE_UP_COUNT_DOWN	11
#define STATE_UP_COUNTING	12
#define STATE_UP_STOP		13

#define STATE_DOWN       	6
#define STATE_DOWN_CFG_MM1	14
#define STATE_DOWN_CFG_MM2	15
#define STATE_DOWN_CFG_SS1  16
#define STATE_DOWN_CFG_SS2  17
#define STATE_DOWN_COUNT_DOWN 18
#define STATE_DOWN_COUNTING 19
#define STATE_DOWN_STOP     20

#define STATE_SPEED				21
#define STATE_SPEED_RUN			22
#define STATE_SPEED_PAUSE			23

#define STATE_INTERVAL 24
#define STATE_INTERVAL_COUNT_DOWN 25
#define STATE_INTERVAL_FORCE 26
#define STATE_INTERVAL_COLD 27
#define STATE_INTERVAL_STOP 28
#define STATE_INTERVAL_CFG_MM1 29
#define STATE_INTERVAL_CFG_MM2 30
#define STATE_INTERVAL_CFG_SS1 31
#define STATE_INTERVAL_CFG_SS2 32

		


//Defines for TIME division for display update
#define NUM_TIME_STEPS 10 //divider for 1 second
#define TIME_STEP 1000/NUM_TIME_STEP //time step in miliseconds


/*Defines for UP*/
#define UP_INITIAL_COUNTDOWN	10
#define DOWN_INITIAL_COUNTDOWN	10

int state, next;
char human_code; //code contains the last received code
char aux1[NUM_DIGITS]; //variable array to hold display values
char buzzer_status; //buzzer_status: 1 buzzer enabled, 0 buzzer disabled

char timer1_counter; //counter for timer1
char timer1_counter_10; //counter for timer1 x10
char timer1_counter_min; //counter for minutes

char timer1_up_limit_sec, timer1_up_limit_min; // upper limit for up counter
char timer1_down_init_min, timer1_down_init_sec; //init values for DOWN counter

char timer1_speed_cents; //centesimas de segundo


#define NUM_INTERVALS 20;

char current_type; //'f': force, 'c': cold
struct Tiempo
{
	char seconds;
	char minutes;
};

struct Intervals
{
	Tiempo force[NUM_INTERVALS]; //init values for force part of intervals
	Tiempo cold[NUM_INTERVALS];  //init values for cold part of intervals
};

Intervals intervals; //
char max_program, current_program; //number of programs to run. A program is a set of force + cold.



/*Display related functions*/
void put_num(char num); //put a single number on the display
void put_nums(char nums[]); //update all the numbers from the display
void put_nums_individual(char num5, char num4, char num3, char num2, char num1, char num0); //update all the numbers from the display
char get_num(char num);     //gets 7segment code for number between 0 and 9
void update_point(int state, char counter);
void update_buzzer(int state, char counter);

void point_enable(void);       //enable display point
void point_disable(void);     //disable display point
void point_toggle(void);      //toggle display point

void buzzer_enable(void); 
void buzzer_disable(void);
void buzzer_toggle(void);



/*Time related functions*/
char get_seconds_reg(char seconds);
char get_hour_reg(char hour);
void get_time(char data[], unsigned char format);
void set_time(char seconds, char minutes, char hour);

/*RTC DS1302 related commands*/
void send_command(char command);
void io_as_input(void);
void io_as_output(void);
void ce_enable(void);
void ce_disable(void);
char byte_read(char address);
void byte_write(char address, char value);

char ir_is_code_number(char human_code); 
char ir_get_human_code(int code); 

/*General control functions*/
int get_next_state(int state,int code); 

void InitApp(void);         /* I/O and Peripheral Initialization */

/*Functions related with up counter*/
void get_timer1_counter( char data[]);
void timer1_enable(void);
void timer1_disable(void);
void update_timer1_counter_10(char state, char *counter);

/*Functions related with down counter*/
void get_timer1_counter_down( char data[]);
/*Functions related with cron*/
void get_timer1_counter_speed(char data[]);
/*Functions related with interval*/
void get_timer1_counter_interval(char data[])
/*Function related with timer 1*/

