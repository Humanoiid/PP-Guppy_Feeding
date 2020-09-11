/* Guppy_Bab_project
//
// Feature: Feed Guppy for four days
// Refill the food on 1~4 plates.
//
// SG90 Servo motor
// angle range (8~188 degrees)
// rotate by 45 degrees
//
// rotate 45 degrees per 24 hours (1sec : 1000ms = 24hours : 86400000ms)
//
// Buzzer ring at Start/ 1sec before Feeding time
// 
// OneWire measure water temp and display at LCD
//
// Ports
// Servo : 9
// Buzzer : 8
// LCD : 4 6 10 11 12 13
// OneWire(waterproof) 2
*/
#include <Servo.h>
#include <OneWire.h>
#include <LiquidCrystal.h>

/* define */
/* tones */
#define C 956   // 도
#define E 1136  // 미
#define G 1432  // 솔
/* time */
#define DAY 86400000    // 하루
#define HOUR 3600000    // 한 시간

/* ports */
int servoPin = 9;
int buzzer = 8;
int ds_pin = 2;
LiquidCrystal lcd(4, 6, 10, 11, 12, 13);
// LiquidCrystal lcd(RS, Enable, DB4, DB5, DB6, DB7);

int angle = 8; // servo position in degrees 
Servo servo;
OneWire ds(ds_pin); //  create OneWire Object

void setup() {
    servo.attach(servoPin);
    servo.write(angle);
    lcd.begin(16, 2);
    Serial.begin(9600);
    
    pinMode(buzzer,OUTPUT);
    delay(3000);
    tone(buzzer,956);
    delay(500);                          // buzzer에 0.5초간 도를 출력하도록 합니다.
    tone(buzzer,1136);
    delay(500);                          //buzzer에 0.5초간 미를 출력하도록 합니다.
    tone(buzzer,1432);
    delay(500);                          //buzzer에 0.5초간 솔를 출력하도록 합니다.
    noTone(buzzer);
     
    /* Welcome Message */
    lcd.setCursor(5,0);
    lcd.print("Hello!!");
    lcd.setCursor(1,1);
    lcd.print("Guppy Project");

    delay(3000);  /* set one day delay later... */
}

void loop() {
  float temp = getTemp();
  Serial.println(temp);
  lcd.setCursor(5,0);
  lcd.print(temp);
  // scan from 0 to 180 degrees
  if(angle > 187)                        // 작동 끝나면 계속 대기
  {
    servo.write(8);
    delay(5000);
  }
  else
  {
    tone(buzzer,1432);
    delay(500);                          //buzzer에 0.5초간 솔를 출력하도록 합니다.  
    tone(buzzer,1136);
    delay(500);                          //buzzer에 0.5초간 미를 출력하도록 합니다.
    tone(buzzer,956);
    delay(500);                          // buzzer에 0.5초간 도를 출력하도록 합니다.
    noTone(buzzer);
    delay(1000);                         // 1초 대기
    
    angle = angle + 36;  
    servo.write(angle);
    //delay(1000);
  
    delay(3000); // 1000ms = 1sec (12hours = 3600000ms)   ////////////////////////////// 나중에 하루로 수정
  }
}

/* get Temperature of water from OneWire Object
   Do Not Modify!!!!!!                        */
float getTemp() {
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.println("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB);
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}

