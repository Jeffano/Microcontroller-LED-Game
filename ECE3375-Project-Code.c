/* This files provides address values that exist in the system */
#include <stdint.h>

#include <stdio.h>
#include <stdbool.h>

#define BOARD                 "DE10-Standard"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_PIXEL_BUF_BASE   0xC8000000
#define FPGA_PIXEL_BUF_END    0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LED_BASE	      0xFF200000
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define TIMER_2_BASE          0xFF202020
#define AV_CONFIG_BASE        0xFF203000
#define RGB_RESAMPLER_BASE    0xFF203010
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define EDGE_DETECT_CTRL_BASE 0xFF203070
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO0_BASE        0xFF708000
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_GPIO2_BASE        0xFF70A000
#define I2C0_BASE             0xFFC04000
#define I2C1_BASE             0xFFC05000
#define I2C2_BASE             0xFFC06000
#define I2C3_BASE             0xFFC07000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define HPS_RSTMGR	      0xFFD05000
#define HPS_RSTMGR_PREMODRST  0xFFD05014
#define FPGA_BRIDGE           0xFFD0501C

#define PIN_MUX		      0xFFD08400
#define CLK_MGR		      0xFFD04000

#define SPIM0_BASE	      0xFFF00000
#define SPIM0_SR	      0xFFF00028
#define SPIM0_DR	      0xFFF00060
/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

#define KEY0_BIT 0
// #define KEY1_BIT 1

int32_t readButtons() {
    return *(volatile uint32_t *)KEY_BASE;
}

void DisplayHex(int value) {
    const uint8_t HEX_VALUES_0_9[] = {
        0b00111111,  // 0
        0b00000110,  // 1
        0b01011011,  // 2
        0b01001111,  // 3
        0b01100110,  // 4
        0b01101101,  // 5
        0b01111101,  // 6
        0b00000111,  // 7
        0b01111111,  // 8
        0b01101111   // 9
    };

    // Limit the value to the range 0 to 9
    value = value % 11;

    // Get the corresponding hex value based on the switch range
    uint8_t hexValue;
    if (value < 10) {
        hexValue = HEX_VALUES_0_9[value];

        // Display on HEX3_HEX0
        *(volatile uint32_t*)(HEX3_HEX0_BASE) = hexValue;  // Use uint32_t for word-sized store
    } 
    else {
        hexValue = HEX_VALUES_0_9[0];
        uint8_t hexValue2 = HEX_VALUES_0_9[1];

        // Display on HEX3_HEX0
        *(volatile uint32_t*)(HEX3_HEX0_BASE) = hexValue;  // Use uint32_t for word-sized store
        *(volatile uint32_t*)(HEX5_HEX4_BASE) = hexValue2;  // Use uint32_t for word-sized store
    }
}

// Define LED address
volatile unsigned int *LED_ADDR = (volatile unsigned int *)0xFF200000; // Address of LEDs

// Manual delay function
int delay(unsigned int milliseconds, int valid) {
    uint32_t buttons = readButtons();
    int returnValue = 0;
    // Simulate delay by doing nothing for the specified duration
    // This can be replaced with actual delay mechanism in your hardware
    // but for simulation purposes, it's left empty
    unsigned int i;
    for (i = 0; i < milliseconds * 1000; i++) {
        if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
            returnValue = 1;
            break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
        }
        else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
            returnValue = 2;
            //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
        }
        // else if (buttons & (1 << KEY1_BIT)) {
        //     returnValue = 3;
        //     //break; // Exit the loop immediately when KEY1_BIT is pressed
        // }
        asm("nop"); // Assembly instruction for no operation
    }
    printf("Delay: %d\n", returnValue);
    return returnValue;
}

int lightLoop(){
    int delay_ms = 1000; // Initial delay in milliseconds
    float decrease_factor = 0.9; // Decrease factor after each reset
    int score = 0; // Score
    int pressed = 0; // Button pressed flag
    int time = 0; // Time
    int valid = 0;
    int decreaseTime = 0; // Decrease time
    int returnValue = 0; // Return value
    uint32_t buttons = readButtons();
    volatile int * gpio_reg = (int *)JP1_BASE; //GPIO DATA REGISTER
    volatile int * gpioC_ptr = (int *)(JP1_BASE+4); //GPIO CONTROL REGISTER
    unsigned int leds = 0x01; // Start with first LED lit

    *(gpioC_ptr) = 1023;	// Set the first 10 bits [9-0] to 1 to enable them as output for the LEDs
    *(gpio_reg) = 0;		//

    //0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023

    while(score < 10){
        DisplayHex(score); // Display reset counter on HEX3_HEX0
        //leds = 0x01;
        if (time <= (10 * delay_ms)){
            valid = 0;
            // if (leds == 0x400) // If 11th LED
            //     *LED_ADDR = leds; // Display only 11th LED
            // else
            //     *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
            // leds <<= 1;
            printf("1");
            *gpio_reg = 1;
            if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                returnValue = 1;
                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
            }
            else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                returnValue = 2;
                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
            }
            // else if (buttons & (1 << KEY1_BIT)) {
            //     returnValue = 3;
            //     //break; // Exit the loop immediately when KEY1_BIT is pressed
            // }
            if (returnValue == 2){
                DisplayHex(score); // Display reset counter on HEX3_HEX0
                *gpio_reg = 1023;
                return 1;
            }
            if (returnValue == 3){
                DisplayHex(score); // Display reset counter on HEX3_HEX0
                return 2;
            }
            pressed = delay(delay_ms, 0); // Delay
            if (pressed == 2){
                DisplayHex(score); // Display reset counter on HEX3_HEX0
                *gpio_reg = 1023;
                return 1;
            }
            if (pressed == 3){
                DisplayHex(score); // Display reset counter on HEX3_HEX0
                return 2;
            }
            time += delay_ms;
        }
        else{
            if (time > (10 * delay_ms) && time <= (20 * delay_ms)){
                if (leds == 0x400) // If 11th LED
                    *LED_ADDR = leds; // Display only 11th LED
                else
                    *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                leds <<= 1;
                printf("2");
                *gpio_reg = 3;
                valid = 0;
                if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                    returnValue = 1;
                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                }
                else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                    returnValue = 2;
                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                }
                // else if (buttons & (1 << KEY1_BIT)) {
                //     returnValue = 3;
                //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                // }
                if (returnValue == 2){
                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                    *gpio_reg = 1023;
                    return 1;
                }
                if (returnValue == 3){
                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                    return 2;
                }
                pressed = delay(delay_ms, 0); // Delay
                if (pressed == 2){
                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                    *gpio_reg = 1023;
                    return 1;
                }
                if (pressed == 3){
                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                    return 2;
                }
                time += delay_ms;
            }
            else{
                if (time > (20 * delay_ms) && time <= (30 * delay_ms)){
                    if (leds == 0x400) // If 11th LED
                        *LED_ADDR = leds; // Display only 11th LED
                    else
                        *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                    leds <<= 1;
                    printf("3");
                    *gpio_reg = 7;
                    valid = 0;
                    if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                        returnValue = 1;
                        //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                    }
                    else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                        returnValue = 2;
                        //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                    }
                    // else if (buttons & (1 << KEY1_BIT)) {
                    //     returnValue = 3;
                    //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                    // }
                    if (returnValue == 2){
                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                        *gpio_reg = 1023;
                        return 1;
                    }
                    if (returnValue == 3){
                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                        return 2;
                    }
                    pressed = delay(delay_ms, 0); // Delay
                    if (pressed == 2){
                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                        *gpio_reg = 1023;
                        return 1;
                    }
                    if (pressed == 3){
                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                        return 2;
                    }
                    time += delay_ms;
                }
                else{
                    if (time > (30 * delay_ms) && time <= (40 * delay_ms)){
                        if (leds == 0x400) // If 11th LED
                            *LED_ADDR = leds; // Display only 11th LED
                        else
                            *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                        leds <<= 1;
                        printf("4");
                        *gpio_reg = 15;
                        valid = 0;
                        if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                            returnValue = 1;
                            //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                        }
                        else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                            returnValue = 2;
                            //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                        }
                        // else if (buttons & (1 << KEY1_BIT)) {
                        //     returnValue = 3;
                        //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                        // }
                        if (returnValue == 2){
                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                            *gpio_reg = 1023;
                            return 1;
                        }
                        if (returnValue == 3){
                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                            return 2;
                        }
                        pressed = delay(delay_ms, 0); // Delay
                        if (pressed == 2){
                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                            *gpio_reg = 1023;
                            return 1;
                        }
                        if (pressed == 3){
                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                            return 2;
                        }
                        time += delay_ms;
                    }
                    else{
                        if (time > (40 * delay_ms) && time <= (50 * delay_ms)){
                            if (leds == 0x400) // If 11th LED
                                *LED_ADDR = leds; // Display only 11th LED
                            else
                                *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                            leds <<= 1;
                            printf("5");
                            *gpio_reg = 31;
                            valid = 0;
                            if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                returnValue = 1;
                                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                            }
                            else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                returnValue = 2;
                                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                            }
                            // else if (buttons & (1 << KEY1_BIT)) {
                            //     returnValue = 3;
                            //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                            // }
                            if (returnValue == 2){
                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                *gpio_reg = 1023;
                                return 1;
                            }
                            if (returnValue == 3){
                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                return 2;
                            }
                            pressed = delay(delay_ms, 0); // Delay
                            if (pressed == 2){
                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                *gpio_reg = 1023;
                                return 1;
                            }
                            if (pressed == 3){
                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                return 2;
                            }
                            time += delay_ms;
                        }
                        else{
                            if (time > (50 * delay_ms) && time <= (60 * delay_ms)){
                                if (leds == 0x400) // If 11th LED
                                    *LED_ADDR = leds; // Display only 11th LED
                                else
                                    *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                                leds <<= 1;
                                printf("6");
                                *gpio_reg = 63;
                                valid = 1;
                                if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                    returnValue = 1;
                                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                                }
                                else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                    returnValue = 2;
                                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                                }
                                // else if (buttons & (1 << KEY1_BIT)) {
                                //     returnValue = 3;
                                //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                                // }
                                if (returnValue == 1){
                                    score++;
                                    decreaseTime = 1;
                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                }
                                if (returnValue == 2){
                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                    *gpio_reg = 1023;
                                    return 1;
                                }
                                if (returnValue == 3){
                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                    return 2;
                                }
                                printf("test1\n");
                                pressed = delay(delay_ms, 1); // Delay
                                if (pressed == 1){
                                    score++;
                                    decreaseTime = 1;
                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                }
                                time += delay_ms;
                                printf("test2\n");
                            }
                            else{
                                if (time > (60 * delay_ms) && time <= (70 * delay_ms)){
                                    printf("Pressed: %d\n", pressed);
                                    // if(pressed == 0){
                                    //     DisplayHex(score);
                                    //     return 1;
                                    // }
                                    if (leds == 0x400) // If 11th LED
                                        *LED_ADDR = leds; // Display only 11th LED
                                    else
                                        *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                                    leds <<= 1;
                                    printf("7");
                                    *gpio_reg = 127;
                                    valid = 0;
                                    if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                        returnValue = 1;
                                        //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                                    }
                                    else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                        returnValue = 2;
                                        //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                                    }
                                    // else if (buttons & (1 << KEY1_BIT)) {
                                    //     returnValue = 3;
                                    //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                                    // }
                                    if (returnValue == 2){
                                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                                        *gpio_reg = 1023;
                                        return 1;
                                    }
                                    if (returnValue == 3){
                                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                                        return 2;
                                    }
                                    pressed = delay(delay_ms, 0); // Delay
                                    if (pressed == 2){
                                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                                        *gpio_reg = 1023;
                                        return 1;
                                    }
                                    if (pressed == 3){
                                        DisplayHex(score); // Display reset counter on HEX3_HEX0
                                        return 2;
                                    }
                                    time += delay_ms;
                                }
                                else{
                                    if (time > (70 * delay_ms) && time <= (80 * delay_ms)){
                                        if (leds == 0x400) // If 11th LED
                                            *LED_ADDR = leds; // Display only 11th LED
                                        else
                                            *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                                        leds <<= 1;
                                        printf("8");
                                        *gpio_reg = 255;
                                        valid = 0;
                                        if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                            returnValue = 1;
                                            //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                                        }
                                        else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                            returnValue = 2;
                                            //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                                        }
                                        // else if (buttons & (1 << KEY1_BIT)) {
                                        //     returnValue = 3;
                                        //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                                        // }
                                        if (returnValue == 2){
                                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                                            *gpio_reg = 1023;
                                            return 1;
                                        }
                                        if (returnValue == 3){
                                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                                            return 2;
                                        }
                                        pressed = delay(delay_ms, 0); // Delay
                                        if (pressed == 2){
                                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                                            *gpio_reg = 1023;
                                            return 1;
                                        }
                                        if (pressed == 3){
                                            DisplayHex(score); // Display reset counter on HEX3_HEX0
                                            return 2;
                                        }
                                        time += delay_ms;
                                    }
                                    else{
                                        if (time > (80 * delay_ms) && time <= (90 * delay_ms)){
                                            if (leds == 0x400) // If 11th LED
                                                *LED_ADDR = leds; // Display only 11th LED
                                            else
                                                *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                                            leds <<= 1;
                                            printf("9");
                                            *gpio_reg = 511;
                                            valid = 0;
                                            if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                                returnValue = 1;
                                                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                                            }
                                            else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                                returnValue = 2;
                                                //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                                            }
                                            // else if (buttons & (1 << KEY1_BIT)) {
                                            //     returnValue = 3;
                                            //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                                            // }
                                            if (returnValue == 2){
                                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                *gpio_reg = 1023;
                                                return 1;
                                            }
                                            if (returnValue == 3){
                                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                return 2;
                                            }
                                            pressed = delay(delay_ms, 0); // Delay
                                            if (pressed == 2){
                                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                *gpio_reg = 1023;
                                                return 1;
                                            }
                                            if (pressed == 3){
                                                DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                return 2;
                                            }
                                            time += delay_ms;
                                        }
                                        else{
                                            if (time > (90 * delay_ms) && time <= (100 * delay_ms)){
                                                if (leds == 0x400) // If 11th LED
                                                    *LED_ADDR = leds; // Display only 11th LED
                                                else
                                                    *LED_ADDR = (leds << 1) - 1; // Turn on all LEDs up to the current LED
                                                leds <<= 1;
                                                printf("10");
                                                *gpio_reg = 1023;
                                                valid = 0;
                                                if ((buttons & (1 << KEY0_BIT)) && valid == 1) {
                                                    returnValue = 1;
                                                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 1
                                                }
                                                else if ((buttons & (1 << KEY0_BIT)) && valid == 0) {
                                                    returnValue = 2;
                                                    //break; // Exit the loop immediately when KEY0_BIT is pressed and valid is 0
                                                }
                                                // else if (buttons & (1 << KEY1_BIT)) {
                                                //     returnValue = 3;
                                                //     //break; // Exit the loop immediately when KEY1_BIT is pressed
                                                // }
                                                if (returnValue == 2){
                                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                    *gpio_reg = 1023;
                                                    return 1;
                                                }
                                                if (returnValue == 3){
                                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                    return 2;
                                                }
                                                pressed = delay(delay_ms, 0); // Delay
                                                if (pressed == 2){
                                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                    *gpio_reg = 1023;
                                                    return 1;
                                                }
                                                if (pressed == 3){
                                                    DisplayHex(score); // Display reset counter on HEX3_HEX0
                                                    return 2;
                                                }
                                                time += delay_ms;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (decreaseTime == 1){
            delay_ms *= decrease_factor;
            printf("Delay: %d\n", delay_ms);
            time = 0;
            decreaseTime = 0;
        }
    }
}

int main(void){
    uint32_t buttons = readButtons();

    int result;
    
    result = lightLoop();
    printf("Result: %d\n", result);

    if (result == 1) {
        printf("Restarting...\n");
        // while (1) {
        //     printf("Waiting...\n");
        //     if (buttons & (1 << KEY1_BIT)) {
        //         break;
        //     }
        // }
        printf("Restarted\n");
        lightLoop();
    }
    else if (result == 2) {
        printf("Starting...\n");
        lightLoop();
    }
}