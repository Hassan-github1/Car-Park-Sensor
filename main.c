#include <xc.h> // Include header file for PIC microcontroller
#include "config.h" // Include configuration header file
#define _XTAL_FREQ 4000000 // Define the crystal oscillator frequency

// Define LCD control pins
#define RS RD2 // Register Select pin
#define EN RD3 // Enable pin
#define D4 RD4 // Data pin 4
#define D5 RD5 // Data pin 5
#define D6 RD6 // Data pin 6
#define D7 RD7 // Data pin 7

// Define ultrasonic sensor pins
#define Trigger1 RC2 // Trigger1 pin connected to RC2
#define Echo1 RC3    // Echo1 pin connected to RC3
#define Trigger2 RC4 // Trigger2 pin connected to RC4
#define Echo2 RC5    // Echo2 pin connected to RC5
#define Trigger3 RC6 // Trigger3 pin connected to RC6
#define Echo3 RC7    // Echo3 pin connected to RC7
#define Trigger4 RB5 // Trigger4 pin connected to RB5
#define Echo4 RB4    // Echo4 pin connected to RB4

// Function Prototypes
void Lcd_SetBit(char data_bit); // Set data bits for LCD
void Lcd_Cmd(char a); // Send command to LCD
void Lcd_Clear(void); // Clear LCD display
void Lcd_Set_Cursor(char a, char b); // Set cursor position on LCD
void Lcd_Start(void); // Initialize the LCD
void Lcd_Print_Char(char data); // Print character on LCD
void Lcd_Print_String(char *a); // Print string on LCD
char calc_dist1(void); // Calculate distance for sensor 1
char calc_dist2(void); // Calculate distance for sensor 2
char calc_dist3(void); // Calculate distance for sensor 3
char calc_dist4(void); // Calculate distance for sensor 4

void main(void) {
    unsigned char dist1 = 0, dist2 = 0, dist3 = 0, dist4 = 0; // Distance variables for sensors

    TRISD = 0x00; // Set PORTD as output for LCD
    TRISB = 0x10; // Set PORTB as output, except RB4 as input for Echo4
    TRISC2 = 0; // Set RC2 as output for Trigger1
    RC2 = 0; // Initialize RC2 to 0
    TRISC4 = 0; // Set RC4 as output for Trigger2
    RC4 = 0; // Initialize RC4 to 0
    TRISC6 = 0; // Set RC6 as output for Trigger3
    RC6 = 0; // Initialize RC6 to 0
    TRISB5 = 0; // Set RB5 as output for Trigger4
    RB5 = 0; // Initialize RB5 to 0
    TRISC3 = 1; // Set RC3 as input for Echo1
    TRISC5 = 1; // Set RC5 as input for Echo2
    TRISC7 = 1; // Set RC7 as input for Echo3
    TRISB4 = 1; // Set RB4 as input for Echo4
    TRISD0 = 0; // Set RD0 as output for Buzzer
    RD0 = 0; // Initialize RD0 to 0

    // Configure Timer1
    T1CKPS0 = 0; // Set Timer1 prescaler to 1
    T1CKPS1 = 0; // Set Timer1 prescaler to 1
    TMR1CS = 0; // Select internal clock for Timer1

    Lcd_Start(); // Initialize the LCD
    Lcd_Set_Cursor(1, 1); // Set LCD cursor to row 1, column 1
    Lcd_Print_String("Ultrasonic sensor"); // Print string on LCD
    Lcd_Set_Cursor(2, 1); // Set LCD cursor to row 2, column 1
    Lcd_Print_String("with PIC16F877A"); // Print string on LCD
    __delay_ms(2000); // Delay for 2 seconds
    Lcd_Clear(); // Clear the LCD

    while (1) {
        dist1 = calc_dist1(); // Calculate distance from sensor 1
        dist2 = calc_dist2(); // Calculate distance from sensor 2
        dist3 = calc_dist3(); // Calculate distance from sensor 3
        dist4 = calc_dist4(); // Calculate distance from sensor 4

        Lcd_Clear(); // Clear the LCD
        Lcd_Set_Cursor(1, 1); // Set LCD cursor to row 1, column 1
        Lcd_Print_String("1:"); // Print string on LCD
        Lcd_Print_Char((dist1 / 100) + '0'); // Print hundreds place of distance
        Lcd_Print_Char(((dist1 / 10) % 10) + '0'); // Print tens place of distance
        Lcd_Print_Char((dist1 % 10) + '0'); // Print units place of distance
        Lcd_Print_String("cm "); // Print "cm" on LCD

        Lcd_Set_Cursor(1, 9); // Set LCD cursor to row 1, column 9
        Lcd_Print_String("2:"); // Print string on LCD
        Lcd_Print_Char((dist2 / 100) + '0'); // Print hundreds place of distance
        Lcd_Print_Char(((dist2 / 10) % 10) + '0'); // Print tens place of distance
        Lcd_Print_Char((dist2 % 10) + '0'); // Print units place of distance
        Lcd_Print_String("cm"); // Print "cm" on LCD

        Lcd_Set_Cursor(2, 1); // Set LCD cursor to row 2, column 1
        Lcd_Print_String("3:"); // Print string on LCD
        Lcd_Print_Char((dist3 / 100) + '0'); // Print hundreds place of distance
        Lcd_Print_Char(((dist3 / 10) % 10) + '0'); // Print tens place of distance
        Lcd_Print_Char((dist3 % 10) + '0'); // Print units place of distance
        Lcd_Print_String("cm "); // Print "cm" on LCD

        Lcd_Set_Cursor(2, 9); // Set LCD cursor to row 2, column 9
        Lcd_Print_String("4:"); // Print string on LCD
        Lcd_Print_Char((dist4 / 100) + '0'); // Print hundreds place of distance
        Lcd_Print_Char(((dist4 / 10) % 10) + '0'); // Print tens place of distance
        Lcd_Print_Char((dist4 % 10) + '0'); // Print units place of distance
        Lcd_Print_String("cm"); // Print "cm" on LCD

        // Update progress bar and buzzer for dist1
        if (dist1 > 0 && dist1 <= 20) {
            PORTB = 0x01; // Turn on first LED
            RD0 = 1; // Turn on buzzer
            __delay_ms(200); // Delay for 200 ms
            RD0 = 0; // Turn off buzzer
            __delay_ms(200); // Delay for 200 ms
        } else if (dist1 > 20 && dist1 <= 40) {
            PORTB = 0x03; // Turn on first two LEDs
            RD0 = 1; // Turn on buzzer
            __delay_ms(100); // Delay for 100 ms
            RD0 = 0; // Turn off buzzer
            __delay_ms(100); // Delay for 100 ms
        } else if (dist1 > 40 && dist1 <= 60) {
            PORTB = 0x07; // Turn on first three LEDs
            RD0 = 1; // Turn on buzzer
            __delay_ms(50); // Delay for 50 ms
            RD0 = 0; // Turn off buzzer
            __delay_ms(50); // Delay for 50 ms
        } else if (dist1 > 60 && dist1 <= 80) {
            PORTB = 0x0F; // Turn on all four LEDs
            RD0 = 1; // Turn on buzzer
            __delay_ms(25); // Delay for 25 ms
            RD0 = 0; // Turn off buzzer
            __delay_ms(25); // Delay for 25 ms
        } else {
            PORTB = 0x00; // Turn off all LEDs
            RD0 = 0; // Turn off buzzer
            __delay_ms(200); // Delay for 200 ms
        }
    }
}

// Set data bits for LCD
void Lcd_SetBit(char data_bit) {
    if (data_bit & 1) D4 = 1; else D4 = 0; // Set or clear D4
    if (data_bit & 2) D5 = 1; else D5 = 0; // Set or clear D5
    if (data_bit & 4) D6 = 1; else D6 = 0; // Set or clear D6
    if (data_bit & 8) D7 = 1; else D7 = 0; // Set or clear D7
}

// Send command to LCD
void Lcd_Cmd(char a) {
    RS = 0; // Set RS to command mode
    Lcd_SetBit(a); // Set data bits
    EN = 1; // Enable the LCD
    __delay_ms(4); // Delay for 4 ms
    EN = 0; // Disable the LCD
}

// Clear the LCD
void Lcd_Clear(void) {
    Lcd_Cmd(0x00); // Clear command
    Lcd_Cmd(0x01); // Move cursor to home position
}

// Set cursor position on LCD
void Lcd_Set_Cursor(char a, char b) {
    char temp, z, y;
    if (a == 1) {
        temp = 0x80 + b - 1; // Calculate position for row 1
        z = temp >> 4;       // Get upper nibble
        y = temp & 0x0F;     // Get lower nibble
        Lcd_Cmd(z);          // Send upper nibble
        Lcd_Cmd(y);          // Send lower nibble
    } else if (a == 2) {
        temp = 0xC0 + b - 1; // Calculate position for row 2
        z = temp >> 4;       // Get upper nibble
        y = temp & 0x0F;     // Get lower nibble
        Lcd_Cmd(z);          // Send upper nibble
        Lcd_Cmd(y);          // Send lower nibble
    }
}

// Initialize the LCD
void Lcd_Start(void) {
    Lcd_SetBit(0x00); // Set data bits to 0
    for (int i = 1065244; i <= 0; i--) NOP(); // Delay loop
    Lcd_Cmd(0x03); // Send initialization command
    __delay_ms(5); // Delay for 5 ms
    Lcd_Cmd(0x03); // Send initialization command
    __delay_ms(11); // Delay for 11 ms
    Lcd_Cmd(0x03); // Send initialization command
    Lcd_Cmd(0x02); // Return home command
    Lcd_Cmd(0x02); // Return home command
    Lcd_Cmd(0x08); // Select row 1
    Lcd_Cmd(0x00); // Clear row 1 display
    Lcd_Cmd(0x0C); // Select row 2
    Lcd_Cmd(0x00); // Clear row 2 display
    Lcd_Cmd(0x06); // Entry mode set
}

// Print character on LCD
void Lcd_Print_Char(char data) {
    char Lower_Nibble, Upper_Nibble;
    Lower_Nibble = data & 0x0F; // Get lower nibble
    Upper_Nibble = data & 0xF0; // Get upper nibble
    RS = 1; // Set RS to data mode
    Lcd_SetBit(Upper_Nibble >> 4); // Send upper nibble
    EN = 1; // Enable the LCD
    __delay_ms(1); // Delay for 1 ms
    EN = 0; // Disable the LCD
    Lcd_SetBit(Lower_Nibble); // Send lower nibble
    EN = 1; // Enable the LCD
    __delay_ms(1); // Delay for 1 ms
    EN = 0; // Disable the LCD
}

// Print string on LCD
void Lcd_Print_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++) Lcd_Print_Char(a[i]); // Print each character
}

// Calculate distance for sensor 1
char calc_dist1(void) {
    unsigned int time = 0; // Time variable
    unsigned char distance = 0; // Distance variable
    TMR1H = 0; // Reset Timer1 high byte
    TMR1L = 0; // Reset Timer1 low byte
    RC2 = 1; // Send trigger pulse
    __delay_us(10); // Delay for 10 microseconds
    RC2 = 0; // End trigger pulse

    while (!RC3); // Wait for echo to go high
    TMR1ON = 1; // Start Timer1

    while (RC3); // Wait for echo to go low
    TMR1ON = 0; // Stop Timer1

    time = (TMR1H << 8) | TMR1L; // Calculate time duration
    distance = time / 58.82; // Calculate distance in cm

    if (distance >= 2 && distance < 150) // Ensure valid distance
        return distance;
    else
        return 0;
}

// Calculate distance for sensor 2
char calc_dist2(void) {
    unsigned int time = 0; // Time variable
    unsigned char distance = 0; // Distance variable
    TMR1H = 0; // Reset Timer1 high byte
    TMR1L = 0; // Reset Timer1 low byte
    RC4 = 1; // Send trigger pulse
    __delay_us(10); // Delay for 10 microseconds
    RC4 = 0; // End trigger pulse

    while (!RC5); // Wait for echo to go high
    TMR1ON = 1; // Start Timer1

    while (RC5); // Wait for echo to go low
    TMR1ON = 0; // Stop Timer1

    time = (TMR1H << 8) | TMR1L; // Calculate time duration
    distance = time / 58.82; // Calculate distance in cm

    if (distance >= 2 && distance < 150) // Ensure valid distance
        return distance;
    else
        return 0;
}

// Calculate distance for sensor 3
char calc_dist3(void) {
    unsigned int time = 0; // Time variable
    unsigned char distance = 0; // Distance variable
    TMR1H = 0; // Reset Timer1 high byte
    TMR1L = 0; // Reset Timer1 low byte
    RC6 = 1; // Send trigger pulse
    __delay_us(10); // Delay for 10 microseconds
    RC6 = 0; // End trigger pulse

    while (!RC7); // Wait for echo to go high
    TMR1ON = 1; // Start Timer1

    while (RC7); // Wait for echo to go low
    TMR1ON = 0; // Stop Timer1

    time = (TMR1H << 8) | TMR1L; // Calculate time duration
    distance = time / 58.82; // Calculate distance in cm

    if (distance >= 2 && distance < 150) // Ensure valid distance
        return distance;
    else
        return 0;
}

// Calculate distance for sensor 4
char calc_dist4(void) {
    unsigned int time = 0; // Time variable
    unsigned char distance = 0; // Distance variable
    TMR1H = 0; // Reset Timer1 high byte
    TMR1L = 0; // Reset Timer1 low byte
    RB5 = 1; // Send trigger pulse
    __delay_us(10); // Delay for 10 microseconds
    RB5 = 0; // End trigger pulse

    while (!RB4); // Wait for echo to go high
    TMR1ON = 1; // Start Timer1

    while (RB4); // Wait for echo to go low
    TMR1ON = 0; // Stop Timer1

    time = (TMR1H << 8) | TMR1L; // Calculate time duration
    distance = time / 58.82; // Calculate distance in cm

    if (distance >= 2 && distance < 150) // Ensure valid distance
        return distance;
    else
        return 0;
}
