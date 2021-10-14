# GPIO Interrupt Example

 This code shows how to configure GPIO and how to use a GPIO interrupt.

## Important Files

 Source file: main/user_main.c
 Output file: main/lab1_q2_816005001.out
 Binary file: build/gpio.bin

## GPIO pins

 GPIO2: output
 GPIO0: input, interrupt on falling edge  

## Pin connections

 Connect GPIO2 with GPIO0. 
 Generate pulses on GPIO2 to trigger an interrupts on GPIO0.

## Expected output
...

I (0) gpio: GPIO[2]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:0
I (0) gpio: GPIO[0]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 0| Pulldown: 0| Intr:2

I (0) main: Semaphore created!

I (0) main: cnt: 0

I (1) main: cnt: 1

I (1) main: Released semaphore!

I (1) main: GPIO[0] intr, val: 0

I (1) main: Semaphore obtained!

I (2) main: cnt: 2

...
