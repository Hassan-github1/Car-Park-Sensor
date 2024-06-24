#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 4000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#define Trigger1 RC2 // Set Trigger1 to RC2
#define Echo1 RC3    // Set Echo1 to RC3
#define Trigger2 RC4 // Set Trigger2 to RC4
#define Echo2 RC5    // Set Echo2 to RC5
#define Trigger3 RC6 // Set Trigger3 to RC6
#define Echo3 RC7    // Set Echo3 to RC7
#define Trigger4 RB5 // Set Trigger4 to RB5
#define Echo4 RB4    // Set Echo4 to RB4

// Function Prototypes
void Lcd_SetBit(char data_bit);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Start(void);
void Lcd_Print_Char(char data);
void Lcd_Print_String(char *a);
char calc_dist1(void);
char calc_dist2(void);
char calc_dist3(void);
char calc_dist4(void);

void main(void) {
    unsigned char dist1 = 0, dist2 = 0, dist3 = 0, dist4 = 0;

    // Set PORTD for LCD
    TRISD = 0x00;
    // Set PORTB To Be Output Port (Except RB4 which will be input)
    TRISB = 0x10; // Set RB4 as input
    // Set RC2, RC4, RC6, RB5 To Be Output Pins  ( Trigger )
    TRISC2 = 0;
    RC2 = 0;
    TRISC4 = 0;
    RC4 = 0;
    TRISC6 = 0;
    RC6 = 0;
    TRISB5 = 0;
    RB5 = 0;
    // Set RC3, RC5, RC7, RB4 To Be Input Pins    ( Echo )
    TRISC3 = 1;
    TRISC5 = 1;
    TRISC7 = 1;
    TRISB4 = 1;
    // Set RD0 to be Output Pin (Buzzer)
    TRISD0 = 0;
    RD0 = 0;

    // Configure Timer Module To Operate in Timer Mode
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    TMR1CS = 0;

    Lcd_Start();
    Lcd_Set_Cursor(1, 1);
    Lcd_Print_String("Ultrasonic sensor");
    Lcd_Set_Cursor(2, 1);
    Lcd_Print_String("with PIC16F877A");
    __delay_ms(2000);
    Lcd_Clear();

    while (1) {
        dist1 = calc_dist1();
        dist2 = calc_dist2();
        dist3 = calc_dist3();
        dist4 = calc_dist4();

        // Update LCD
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("1:");
        Lcd_Print_Char((dist1 / 100) + '0');
        Lcd_Print_Char(((dist1 / 10) % 10) + '0');
        Lcd_Print_Char((dist1 % 10) + '0');
        Lcd_Print_String("cm ");

        Lcd_Set_Cursor(1, 9);
        Lcd_Print_String("2:");
        Lcd_Print_Char((dist2 / 100) + '0');
        Lcd_Print_Char(((dist2 / 10) % 10) + '0');
        Lcd_Print_Char((dist2 % 10) + '0');
        Lcd_Print_String("cm");

        Lcd_Set_Cursor(2, 1);
        Lcd_Print_String("3:");
        Lcd_Print_Char((dist3 / 100) + '0');
        Lcd_Print_Char(((dist3 / 10) % 10) + '0');
        Lcd_Print_Char((dist3 % 10) + '0');
        Lcd_Print_String("cm ");

        Lcd_Set_Cursor(2, 9);
        Lcd_Print_String("4:");
        Lcd_Print_Char((dist4 / 100) + '0');
        Lcd_Print_Char(((dist4 / 10) % 10) + '0');
        Lcd_Print_Char((dist4 % 10) + '0');
        Lcd_Print_String("cm");

        // Update progress bar and buzzer for dist1
        if (dist1 > 0 && dist1 <= 20) {
            PORTB = 0x01;
            RD0 = 1;
            __delay_ms(200);
            RD0 = 0;
            __delay_ms(200);
        } else if (dist1 > 20 && dist1 <= 40) {
            PORTB = 0x03;
            RD0 = 1;
            __delay_ms(100);
            RD0 = 0;
            __delay_ms(100);
        } else if (dist1 > 40 && dist1 <= 60) {
            PORTB = 0x07;
            RD0 = 1;
            __delay_ms(50);
            RD0 = 0;
            __delay_ms(50);
        } else if (dist1 > 60 && dist1 <= 80) {
            PORTB = 0x0F;
            RD0 = 1;
            __delay_ms(25);
            RD0 = 0;
            __delay_ms(25);
        } else {
            PORTB = 0x00;
            RD0 = 0;
            __delay_ms(200);
        }
    }
}

// LCD Functions Implementation
void Lcd_SetBit(char data_bit) {
    if (data_bit & 1) D4 = 1; else D4 = 0;
    if (data_bit & 2) D5 = 1; else D5 = 0;
    if (data_bit & 4) D6 = 1; else D6 = 0;
    if (data_bit & 8) D7 = 1; else D7 = 0;
}

void Lcd_Cmd(char a) {
    RS = 0;
    Lcd_SetBit(a);
    EN = 1;
    __delay_ms(4);
    EN = 0;
}

void Lcd_Clear(void) {
    Lcd_Cmd(0x00); // Clear the LCD
    Lcd_Cmd(0x01); // Move the cursor to the first position
}

void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1; // 80H is used to move the cursor
        z = temp >> 4;       // Lower 8-bits
        y = temp & 0x0F;     // Upper 8-bits
        Lcd_Cmd(z);          // Set Row
        Lcd_Cmd(y);          // Set Column
    } else if (a == 2) {
        temp = 0xC0 + b - 1;
        z = temp >> 4; // Lower 8-bits
        y = temp & 0x0F; // Upper 8-bits
        Lcd_Cmd(z);      // Set Row
        Lcd_Cmd(y);      // Set Column
    }
}

void Lcd_Start(void) {
    Lcd_SetBit(0x00);
    for (int i = 1065244; i <= 0; i--) NOP();
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(11);
    Lcd_Cmd(0x03);
    Lcd_Cmd(0x02); // 02H is used for Return home -> Clears the RAM and initializes the LCD
    Lcd_Cmd(0x02); // 02H is used for Return home -> Clears the RAM and initializes the LCD
    Lcd_Cmd(0x08); // Select Row 1
    Lcd_Cmd(0x00); // Clear Row 1 Display
    Lcd_Cmd(0x0C); // Select Row 2
    Lcd_Cmd(0x00); // Clear Row 2 Display
    Lcd_Cmd(0x06);
}

void Lcd_Print_Char(char data) {
    char Lower_Nibble, Upper_Nibble;
    Lower_Nibble = data & 0x0F;
    Upper_Nibble = data & 0xF0;
    RS = 1; // => RS = 1
    Lcd_SetBit(Upper_Nibble >> 4); // Send upper half by shifting by 4
    EN = 1;
    __delay_ms(1);
    EN = 0;
    Lcd_SetBit(Lower_Nibble); // Send Lower half
    EN = 1;
    __delay_ms(1);
    EN = 0;
}

void Lcd_Print_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++) Lcd_Print_Char(a[i]); // Split the string using pointers and call the Char function
}

// Distance Calculation Function Implementation
char calc_dist1(void) {
    unsigned int time = 0;
    unsigned char distance = 0;
    TMR1H = 0;  // Reset Timer1 High byte
    TMR1L = 0;  // Reset Timer1 Low byte
    RC2 = 1;    // Send Trigger pulse
    __delay_us(10); // 10us delay
    RC2 = 0;    // End Trigger pulse

    // Wait for Echo to go high
    while (!RC3);
    TMR1ON = 1; // Start Timer1

    // Wait for Echo to go low
    while (RC3);
    TMR1ON = 0; // Stop Timer1

    // Calculate the time duration
    time = (TMR1H << 8) | TMR1L;
    distance = time / 58.82; // Calculate distance in cm

    // Ensure valid distance measurement
    if (distance >= 2 && distance < 150)
        return distance;
    else
        return 0;
}

char calc_dist2(void) {
    unsigned int time = 0;
    unsigned char distance = 0;
    TMR1H = 0;  // Reset Timer1 High byte
    TMR1L = 0;  // Reset Timer1 Low byte
    RC4 = 1;    // Send Trigger pulse
    __delay_us(10); // 10us delay
    RC4 = 0;    // End Trigger pulse

    // Wait for Echo to go high
    while (!RC5);
    TMR1ON = 1; // Start Timer1

    // Wait for Echo to go low
    while (RC5);
    TMR1ON = 0; // Stop Timer1

    // Calculate the time duration
    time = (TMR1H << 8) | TMR1L;
    distance = time / 58.82; // Calculate distance in cm

    // Ensure valid distance measurement
    if (distance >= 2 && distance < 150)
        return distance;
    else
        return 0;
}

char calc_dist3(void) {
    unsigned int time = 0;
    unsigned char distance = 0;
    TMR1H = 0;  // Reset Timer1 High byte
    TMR1L = 0;  // Reset Timer1 Low byte
    RC6 = 1;    // Send Trigger pulse
    __delay_us(10); // 10us delay
    RC6 = 0;    // End Trigger pulse

    // Wait for Echo to go high
    while (!RC7);
    TMR1ON = 1; // Start Timer1

    // Wait for Echo to go low
    while (RC7);
    TMR1ON = 0; // Stop Timer1

    // Calculate the time duration
    time = (TMR1H << 8) | TMR1L;
    distance = time / 58.82; // Calculate distance in cm

    // Ensure valid distance measurement
    if (distance >= 2 && distance < 150)
        return distance;
    else
        return 0;
}

char calc_dist4(void) {
    unsigned int time = 0;
    unsigned char distance = 0;
    TMR1H = 0;  // Reset Timer1 High byte
    TMR1L = 0;  // Reset Timer1 Low byte
    RB5 = 1;    // Send Trigger pulse
    __delay_us(10); // 10us delay
    RB5 = 0;    // End Trigger pulse

    // Wait for Echo to go high
    while (!RB4);
    TMR1ON = 1; // Start Timer1

    // Wait for Echo to go low
    while (RB4);
    TMR1ON = 0; // Stop Timer1

    // Calculate the time duration
    time = (TMR1H << 8) | TMR1L;
    distance = time / 58.82; // Calculate distance in cm

    // Ensure valid distance measurement
    if (distance >= 2 && distance < 150)
        return distance;
    else
        return 0;
}