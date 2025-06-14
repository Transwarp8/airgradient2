# AirGradient ONE - LED System Modifications

## Overview

This document describes the modifications made to the AirGradient ONE LED system to provide more intuitive air quality visualization. The changes add a new "fuel gauge" style LED pattern where **more LEDs indicate better air quality**, alongside preserving the original pattern for backward compatibility.

## Hardware Specifications

### LED Bar Configuration
- **Total LEDs**: 11 individually addressable RGB LEDs
- **Hardware**: WS2812/WS2813 type (NeoPixel compatible)
- **GPIO Pin**: 10
- **LED Numbering**: 0-10 (physical layout right-to-left)
- **Control**: Adafruit NeoPixel library

### Color Definitions
- **Green**: RGB(0, 255, 0) - Good air quality
- **Yellow**: RGB(255, 150, 0) - Moderate air quality  
- **Orange**: RGB(255, 40, 0) - Unhealthy for sensitive groups
- **Red**: RGB(255, 0, 0) - Unhealthy
- **Purple**: RGB(180, 0, 255) - Very unhealthy/hazardous

## LED Operating Modes

The system now supports **four LED modes** configurable through the device interface:

### 1. `"off"` - LEDs Disabled
- All LEDs turned off
- No air quality indication

### 2. `"pm"` - PM2.5 Mode (Unchanged)
- Displays PM2.5 particulate matter levels
- Uses original algorithm
- More LEDs = worse air quality

### 3. `"co2"` - CO2 Fuel Gauge Mode (NEW)
- **Intuitive "fuel gauge" pattern**
- More LEDs = better air quality
- Recommended for most users

### 4. `"co2classic"` - CO2 Classic Mode 
- Original CO2 pattern preserved
- Fewer LEDs = better air quality
- Maintains backward compatibility

## CO2 LED Patterns

### New Fuel Gauge Mode (`"co2"`)

**Philosophy**: More lights = better air quality (like a fuel gauge)

| CO2 Range (ppm) | LEDs Lit | Colors | Visual Pattern | Air Quality |
|-----------------|----------|---------|----------------|-------------|
| **≤ 600** | **9 LEDs** | Green | `GGGGGGGGG__` | Excellent |
| **601-800** | **8 LEDs** | Green | `GGGGGGGG___` | Good |
| **801-1000** | **7 LEDs** | Yellow | `YYYYYYY____` | Moderate |
| **1001-1250** | **6 LEDs** | Orange | `OOOOOO_____` | Unhealthy for Sensitive |
| **1251-1500** | **5 LEDs** | Orange | `OOOOO______` | Unhealthy for Sensitive |
| **1501-1750** | **4 LEDs** | Red | `RRRR_______` | Unhealthy |
| **1751-2000** | **3 LEDs** | Red | `RRR________` | Unhealthy |
| **2001-3000** | **2 LEDs** | Red | `RR_________` | Very Unhealthy |
| **> 3000** | **9 LEDs** | Purple/Red | `PRPRPRPRP__` | Hazardous |

### Classic Mode (`"co2classic"`)

**Philosophy**: More lights = worse air quality (original behavior)

| CO2 Range (ppm) | LEDs Lit | Colors | Visual Pattern | Air Quality |
|-----------------|----------|---------|----------------|-------------|
| **≤ 600** | **1 LED** | Green | `G__________` | Excellent |
| **601-800** | **2 LEDs** | Green | `GG_________` | Good |
| **801-1000** | **3 LEDs** | Yellow | `YYY________` | Moderate |
| **1001-1250** | **4 LEDs** | Orange | `OOOO_______` | Unhealthy for Sensitive |
| **1251-1500** | **5 LEDs** | Orange | `OOOOO______` | Unhealthy for Sensitive |
| **1501-1750** | **6 LEDs** | Red | `RRRRRR_____` | Unhealthy |
| **1751-2000** | **7 LEDs** | Red | `RRRRRRR____` | Unhealthy |
| **2001-3000** | **8 LEDs** | Purple | `PPPPPPPP___` | Very Unhealthy |
| **> 3000** | **9 LEDs** | Purple/Red | `PRPRPRPRP__` | Hazardous |

## User Configuration

### Web Interface Configuration

1. **Connect to device WiFi** or access via home network
2. **Navigate to device IP address** in web browser
3. **Locate LED settings** in configuration panel
4. **Select desired mode**:
   - `co2` - New fuel gauge pattern (recommended)
   - `co2classic` - Original pattern
   - `pm` - PM2.5 mode
   - `off` - Disabled

### Cloud Dashboard Configuration

If connected to AirGradient cloud service:
1. **Access AirGradient dashboard**
2. **Select your device**
3. **Navigate to LED configuration**
4. **Choose LED mode** from dropdown

### JSON Configuration

Direct JSON configuration via API:
```json
{
  "ledBarMode": "co2"
}
```

Valid values: `"co2"`, `"co2classic"`, `"pm"`, `"off"`

## Technical Implementation

### Files Modified

#### Core LED Logic
- **`src/AgStateMachine.cpp`**:
  - Added `co2handleLedsClassic()` function
  - Modified `co2handleLeds()` for fuel gauge pattern
  - Updated state machine LED handling

- **`src/AgStateMachine.h`**:
  - Added function declaration for `co2handleLedsClassic()`

#### Configuration System
- **`src/App/AppDef.h`**:
  - Added `LedBarModeCO2Classic` enum value
  - Updated enum documentation

- **`src/AgConfigure.cpp`**:
  - Added `"co2classic"` to `LED_BAR_MODE_NAMES` array
  - Updated `getLedBarModeName()` function
  - Updated validation functions for new mode

### Code Architecture

```
StateMachine::handleLeds()
├── LedBarModeCO2 → co2handleLeds() [NEW FUEL GAUGE]
├── LedBarModeCO2Classic → co2handleLedsClassic() [ORIGINAL]
├── LedBarModePm → pm25handleLeds() [UNCHANGED]
└── LedBarModeOff → ag->ledBar.clear()
```

### LED Control Functions

#### New Fuel Gauge Function
```cpp
int StateMachine::co2handleLeds(void) {
  // Inverted pattern: more LEDs = better air quality
  // Uses 9 LEDs maximum (LEDs 2-10)
  // Maintains purple/red warning for >3000 ppm
}
```

#### Classic Pattern Function  
```cpp
int StateMachine::co2handleLedsClassic(void) {
  // Original pattern: fewer LEDs = better air quality
  // Uses 9 LEDs maximum (LEDs 2-10)
  // Exact replica of original behavior
}
```

## LED Physical Layout

**AirGradient ONE LED Bar** (11 LEDs, right-to-left addressing):
```
Physical: [LED10][LED9][LED8][LED7][LED6][LED5][LED4][LED3][LED2][LED1][LED0]
Software:   10     9     8     7     6     5     4     3     2     1     0
Usage:    [←──────────── Active LEDs (2-10) ──────────→] [Reserved] [Reserved]
```

**LED Usage**:
- **LEDs 2-10**: Air quality indication (9 LEDs total)
- **LEDs 0-1**: Reserved/unused
- **Right-to-left**: LEDs light from right side first

## Air Quality Reference

### CO2 Concentration Guidelines

| Range (ppm) | Health Impact | Recommendation |
|-------------|---------------|----------------|
| **≤ 600** | Excellent | Optimal indoor air quality |
| **600-800** | Good | Acceptable for most activities |
| **800-1000** | Moderate | Consider ventilation |
| **1000-1500** | Poor | Increase ventilation |
| **1500-2000** | Unhealthy | Immediate ventilation needed |
| **2000-3000** | Very Unhealthy | Health risks present |
| **> 3000** | Hazardous | Dangerous - evacuate if possible |

### Visual Advantages of Fuel Gauge Mode

**Intuitive Understanding**:
- **Full bar** (9 LEDs) = **Great air quality** ✓
- **Half bar** (4-5 LEDs) = **Moderate air quality** ⚠️
- **Few LEDs** (1-3 LEDs) = **Poor air quality** ⚠️
- **Flashing purple/red** = **Dangerous levels** ⚠️

**User Benefits**:
- Matches common "fuel gauge" mental model
- Immediate visual feedback without learning curve
- "More is better" aligns with positive reinforcement
- Clear differentiation between good and bad air quality

## Installation and Compilation

### Prerequisites
- Arduino IDE 2.x
- ESP32 board package version 2.0.17 (NOT 3.x)
- AirGradient ONE hardware (ESP32C3)

### Board Settings
```
Board: ESP32C3 Dev Module
USB CDC On Boot: Enabled
CPU Frequency: 160MHz (WiFi)
Flash Size: 4MB (32Mb)
Partition Scheme: Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)
Upload Speed: 921600
```

### Upload Process
1. Connect AirGradient ONE via USB
2. Select correct COM port
3. Click Upload (→) in Arduino IDE
4. Wait for "Done uploading" confirmation

## Troubleshooting

### Common Issues

**LEDs not responding**:
- Check LED mode is not set to `"off"`
- Verify sensor readings are valid
- Restart device

**Wrong LED pattern**:
- Check selected LED mode in configuration
- Ensure firmware uploaded successfully
- Verify CO2 sensor functionality

**Configuration not saving**:
- Check WiFi connectivity
- Verify web interface accessibility
- Try factory reset and reconfigure

### Debug Information

Enable serial debugging to monitor:
- LED mode selection
- CO2 readings
- LED pattern calculations
- Configuration changes

## Backward Compatibility

**Existing Users**:
- Default behavior unchanged
- Classic mode preserves original functionality
- No breaking changes to API or configuration
- Upgrade path maintains user settings

**Migration Path**:
1. Flash new firmware
2. Access configuration interface
3. Select preferred LED mode
4. Save configuration

## Future Enhancements

**Potential Improvements**:
- **Multi-sensor LED mode**: Combine CO2, PM2.5, temperature, and humidity
- **Custom thresholds**: User-configurable CO2 level ranges
- **Animation effects**: Breathing, pulsing, or gradient transitions
- **Color customization**: User-selectable color schemes
- **Brightness scheduling**: Automatic dimming based on time of day

## Conclusion

The LED modifications provide a more intuitive user experience while maintaining full backward compatibility. The new fuel gauge mode aligns with user expectations and provides immediate visual feedback about air quality without requiring technical knowledge of CO2 concentration levels.

The implementation maintains the robustness and reliability of the original AirGradient system while enhancing user experience through better visual communication of air quality status. 