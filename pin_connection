# Women Safety Bangle – Pin Connections and Wiring Guide

This document describes the hardware connections for the Women Safety Bangle using:

- Arduino Nano
- SIM900A GSM module
- NEO-6M GPS module
- 3.7V 5000mAh Li-Po battery
- TP4056 charging module
- 4V voltage regulator for SIM900A
- 3.3V-to-5V bidirectional TTL logic-level converter
- Emergency push button

---

## 1. Power Supply Architecture

The system uses a single rechargeable Li-Po battery connected to a TP4056 charging module. The battery output supplies two branches:

- **Arduino and GPS branch:** Battery output supplies the Arduino Nano through its VIN input, subject to the Nano board's specified input-voltage range. The GPS is powered from a suitable regulated supply compatible with its breakout board.
- **GSM branch:** Battery output passes through a dedicated 4V regulator before reaching the SIM900A.

### Power Distribution Diagram

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
               Battery  Battery
               Positive Negative

                 OUT+    OUT-
                   |       |
             +-----+-------+------+
             |                    |
             |                    |
       Arduino Nano          4V Regulator
          VIN                     |
             |                  SIM900A
          Arduino
             |
        GPS Supply*
             |
          NEO-6M

       All grounds connected together
```

*The GPS must receive the voltage specified for its particular breakout board. Do not assume that the raw battery voltage is suitable.*

### TP4056 Connections

| TP4056 Pin | Connection |
|---|---|
| B+ | Li-Po battery positive |
| B- | Li-Po battery negative |
| OUT+ | Arduino supply branch and 4V regulator input |
| OUT- | Common ground |

**Important:**

1. Use a TP4056 module with an appropriately rated battery protection circuit.
2. The TP4056 is a charger, not a voltage regulator.
3. Its output voltage follows the battery voltage and is not a fixed 3.7V.
4. Do not connect the battery directly to the Arduino 5V pin.
5. Check the battery, protection circuit, connectors and wiring current ratings.
6. The TP4056 does not provide power-path management for simultaneous charging and operation.

---

## 2. Arduino Nano Power Connections

| Arduino Nano Pin | Connection |
|---|---|
| VIN | TP4056 OUT+ (only if the battery voltage is within the board's VIN specification) |
| GND | TP4056 OUT- / common GND |
| 5V | Regulated 5V output only, if using an external 5V regulator |

**Important:** A single-cell Li-Po battery varies from approximately 3.0V to 4.2V. It may be too low for reliable operation through the Nano's VIN regulator. Check the actual Nano board specification and verify its operating voltage before choosing this connection. If the voltage is insufficient, a suitable regulator is necessary.

---

## 3. GPS Module Power and Connections

The GPS breakout board must receive its specified supply voltage.

| NEO-6M Pin | Connection |
|---|---|
| VCC | Supply voltage specified by the GPS breakout board |
| GND | Common GND |
| TX | Arduino D4 |
| RX | Arduino D3 (optional) |

If your NEO-6M breakout requires 5V, provide a regulated 5V supply. Do not connect it directly to the variable Li-Po battery unless the board explicitly supports that voltage range.

### GPS UART Connections

```text
       NEO-6M GPS             Arduino Nano

          TX  ----------------> D4 (RX)
          RX  <---------------- D3 (TX)
          GND ----------------- GND
          VCC ----------------- Suitable regulated supply
```

---

## 4. 4V Regulator Connections for SIM900A

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
- Add a 1000µF low-ESR capacitor and a 100nF ceramic capacitor near the GSM power input.

**Important:** Check whether your SIM900A is a bare module or a carrier board. Some carrier boards have onboard regulation and may require a different input voltage.

---

## 5. Arduino Nano to SIM900A Through TTL Logic-Level Converter

The Arduino Nano operates at 5V logic, while the SIM900A UART uses lower-voltage logic.

A 3.3V-to-5V bidirectional logic-level converter is used for UART communication, provided the particular SIM900A board supports 3.3V logic.

### Logic Converter Connections

| Converter Pin | Connection |
|---|---|
| HV | Arduino Nano 5V |
| LV | Regulated 3.3V reference |
| GND | Common GND |

### UART Signal Connections

| Arduino Nano | Converter Side | SIM900A |
|---|---|---|
| D8 (TX) | HV channel input | LV channel output → RXD |
| D7 (RX) | HV channel output | LV channel input ← TXD |
| 5V | HV | — |
| 3.3V reference | LV | — |
| GND | GND | GND |

**UART compatibility note:** The bare SIM900A UART operates at approximately 2.8V logic. Verify that your particular GSM breakout board accepts 3.3V logic on RXD and provides a compatible TXD output. Otherwise, use a suitable 2.8V-compatible interface.

Use a converter designed for UART push-pull signals.

---

## 6. Emergency Push Button Connections

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

## 7. Complete Arduino Pin Configuration

| Arduino Nano Pin | Connected Component | Function |
|---|---|---|
| VIN | Battery output, if within VIN specification | Arduino power input |
| 5V | Regulated 5V reference for logic converter | Logic supply |
| GND | Common ground | Ground reference |
| D2 | Emergency push button | SOS trigger |
| D3 | GPS RX (optional) | GPS configuration data |
| D4 | GPS TX | GPS location data |
| D7 | TTL converter HV output | GSM serial receive |
| D8 | TTL converter HV input | GSM serial transmit |

---

## 8. Common Ground Connections

All modules must share a common ground reference.

| Component | Ground Connection |
|---|---|
| Arduino Nano | Common GND |
| NEO-6M GPS | Common GND |
| SIM900A | Common GND |
| TTL converter | Common GND |
| 4V regulator | Common GND |
| TP4056 | OUT- |

Never connect the positive outputs of different regulators together. Only connect their grounds together.

---

## 9. Pre-Power Checklist

- [ ] Battery polarity is correct.
- [ ] TP4056 battery connections are correct.
- [ ] TP4056 protection circuit is appropriately rated.
- [ ] Arduino VIN input voltage is within its specification.
- [ ] GPS receives its specified supply voltage.
- [ ] GSM regulator output matches the SIM900A board specification.
- [ ] GSM regulator can handle transmission current bursts.
- [ ] All grounds are connected together.
- [ ] Logic converter is connected in the correct direction.
- [ ] SIM900A UART is compatible with the 3.3V logic interface.
- [ ] No positive power outputs are accidentally shorted together.
- [ ] SIM card is inserted correctly.

---

## 10. Safety Considerations

- Never short-circuit the Li-Po battery.
- Do not charge the battery beyond its specified charging voltage.
- Use a battery with adequate discharge-current capability.
- Ensure the TP4056 protection circuit and wiring can support the complete load.
- Do not power the SIM900A from the Arduino Nano.
- Use short, sufficiently thick wires for GSM power.
- Verify the exact input-voltage requirements of every breakout board before connecting power.

**Note:** This wiring guide is intended for an educational prototype. Always verify the datasheets of your exact modules before applying power.
