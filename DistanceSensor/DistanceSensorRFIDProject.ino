#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);

#define TRIG_PIN 9 // TRIG pin
#define ECHO_PIN 8 // ECHO pin

float filterArray[20]; // array to store data samples from sensor
float distance; // store the distance from sensor

void setup() {
  // begin serial port
  Serial.begin (9600);

  // configure the trigger and echo pins to output mode
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {
  // Following code is used to limit interference with the sensor
  for (int sample = 0; sample < 20; sample++) {
    filterArray[sample] = ultrasonicMeasure();
    delay(30); // to avoid untrasonic interfering
  }

  for (int i = 0; i < 19; i++) {
    for (int j = i + 1; j < 20; j++) {
      if (filterArray[i] > filterArray[j]) {
        float swap = filterArray[i];
        filterArray[i] = filterArray[j];
        filterArray[j] = swap;
      }
    }
  }

  double sum = 0;
  for (int sample = 5; sample < 15; sample++) {
    sum += filterArray[sample];
  }

  distance = sum / 10;

  // print the value to Serial Monitor. This is what the polling program will see to track the distance
  Serial.println(distance);

  //This code is for the RFID tag. Currently, there is a tiny delay when the card is near the reader so keep that in mind
  
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      //Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      // print NUID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println();

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }

}

float ultrasonicMeasure() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  float duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  float distance_cm = 0.017 * duration_us;

  return distance_cm;
}
