# Technical Context: Joy-Con ESP32 Interface

## Development Environment

### Hardware Requirements
- ESP32 microcontroller
- Nintendo Joy-Con controller(s)
- USB connection for programming/debugging

### Software Requirements
- Arduino IDE or Platform IO
- ESP32 board support package
- Bluepad32 library
- Serial monitor (for debugging)

## Dependencies

### Core Libraries
1. **Bluepad32**
   - Version: Latest
   - Purpose: Nintendo Joy-Con communication
   - Key Features:
     - Bluetooth connectivity
     - Controller state management
     - Input processing

### Arduino Framework
- Core ESP32 functions
- Hardware interfaces
- Task management

## Implementation Details

### Button Mapping

```cpp
// Standard Buttons (buttons() bitmask)
0x0001 - A Button
0x0002 - X Button
0x0004 - B Button
0x0008 - Y Button
0x0010 - SL Button
0x0020 - SR Button
0x0040 - R Button
0x0080 - ZR Button
0x0100 - Thumbstick Button

// D-Pad (dpad() bitmask)
0x01 - Up
0x02 - Down
0x08 - Left
0x04 - Right

// System Buttons (miscButtons() bitmask)
0x02 - Home Button
0x04 - Plus Button
```

### Analog Stick Processing

```cpp
// Calibration
const int JOYCON_IDLE_X = -55;  // Neutral X position
const int JOYCON_IDLE_Y = 13;   // Neutral Y position

// Movement Thresholds
const int JOYCON_LEFT_THRESHOLD = -200;
const int JOYCON_RIGHT_THRESHOLD = 150;
const int JOYCON_UP_THRESHOLD = -200;
const int JOYCON_DOWN_THRESHOLD = 200;

// Deadzone
const int DEADZONE_THRESHOLD = 25;
```

### Data Ranges

1. **Analog Stick**
   - X-axis: -511 to 512
   - Y-axis: -511 to 512
   - Practical ranges after calibration:
     - Left: -362 from neutral
     - Right: +360 from neutral
     - Up: -453 from neutral
     - Down: +427 from neutral

2. **Movement Intensity**
   - Range: 0.0 to 1.0
   - Calculated based on distance from neutral position
   - Normalized to maximum observed ranges

## Configuration

### Bluetooth Setup
```cpp
void setup() {
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
}
```

### Controller Management
```cpp
ControllerPtr myControllers[BP32_MAX_GAMEPADS];
```

### Debug Output
```cpp
Serial.begin(115200);
```

## Performance Considerations

### Timing
1. **Main Loop**
   - 150ms delay between updates
   - Balances responsiveness and stability
   ```cpp
   delay(150);
   ```

2. **Watchdog Timer**
   - Requires task yield in main loop
   - Prevents system resets

### Memory Usage
- Controller array: `BP32_MAX_GAMEPADS * sizeof(ControllerPtr)`
- Static button states: 1 byte per button
- Calibration data: 4 bytes per constant

### Processing Overhead
1. **Input Processing**
   - Button state comparisons
   - Analog value calculations
   - Movement detection logic

2. **Bluetooth Communication**
   - Connection management
   - Data packet processing
   - State synchronization

## Error Handling

### Connection Issues
```cpp
if (!foundEmptySlot) {
  Serial.println("CALLBACK: Controller connected, but could not found empty slot");
}
```

### Disconnection Management
```cpp
if (!foundController) {
  Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
}
```

### Input Validation
```cpp
if (abs(calibratedX) < DEADZONE_THRESHOLD && abs(calibratedY) < DEADZONE_THRESHOLD) {
  // Handle neutral position
}
```

## Debugging Tools

### Serial Output Format
```cpp
void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, ...",
    ctl->index(),
    ctl->dpad(),
    ctl->buttons(),
    ctl->axisX(),
    ctl->axisY()
  );
}
```

### Connection Events
```cpp
Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
  ctl->getModelName().c_str(),
  properties.vendor_id,
  properties.product_id
);
```

## Development Guidelines

1. **Code Organization**
   - Separate connection management
   - Structured input processing
   - Clear state tracking

2. **Error Prevention**
   - Range validation
   - State verification
   - Connection checks

3. **Performance Optimization**
   - Efficient state tracking
   - Minimal memory usage
   - Optimized loop timing

4. **Debugging**
   - Comprehensive serial output
   - State monitoring
   - Error reporting

## Testing Procedures

1. **Connection Testing**
   - Multiple connect/disconnect cycles
   - Multiple controller support
   - Recovery from disconnection

2. **Input Testing**
   - Button response verification
   - Analog movement accuracy
   - Diagonal detection
   - Deadzone effectiveness

3. **System Testing**
   - Memory stability
   - Processing efficiency
   - Bluetooth reliability
