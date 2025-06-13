/*
 * Innovation Lightbulb Badge - Touch-Activated LED Controller
 * PCBWay 11th Badge Design Contest Entry
 * 
 * Description: Interactive badge featuring touch-activated cog/gear LED animation
 *              Celebrates "11 Years of Innovation and Beyond with PCBWay"
 * 
 * Hardware: STM8S003F3 microcontroller (20-pin TSSOP)
 *           28x WS2812B LEDs arranged in 14×2 cog/gear pattern
 *           Touch sensor pads (golden snake base)
 *           LiPo battery with charging circuit
 * 
 * Author: PlatimaTinkers
 * Date: June 2025
 * License: MIT License
 * 
 * Repository: https://github.com/platima/PCBWay-11th-Design-Contest
 * Board Library: https://github.com/akashkapashia/stm8_arduino/raw/master/package_sduino_stm8_index.json
 * WS2812 Library: https://github.com/ctxz/TinyWS2812 / https://ctxz.github.io/TinyWS2812/ 
 * PCBWay Shared: https://www.pcbway.com/project/shareproject/
 * 
 * Contest Theme: "11 Years of Innovation and Beyond with PCBWay"
 * 
 * Features:
 * - Touch-activated cog/gear LED animation
 * - Low power battery operation with sleep mode
 * - Customisable colors and patterns
 * - Cultural integration (Year of the Wood Snake 2025)
 * - Interactive badge encouraging human connection
 * - Demonstrates PCBWay's precision manufacturing capabilities
 */

#include <Arduino.h>
#include "ws2812.h"

// Adjust total number of LEDs in your strip
#define N_LEDS          32
#define DATA_PORT_BASE  GPIOD_BaseAddress
#define DATA_PIN        {GPIO_PIN_4}
#define RESET_TIME      30
#define COLOR_ORDER     grb

// Input settings
#define SNAKE_PORT      GPIOD
#define SNAKE_PIN       GPIO_PIN_5

// Animation settings
#define ANIMATION_DELAY 200     // Animation delay in milliseconds
#define TOOTH_SIZE      4       // Number of LEDs per tooth
#define TOOTH_GAP       4       // Gap between teeth
#define SHIFT_AMOUNT    2       // How many LEDs to shift by each step
#define BRIGHTNESS      64      // Half brightness (0-255)

// Voltage values for touch
#define TOUCH_THRESHOLD_MV 800  // 500mV threshold for reliable touch detection
#define ADC_VREF_MV     4100    // 3.3V reference voltage in millivolts
#define ADC_RESOLUTION  1023    // 10-bit ADC (2^10 - 1)
#define ADC_TO_MV_FACTOR (ADC_VREF_MV / ADC_RESOLUTION)  // ~4mV per ADC count

// Animation modes for different occasions
#define MODE_RAINBOW    0     // Rainbow color cycling
#define MODE_PCBWAY     1     // PCBWay green theme  
#define MODE_FORTUNE    2     // Red for good fortune
#define MODE_AUSTRALIA  3     // National colors
#define MODE_WHITE      4     // Just plain white
#define MODE_COUNT      5     // Number of modes, always top number
#define MODE_CHANGE_MS  10000 // 10 seconds
#define MODE_DEFAULT    MODE_RAINBOW

// LED Globals
uint8_t led_pin[] = DATA_PIN;
ws2812_rgb leds[N_LEDS]; 
ws2812 ws2812_dev;
uint8_t current_mode = MODE_DEFAULT;

// On to the functions
void STM8_Init_16MHz_Clock() {
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

void ADC_Init() {
  // Enable ADC clock
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
  
  // Configure PD5 as analog input
  GPIO_Init(SNAKE_PORT, SNAKE_PIN, GPIO_MODE_IN_FL_NO_IT);
  
  // ADC configuration
  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, 
            ADC1_CHANNEL_5,
            ADC1_PRESSEL_FCPU_D18, 
            ADC1_EXTTRIG_TIM, 
            DISABLE, 
            ADC1_ALIGN_RIGHT, 
            ADC1_SCHMITTTRIG_ALL, 
            DISABLE);
  
  ADC1_Cmd(ENABLE);
}

uint16_t ADC_ReadValue() {
  unsigned int result = 0;

  // Start conversion
  ADC1_StartConversion();
  
  // Wait for conversion to complete
  while (ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
  
  // Read the result
  result = ADC1_GetConversionValue();

  // Clear the conversion flag
  ADC1_ClearFlag(ADC1_FLAG_EOC);

  // Return the value
  return result;
}

bool CheckTouchSensor() {
  uint16_t sensorValue = ADC_ReadValue();
  return ((sensorValue * ADC_TO_MV_FACTOR) > TOUCH_THRESHOLD_MV);
}

void WS2812_LEDs_Write(ws2812 *ws2812_dev) {
  ws2812_prep_tx(ws2812_dev);
  ws2812_tx(ws2812_dev, leds, N_LEDS);
  ws2812_close_tx(ws2812_dev);
}

// Clear all LEDs (turn off)
void WS2812_LEDs_Clear() {
  for (unsigned int i = 0; i < N_LEDS; i++) {
    leds[i].r = 0;
    leds[i].g = 0;
    leds[i].b = 0;              
  }
}

// Set LED color based on current animation mode and index
void WS2812_LEDs_SetColor(int index, uint8_t mode) {
  if (index < 0 || index >= N_LEDS) return;
  
  switch(mode) {
    case MODE_RAINBOW: {
      // Rainbow mode - cycle through colors based on position
      int color_section = (index * 6) / N_LEDS;
      int fade = ((index * 6) % N_LEDS) * 255 / N_LEDS;
      
      switch(color_section) {
        case 0: // Red to Yellow
          leds[index].r = BRIGHTNESS;
          leds[index].g = (fade * BRIGHTNESS) / 255;
          leds[index].b = 0;
          break;
        case 1: // Yellow to Green  
          leds[index].r = ((255 - fade) * BRIGHTNESS) / 255;
          leds[index].g = BRIGHTNESS;
          leds[index].b = 0;
          break;
        case 2: // Green to Cyan
          leds[index].r = 0;
          leds[index].g = BRIGHTNESS;
          leds[index].b = (fade * BRIGHTNESS) / 255;
          break;
        case 3: // Cyan to Blue
          leds[index].r = 0;
          leds[index].g = ((255 - fade) * BRIGHTNESS) / 255;
          leds[index].b = BRIGHTNESS;
          break;
        case 4: // Blue to Magenta
          leds[index].r = (fade * BRIGHTNESS) / 255;
          leds[index].g = 0;
          leds[index].b = BRIGHTNESS;
          break;
        default: // Magenta to Red
          leds[index].r = BRIGHTNESS;
          leds[index].g = 0;
          leds[index].b = ((255 - fade) * BRIGHTNESS) / 255;
          break;
      }
      break;
    }
    
    case MODE_PCBWAY:
      // PCBWay green theme - RGB(17, 167, 59) scaled to brightness
      leds[index].r = (17 * BRIGHTNESS) / 255;   // Red component scaled
      leds[index].g = (167 * BRIGHTNESS) / 255;  // Green component scaled  
      leds[index].b = (59 * BRIGHTNESS) / 255;   // Blue component scaled
      break;
      
    case MODE_FORTUNE:
      // Red for good fortune and joy
      leds[index].r = BRIGHTNESS;
      leds[index].g = 0;
      leds[index].b = 0;
      break;
      
    case MODE_AUSTRALIA:
      // Green and gold for Australia (customizable)
      if ((index / TOOTH_SIZE) % 2 == 0) {
        // Gold
        leds[index].r = BRIGHTNESS;
        leds[index].g = BRIGHTNESS * 3/4;
        leds[index].b = 0;
      } else {
        // Green
        leds[index].r = 0;
        leds[index].g = BRIGHTNESS;
        leds[index].b = 0;
      }
      break;
      
    case MODE_WHITE:
      // Red for good fortune and joy
      leds[index].r = BRIGHTNESS;
      leds[index].g = BRIGHTNESS;
      leds[index].b = BRIGHTNESS;
      break;
  }
}

void setup() {
  // Setup the clock
  STM8_Init_16MHz_Clock();

  // Disable unnecessary peripherals
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, DISABLE);
  UART1_DeInit();

  // ADC Init
  ADC_Init();

  // LEDs setup
  ws2812_cfg cfg;
  cfg.pins          = led_pin;
  cfg.rst_time_us   = RESET_TIME;
  cfg.order         = COLOR_ORDER;
  cfg.n_dev         = sizeof(led_pin);
  cfg.port_baseaddr = DATA_PORT_BASE;
  ws2812_config(&ws2812_dev, &cfg);
  
  // Initial clear of LEDs
  WS2812_LEDs_Clear();
  WS2812_LEDs_Write(&ws2812_dev);
}

void loop() {
  static uint8_t position = 0;
  static uint32_t last_mode_change = 0;
  
  // Check touch sensor
  if (! CheckTouchSensor()) {
    // Touch not detected - clear LEDs
    WS2812_LEDs_Clear();
    WS2812_LEDs_Write(&ws2812_dev);
    
    while (! CheckTouchSensor()) {
      // Small delay to prevent excessive checking
      delay(150);
    }
    
    // Touch detected
    // Reset animation position and mode change
    position = 0;
    last_mode_change = millis();
    current_mode = MODE_DEFAULT;
  }
  
  // Touch is active - run normal animation
  // Clear all LEDs
  WS2812_LEDs_Clear();
  
  // Light up every TOOTH_SIZE + TOOTH_GAP LEDs, starting at current position
  for (int tooth = 0; tooth < (N_LEDS / (TOOTH_SIZE + TOOTH_GAP)) + 1; tooth++) {
    // Calculate starting LED for this tooth
    int startLed = position + tooth * (TOOTH_SIZE + TOOTH_GAP);
    
    // Light up TOOTH_SIZE LEDs for this tooth
    for (int i = 0; i < TOOTH_SIZE; i++) {
      int ledIndex = (startLed + i) % N_LEDS;  // Wrap around if needed
      
      // Do the LED thing!
      WS2812_LEDs_SetColor(ledIndex, current_mode);
    }
  }
  
  // Update the LEDs
  WS2812_LEDs_Write(&ws2812_dev);
  
  // Increment position for next cycle
  position = (position + SHIFT_AMOUNT) % (TOOTH_SIZE + TOOTH_GAP);

  // Change animation mode occasionally for variety
  if (millis() - last_mode_change > MODE_CHANGE_MS) {  // Change every 30 seconds of cumulative use
    current_mode = (current_mode + 1) % MODE_COUNT;
    last_mode_change = millis();
  }
  
  // Wait for specified time
  delay(ANIMATION_DELAY);
}

// See: https://community.st.com/s/question/0D50X00009XkhigSAB/what-is-the-purpose-of-define-usefullassert
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) { 
  while (1) {}
}
#endif