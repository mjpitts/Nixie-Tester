#include <avr/io.h>
#include <util/delay.h>

#define DEFAULT_SPEED 500
#define MAX_SPEED 10
#define DATA_BITS 4

// Increases speed until it reaches max speed, then i returns to default.
void change_speed(uint16_t *speed_ptr){
    if((*speed_ptr) > MAX_SPEED){
        (*speed_ptr) *= .6;
    }
    else{
        (*speed_ptr) = DEFAULT_SPEED;
    }
}

// Checks if button is pressed, if so it waits can checks again before incrementing speed. 
void check_button(uint8_t *button_ptr, uint16_t *speed_ptr){
    if(bit_is_clear(PINB, 1) && (*button_ptr) == 0){
            change_speed(speed_ptr);
            _delay_ms(75);
            if(bit_is_clear(PINB, 1)){
                (*button_ptr) = 1;
                change_speed(speed_ptr);
                PORTC |= (1 << 0);
            }
        }
        else{
            (*button_ptr) = 0;
            PORTC &= ~(1 << 0);
        }
}

/*  This works just like the normal delay function but allows for variable delays and allows
buttons checks between each interval.   */
void delay(uint16_t mS, uint8_t *button_ptr, uint16_t *speed_ptr){
    for(uint16_t i = 0; i < mS; i++){
        check_button(button_ptr, speed_ptr);
        _delay_ms(1);
    }
}

// Sets output pins for nixe driver.
void set_output(uint8_t arr[4], uint8_t arr_size){
    uint8_t j = 4;
    // Parses through array from most significant bit to least. USe j to flip corrent register.
    for(uint8_t i = 0; i < arr_size; i++){
        if(arr[i] == 1){
            PORTC |= (1 << j);
        }
        else{
            PORTC &= ~(1 << j);
        }
        j--;
    }
}

int main(void){

    // State values and corresponding ptrs
    uint8_t button_state = 0;
    uint8_t *button_ptr = &button_state;
    uint16_t speed = 500;
    uint16_t *speed_prt = &speed;

    // Core time value.
    uint8_t time = 0;

    /* Control variable that determines if we are in clk or BCD mode.
    Microcontroller is init in clk mode.*/
    uint8_t bcd_mode = 0;

    // Stores the value of the BCD thumbwheel input. Init to zero.
    uint8_t current_BCD_wheel[4] = {0,0,0,0};
    // BCD decimal value, set time with this so that clk starts where wheel leaves off.
    uint8_t BCD_decimal_val = 0;

    uint8_t mappings[10][4] = {{0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},
    {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, {1,0,0,0}, {1,0,0,1}};

    // Init output pins, all portc/portd will be input; Just because.
    DDRC = 0xff;
    DDRD = 0xff;

    // Turn on pull-up res.
    PORTB |= 0xff;

    // Main event loop.
    while(1){
        

        if(bcd_mode == 0){

            // Clear previous output value before every clk loop.
            PORTC = 0x00;

            set_output(mappings[time], DATA_BITS);

            // Call delay function which also checks for button press while delaying.
            delay(speed, button_ptr, speed_prt);

            // Increment time.
            time += 1;

             // Loop back to 0 when 10 is reached.
            if(time >= 10){
                time = 0;
            }
        }
        else{
            BCD_decimal_val = 0;

            // 1's
            if(bit_is_clear(PINB, 2)){
                current_BCD_wheel[3] = 1;
                BCD_decimal_val += 1;
            }
            else{
                current_BCD_wheel[3] = 0;
            }

            // 2's
            if(bit_is_clear(PINB, 3)){
                current_BCD_wheel[2] = 1;
                BCD_decimal_val += 2;
            }
            else{
                current_BCD_wheel[2] = 0;
            }

            // 4's
            if(bit_is_clear(PINB, 4)){
                current_BCD_wheel[1] = 1;
                BCD_decimal_val += 4;
            }
            else{
                current_BCD_wheel[1] = 0;
            }

            // 8's
            if(bit_is_clear(PINB, 5)){
                current_BCD_wheel[0] = 1;
                BCD_decimal_val += 8;
            }
            else{
                current_BCD_wheel[0] = 0;
            }

            time = BCD_decimal_val;
        }

        if(bit_is_clear(PINB, 0)){
            PORTD |= (1 << 0);
            bcd_mode = 1;
            set_output(current_BCD_wheel, DATA_BITS);
            _delay_ms(25);
        }
        else{
            PORTD &= ~(1 << 0);
            bcd_mode = 0;
        }
    }
}