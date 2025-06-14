#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Joy-Con calibration values (based on your actual idle readings)
const int JOYCON_IDLE_X = -55;
const int JOYCON_IDLE_Y = 13;
const int DEADZONE_THRESHOLD = 25;

// Joy-Con movement ranges (based on your actual readings)
const int JOYCON_LEFT_THRESHOLD = -200;   // Left: -417
const int JOYCON_RIGHT_THRESHOLD = 150;   // Right: 305
const int JOYCON_UP_THRESHOLD = -200;     // Up: -440
const int JOYCON_DOWN_THRESHOLD = 200;    // Down: 440

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
      }
    }

    if (!foundEmptySlot) {
      Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

    if (!foundController) {
      Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

// ========= SEE CONTROLLER VALUES IN SERIAL MONITOR ========= //

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
  "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
  "misc: 0x%02x, gyro x:%6d y:%6d z:%6d, accel x:%6d y:%6d z:%6d\n",
  ctl->index(),        // Controller Index
  ctl->dpad(),         // D-pad
  ctl->buttons(),      // bitmask of pressed buttons
  ctl->axisX(),        // (-511 - 512) left X Axis
  ctl->axisY(),        // (-511 - 512) left Y axis
  ctl->axisRX(),       // (-511 - 512) right X axis
  ctl->axisRY(),       // (-511 - 512) right Y axis
  ctl->brake(),        // (0 - 1023): brake button
  ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
  ctl->miscButtons(),  // bitmask of pressed "misc" buttons
  ctl->gyroX(),        // Gyro X
  ctl->gyroY(),        // Gyro Y
  ctl->gyroZ(),        // Gyro Z
  ctl->accelX(),       // Accelerometer X
  ctl->accelY(),       // Accelerometer Y
  ctl->accelZ()        // Accelerometer Z
  );
}

// ========= GAME CONTROLLER ACTIONS SECTION ========= //

void processGamepad(ControllerPtr ctl) {
  // There are different ways to query whether a button is pressed.
  // By query each button individually:
  //  a(), b(), x(), y(), l1(), etc...
 
  //== Joy-Con A button = 0x0001 ==//
  static bool prevAButton = false;
  bool currentAButton = (ctl->buttons() & 0x0001);
  if (currentAButton && !prevAButton) {
    Serial.println("Button: A PRESSED");
    // code for when A button is pushed
  }
  if (!currentAButton && prevAButton) {
    Serial.println("Button: A RELEASED");
    // code for when A button is released
  }
  prevAButton = currentAButton;

  //== Joy-Con X button = 0x0002 ==//
  static bool prevXButton = false;
  bool currentXButton = (ctl->buttons() & 0x0002);
  if (currentXButton && !prevXButton) {
    Serial.println("Button: X PRESSED");
    // code for when X button is pushed
  }
  if (!currentXButton && prevXButton) {
    Serial.println("Button: X RELEASED");
    // code for when X button is released
  }
  prevXButton = currentXButton;

  //== Joy-Con B button = 0x0004 ==//
  static bool prevBButton = false;
  bool currentBButton = (ctl->buttons() & 0x0004);
  if (currentBButton && !prevBButton) {
    Serial.println("Button: B PRESSED");
    // code for when B button is pushed
  }
  if (!currentBButton && prevBButton) {
    Serial.println("Button: B RELEASED");
    // code for when B button is released
  }
  prevBButton = currentBButton;

  //== Joy-Con Y button = 0x0008 ==//
  static bool prevYButton = false;
  bool currentYButton = (ctl->buttons() & 0x0008);
  if (currentYButton && !prevYButton) {
    Serial.println("Button: Y PRESSED");
    // code for when Y button is pushed
  }
  if (!currentYButton && prevYButton) {
    Serial.println("Button: Y RELEASED");
    // code for when Y button is released
  }
  prevYButton = currentYButton;

  //== Joy-Con Dpad UP button = 0x01 ==//
  static bool prevDpadUp = false;
  bool currentDpadUp = (ctl->dpad() & 0x01);
  if (currentDpadUp && !prevDpadUp) {
    Serial.println("D-pad: UP PRESSED");
    // code for when dpad up button is pushed
  }
  if (!currentDpadUp && prevDpadUp) {
    Serial.println("D-pad: UP RELEASED");
    // code for when dpad up button is released
  }
  prevDpadUp = currentDpadUp;

  //== Joy-Con Dpad DOWN button = 0x02 ==//
  static bool prevDpadDown = false;
  bool currentDpadDown = (ctl->dpad() & 0x02);
  if (currentDpadDown && !prevDpadDown) {
    Serial.println("D-pad: DOWN PRESSED");
    // code for when dpad down button is pushed
  }
  if (!currentDpadDown && prevDpadDown) {
    Serial.println("D-pad: DOWN RELEASED");
    // code for when dpad down button is released
  }
  prevDpadDown = currentDpadDown;

  //== Joy-Con Dpad LEFT button = 0x08 ==//
  static bool prevDpadLeft = false;
  bool currentDpadLeft = (ctl->dpad() & 0x08);
  if (currentDpadLeft && !prevDpadLeft) {
    Serial.println("D-pad: LEFT PRESSED");
    // code for when dpad left button is pushed
  }
  if (!currentDpadLeft && prevDpadLeft) {
    Serial.println("D-pad: LEFT RELEASED");
    // code for when dpad left button is released
  }
  prevDpadLeft = currentDpadLeft;

  //== Joy-Con Dpad RIGHT button = 0x04 ==//
  static bool prevDpadRight = false;
  bool currentDpadRight = (ctl->dpad() & 0x04);
  if (currentDpadRight && !prevDpadRight) {
    Serial.println("D-pad: RIGHT PRESSED");
    // code for when dpad right button is pushed
  }
  if (!currentDpadRight && prevDpadRight) {
    Serial.println("D-pad: RIGHT RELEASED");
    // code for when dpad right button is released
  }
  prevDpadRight = currentDpadRight;

  //== Joy-Con SL button = 0x0010 ==//
  static bool prevSLButton = false;
  bool currentSLButton = (ctl->buttons() & 0x0010);
  if (currentSLButton && !prevSLButton) {
    Serial.println("Button: SL PRESSED");
    // code for when SL button is pushed
  }
  if (!currentSLButton && prevSLButton) {
    Serial.println("Button: SL RELEASED");
    // code for when SL button is released
  }
  prevSLButton = currentSLButton;

  //== Joy-Con SR button = 0x0020 ==//
  static bool prevSRButton = false;
  bool currentSRButton = (ctl->buttons() & 0x0020);
  if (currentSRButton && !prevSRButton) {
    Serial.println("Button: SR PRESSED");
    // code for when SR button is pushed
  }
  if (!currentSRButton && prevSRButton) {
    Serial.println("Button: SR RELEASED");
    // code for when SR button is released
  }
  prevSRButton = currentSRButton;

  //== Joy-Con R button = 0x0040 ==//
  static bool prevRButton = false;
  bool currentRButton = (ctl->buttons() & 0x0040);
  if (currentRButton && !prevRButton) {
    Serial.println("Button: R PRESSED");
    // code for when R button is pushed
  }
  if (!currentRButton && prevRButton) {
    Serial.println("Button: R RELEASED");
    // code for when R button is released
  }
  prevRButton = currentRButton;

  //== Joy-Con ZR button = 0x0080 ==//
  static bool prevZRButton = false;
  bool currentZRButton = (ctl->buttons() & 0x0080);
  if (currentZRButton && !prevZRButton) {
    Serial.println("Button: ZR PRESSED");
    // code for when ZR button is pushed
  }
  if (!currentZRButton && prevZRButton) {
    Serial.println("Button: ZR RELEASED");
    // code for when ZR button is released
  }
  prevZRButton = currentZRButton;

  //== Joy-Con Thumbstick button = 0x0100 ==//
  static bool prevThumbstickButton = false;
  bool currentThumbstickButton = (ctl->buttons() & 0x0100);
  if (currentThumbstickButton && !prevThumbstickButton) {
    Serial.println("Button: THUMBSTICK PRESSED");
    // code for when thumbstick button is pushed
  }
  if (!currentThumbstickButton && prevThumbstickButton) {
    Serial.println("Button: THUMBSTICK RELEASED");
    // code for when thumbstick button is released
  }
  prevThumbstickButton = currentThumbstickButton;

  //== Joy-Con Home button = misc: 0x02 ==//
  static bool prevHomeButton = false;
  bool currentHomeButton = (ctl->miscButtons() & 0x02);
  if (currentHomeButton && !prevHomeButton) {
    Serial.println("Button: HOME PRESSED");
    // code for when Home button is pushed
  }
  if (!currentHomeButton && prevHomeButton) {
    Serial.println("Button: HOME RELEASED");
    // code for when Home button is released
  }
  prevHomeButton = currentHomeButton;

  //== Joy-Con Plus button = misc: 0x04 ==//
  static bool prevPlusButton = false;
  bool currentPlusButton = (ctl->miscButtons() & 0x04);
  if (currentPlusButton && !prevPlusButton) {
    Serial.println("Button: PLUS PRESSED");
    // code for when Plus button is pushed
  }
  if (!currentPlusButton && prevPlusButton) {
    Serial.println("Button: PLUS RELEASED");
    // code for when Plus button is released
  }
  prevPlusButton = currentPlusButton;

  // Calculate calibrated axis values
  int calibratedX = ctl->axisX() - JOYCON_IDLE_X;
  int calibratedY = ctl->axisY() - JOYCON_IDLE_Y;

  //== LEFT JOYSTICK - UP ==//
  // Based on your actual reading: Up: axis L: -50, -440
  if (ctl->axisY() <= JOYCON_UP_THRESHOLD) {
    // code for when left joystick is pushed up
    Serial.println("Joystick: UP");
  }

  //== LEFT JOYSTICK - DOWN ==//
  // Based on your actual reading: Down: axis L: -100, 440
  if (ctl->axisY() >= JOYCON_DOWN_THRESHOLD) {
    // code for when left joystick is pushed down
    Serial.println("Joystick: DOWN");
  }

  //== LEFT JOYSTICK - LEFT ==//
  // Based on your actual reading: Left: axis L: -417, -30
  if (ctl->axisX() <= JOYCON_LEFT_THRESHOLD) {
    // code for when left joystick is pushed left
    Serial.println("Joystick: LEFT");
  }

  //== LEFT JOYSTICK - RIGHT ==//
  // Based on your actual reading: Right: axis L: 305, -29
  if (ctl->axisX() >= JOYCON_RIGHT_THRESHOLD) {
    // code for when left joystick is pushed right
    Serial.println("Joystick: RIGHT");
  }

  //== LEFT JOYSTICK DEADZONE (Joy-Con calibrated) ==//
  // Based on your neutral reading: axis L: -55, 13
  if (abs(calibratedX) < DEADZONE_THRESHOLD && abs(calibratedY) < DEADZONE_THRESHOLD) {
    // code for when left joystick is at idle/neutral position
    // This prevents drift from triggering false inputs
  }

  //== DIAGONAL MOVEMENTS ==//
  // Detect diagonal movements when both axes are outside deadzone
  if (abs(calibratedX) >= DEADZONE_THRESHOLD && abs(calibratedY) >= DEADZONE_THRESHOLD) {
    if (ctl->axisX() <= JOYCON_LEFT_THRESHOLD && ctl->axisY() <= JOYCON_UP_THRESHOLD) {
      Serial.println("Joystick: UP-LEFT");
    }
    else if (ctl->axisX() >= JOYCON_RIGHT_THRESHOLD && ctl->axisY() <= JOYCON_UP_THRESHOLD) {
      Serial.println("Joystick: UP-RIGHT");
    }
    else if (ctl->axisX() <= JOYCON_LEFT_THRESHOLD && ctl->axisY() >= JOYCON_DOWN_THRESHOLD) {
      Serial.println("Joystick: DOWN-LEFT");
    }
    else if (ctl->axisX() >= JOYCON_RIGHT_THRESHOLD && ctl->axisY() >= JOYCON_DOWN_THRESHOLD) {
      Serial.println("Joystick: DOWN-RIGHT");
    }
  }

  //== MOVEMENT INTENSITY (0.0 to 1.0) ==//
  // Calculate movement intensity based on your Joy-Con's actual range
  float xIntensity = 0.0;
  float yIntensity = 0.0;
  
  if (abs(calibratedX) >= DEADZONE_THRESHOLD) {
    // X-axis range: -362 (left) to +360 (right) from neutral
    xIntensity = constrain(abs(calibratedX) / 362.0, 0.0, 1.0);
  }
  
  if (abs(calibratedY) >= DEADZONE_THRESHOLD) {
    // Y-axis range: -453 (up) to +427 (down) from neutral
    yIntensity = constrain(abs(calibratedY) / 453.0, 0.0, 1.0);
  }

  //== RIGHT JOYSTICK - X AXIS ==//
  if (ctl->axisRX()) {
    // code for when right joystick moves along x-axis
    // Note: Right Joy-Con doesn't have analog stick, this might not work
  }

  //== RIGHT JOYSTICK - Y AXIS ==//
  if (ctl->axisRY()) {
  // code for when right joystick moves along y-axis
  // Note: Right Joy-Con doesn't have analog stick, this might not work
  }
  
  dumpGamepad(ctl);
}

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
         processGamepad(myController);
      }
      else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
  // This call fetches all the controllers' data.
  // Call this function in your main loop.
  bool dataUpdated = BP32.update();
  if (dataUpdated)
    processControllers();

    // The main loop must have some kind of "yield to lower priority task" event.
    // Otherwise, the watchdog will get triggered.
    // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
    // Detailed info here:
    // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

    // vTaskDelay(1);
  delay(150);
}