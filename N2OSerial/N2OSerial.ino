/*  5994    N2O
 *  P3_0    TX
 *  P3_1    RX
 *  3V3     VCC
 *  GND     GND
 */
#include <SoftwareSerial.h>
#define MeasureInterval 10000
#define SoftUse true
#if SoftUse == true
  SoftwareSerial N2OSerial(P1_4, P1_3); // RX, TX
#else
  #define N2OSerial Serial1
#endif
byte N2OCmd[7] = {0x10,0x13,0x06,0x10,0x1F,0x00,0x58};
byte N2ORece[15];
union FloatByte{
  float Float;
  byte Bytes[4];
};
FloatByte N2OData;
unsigned long MeasureTime;
void setup() {
  Serial.begin(4800);
  N2OSerial.begin(4800);
  Serial.println("System Ready");
}
void loop() {
  if(millis() - MeasureTime >= MeasureInterval){
    MeasureTime = millis();
    N2OGet();
  }
//N2OGet();
//delay(5000);
}
void N2OGet(){
  N2OSerial.write(N2OCmd,7);
  Serial.println("Request");
  delay(1000);
  if(N2OSerial.available()){
    byte recebyte = N2OSerial.read();
    Serial.print(recebyte, HEX);
    Serial.print("|");
    if(recebyte == 0x10){
      N2ORece[0] = 0x10;
      int i;
      while (N2OSerial.available()){
        for(i = 1;i < 15;i++){
          N2ORece[i] = N2OSerial.read();
          Serial.print(N2ORece[i], HEX);
          Serial.print("|");
          if(i>=8 && i<=11){
           N2OData.Bytes[i-8] = N2ORece[i];
          }
        }
        Serial.println();
        Serial.print("N2O Data is:");
        Serial.println(N2OData.Float);
      }
    }
  } 
}
