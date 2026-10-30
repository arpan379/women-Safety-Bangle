
/*
========================================================
       WOMEN SAFETY BANGLE
       Arduino Nano + SIM900A + NEO-6M GPS
========================================================

WORKING:
1. Continuously reads GPS coordinates.
2. Detects emergency button press.
3. Waits up to 45 seconds for GPS fix.
4. Sends emergency SMS with Google Maps location.
5. Calls the predefined emergency contact.
6. Disconnects the call after 30 seconds.

LIBRARY:
TinyGPSPlus by Mikal Hart

CONNECTIONS:
Button       -> D2
GPS TX       -> D4
GPS RX       -> D3 (optional)
SIM900A TXD  -> D7 (through 74HCT125)
SIM900A RXD  -> D8 (through voltage divider)

SIM900A UART baud rate: 9600
GPS UART baud rate: 9600
========================================================
*/

#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

// ---------------- PIN CONFIGURATION ----------------

#define BUTTON_PIN 2

// GPS serial: Arduino RX, Arduino TX
SoftwareSerial gpsSerial(4, 3);

// GSM serial: Arduino RX, Arduino TX
SoftwareSerial gsmSerial(7, 8);

TinyGPSPlus gps;

// ---------------- USER SETTINGS ----------------

// Enter the emergency contact number
// Include the country code.

const char PHONE_NUMBER[] = "+91XXXXXXXXXX";

// Maximum time to acquire GPS location
const unsigned long GPS_TIMEOUT = 45000;

// Maximum call duration
const unsigned long CALL_DURATION = 30000;

// Button debounce time
const unsigned long DEBOUNCE_TIME = 50;

// ---------------- GPS VARIABLES ----------------

double latitude = 0.0;
double longitude = 0.0;

bool gpsAvailable = false;

// =================================================
//                 GSM FUNCTIONS
// =================================================

// Wait for a particular response from SIM900A.

bool waitForResponse(
  const char *target,
  unsigned long timeout
) {

  char response[160];
  int index = 0;

  response[0] = '\0';

  unsigned long startTime = millis();

  while (millis() - startTime < timeout) {

    while (gsmSerial.available()) {

      char c = gsmSerial.read();

      if (index < 158) {

        response[index++] = c;
        response[index] = '\0';

      } else {

        // Keep the latest response characters
        memmove(response, response + 1, 158);
        index = 158;
        response[index] = '\0';
      }

      if (strstr(response, target) != NULL) {
        return true;
      }

      if (strstr(response, "ERROR") != NULL) {
        return false;
      }
    }

    delay(1);
  }

  return false;
}


// Send AT command to SIM900A.

void sendAT(
  const char *command,
  unsigned long waitTime = 1000
) {

  gsmSerial.listen();

  while (gsmSerial.available()) {
    gsmSerial.read();
  }

  gsmSerial.println(command);

  unsigned long startTime = millis();

  while (millis() - startTime < waitTime) {

    while (gsmSerial.available()) {

      Serial.write(gsmSerial.read());
    }
  }
}


// =================================================
//                  GPS FUNCTIONS
// =================================================

// Read incoming GPS data.

void updateGPS() {

  gpsSerial.listen();

  while (gpsSerial.available()) {

    gps.encode(gpsSerial.read());
  }

  if (
    gps.location.isValid() &&
    gps.location.age() < 5000
  ) {

    latitude = gps.location.lat();
    longitude = gps.location.lng();

    gpsAvailable = true;
  }
  else {

    gpsAvailable = false;
  }
}


// Acquire a fresh GPS location.

bool getGPSLocation() {

  Serial.println();
  Serial.println("Acquiring GPS location...");

  unsigned long startTime = millis();

  gpsSerial.listen();

  while (millis() - startTime < GPS_TIMEOUT) {

    while (gpsSerial.available()) {

      gps.encode(gpsSerial.read());
    }

    if (
      gps.location.isValid() &&
      gps.location.age() < 5000
    ) {

      latitude = gps.location.lat();
      longitude = gps.location.lng();

      gpsAvailable = true;

      Serial.println("GPS FIX ACQUIRED");

      Serial.print("Latitude: ");
      Serial.println(latitude, 6);

      Serial.print("Longitude: ");
      Serial.println(longitude, 6);

      return true;
    }
  }

  gpsAvailable = false;

  Serial.println("GPS FIX NOT AVAILABLE");

  return false;
}


// =================================================
//                  SMS FUNCTIONS
// =================================================

// Send emergency SMS.

bool sendEmergencySMS() {

  gsmSerial.listen();

  Serial.println();
  Serial.println("Sending Emergency SMS...");

  // Set SMS text mode

  gsmSerial.println("AT+CMGF=1");

  if (!waitForResponse("OK", 3000)) {

    Serial.println("SMS mode configuration failed!");
    return false;
  }

  // Set recipient number

  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(PHONE_NUMBER);
  gsmSerial.println("\"");

  // Wait for SMS input prompt

  if (!waitForResponse(">", 10000)) {

    Serial.println("SMS prompt not received!");
    return false;
  }

  // SMS message

  gsmSerial.println("!!! EMERGENCY ALERT !!!");

  gsmSerial.println(
    "I am in an emergency situation. "
    "Please contact me immediately."
  );

  // Include GPS coordinates

  if (gpsAvailable) {

    gsmSerial.println("MY CURRENT LOCATION:");

    gsmSerial.print(
      "https://www.google.com/maps?q="
    );

    gsmSerial.print(latitude, 6);

    gsmSerial.print(",");

    gsmSerial.println(longitude, 6);

  }
  else {

    gsmSerial.println(
      "GPS LOCATION UNAVAILABLE. "
      "Please try contacting me."
    );
  }

  // CTRL+Z sends the SMS

  gsmSerial.write(26);

  // Wait for SMS sending confirmation

  if (waitForResponse("+CMGS:", 20000)) {

    Serial.println("SMS SENT SUCCESSFULLY!");

    return true;
  }

  Serial.println("SMS SENDING FAILED!");

  return false;
}


// =================================================
//                  CALL FUNCTIONS
// =================================================

// Call emergency contact.

void makeEmergencyCall() {

  gsmSerial.listen();

  Serial.println();
  Serial.println("Calling Emergency Contact...");

  // Dial number

  gsmSerial.print("ATD");
  gsmSerial.print(PHONE_NUMBER);
  gsmSerial.println(";");

  // Allow call to continue

  unsigned long startTime = millis();

  while (millis() - startTime < CALL_DURATION) {

    // Keep SIM900A serial buffer clear

    while (gsmSerial.available()) {

      Serial.write(gsmSerial.read());
    }

    delay(10);
  }

  // Disconnect call

  gsmSerial.println("ATH");

  delay(1000);

  Serial.println("Call disconnected.");
}


// =================================================
//                EMERGENCY FUNCTION
// =================================================

void emergencyAlert() {

  Serial.println();
  Serial.println("================================");
  Serial.println("       EMERGENCY ACTIVATED      ");
  Serial.println("================================");

  // STEP 1: Acquire GPS location

  getGPSLocation();

  // STEP 2: Send emergency SMS

  bool smsStatus = sendEmergencySMS();

  if (smsStatus) {

    Serial.println("Emergency SMS delivered to GSM network.");
  }
  else {

    Serial.println("SMS confirmation not received.");
  }

  // STEP 3: Make emergency phone call

  makeEmergencyCall();

  Serial.println();
  Serial.println("Emergency sequence completed.");

  Serial.println("================================");
}


// =================================================
//                    SETUP
// =================================================

void setup() {

  Serial.begin(9600);

  gpsSerial.begin(9600);
  gsmSerial.begin(9600);

  // Internal pull-up resistor

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println();
  Serial.println("WOMEN SAFETY BANGLE");
  Serial.println("System Initializing...");

  delay(3000);

  // Initialize GSM

  gsmSerial.listen();

  sendAT("AT", 1000);

  sendAT("ATE0", 1000);

  sendAT("AT+CMGF=1", 1000);

  sendAT("AT+CSCS=\"GSM\"", 1000);

  sendAT("AT+CSQ", 1000);

  sendAT("AT+CREG?", 1000);

  Serial.println();
  Serial.println("System Ready!");
  Serial.println("Waiting for Emergency Button...");

  gpsSerial.listen();
}


// =================================================
//                     LOOP
// =================================================

void loop() {

  // Continuously update GPS

  updateGPS();

  // Check emergency button

  if (digitalRead(BUTTON_PIN) == LOW) {

    delay(DEBOUNCE_TIME);

    // Confirm button press

    if (digitalRead(BUTTON_PIN) == LOW) {

      emergencyAlert();

      // Wait for button release
      // Prevent repeated alerts from one press

      while (digitalRead(BUTTON_PIN) == LOW) {

        delay(20);
      }

      Serial.println();
      Serial.println("System Ready Again.");

      gpsSerial.listen();
    }
  }
}
