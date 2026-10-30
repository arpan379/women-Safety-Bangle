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
- **Separate Power Regulation:** Provides regulated 5V for the Arduino and GPS, and regulated 4V for the GSM module.
- **UART Logic-Level Conversion:** Uses a 3.3V-to-5V bidirectional logic-level converter for communication between the Arduino and GSM module, subject to UART voltage compatibility.

## Hardware Components

| Component | Purpose |
|---|---|
| Arduino Nano | Main microcontroller |
| SIM900A GSM module | Emergency SMS and calling |
| NEO-6M GPS module | GPS location tracking |
| 3.7V 5000mAh Li-Po battery | Main power source |
| TP4056 charging module | Battery charging and protection (protected version) |
| 5V boost converter | Supplies 5V to Arduino and GPS |
| 4V voltage regulator | Dedicated power supply for SIM900A |
| 3.3V-to-5V bidirectional TTL logic-level converter | UART communication |
| Push button | Emergency activation |
| Connecting wires | Electrical connections |

## System Architecture

### 1. Power Supply Architecture

The project uses one rechargeable Li-Po battery and two separate regulated power branches.

```text
             3.7V Li-Po Battery
                     |
                     |
             TP4056 Charging Module
                     |
                OUT+ and OUT-
                     |
             +-------+-------+
             |               |
             |               |
        5V Boost          4V Regulator
        Converter             |
             |              SIM900A
         +---+---+
         |       |
      Arduino   NEO-6M
       Nano      GPS
       5V        5V
```

**Power distribution:**

- Connect the Li-Po battery to the TP4056 battery terminals (B+ and B-).
- Connect the TP4056 output terminals (OUT+ and OUT-) to the inputs of both voltage regulators.
- The 5V boost converter supplies the Arduino Nano and GPS module.
- The 4V regulator supplies the SIM900A GSM module.
- Connect all circuit grounds together.

**Important power precautions:**

- The TP4056 is a battery charger, not a voltage regulator or power-path management circuit.
- Use a TP4056 module with a suitable battery protection circuit.
- Ensure that the battery, protection circuit, connectors and wiring can handle the GSM module's current bursts.
- The SIM900A can draw current bursts approaching 2A during transmission. A 4V regulator rated around 3A with good transient response is a reasonable design target.
- Set the boost converter output to exactly 5V before connecting the Arduino and GPS.
- Check the input-voltage requirements of your particular GPS and GSM breakout boards.

### 2. Communication Architecture

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
       GSM UART Interface
```

The Arduino Nano operates at 5V logic. The SIM900A uses lower-voltage UART logic. A 3.3V-to-5V bidirectional logic-level converter is used for UART communication, provided the particular SIM900A board is compatible with 3.3V logic.

Both UART signal directions must be level-shifted:

- Arduino TX → converter → SIM900A RX.
- SIM900A TX → converter → Arduino RX.

**UART compatibility note:** The bare SIM900A UART operates at approximately 2.8V. Before connecting a 3.3V-to-5V converter, verify that your SIM900A breakout board accepts 3.3V logic at RXD and provides a compatible TXD output. If it does not, use a suitable 2.8V-compatible interface.

## Circuit Connections

### 1. Battery and Charging Module

| TP4056 Terminal | Connection |
|---|---|
| B+ | Li-Po battery positive |
| B- | Li-Po battery negative |
| OUT+ | Positive input of both voltage regulators |
| OUT- | Common ground |

Use a TP4056 module with an appropriate battery protection circuit. Verify its current rating before connecting the complete circuit.

### 2. 5V Boost Converter

| Boost Converter | Connection |
|---|---|
| Input + | TP4056 OUT+ |
| Input - | TP4056 OUT- |
| Output + | Arduino Nano 5V and GPS VCC |
| Output - | Common GND |

Set the boost converter output to exactly 5V before connecting the Arduino or GPS.

The GPS VCC connection assumes that your NEO-6M breakout board supports a 5V supply. Check the specifications of your particular board.

### 3. 4V Regulator for SIM900A

| Voltage Regulator | Connection |
|---|---|
| Input + | TP4056 OUT+ |
| Input - | TP4056 OUT- |
| Output + | SIM900A power input |
| Output - | Common GND |

The regulator should provide a stable supply with sufficient transient-current capability. Design for GSM transmission peaks approaching 2A; a regulator rated around 3A with good transient response is a reasonable target.

**Note:** This 4V connection is for a SIM900A module or breakout whose specified supply input is approximately 4V. If your carrier board has an onboard regulator or a dedicated 5V input, follow its datasheet instead.

Place suitable bulk and ceramic decoupling capacitors close to the GSM power input.

### 4. Arduino Nano to NEO-6M GPS

| NEO-6M GPS | Arduino Nano |
|---|---|
| VCC | 5V |
| GND | GND |
| TX | D4 |
| RX | D3 (optional) |

GPS serial communication uses the Arduino SoftwareSerial interface.

### 5. Arduino Nano to SIM900A Through TTL Converter

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

Use a bidirectional converter suitable for UART push-pull signals. Do not assume that every generic I2C level shifter is suitable for UART.

**Important:** Verify the SIM900A board's UART voltage compatibility before connecting the 3.3V side. The bare SIM900A UART is approximately 2.8V, so a 3.3V logic converter must only be used if the breakout board supports it.

### 6. Emergency Push Button

| Push Button | Arduino Nano |
|---|---|
| Terminal 1 | D2 |
| Terminal 2 | GND |

The Arduino uses its internal pull-up resistor (`INPUT_PULLUP`), so no external pull-up resistor is required.

## Software Requirements

- Arduino IDE
- Arduino Nano
- TinyGPSPlus library by Mikal Hart
- SoftwareSerial library (included with the Arduino AVR core)

### Installing TinyGPSPlus

1. Open Arduino IDE.
2. Navigate to **Sketch → Include Library → Manage Libraries**.
3. Search for `TinyGPSPlus`.
4. Install the library by Mikal Hart.

## Arduino Pin Configuration

| Function | Arduino Pin |
|---|---|
| Emergency push button | D2 |
| GPS RX | D4 |
| GPS TX | D3 |
| GSM RX | D7 |
| GSM TX | D8 |

## Configuration

Before uploading the Arduino program, replace the placeholder phone number in `src/WomenSafetyBangle.ino`:

```cpp
const char PHONE_NUMBER[] = "+91XXXXXXXXXX";
```

Replace it with the intended emergency contact number, including the country code.

## Working Principle

1. The device remains in standby mode.
2. When the SOS button is pressed, the Arduino starts the emergency sequence.
3. The GPS module is checked for a valid location.
4. The Arduino extracts the latitude and longitude.
5. A Google Maps link is generated using the GPS coordinates.
6. The SIM900A sends an emergency SMS to the predefined mobile number.
7. The SIM900A initiates a call to the same number.
8. The call is terminated after the configured call duration.

If a valid GPS location cannot be obtained within the configured timeout, the SMS indicates that the GPS location is unavailable.

## GSM Module Testing

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

## Testing Procedure

### GPS Testing

1. Connect the GPS module.
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

## Repository Structure

```text
women-safety-bangle/
│
├── README.md
├── LICENSE
├── pin_connection
└── WomenSafetyBangle
```

### File Description

| File | Description |
|---|---|
| `README.md` | Project overview, features, hardware requirements, working principle, power architecture and instructions. |
| `LICENSE` | MIT License for the project. |
| `pin_connection` | Detailed pin connections and power supply connections. |
| `WomenSafetyBangle` | Main Arduino Nano program for GPS tracking, emergency SMS and automatic calling. |

## Limitations

- GPS reception may be unreliable indoors.
- SIM900A requires a compatible 2G GSM network.
- Arduino Nano SoftwareSerial can listen to only one serial port at a time. GPS reception may pause during GSM communication.
- An SMS submission response does not guarantee successful delivery or that the recipient has read the message.
- Battery operating time depends on GSM transmission activity, GPS reception, regulator efficiency and standby current.
- The current implementation is a prototype and does not include certified emergency-response functionality.

## Future Improvements

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
