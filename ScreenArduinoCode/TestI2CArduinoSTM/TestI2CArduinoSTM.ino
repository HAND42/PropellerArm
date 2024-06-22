#include <Wire.h>

#define SLAVE_ADDRESS 0x12  // I2C slave address (7-bit format)
uint8_t TxData[2] = {0x1, 0x2};  // Data array to be sent

void setup() {
    Wire.begin();  // Join I2C bus as master
    Serial.begin(115200);
    Serial.println("I2C Master: Begin transmission...");
}

void loop() {
        Wire.beginTransmission(SLAVE_ADDRESS);  // Begin I2C transmission to the slave
        Wire.write(TxData, sizeof(TxData));     // Send data array
        uint8_t error = Wire.endTransmission(); // End transmission and check for errors

        if (error == 0) {
            Serial.print("Data sent: ");
            for (int i = 0; i < sizeof(TxData); i++) {
                Serial.print("0x");
                Serial.print(TxData[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        } else {
            Serial.print("Error: ");
            Serial.println(error);
        }
  

    delay(1000);  // Wait for 1 second before checking again
}
