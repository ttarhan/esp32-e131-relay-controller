# esp32-e131-relay-controller

An ESP32-based E1.31 (sACN) receiver that controls a relay based on DMX channel data. This project listens for E1.31 packets over WiFi and toggles a relay output when channel 1 exceeds a threshold value.

## Features

- E1.31/sACN unicast receiver
- WiFi connectivity
- Relay control based on DMX channel 1 threshold (>127 = ON)
- Serial output for monitoring packets and status
- Configurable universe (default: 75)

## Hardware Requirements

- ESP32 board
- Relay module connected to GPIO pin 4
- WiFi network access

## Software Requirements

- [PlatformIO](https://platformio.org/) (VS Code extension or CLI)
- ESP32 platform support

## Setup Instructions

### 1. Install PlatformIO

If you haven't already, install PlatformIO:

**VS Code Extension:**
Install the PlatformIO IDE extension in VS Code

**Or use PlatformIO CLI:**
```bash
brew install platformio # or pip install
```

### 2. Clone and Setup Project

```bash
git clone <repository-url>
cd esp32-e131-relay-controller
```

### 3. Configure WiFi Credentials

1. Copy the credentials template:
   ```bash
   cp src/credentials.h.example src/credentials.h
   ```

2. Edit `src/credentials.h` with your WiFi network details:
   ```cpp
   #ifndef CREDENTIALS_H
   #define CREDENTIALS_H
   
   const char ssid[] = "YOUR_WIFI_SSID";
   const char passphrase[] = "YOUR_WIFI_PASSWORD";
   
   #endif
   ```

### 4. Build and Upload

**Using PlatformIO in VS Code:**
1. Open the project folder in VS Code
2. PlatformIO should automatically detect the project
3. Click the upload button in the PlatformIO toolbar, or use `Ctrl+Alt+U`

**Using PlatformIO CLI:**
```bash
# Build the project
pio run

# Upload to connected ESP32
pio run --target upload

# Monitor serial output
pio device monitor
```

## Configuration

### Hardware Connections

- **Relay Control:** GPIO pin 4 (configurable via `RELAY_PIN` in main.cpp)
- **Power:** Connect ESP32 to appropriate power source
- **Relay:** Connect relay module VCC, GND, and signal to GPIO 4

### Software Configuration

Edit `src/main.cpp` to modify:

- `UNIVERSE`: E1.31 universe to listen on (default: 75)
- `UNIVERSE_COUNT`: Number of universes (default: 1)
- `RELAY_PIN`: GPIO pin for relay control (default: 4)

### Network Configuration

The device connects to your WiFi network using the credentials in `src/credentials.h`. Once connected, it will:

1. Print the assigned IP address to serial console
2. Begin listening for E1.31 unicast packets on the configured universe
3. Control the relay based on channel 1 data (>127 = relay ON)

## Usage

1. Connect your E1.31 lighting controller to the same network
2. Configure your lighting software to send unicast E1.31 data to the ESP32's IP address
3. Set the universe to match the configured value (default: 75)
4. Channel 1 values above 127 will turn the relay ON, values 127 and below turn it OFF

## Serial Monitor Output

The device provides real-time feedback via serial console:

```
Connecting to YourWiFiNetwork....
Connected with IP: 192.168.1.100
Listening for data...
Universe 75 / 512 Channels | Packet#: 1 / Errors: 0 / CH1: 255 / Relay: High
Universe 75 / 512 Channels | Packet#: 2 / Errors: 0 / CH1: 0 / Relay: Low
```

## Troubleshooting

### WiFi Connection Issues
- Verify credentials in `src/credentials.h`
- Check WiFi network availability
- Ensure ESP32 is within range of access point

### E1.31 Reception Issues
- Verify universe number matches your controller
- Check network connectivity
- Ensure firewall allows E1.31 traffic (UDP port 5568)
- Confirm unicast mode is enabled on your controller

### Build Issues
- Ensure PlatformIO is properly installed
- Check that `src/credentials.h` exists (copy from example if missing)
- Verify ESP32 platform is installed: `pio platform install espressif32`

## Development

### Project Structure
```
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp            # Main application code
│   ├── credentials.h       # WiFi credentials (ignored by git)
│   └── credentials.h.example # Template for credentials
├── include/                # Header files
├── lib/                    # Project libraries
└── test/                   # Test files
```

### Dependencies

The project uses the following libraries (automatically installed by PlatformIO):
- `ESPAsyncE131` - E1.31/sACN protocol implementation
- `WiFi` - ESP32 WiFi functionality (built-in)