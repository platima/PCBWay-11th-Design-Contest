# Innovation Lightbulb Badge - Touch-Activated LED Controller

**🏆 PCBWay 11th Badge Design Contest Entry**

*"11 Years of Innovation and Beyond with PCBWay"*

A creative lightbulb-shaped badge featuring touch-activated LED animations, combining PCB art with functional electronics. The badge celebrates PCBWay's 11-year journey with an interactive cog/gear LED pattern that comes to life when you touch the golden snake base.

![Innovation Lightbulb Badge](images/badge_front.jpg)
*The completed Innovation Lightbulb Badge showcasing PCBWay's precision manufacturing*

![Project Banner](images/banner.jpg)
*Add your project photos here*

## 🌟 Features

- **Touch Activation**: Cog/gear LEDs illuminate when touching the golden snake base
- **Rotating Animation**: LED pattern simulates a turning gear/cog mechanism  
- **Cultural Integration**: Year of the Wood Snake (2025) with bamboo and Chinese pine tree artwork
- **PCB Art**: Complex lightbulb cutout showcasing PCBWay's precision manufacturing and UV print capabilities
- **Low Power Design**: Battery-efficient with sleep mode when not in use
- **Programmable Patterns**: Customisable animations for different events and occasions
- **Interactive Badge**: Encourages human connection and conversation
- **Open Source**: Full code and designs shared with the maker community

## 🔧 Hardware Requirements

### Components
- PCB _(custom design for PCBWay contest)_
- STM8S TSSOP20 MCU _(STM8S003 or higher)_
- Other supporting components _(see `/hardware/BOM.csv`)_
- Power supply _(3.3-5V, designed for LiPo)_

## 🚀 Getting Started

### Prerequisites
- Arduino IDE 2.0 with [akashkapashia/stm8_arduino](https://github.com/akashkapashia/stm8_arduino/) added
- ST-Link programmer _(Recommend ST-Link v2)_
- Battery or power source _(Recommend 503035 or 602535 with protection circuit)_
- This code (or `firmware.ino.hex.zip` which you can just unzip and flash with `stm8flash`)

### Assembly

#### Soldering Order

1. Apply solder paste and reflow the LEDs on the front first
2. Apply solder paste and reflow the SMD components on the back next

#### Programming pads
( ) RST
( ) GND
( ) SWIM
[ ] 5V

#### Notes
- LED capacitors are optional but recommended
- The 0 Ohm resistor (R4) on the back can be used to force-on without code changes

### Installation

1. **Clone or download the repository:**
   - `git clone https://github.com/platima/PCBWay-11th-Design-Contest.git`
   - Open `firmware/firmware.ino`

2. **Install dependencies:**
   - Ensure you have added `https://github.com/akashkapashia/stm8_arduino/raw/master/package_sduino_stm8_index.json` to your Arduino IDE boards URLs
   - Install any required drivers

3. **Hardware setup:**
   - Connect your ST-Link programmer to your computer
   - Wire the PCB programming pads to the ST-Link

4. **Upload the code:**
   - Compile and upload the Arduino project to your STM8 microcontroller

## 🎨 How It Works

1. **Standby Mode**: When no touch is detected:
   - All LEDs remain off to preserve battery
   - MCU enters low-power halt mode
   - Periodically checks touch sensor for activation

2. **Touch Activation**: When the golden snake base is touched:
   - Wakes from low-power mode instantly
   - Activates the cog/gear LED pattern
   - Begins rotating animation sequence

3. **Cog Animation**: 
   - 28 LEDs arranged in 14×2 gear tooth pattern _(the code is configured with 32 LEDs due to the wrapping modulus)_
   - Smooth rotation effect simulating mechanical gear
   - Customizable colors and speeds for different occasions
   - Can display rainbow patterns, solid colors, or custom animations

4. **Interactive Features**:
   - Encourages social interaction and conversation
   - Badge becomes a talking point at events
   - Demonstrates PCBWay's precision manufacturing capabilities

## ⚙️ Configuration

### Cog Animation Settings
```c
#define ANIMATION_DELAY 200  // Animation speed (ms)
#define TOOTH_SIZE      4    // LEDs per color segment
#define TOOTH_GAP       4    // Gap between segments
#define SHIFT_AMOUNT    2    // Animation shift speed
#define BRIGHTNESS      64   // LED brightness (0-255)
```

### Touch Sensitivity
```c
#define TOUCH_THRESHOLD_MV 800 // ADC threshold for touch detection
```

### LED Strip Configuration
```c
#define N_LEDS          32   // Total number of LEDs
```

## 🎨 How It Works

1. **Standby Mode**: When no touch is detected, the controller:
   - Turns off all LEDs
   - Enters low-power halt mode
   - Periodically checks for touch input

2. **Active Mode**: When touch is detected:
   - Wakes from low-power mode
   - Activates rainbow animation
   - Continuously updates LED colors
   - Monitors touch sensor

3. **Animation Pattern**: 
   - Creates "teeth" of colored LEDs
   - Each tooth cycles through rainbow colors
   - Pattern shifts smoothly along the strip

## 🔨 PCB Design

The custom PCB design includes:
- Compact STM8 microcontroller footprint
- Gold snake as touch pads
- Debug / programming pads
- Space for battery to be attached _(Recommend velcro)_
- Landyard loop

*PCB files and schematics available in the `/hardware` directory*

## 📈 Performance

- **Response Time**: <50ms touch detection
- **Animation Frame Rate**: ~5 FPS (configurable)
- **Color Accuracy**: 24-bit RGB with gamma correction
- **Power Efficiency**: >95% idle time power savings

## 🛠️ Troubleshooting

### Common Issues

**LEDs don't light up:**
- Check power supply voltage (>3.5V for WS2812)

**Touch sensor not responding:**
- Ensure you're touching more than one 'snake' pad
- Check ADC threshold setting
- Verify PD5 connection
- Test with multimeter for voltage changes
- Add R0 tying PD4 to VCC on the back

**Animation stuttering:**
- Reduce `ANIMATION_DELAY` for smoother motion
- Check for power supply noise
- Verify clock configuration

## 🚧 Future Enhancements

- [ ] **Multiple Animation Modes**: Different patterns for various occasions
- [ ] **Color Customization**: Easy programming for event-specific colors  
- [ ] **Battery Monitoring**: LED indicators for charge level
- [ ] **Community Patterns**: Shareable animation library

## 📋 Version History

### Version 3.0 (Current)
- Added touch sensor functionality
- Implemented low-power mode
- Rainbow color cycling
- Optimized power consumption

### Version 2.0
- Basic animation patterns
- STM8 port completion

### Version 1.0
- Initial WS2812 control
- Basic LED functions

## 🤝 Contributing

This project is part of the PCBWay 11th Design Contest. While the contest is ongoing, feedback and suggestions are welcome!

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **PCBWay** for hosting the design contest
- **akashkapashia** for patching up the Sduino library
- **ctxz** for the fantastic WS2812 library
- **STMicroelectronics** for the STM8 platform
- **Adafruit** for WS2812 inspiration
- The embedded systems community for support and resources

## 📞 Contact

- **GitHub**: [@platima](https://github.com/platima)
- **Project Link**: [PCBWay 11th Design Contest](https://github.com/platima/PCBWay-11th-Design-Contest)

---

**🏆 PCBWay 11th Design Contest Entry**

*Showcasing innovative PCB design with practical embedded systems implementation*
