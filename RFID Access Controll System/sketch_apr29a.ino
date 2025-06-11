#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN         10
#define RST_PIN        9
#define GREEN_LED_PIN  6
#define RED_LED_PIN    7
#define BUZZER_PIN     8

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Ensure everything starts OFF
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Read card UID and send to Python
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  
  // Send the UID to Python
  Serial.println(uidString);

  // Wait for response from Python (0 = denied, 1 = granted)
  while (!Serial.available()) {
    // Wait for response with timeout
    delay(10);
  }
  
  char response = Serial.read();
  
  if (response == '1') {
    grantAccess();
  } else {
    denyAccess();
  }

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  
  // Clear any remaining data from serial
  while (Serial.available()) {
    Serial.read();
  }
}

void grantAccess() {
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void denyAccess() {
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
}