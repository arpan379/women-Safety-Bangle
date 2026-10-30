# Women Safety Bangle – Pin Connections and Wiring Guide

This document describes the hardware connections for the Women Safety Bangle using:

- Arduino Nano
- SIM900A GSM module
- NEO-6M GPS module
- 3.7V 5000mAh Li-Po battery
- TP4056 charging module
- 5V boost converter
- Common 5V power rail
- Emergency push button

---

## 1. Power Supply Architecture

The system uses a single rechargeable Li-Po battery connected to a TP4056 charging module. The TP4056 output feeds a 5V boost converter. The boost converter creates a common 5V power rail that supplies the Arduino Nano, NEO-6M GPS module, and SIM900A GSM module.

### Power Distribution Diagram

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

The Arduino Nano, NEO-6M GPS module, and SIM900A GSM module are all supplied by the common 5V rail.

### TP4056 Connections

| TP4056 Pin | Connection |
|---|---|
| B+ | Li-Po battery positive |
| B- | Li-Po battery negative |
| OUT+ | 5V boost converter IN+ |
| OUT- | Common ground |

**Note:** The TP4056 is the battery charging module. Its OUT+ and OUT- terminals feed the input of the 5V boost converter.

---

## 2. 5V Boost Converter and Arduino Power Connections

### TP4056 to 5V Boost Converter

| TP4056 Pin | 5V Boost Converter |
|---|---|
| OUT+ | IN+ |
| OUT- | IN- |

### 5V Power Rail Distribution

| 5V Boost Converter Output | Connection |
|---|---|
| OUT+ | Arduino Nano 5V pin |
| OUT+ | NEO-6M GPS VCC |
| OUT+ | SIM900A GSM VCC/5V input |
| OUT- | Common GND for all modules |

The boost converter output is the common 5V power rail. Connect the module power inputs in parallel to this rail. Connect all grounds together.

| Arduino Nano Pin | Connection |
|---|---|
| 5V | Common 5V power rail |
| GND | Common GND |

## 3. GPS Module Power and Connections

The NEO-6M GPS module is powered from the common 5V power rail.

| NEO-6M Pin | Connection |
|---|---|
| VCC | Common 5V power rail |
| GND | Common GND |
| TX | Arduino D4 |
| RX | Arduino D3 (optional) |

### GPS UART Connections

```text
       NEO-6M GPS             Arduino Nano

          TX  ----------------> D4 (RX)
          RX  <---------------- D3 (TX)
          GND ----------------- GND
          VCC ----------------- Common 5V power rail
```

---

## 4. SIM900A GSM Power and UART Connections

### GSM Power Connection

| SIM900A Pin | Connection |
|---|---|
| VCC/5V power input | Common 5V power rail |
| GND | Common GND |

### Direct UART Signal Connections

Connect the GSM board's 5V UART pins directly to the Arduino Nano.

| SIM900A Pin | Arduino Nano Pin |
|---|---|
| TX5V | D7 (RX) |
| RX5V | D8 (TX) |

```text
       SIM900A GSM             Arduino Nano

          TX5V  ----------------> D7 (RX)
          RX5V  <---------------- D8 (TX)
          VCC   ----------------- Common 5V rail
          GND   ----------------- Common GND
```

## 5. Emergency Push Button Connections

| Push Button Terminal | Arduino Nano |
|---|---|
| Terminal 1 | D2 |
| Terminal 2 | GND |

Use the Arduino's internal pull-up resistor:

```cpp
pinMode(2, INPUT_PULLUP);
```

- Button released: HIGH.
- Button pressed: LOW.

---

## 6. Complete Arduino Pin Configuration

| Arduino Nano Pin | Connected Component | Function |
|---|---|---|
| 5V | Common 5V power rail | Arduino power input |
| GND | Common ground | Ground reference |
| D2 | Emergency push button | SOS trigger |
| D3 | GPS RX (optional) | GPS configuration data |
| D4 | GPS TX | GPS location data |
| D7 | GSM TX5V | GSM serial receive |
| D8 | GSM RX5V | GSM serial transmit |

---

## 7. Common Ground Connections

All modules share the common ground connected to the TP4056 OUT- and the 5V boost converter OUT-.

| Component | Ground Connection |
|---|---|
| Arduino Nano | Common GND |
| NEO-6M GPS | Common GND |
| SIM900A GSM | Common GND |
| 5V boost converter | OUT- |
| TP4056 | OUT- |

## 8. Pre-Power Checklist

- [ ] Battery polarity is correct.
- [ ] TP4056 battery connections are correct.
- [ ] TP4056 protection circuit is appropriately rated.
- [ ] TP4056 OUT+ and OUT- are connected to the boost converter IN+ and IN-.
- [ ] The boost converter output is set to 5V.
- [ ] The 5V rail is connected to the Arduino Nano, NEO-6M GPS, and SIM900A GSM power inputs.
- [ ] All module grounds are connected to common GND.
- [ ] GSM TX5V is connected to Arduino D7 (RX).
- [ ] GSM RX5V is connected to Arduino D8 (TX).
- [ ] No positive power outputs are accidentally shorted together.
- [ ] SIM card is inserted correctly.

---

## 9. Safety Considerations

- Never short-circuit the Li-Po battery.
- Do not charge the battery beyond its specified charging voltage.
- Use a battery with adequate discharge-current capability.
- Ensure the TP4056 protection circuit and wiring can support the complete load.

**Note:** This wiring guide is intended for an educational prototype. Always verify the datasheets of your exact modules before applying power.
