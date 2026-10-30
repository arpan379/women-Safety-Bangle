# Women Safety Bangle

A GPS-enabled women's safety wearable developed using **Arduino Nano, SIM900A GSM module, and NEO-6M GPS module**.

The device is designed to provide emergency assistance through an SOS push button. When activated, it retrieves the current GPS coordinates, sends an emergency SMS containing a Google Maps location link to a predefined mobile number, and automatically initiates a call to the same number.

## Features

- **SOS Emergency Button:** Activates the emergency sequence with a single press.
- **GPS Location Tracking:** Retrieves latitude and longitude using the NEO-6M GPS module.
- **Emergency SMS:** Sends an emergency message to a predefined contact.
- **Google Maps Integration:** Generates a clickable Google Maps link using GPS coordinates.
- **Automatic Emergency Call:** Calls the predefined emergency contact after sending the SMS.
- **Rechargeable Battery:** Uses a 3.7V Li-Po battery with a TP4056 charging module.
- **5V Power Distribution:** Uses a 5V boost converter after the TP4056 charging module to provide a common 5V power rail for the Arduino Nano, GPS module, and GSM module.
- **Direct GSM UART Connection:** Connects the GSM module's `TX5V` and `RX5V` pins directly to the Arduino Nano.

## Hardware Components

| Component | Purpose |
|---|---|
| Arduino Nano | Main microcontroller |
| SIM900A GSM module | Emergency SMS and calling |
| NEO-6M GPS module | GPS location tracking |
| 3.7V 5000mAh Li-Po battery | Main power source |
| TP4056 charging module | Battery charging and protection |
| 5V boost converter | Converts the battery/charger output into the system's 5V power rail |
| Push button | Emergency activation |
| Connecting wires | Electrical connections |

**Note:** The 5V boost converter is connected between the TP4056 charging module output and the common 5V power rail. This rail supplies the Arduino Nano, NEO-6M GPS module, and SIM900A GSM module.

## System Architecture

### 1. Power Supply Architecture

The project uses a single rechargeable Li-Po battery connected to a TP4056 charging module. The TP4056 output is connected to a 5V boost converter. The boost converter output forms one common 5V power rail that supplies all three main modules: Arduino Nano, NEO-6M GPS, and SIM900A GSM.

```text
             3.7V Li-Po Battery
                      |
                   TP4056
               Charging Module
                      |
                OUT+ / OUT-
                      |
                5V Boost Converter
                 IN+       IN-
                  |         |
                 OUT+      OUT-
                   |        |
              +----+--------+----+
              |        5V Rail   |
              |         GND      |
          +---+---+-----+-----+---+
          |       |           |
       Arduino  NEO-6M     SIM900A
        Nano     GPS         GSM
         5V      VCC         VCC

       All module grounds share common GND
```

The Arduino Nano, GPS module, and GSM module are powered from the common 5V rail.

### TP4056 Connections

| TP4056 Pin | Connection |
|---|---|
| B+ | Li-Po battery positive |
| B- | Li-Po battery negative |
| OUT+ | 5V boost converter IN+ |
| OUT- | Common ground |

**Note:** The TP4056 is the battery charging module. Its OUT+ and OUT- terminals feed the input of the 5V boost converter.

### 5V Power Rail Connections

The output of the 5V boost converter is distributed in parallel to the 5V input of the Arduino Nano, the VCC pin of the NEO-6M GPS breakout, and the VCC/5V input of the SIM900A GSM board. Connect all module grounds to the common ground rail.

## 2. Common 5V Power Supply

The 5V boost converter is connected directly after the TP4056 charging module and creates the common 5V power rail.

| Connection | Destination |
|---|---|
| TP4056 OUT+ | 5V boost converter IN+ |
| TP4056 OUT- | 5V boost converter IN- |
| Boost converter OUT+ | Common 5V rail |
| Boost converter OUT- | Common GND rail |
| 5V rail | Arduino Nano 5V pin |
| 5V rail | NEO-6M GPS VCC |
| 5V rail | SIM900A GSM board VCC/5V input |

All three modules are powered from this same 5V rail.

## 3. GSM Communication Architecture

The GSM board's `TX5V` and `RX5V` UART pins connect directly to the Arduino Nano.

- Arduino D8 (TX) → GSM `RX5V`.
- GSM `TX5V` → Arduino D7 (RX).

## 4. Circuit Connections

### Arduino Nano to NEO-6M GPS

| NEO-6M GPS | Arduino Nano |
|---|---|
| VCC | Common 5V power rail |
| GND | GND |
| TX | D4 |
| RX | D3 (optional) |

GPS serial communication uses the Arduino SoftwareSerial interface.

### Arduino Nano to SIM900A Direct UART Connection

| Arduino Nano | SIM900A GSM |
|---|---|
| D8 (TX) | RX5V |
| D7 (RX) | TX5V |
| 5V power rail | VCC/5V power input |
| GND | GND |

Connect the GSM module's `TX5V` and `RX5V` pins directly to the corresponding Arduino Nano serial pins.

### Emergency Push Button

| Push Button Terminal | Arduino Nano |
|---|---|
| Terminal 1 | D2 |
| Terminal 2 | GND |

The Arduino uses its internal pull-up resistor:

```cpp
pinMode(2, INPUT_PULLUP);
```

- Button released: HIGH.
- Button pressed: LOW.

## 5. Arduino Pin Configuration

| Arduino Nano Pin | Connected Component | Function |
|---|---|---|
| D2 | Emergency push button | SOS trigger |
| D3 | GPS RX (optional) | GPS configuration data |
| D4 | GPS TX | GPS location data |
| D7 | GSM `TX5V` | GSM serial receive |
| D8 | GSM `RX5V` | GSM serial transmit |
| 5V | Common 5V power rail | Arduino and module supply |
| GND | Common ground | Ground reference |

## 6. Software Requirements

- Arduino IDE
- Arduino Nano
- TinyGPSPlus library by Mikal Hart
- SoftwareSerial library (included with the Arduino AVR core)

### Installing TinyGPSPlus

1. Open Arduino IDE.
2. Navigate to **Sketch → Include Library → Manage Libraries**.
3. Search for `TinyGPSPlus`.
4. Install the library by Mikal Hart.

## 7. Configuration

Before uploading the Arduino program, replace the placeholder phone number in `WomenSafetyBangle.ino`:

```cpp
const char PHONE_NUMBER[] = "+91XXXXXXXXXX";
```

Replace it with the intended emergency contact number, including the country code.

## 8. Working Principle

1. The device remains in standby mode.
2. When the SOS button is pressed, the Arduino starts the emergency sequence.
3. The GPS module is checked for a valid location.
4. The Arduino extracts the latitude and longitude.
5. A Google Maps link is generated using the GPS coordinates.
6. The SIM900A sends an emergency SMS to the predefined mobile number.
7. The SIM900A initiates a call to the same number.
8. The call is terminated after the configured call duration.

If a valid GPS location cannot be obtained within the configured timeout, the SMS indicates that the GPS location is unavailable.

## 9. GSM Module Testing

Before testing the complete system, verify that the SIM900A can communicate with the mobile network.

Use a serial terminal to send these AT commands:

```text
AT
AT+CPIN?
AT+CSQ
AT+CREG?
AT+CMGF=1
```

Expected results:

- `AT` should return `OK`.
- `AT+CPIN?` should indicate that the SIM is ready.
- `AT+CSQ` should provide signal-quality information.
- `AT+CREG?` should indicate network registration.
- `AT+CMGF=1` should enable SMS text mode.

**Network compatibility:** SIM900A supports 2G GSM only. Confirm that a compatible 2G network is available in your operating area. A 4G/5G SIM card does not guarantee 2G network availability.

## 10. Testing Procedure

### GPS Testing

1. Connect the GPS module to the common 5V power rail.
2. Power the system.
3. Place the GPS antenna outdoors with a clear view of the sky.
4. Verify that valid latitude and longitude values are received.

### GSM Testing

1. Insert a working SIM card.
2. Power the SIM900A from the common 5V power rail.
3. Verify network registration.
4. Test SMS sending and calling independently.

### Complete System Testing

1. Charge the battery.
2. Power on the complete circuit.
3. Wait for GPS fix and GSM network registration.
4. Press the SOS button.
5. Verify that the emergency SMS is received.
6. Open the Google Maps link and verify the location.
7. Confirm that the emergency call is initiated.

## 11. Repository Structure

```text
women-safety-bangle/
│
├── README.md
├── LICENSE
├── pin_connection.md
├── BOM
└── WomenSafetyBangle.ino
```

### File Description

| File | Description |
|---|---|
| `README.md` | Project overview, features, hardware requirements, power architecture and instructions. |
| `LICENSE` | MIT License for the project. |
| `pin_connection.md` | Detailed hardware pin connections, power connections and UART wiring. |
| `BOM` | Bill of Materials containing the component list and quantities. |
| `WomenSafetyBangle.ino` | Main Arduino program for GPS tracking, emergency SMS and automatic calling. |

## 12. Limitations

- GPS reception may be unreliable indoors.
- SIM900A requires a compatible 2G GSM network.
- Arduino Nano SoftwareSerial can listen to only one serial port at a time. GPS reception may pause during GSM communication.
- An SMS submission response does not guarantee successful delivery or that the recipient has read the message.
- Battery operating time depends on GSM transmission activity, GPS reception, regulator efficiency and standby current.
- The current implementation is a prototype and does not include certified emergency-response functionality.

## 13. Future Improvements

- Battery voltage monitoring.
- Low-battery warning.
- Vibration motor and buzzer feedback.
- SOS cancellation mechanism.
- Automatic GPS and GSM retry logic.
- Low-power standby operation.
- Compact wearable enclosure.
- Improved antenna placement and GPS reception.

## Safety Disclaimer

This project is an educational electronics and embedded-systems prototype. It is not a certified emergency-response device. Do not depend on it as the sole means of emergency communication. Thoroughly test the complete circuit and keep a working mobile phone available.

## License

This project is released under the MIT License. Refer to the `LICENSE` file for details.

---

**Developed as an electronics and embedded-systems project.**
