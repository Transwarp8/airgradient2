# AirGradient Arduino Library - Complete Function Map

## Overview

This document provides a comprehensive mapping of all functions in the AirGradient Arduino library that controls the Air Gradient One and Air Gradient Outdoor air quality monitors. The library is designed to support multiple hardware configurations including:

- **DIY_BASIC**: ESP8266-based basic sensor setup
- **DIY_PRO_INDOOR_V4_2**: ESP8266-based professional indoor monitor V4.2
- **DIY_PRO_INDOOR_V3_3**: ESP8266-based professional indoor monitor V3.3
- **ONE_INDOOR**: ESP32C3-based indoor monitor
- **OPEN_AIR_OUTDOOR**: ESP32C3-based outdoor monitor

## Core Library Architecture

### Main Classes and Their Relationships

```
AirGradient (Main Factory Class)
├── Sensor Classes
│   ├── PMS5003/PMS5003T (Particulate Matter Sensors)
│   ├── S8 (CO2 Sensor)
│   ├── Sht (Temperature/Humidity Sensor)
│   └── Sgp41 (TVOC/NOx Sensor)
├── Hardware Interface Classes
│   ├── Display (OLED Display)
│   ├── PushButton (User Input)
│   ├── StatusLed (Status Indicator)
│   ├── LedBar (RGB LED Array)
│   └── HardwareWatchdog (External Watchdog)
├── Application Management Classes
│   ├── Configuration (Config Management)
│   ├── Measurements (Data Management)
│   ├── StateMachine (State Management)
│   ├── WifiConnector (Network Setup)
│   ├── AgApiClient (Cloud Communication)
│   └── MqttClient (MQTT Communication)
└── Utility Classes
    ├── OpenMetrics (Prometheus Metrics)
    ├── LocalServer (HTTP Server)
    └── AgSchedule (Task Scheduling)
```

---

## 1. AirGradient Core Class

**File**: `src/AirGradient.h`, `src/AirGradient.cpp`

The main factory class that provides access to all sensors and hardware components.

### Constructor
- **`AirGradient(BoardType type)`**
  - Initializes all sensor and hardware objects for the specified board type
  - Sets up hardware-specific configurations based on board definition

### Board Information Functions
- **`int getI2cSdaPin(void)`**
  - Returns I2C SDA pin number for the board, -1 if invalid
  
- **`int getI2cSclPin(void)`**
  - Returns I2C SCL pin number for the board, -1 if invalid
  
- **`BoardType getBoardType(void)`**
  - Returns the board type enum value
  
- **`String getVersion(void)`**
  - Returns the library/firmware version string (GIT_VERSION)
  
- **`String getBoardName(void)`**
  - Returns human-readable board name
  
- **`String deviceId(void)`**
  - Returns unique device ID derived from WiFi MAC address (lowercase, no colons)

### Board Type Check Functions
- **`bool isOne(void)`**
  - Returns true if board is ONE_INDOOR type
  
- **`bool isOpenAir(void)`**
  - Returns true if board is OPEN_AIR_OUTDOOR type
  
- **`bool isPro4_2(void)`**
  - Returns true if board is DIY_PRO_INDOOR_V4_2 type
  
- **`bool isPro3_3(void)`**
  - Returns true if board is DIY_PRO_INDOOR_V3_3 type
  
- **`bool isBasic(void)`**
  - Returns true if board is DIY_BASIC type

### Utility Functions
- **`double round2(double value)`**
  - Rounds double value to 2 decimal places

---

## 2. Sensor Classes

### 2.1 PMS5003/PMS5003T (Particulate Matter Sensors)

**Files**: `src/PMS/PMS.h`, `src/PMS/PMS5003.h`, `src/PMS/PMS5003T.h`

Handles Plantower PMS5003 and PMS5003T particulate matter sensors.

#### PMSBase (Base Class) Functions
- **`bool begin(Stream *stream)`**
  - Initializes sensor with UART stream
  
- **`void readPackage(Stream *stream)`**
  - Reads data package from sensor stream
  
- **`void updateFailCount(void)`** / **`void resetFailCount(void)`**
  - Manages sensor failure counting for reliability
  
- **`bool connected(void)`**
  - Returns sensor connection status

#### Particulate Matter Data Functions
- **`uint16_t getRaw0_1(void)`** / **`uint16_t getPM0_1(void)`**
  - PM1.0 readings (raw/atmospheric environment)
  
- **`uint16_t getRaw2_5(void)`** / **`uint16_t getPM2_5(void)`**
  - PM2.5 readings (raw/atmospheric environment)
  
- **`uint16_t getRaw10(void)`** / **`uint16_t getPM10(void)`**
  - PM10 readings (raw/atmospheric environment)

#### Particle Count Functions
- **`uint16_t getCount0_3(void)`** through **`uint16_t getCount10(void)`**
  - Particle counts for different size categories (0.3μm, 0.5μm, 1.0μm, 2.5μm, 5.0μm, 10μm)

#### PMS5003T Extended Functions (Temperature/Humidity)
- **`int16_t getTemp(void)`**
  - Temperature reading from PMS5003T sensor
  
- **`uint16_t getHum(void)`**
  - Humidity reading from PMS5003T sensor
  
- **`uint8_t getFirmwareVersion(void)`**
  - Sensor firmware version
  
- **`uint8_t getErrorCode(void)`**
  - Last error code from sensor

#### Correction Functions
- **`int pm25ToAQI(int pm02)`**
  - Converts PM2.5 to Air Quality Index
  
- **`float slrCorrection(float pm25, float pm003Count, float scalingFactor, float intercept)`**
  - Simple Linear Regression correction for PM2.5
  
- **`float compensate(float pm25, float humidity)`**
  - Humidity compensation for PM2.5 readings

### 2.2 S8 (CO2 Sensor)

**Files**: `src/S8/S8.h`, `src/S8/S8.cpp`

Handles SenseAir S8 CO2 sensor via Modbus protocol.

#### Initialization Functions
- **`bool begin(void)`** / **`bool begin(Stream *_serialDebug)`** / **`bool begin(HardwareSerial &serial)`**
  - Initialize sensor with optional debug stream
  
- **`void end(void)`**
  - Cleanup sensor resources

#### Data Reading Functions
- **`int16_t getCo2(void)`**
  - Returns CO2 concentration in ppm
  
- **`int getAbcPeriod(void)`**
  - Returns Automatic Baseline Correction period in hours

#### Calibration Functions
- **`bool setBaselineCalibration(void)`**
  - Triggers manual baseline calibration (400ppm)
  
- **`bool isBaseLineCalibrationDone(void)`**
  - Checks if baseline calibration is complete
  
- **`bool setAbcPeriod(int hours)`**
  - Sets Automatic Baseline Correction period (max 200 days)

#### Low-Level Modbus Functions
- **`void uartWriteBytes(uint8_t size)`**
  - Sends bytes to sensor via UART
  
- **`uint8_t uartReadBytes(uint8_t max_bytes, uint32_t timeout_seconds)`**
  - Reads response bytes from sensor
  
- **`bool validResponse(uint8_t func, uint8_t nb)`**
  - Validates Modbus response
  
- **`void sendCommand(uint8_t func, uint16_t reg, uint16_t value)`**
  - Sends Modbus command to sensor

### 2.3 Sht (Temperature/Humidity Sensor)

**Files**: `src/Sht/Sht.h`, `src/Sht/Sht.cpp`

Handles Sensirion SHT3x and SHT4x temperature/humidity sensors.

#### Initialization Functions
- **`bool begin(TwoWire &wire)`** / **`bool begin(TwoWire &wire, Stream &debugStream)`**
  - Initialize sensor with I2C interface
  
- **`void end(void)`**
  - Cleanup sensor resources

#### Data Reading Functions
- **`bool measure(void)`**
  - Triggers new measurement
  
- **`float getTemperature(void)`**
  - Returns temperature in Celsius
  
- **`float getRelativeHumidity(void)`**
  - Returns relative humidity percentage

### 2.4 Sgp41 (TVOC/NOx Sensor)

**Files**: `src/Sgp41/Sgp41.h`, `src/Sgp41/Sgp41.cpp`

Handles Sensirion SGP41 volatile organic compound and nitrogen oxide sensor.

#### Initialization Functions
- **`bool begin(TwoWire &wire)`** / **`bool begin(TwoWire &wire, Stream &stream)`**
  - Initialize sensor with I2C interface
  
- **`void end(void)`**
  - Cleanup sensor resources

#### Task Management Functions (ESP32)
- **`void pause()`** / **`void resume()`**
  - Pause/resume sensor reading task
  
- **`void _handle(void)`**
  - Main sensor handling task function

#### Data Reading Functions
- **`int getTvocIndex(void)`** / **`int getTvocRaw(void)`**
  - TVOC index value and raw sensor data
  
- **`int getNoxIndex(void)`** / **`int getNoxRaw(void)`**
  - NOx index value and raw sensor data

#### Calibration Functions
- **`void setCompensationTemperatureHumidity(float temp, float hum)`**
  - Sets temperature/humidity compensation values
  
- **`void setNoxLearningOffset(int offset)`** / **`void setTvocLearningOffset(int offset)`**
  - Sets learning offsets for calibration
  
- **`int getNoxLearningOffset(void)`** / **`int getTvocLearningOffset(void)`**
  - Gets current learning offset values

---

## 3. Hardware Interface Classes

### 3.1 Display (OLED Display)

**Files**: `src/Display/Display.h`, `src/Display/Display.cpp`

Manages OLED display functionality.

#### Initialization Functions
- **`void begin(TwoWire &wire)`** / **`void begin(TwoWire &wire, Stream &debugStream)`**
  - Initialize display with I2C interface
  
- **`void end(void)`**
  - Cleanup display resources

#### Display Control Functions
- **`void clear(void)`**
  - Clear display buffer
  
- **`void show()`**
  - Update display with buffer contents
  
- **`void setContrast(uint8_t value)`**
  - Set display brightness/contrast
  
- **`void invertDisplay(uint8_t i)`**
  - Invert display colors

#### Drawing Functions
- **`void drawPixel(int16_t x, int16_t y, uint16_t color)`**
  - Draw single pixel
  
- **`void drawLine(int x0, int y0, int x1, int y1, uint16_t color)`**
  - Draw line between two points
  
- **`void drawCircle(int x, int y, int r, uint16_t color)`**
  - Draw circle
  
- **`void drawRect(int x0, int y0, int x1, int y1, uint16_t color)`**
  - Draw rectangle
  
- **`void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color)`**
  - Draw bitmap image

#### Text Functions
- **`void setTextSize(int size)`**
  - Set text size multiplier
  
- **`void setCursor(int16_t x, int16_t y)`**
  - Set text cursor position
  
- **`void setTextColor(uint16_t color)`** / **`void setTextColor(uint16_t foreGroundColor, uint16_t backGroundColor)`**
  - Set text colors
  
- **`void setText(String text)`** / **`void setText(const char text[])`**
  - Display text at current cursor position
  
- **`void setRotation(uint8_t r)`**
  - Set display rotation

### 3.2 PushButton

**Files**: `src/Main/PushButton.h`, `src/Main/PushButton.cpp`

Handles push button input with debouncing.

#### Initialization Functions
- **`void begin(void)`** / **`void begin(Stream &debugStream)`**
  - Initialize button with optional debug stream

#### Button State Functions
- **`int getState(void)`**
  - Returns current button state (BUTTON_PRESSED, BUTTON_RELEASED, etc.)
  
- **`bool isPressed(void)`**
  - Returns true if button is currently pressed

### 3.3 StatusLed

**Files**: `src/Main/StatusLed.h`, `src/Main/StatusLed.cpp`

Controls single status LED.

#### Initialization Functions
- **`void begin(void)`** / **`void begin(Stream &debugStream)`**
  - Initialize LED with optional debug stream
  
- **`void end(void)`**
  - Cleanup LED resources

#### LED Control Functions
- **`void setOn(void)`** / **`void setOff(void)`**
  - Turn LED on/off
  
- **`void setToggle(void)`**
  - Toggle LED state

### 3.4 LedBar (RGB LED Array)

**Files**: `src/Main/LedBar.h`, `src/Main/LedBar.cpp`

Controls RGB LED bar for status indication.

#### Initialization Functions
- **`void begin(void)`** / **`void begin(Stream &debugStream)`**
  - Initialize LED bar with optional debug stream

#### LED Control Functions
- **`void setColor(uint8_t red, uint8_t green, uint8_t blue, int ledNum)`**
  - Set color for specific LED
  
- **`void setColor(uint8_t red, uint8_t green, uint8_t blue)`**
  - Set color for all LEDs
  
- **`void setBrightness(uint8_t brightness)`**
  - Set overall brightness (0-255)
  
- **`void show(void)`**
  - Update LED display
  
- **`void clear(void)`**
  - Turn off all LEDs
  
- **`void setEnable(bool enable)`**
  - Enable/disable LED bar
  
- **`bool isEnabled(void)`**
  - Check if LED bar is enabled

### 3.5 HardwareWatchdog

**Files**: `src/Main/HardwareWatchdog.h`, `src/Main/HardwareWatchdog.cpp`

Manages external hardware watchdog timer.

#### Initialization Functions
- **`void begin(void)`** / **`void begin(Stream &debugStream)`**
  - Initialize watchdog with optional debug stream

#### Watchdog Functions
- **`void reset(void)`**
  - Reset/feed the watchdog timer

---

## 4. Application Management Classes

### 4.1 Configuration

**Files**: `src/AgConfigure.h`, `src/AgConfigure.cpp`

Manages device configuration storage and cloud synchronization.

#### Initialization Functions
- **`bool begin(void)`**
  - Initialize configuration system
  
- **`void setAirGradient(AirGradient *ag)`**
  - Set AirGradient instance reference

#### Configuration Parsing Functions
- **`bool parse(String data, bool isLocal)`**
  - Parse configuration JSON data
  
- **`String toString(void)`** / **`String toString(AgFirmwareMode fwMode)`**
  - Convert configuration to JSON string

#### Configuration Access Functions
- **`bool isTemperatureUnitInF(void)`**
  - Check if temperature unit is Fahrenheit
  
- **`String getCountry(void)`**
  - Get configured country code
  
- **`bool isPmStandardInUSAQI(void)`**
  - Check if PM standard is US AQI
  
- **`int getCO2CalibrationAbcDays(void)`**
  - Get CO2 ABC calibration period
  
- **`LedBarMode getLedBarMode(void)`** / **`String getLedBarModeName(void)`**
  - Get LED bar mode configuration
  
- **`bool getDisplayMode(void)`**
  - Get display mode setting
  
- **`String getMqttBrokerUri(void)`**
  - Get MQTT broker URI
  
- **`String getHttpDomain(void)`**
  - Get custom HTTP domain
  
- **`bool isPostDataToAirGradient(void)`**
  - Check if data posting to AirGradient cloud is enabled
  
- **`ConfigurationControl getConfigurationControl(void)`**
  - Get configuration control mode (local/cloud/both)

#### Calibration Request Functions
- **`bool isCo2CalibrationRequested(void)`**
  - Check if CO2 calibration is requested
  
- **`bool isLedBarTestRequested(void)`**
  - Check if LED bar test is requested

#### Learning Offset Functions
- **`bool noxLearnOffsetChanged(void)`** / **`bool tvocLearnOffsetChanged(void)`**
  - Check if learning offsets have changed
  
- **`int getTvocLearningOffset(void)`** / **`int getNoxLearningOffset(void)`**
  - Get current learning offset values

#### WiFi Configuration Functions
- **`String wifiSSID(void)`** / **`String wifiPass(void)`**
  - Get WiFi credentials

#### Brightness Control Functions
- **`bool isLedBarBrightnessChanged(void)`** / **`int getLedBarBrightness(void)`**
  - LED bar brightness management
  
- **`bool isDisplayBrightnessChanged(void)`** / **`int getDisplayBrightness(void)`**
  - Display brightness management

#### Mode Control Functions
- **`bool isOfflineMode(void)`**
  - Check if device is in offline mode
  
- **`void setOfflineMode(bool offline)`** / **`void setOfflineModeWithoutSave(bool offline)`**
  - Set offline mode with/without saving
  
- **`bool isCloudConnectionDisabled(void)`**
  - Check if cloud connection is disabled
  
- **`void setDisableCloudConnection(bool disable)`**
  - Disable/enable cloud connection

#### Correction Functions
- **`bool isPMCorrectionChanged(void)`** / **`bool isPMCorrectionEnabled(void)`**
  - PM correction status functions
  
- **`PMCorrection getPMCorrection(void)`**
  - Get PM correction configuration
  
- **`TempHumCorrection getTempCorrection(void)`** / **`TempHumCorrection getHumCorrection(void)`**
  - Get temperature/humidity correction configurations

### 4.2 Measurements

**Files**: `src/AgValue.h`, `src/AgValue.cpp`

Manages sensor data collection, averaging, and validation.

#### Initialization Functions
- **`void setAirGradient(AirGradient *ag)`**
  - Set AirGradient instance reference
  
- **`void setDebug(bool debug)`**
  - Enable/disable debug output for measurements

#### Data Update Functions
- **`bool update(MeasurementType type, int val, int ch = 1)`**
  - Update measurement with integer value
  
- **`bool update(MeasurementType type, float val, int ch = 1)`**
  - Update measurement with float value
  
- **`void maxPeriod(MeasurementType, int max)`**
  - Set maximum averaging period for measurement type

#### Data Retrieval Functions
- **`int get(MeasurementType type, int ch = 1)`**
  - Get latest integer measurement value
  
- **`float getFloat(MeasurementType type, int ch = 1)`**
  - Get latest float measurement value
  
- **`float getAverage(MeasurementType type, int ch = 1)`**
  - Get moving average value

#### Corrected Value Functions
- **`float getCorrectedTempHum(MeasurementType type, int ch = 1, bool forceCorrection = false)`**
  - Get temperature/humidity with correction applied
  
- **`float getCorrectedPM25(bool useAvg = false, int ch = 1, bool forceCorrection = false)`**
  - Get PM2.5 with correction algorithm applied

#### Data Export Functions
- **`String toString(bool localServer, AgFirmwareMode fwMode, int rssi)`**
  - Convert measurements to JSON string
  
- **`Measures getMeasures()`**
  - Get complete measurement structure
  
- **`std::string buildMeasuresPayload(Measures &measures)`**
  - Build measurement payload for transmission

#### Boot Count Functions
- **`int bootCount()`** / **`void setBootCount(int bootCount)`**
  - Manage device boot counter

#### Debug Functions
- **`void printCurrentAverage()`**
  - Print current average values to debug stream

### 4.3 StateMachine

**Files**: `src/AgStateMachine.h`, `src/AgStateMachine.cpp`

Manages device state and display/LED behavior.

#### Initialization Functions
- **`void setAirGradient(AirGradient* ag)`**
  - Set AirGradient instance reference

#### Display Management Functions
- **`void displayHandle(AgStateMachineState state)`** / **`void displayHandle(void)`**
  - Handle display updates based on state
  
- **`void displaySetAddToDashBoard(void)`** / **`void displayClearAddToDashBoard(void)`**
  - Show/hide "Add to Dashboard" message
  
- **`void displayWiFiConnectCountDown(int count)`**
  - Show WiFi connection countdown
  
- **`void setDisplayState(AgStateMachineState state)`**
  - Set current display state
  
- **`AgStateMachineState getDisplayState(void)`**
  - Get current display state

#### LED Management Functions
- **`void ledAnimationInit(void)`**
  - Initialize LED animations
  
- **`void handleLeds(AgStateMachineState state)`** / **`void handleLeds(void)`**
  - Handle LED updates based on state
  
- **`AgStateMachineState getLedState(void)`**
  - Get current LED state

#### Calibration Functions
- **`void executeCo2Calibration(void)`**
  - Execute CO2 calibration process

#### LED Bar Test Functions
- **`void executeLedBarTest(void)`**
  - Execute LED bar test sequence
  
- **`void executeLedBarPowerUpTest(void)`**
  - Execute power-up LED test

### 4.4 WifiConnector

**Files**: `src/AgWiFiConnector.h`, `src/AgWiFiConnector.cpp`

Manages WiFi connection and configuration portal.

#### Initialization Functions
- **`void setAirGradient(AirGradient *ag)`**
  - Set AirGradient instance reference

#### Connection Management Functions
- **`bool connect(void)`**
  - Initiate WiFi connection
  
- **`void disconnect(void)`**
  - Disconnect from WiFi
  
- **`bool isConnected(void)`**
  - Check WiFi connection status
  
- **`int RSSI(void)`**
  - Get WiFi signal strength
  
- **`String localIpStr(void)`**
  - Get local IP address as string

#### Configuration Functions
- **`void handle(void)`**
  - Process WiFi manager events
  
- **`bool hasConfigurated(void)`**
  - Check if WiFi is configured
  
- **`bool isConfigurePorttalTimeout(void)`**
  - Check if configuration portal timed out
  
- **`void reset(void)`**
  - Reset WiFi configuration
  
- **`void setDefault(void)`**
  - Set default SSID/password

#### Callback Functions
- **`void _wifiApCallback(void)`**
  - Access point callback
  
- **`void _wifiSaveConfig(void)`**
  - Save configuration callback
  
- **`void _wifiSaveParamCallback(void)`**
  - Save parameters callback
  
- **`bool _wifiConfigPortalActive(void)`**
  - Check if config portal is active
  
- **`void _wifiTimeoutCallback(void)`**
  - Timeout callback
  
- **`void _wifiProcess()`**
  - Main WiFi processing function

### 4.5 AgApiClient

**Files**: `src/AgApiClient.h`, `src/AgApiClient.cpp`

Handles communication with AirGradient cloud services.

#### Initialization Functions
- **`void begin(void)`**
  - Initialize API client
  
- **`void setAirGradient(AirGradient *ag)`**
  - Set AirGradient instance reference

#### Configuration Functions
- **`bool fetchServerConfiguration(void)`**
  - Fetch configuration from AirGradient server
  
- **`bool isFetchConfigurationFailed(void)`**
  - Check if configuration fetch failed
  
- **`void resetFetchConfigurationStatus(void)`**
  - Reset fetch status

#### Data Transmission Functions
- **`bool postToServer(String data)`**
  - Post measurement data to server
  
- **`bool isPostToServerFailed(void)`**
  - Check if data posting failed
  
- **`bool sendPing(int rssi, int bootCount)`**
  - Send ping/heartbeat to server

#### Server Configuration Functions
- **`String getApiRoot() const`** / **`void setApiRoot(const String &apiRoot)`**
  - Get/set API root URL
  
- **`void setTimeout(uint16_t timeoutMs)`**
  - Set HTTP timeout
  
- **`bool isNotAvailableOnDashboard(void)`**
  - Check if device is not configured on dashboard

### 4.6 MqttClient

**Files**: `src/MqttClient.h`, `src/MqttClient.cpp`

Manages MQTT communication for data publishing.

#### Initialization Functions
- **`bool begin(String uri)`**
  - Initialize MQTT client with broker URI
  
- **`void end(void)`**
  - Cleanup MQTT resources

#### Connection Management Functions
- **`bool isConnected(void)`**
  - Check MQTT connection status
  
- **`int getConnectionFailedCount(void)`**
  - Get connection failure count
  
- **`bool isCurrentUri(String &uri)`**
  - Check if URI matches current configuration

#### Data Publishing Functions
- **`bool publish(const char* topic, const char* payload, int len)`**
  - Publish data to MQTT topic

#### ESP8266 Specific Functions
- **`bool connect(String id)`**
  - Connect with client ID
  
- **`void handle(void)`**
  - Process MQTT events

#### Internal Functions
- **`void _updateConnected(bool connected)`**
  - Update connection status

---

## 5. Utility Classes

### 5.1 OpenMetrics

**Files**: `examples/OneOpenAir/OpenMetrics.h`, `examples/OneOpenAir/OpenMetrics.cpp`

Provides Prometheus-compatible metrics endpoint.

#### Initialization Functions
- **`void setAirGradient(AirGradient *ag)`**
  - Set AirGradient instance reference

#### Metrics Functions
- **`String getMetrics(void)`**
  - Generate Prometheus metrics format
  
- **`void getMetrics(String &result)`**
  - Generate metrics into provided string

### 5.2 LocalServer

**Files**: `examples/OneOpenAir/LocalServer.h`, `examples/OneOpenAir/LocalServer.cpp`

Implements local HTTP server for configuration and data access.

#### Initialization Functions
- **`void begin(void)`**
  - Start HTTP server
  
- **`void end(void)`**
  - Stop HTTP server

#### Server Functions
- **`void handle(void)`**
  - Process HTTP requests
  
- **`bool isBegin(void)`**
  - Check if server is running

### 5.3 AgSchedule

**Files**: `src/AgSchedule.h`, `src/AgSchedule.cpp`

Simple task scheduler for periodic operations.

#### Constructor
- **`AgSchedule(uint32_t period, void (*callback)(void))`**
  - Create scheduled task with period and callback

#### Scheduling Functions
- **`void run(void)`**
  - Execute callback if period elapsed
  
- **`void update(void)`**
  - Update last execution time
  
- **`void setIntervalMs(uint32_t intervalMs)`**
  - Set execution interval
  
- **`uint32_t getIntervalMs(void)`**
  - Get current interval

---

## 6. Board Definition System

### Files: `src/Main/BoardDef.h`, `src/Main/BoardDef.cpp`

Defines hardware-specific configurations for different board types.

#### Board Information Functions
- **`const BoardDef *getBoardDef(BoardType def)`**
  - Get board definition structure
  
- **`const char *getBoardDefName(BoardType type)`**
  - Get board name string
  
- **`void printBoardDef(Stream *_debug)`**
  - Print board configuration to debug stream

#### Hardware Support Check Functions
- **`bool getBoardDef_I2C_Supported(const BoardDef *bsp)`**
  - Check I2C support
  
- **`int getBoardDef_I2C_SDA(const BoardDef *bsp)`** / **`int getBoardDef_I2C_SCL(const BoardDef *bsp)`**
  - Get I2C pin assignments
  
- **`bool getBoardDef_SW_Supported(const BoardDef *bsp)`**
  - Check switch/button support
  
- **`int getBoardDef_SW_Pin(const BoardDef *bsp)`** / **`int getBoardDef_SW_ActiveLevel(const BoardDef *bsp)`**
  - Get switch pin and active level

#### Watchdog Functions
- **`void AirGradientBspWdgInit(const BoardDef *bsp)`**
  - Initialize hardware watchdog
  
- **`void AirGradientBspWdgFeedBegin(const BoardDef *bsp)`** / **`void AirGradientBspWdgFeedEnd(const BoardDef *bsp)`**
  - Begin/end watchdog feeding sequence

---

## 7. Utility Functions

### Files: `src/Main/utils.h`, `src/Main/utils.cpp`

General utility functions for data validation and processing.

#### Data Validation Functions
- **`static bool isValidTemperature(float value)`**
  - Validate temperature reading range
  
- **`static bool isValidHumidity(float value)`**
  - Validate humidity reading range
  
- **`static bool isValidCO2(int16_t value)`**
  - Validate CO2 reading range
  
- **`static bool isValidPm(int value)`**
  - Validate particulate matter reading
  
- **`static bool isValidPm03Count(int value)`**
  - Validate particle count reading
  
- **`static bool isValidNOx(int value)`** / **`static bool isValidVOC(int value)`**
  - Validate NOx and VOC readings

#### String Processing Functions
- **`String getStringAfter(String data, char separator, int index)`**
  - Extract substring after separator at index
  
- **`String getStringBefore(String data, char separator)`**
  - Extract substring before separator
  
- **`uint32_t calculateUpdateUsTick(uint32_t period)`**
  - Calculate microsecond tick for update period

---

## 8. Application Enums and Constants

### File: `src/App/AppDef.h`

#### State Machine States (`AgStateMachineState`)
- **WiFi Manager States**: `AgStateMachineWiFiManagerMode`, `AgStateMachineWiFiManagerPortalActive`, etc.
- **Connection States**: `AgStateMachineWiFiOkServerConnecting`, `AgStateMachineWiFiOkServerConnected`, etc.
- **Error States**: `AgStateMachineWiFiManagerConnectFailed`, `AgStateMachineWiFiOkServerConnectFailed`, etc.
- **Operation States**: `AgStateMachineNormal`, `AgStateMachineCo2Calibration`, `AgStateMachineLedBarTest`, etc.

#### LED Bar Modes (`LedBarMode`)
- **`LedBarModeOff`**: LED bar disabled
- **`LedBarModePm`**: Show PM2.5 levels
- **`LedBarModeCO2`**: Show CO2 levels

#### Configuration Control (`ConfigurationControl`)
- **`ConfigurationControlLocal`**: Local configuration only
- **`ConfigurationControlCloud`**: Cloud configuration only
- **`ConfigurationControlBoth`**: Both local and cloud

#### Correction Algorithms
- **PM Correction**: `COR_ALGO_PM_NONE`, `COR_ALGO_PM_EPA_2021`, `COR_ALGO_PM_SLR_CUSTOM`
- **Temperature/Humidity Correction**: `COR_ALGO_TEMP_HUM_NONE`, `COR_ALGO_TEMP_HUM_AG_PMS5003T_2024`, `COR_ALGO_TEMP_HUM_SLR_CUSTOM`

#### Firmware Modes (`AgFirmwareMode`)
- **`FW_MODE_I_9PSL`**: ONE_INDOOR (PM, S8, SGP, LED, Display)
- **`FW_MODE_O_1PST`**: Outdoor with PMS5003T, S8, SGP41
- **`FW_MODE_O_1PPT`**: Outdoor with dual PMS5003T, SGP41
- **`FW_MODE_I_42PS`**: DIY_PRO 4.2 (PM, S8, SGP)
- **`FW_MODE_I_33PS`**: DIY_PRO 3.3 (PM, S8, SGP)
- **`FW_MODE_I_BASIC_40PS`**: DIY_BASIC 4.0 (PM, S8)

---

## 9. Example Applications

### 9.1 OneOpenAir.ino
The main application for AirGradient ONE and Open Air devices featuring:
- **Network Management**: WiFi and Cellular connectivity
- **Sensor Integration**: All supported sensors with error handling
- **Data Processing**: Moving averages, corrections, validation
- **Communication**: Local server, MQTT, cloud sync
- **OTA Updates**: Automatic firmware updates
- **State Management**: Comprehensive state machine for all operations

### 9.2 BASIC.ino
Simplified application for DIY Basic devices:
- **Core Sensors**: PM, CO2, Temperature/Humidity
- **WiFi Only**: Single network interface
- **Basic Display**: Simple OLED output
- **Essential Features**: Configuration, data posting, local server

### 9.3 DiyProIndoorV4_2.ino
Advanced DIY Pro application:
- **Full Sensor Suite**: PM, CO2, TVOC/NOx, Temperature/Humidity
- **Enhanced Display**: Larger OLED with more information
- **Button Control**: Configuration and calibration via push button
- **Advanced Features**: All correction algorithms, LED patterns

---

## 10. Configuration and Calibration

### Local HTTP API Endpoints
- **`GET /measures/current`**: Current sensor readings
- **`GET /config`**: Current configuration
- **`PUT /config`**: Update configuration
- **`GET /metrics`**: Prometheus metrics

### Calibration Procedures
- **CO2 Calibration**: Manual baseline (400ppm) or automatic baseline correction (ABC)
- **PM Correction**: EPA 2021 factors, Simple Linear Regression, sensor-specific corrections
- **Temperature/Humidity**: AirGradient standard correction, custom algorithms

### Network Configuration
- **WiFi Setup**: Captive portal with SSID/password entry
- **Cellular Backup**: Automatic failover for outdoor units
- **mDNS Discovery**: `airgradient_{serialnumber}.local`
- **Cloud Integration**: Automatic device registration and configuration sync

---

## 11. Data Flow Architecture

```
Sensors → Measurements → Corrections → Local Storage
    ↓           ↓           ↓              ↓
Display ←   State     →  Network   →  Cloud/MQTT
         Machine       Transmission
```

### Data Processing Pipeline
1. **Raw Sensor Data**: Individual sensor readings
2. **Validation**: Range checking and error detection
3. **Moving Averages**: Smoothing over configurable periods
4. **Corrections**: EPA, SLR, and custom correction algorithms
5. **State Updates**: Display and LED updates based on values
6. **Network Transmission**: Local server, MQTT, and cloud posting

### Error Handling Strategy
- **Sensor Failures**: Consecutive failure counting with automatic recovery
- **Network Issues**: Retry logic with exponential backoff
- **Configuration Errors**: Validation with fallback to defaults
- **Memory Management**: Queue limits and buffer management
- **Watchdog Protection**: Hardware and software watchdog timers

---

## 12. Hardware Abstraction

The library provides complete hardware abstraction through the BoardDef system:

### Supported Platforms
- **ESP8266**: D1 Mini/Wemos boards for DIY versions
- **ESP32-C3**: Custom boards for ONE and Open Air versions

### Pin Mappings
Each board type has specific pin assignments for:
- **UART**: Sensor communication (PMS, S8)
- **I2C**: Display and environmental sensors
- **GPIO**: LEDs, buttons, watchdog control
- **Power**: Module power control for cellular variants

### Platform-Specific Features
- **ESP32**: Hardware task management, dual-core processing
- **ESP8266**: Software task scheduling, single-core optimization
- **Cellular**: LTE module management for outdoor units
- **OTA**: Over-the-air update capabilities

This comprehensive function map provides complete coverage of the AirGradient Arduino library's capabilities, enabling developers to understand and extend the codebase effectively while maintaining the direct, functional approach required for embedded air quality monitoring applications.