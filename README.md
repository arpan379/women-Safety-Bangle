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
- **Dedicated GSM Power Supply:** Uses a separate 4V regulator for the SIM900A.
- **UART Logic-Level Conversion:** Uses a 3.3V-to-5V bidirectional logic-level converter for communication between the Arduino and GSM module, subject to UART compatibility.

## Hardware Components

| Component | Purpose |
|---|---|
| Arduino Nano | Main microcontroller |
| SIM900A GSM module | Emergency SMS and calling |
| NEO-6M GPS module | GPS location tracking |
| 3.7V 5000mAh Li-Po battery | Main power source |
| TP4056 charging module | Battery charging and protection |
| 4V voltage regulator | Dedicated power supply for SIM900A |
| 3.3V-to-5V bidirectional TTL logic-level converter | UART communication |
| Push button | Emergency activation |
| Connecting wires | Electrical connections |

**Note:** This design does not include a 5V boost converter. The Arduino and GPS supply must be compatible with the variable Li-Po battery voltage. See the power supply section before assembling the circuit.

## System Architecture

### 1. Power Supply Architecture

The project uses a single rechargeable Li-Po battery connected to a TP4056 charging module. The battery output is distributed into two branches:

- **Arduino and GPS branch:** Supplied from the battery output, provided that the particular Arduino and GPS boards support the battery's full voltage range. A standard 5V Arduino Nano generally needs an appropriate regulated supply.
- **GSM branch:** Supplied through a dedicated 4V regulator before reaching the SIM900A.

```text
              3.7V Li-Po Battery
                      |
                      |
              +---------------+
              |     TP4056    |
              | Charging      |
              | Module        |
              +---------------+
                 B+       B-
                  |        |
                  |        |
                 Battery Connections

                 OUT+    OUT-
                   |       |
             +-----+-------+------+
             |                    |
             |                    |
       Arduino/GPS            4V Regulator
       Supply Branch               |
             |                  SIM900A
       Arduino Nano
             |
          NEO-6M*
          
          Common GND
```

*The Arduino and GPS must receive a voltage within their specified operating ranges. A direct battery connection is permitted only if both boards support the complete Li-Po voltage range.*

### TP4056 Connections

| TP4056 Pin | Connection |
|---|---|
| B+ | Li-Po battery positive |
| B- | Li-Po battery negative |
| OUT+ | Positive supply branch for Arduino/GPS and 4V regulator |
| OUT- | Common ground |

**Important:**

1. Use a TP4056 module with an appropriately rated battery protection circuit.
2. The TP4056 is a charger, not a voltage regulator.
3. Its output voltage follows the battery voltage and is not a fixed 3.7V.
4. Do not connect the battery directly to the Arduino 5V pin.
5. Check the battery, protection circuit, connectors and wiring current ratings.
6. The TP4056 does not provide power-path management for simultaneous charging and operation.

### Arduino Nano Power Considerations

A standard 5V Arduino Nano is not designed to receive a variable 3.0–4.2V Li-Po supply directly on its 5V pin.

Connecting the battery to VIN may also be unsuitable because the onboard regulator typically requires a higher input voltage.

Therefore, before assembly, choose a suitable supply arrangement for your exact Nano board. If it requires regulated 5V, a regulated 5V supply is necessary even though this project version does not include a boost converter.

### GPS Power Considerations

Check the exact NEO-6M breakout-board specifications.

- Some breakout boards accept 5V through an onboard regulator.
- The bare GPS module requires a regulated voltage around 3.3V.
- Do not assume that a 5V-compatible GPS breakout can operate directly from the variable battery voltage.
- Direct battery connection is appropriate only if the board explicitly supports the full battery voltage range.

## 2. GSM Power Supply

The SIM900A requires a dedicated power supply because GSM transmission can cause high current peaks.

| 4V Regulator Pin | Connection |
|---|---|
| IN+ | TP4056 OUT+ |
| IN- | TP4056 OUT- |
| OUT+ | SIM900A power input |
| OUT- | Common GND |

### Power Requirements

- Target supply: approximately 4.0V, subject to the exact SIM900A board specification.
- Design for current bursts approaching 2A.
- A regulator rated around 3A with good transient response is a reasonable design target.
- Use short, sufficiently thick power wires.
- Add a 1000µF low-ESR capacitor and a 100nF ceramic capacitor close to the GSM power input.

**Important:** Check whether your SIM900A is a bare module or a carrier board. Some carrier boards have onboard regulation and may require a different input voltage.

## 3. Communication Architecture

The Arduino Nano operates at 5V logic, while the SIM900A UART uses lower-voltage logic.

A 3.3V-to-5V bidirectional logic-level converter is used for UART communication, provided the particular SIM900A board supports 3.3V logic.

```text
       Arduino Nano
          5V Logic
              |
              |
      3.3V ↔ 5V Bidirectional
       UART Logic-Level Converter
              |
              |
           SIM900A
```

Both UART signal directions must be level-shifted:

- Arduino TX → converter → SIM900A RX.
- SIM900A TX → converter → Arduino RX.

**UART compatibility note:** The bare SIM900A UART operates at approximately 2.8V logic. Verify that your particular GSM breakout board accepts 3.3V logic on RXD and provides a compatible TXD output. Otherwise, use a suitable 2.8V-compatible interface.

Use a converter designed for UART push-pull signals.

## 4. Circuit Connections

### Arduino Nano to NEO-6M GPS

| NEO-6M GPS | Arduino Nano |
|---|---|
| VCC | Suitable regulated supply according to GPS board specification |
| GND | GND |
| TX | D4 |
| RX | D3 (optional) |

GPS serial communication uses the Arduino SoftwareSerial interface.

### Arduino Nano to SIM900A Through TTL Converter

| Arduino Nano | TTL Converter | SIM900A |
|---|---|---|
| 5V | HV | — |
| GND | GND | GND |
| D8 (TX) | High-voltage input | Low-voltage output → RXD |
| D7 (RX) | High-voltage output | Low-voltage input ← TXD |
| — | LV | 3.3V reference |

Connect the converter as follows:

- **HV:** Arduino Nano 5V.
- **LV:** Regulated 3.3V reference.
- **GND:** Common ground.
- **Arduino D8 (TX):** Converter high-voltage input.
- **Converter low-voltage output:** SIM900A RXD.
- **SIM900A TXD:** Converter low-voltage input.
- **Converter high-voltage output:** Arduino D7 (RX).

Verify that the SIM900A breakout board supports 3.3V UART logic before connecting it.

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
| D7 | TTL converter HV output | GSM serial receive |
| D8 | TTL converter HV input | GSM serial transmit |
| 5V | TTL converter HV reference | Logic supply |
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

1. Connect the GPS module using its specified supply voltage.
2. Power the system.
3. Place the GPS antenna outdoors with a clear view of the sky.
4. Verify that valid latitude and longitude values are received.

### GSM Testing

1. Insert a working SIM card.
2. Power the SIM900A using its dedicated regulated supply.
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
