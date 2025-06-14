#ifndef _APP_DEF_H_
#define _APP_DEF_H_

/**
 * @brief Application state machine state
 *
 */
enum AgStateMachineState {
  /** In WiFi Manger Mode */
  AgStateMachineWiFiManagerMode,

  /** WiFi Manager has connected to mobile phone */
  AgStateMachineWiFiManagerPortalActive,

  /** After SSID and PW entered and OK clicked, connection to WiFI network is
     attempted*/
  AgStateMachineWiFiManagerStaConnecting,

  /** Connecting to WiFi worked */
  AgStateMachineWiFiManagerStaConnected,

  /** Once connected to WiFi an attempt to reach the server is performed */
  AgStateMachineWiFiOkServerConnecting,

  /** Server is reachable, all ﬁne */
  AgStateMachineWiFiOkServerConnected,

  /** =================================== *
   * Exceptions during WIFi Setup         *
   * =================================== **/
  /** Cannot connect to WiFi (e.g. wrong password, WPA Enterprise etc.) */
  AgStateMachineWiFiManagerConnectFailed,

  /** Connected to WiFi but server not reachable, e.g. firewall
     block/whitelisting needed etc. */
  AgStateMachineWiFiOkServerConnectFailed,

  /** Server reachable but sensor not configured correctly*/
  AgStateMachineWiFiOkServerOkSensorConfigFailed,

  /** =================================== *
   * During Normal Operation              *
   * =================================== **/

  /** Connection to WiFi network failed credentials incorrect encryption not
     supported etc. */
  AgStateMachineWiFiLost,

  /** Connected to WiFi network but the server cannot be reached through the
     internet, e.g. blocked by firewall */
  AgStateMachineServerLost,

  /** Server is reachable but there is some conﬁguration issue to be fixed on
     the server side */
  AgStateMachineSensorConfigFailed,

  /** CO2 calibration */
  AgStateMachineCo2Calibration,

  /* LED bar testing */
  AgStateMachineLedBarTest,
  AgStateMachineLedBarPowerUpTest,

  /** OTA perform, show display status */
  AgStateMachineOtaPerform,

  /** LED: Show working state.
   * Display: Show dashboard */
  AgStateMachineNormal,
};

/**
 * @brief RGB LED bar mode for ONE_INDOOR board
 */
enum LedBarMode {
  /** Don't use LED bar */
  LedBarModeOff,

  /** Use LED bar for show PM2.5 value level */
  LedBarModePm,

  /** Use LED bar for show CO2 value level (reversed fuel gauge style) */
  LedBarModeCO2,

  /** Use LED bar for show CO2 value level (original pattern) */
  LedBarModeCO2Classic,
};

enum ConfigurationControl {
  /** Allow set configuration from local over device HTTP server */
  ConfigurationControlLocal,

  /** Allow set configuration from Airgradient cloud */
  ConfigurationControlCloud,

  /** Allow set configuration from Local and Cloud */
  ConfigurationControlBoth
};

enum PMCorrectionAlgorithm {
  COR_ALGO_PM_UNKNOWN, // Unknown algorithm
  COR_ALGO_PM_NONE,    // No PM correction
  COR_ALGO_PM_EPA_2021,
  COR_ALGO_PM_SLR_CUSTOM,
};

// Don't change the order of the enum
enum TempHumCorrectionAlgorithm {
  COR_ALGO_TEMP_HUM_UNKNOWN, // Unknown algorithm
  COR_ALGO_TEMP_HUM_NONE,    // No PM correction
  COR_ALGO_TEMP_HUM_AG_PMS5003T_2024,
  COR_ALGO_TEMP_HUM_SLR_CUSTOM
};

enum AgFirmwareMode {
  FW_MODE_I_9PSL,       /** ONE_INDOOR */
  FW_MODE_O_1PST,       /** PMS5003T, S8 and SGP41 */
  FW_MODE_O_1PPT,       /** PMS5003T_1, PMS5003T_2, SGP41 */
  FW_MODE_O_1PP,        /** PMS5003T_1, PMS5003T_2 */
  FW_MODE_O_1PS,        /** PMS5003T, S8 */
  FW_MODE_O_1P,         /** PMS5003T */
  FW_MODE_I_42PS,       /** DIY_PRO 4.2 */
  FW_MODE_I_33PS,       /** DIY_PRO 3.3 */
  FW_MODE_I_BASIC_40PS, /** DIY_BASIC 4.0 */
};
const char *AgFirmwareModeName(AgFirmwareMode mode);

#endif /** _APP_DEF_H_ */
