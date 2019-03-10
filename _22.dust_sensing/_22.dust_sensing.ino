//1602 LCD 세팅
#include <LiquidCrystal.h>               // LCD 라이브러리 포함
LiquidCrystal lcd(13, 12, 5, 4, 3, 2);   // LCD 핀 설정

//DHT11 온습도센서 세팅
#include "DHT.h"                // DHT11 라이브러리 포함 // 라이브러리 설정할떄 Adafruit_DHT 설정이용 ==> 만드는 회사의 센서를 다운 받아야함
#define DHTPIN 7                // DHT11 연결핀
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);     // 클래스로 만들거라 불러오기만 하면 된다.

//먼지센서 세팅
int LED = 11;
int DUST = 0;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float dustval = 0;
float voltage = 0;
float dustug = 0;
float dus = 0;
float a = 0;

void setup() {
  Serial.begin(9600);        // 시리얼 통신 시작
  lcd.begin(16, 2);           // 16x2 LCD 선언
  pinMode(LED, OUTPUT);  // 먼지센서 LED 핀 설정
}

void loop() {
  //먼지센서 코드
  digitalWrite(LED, LOW); // 적외선 LED ON
  delayMicroseconds(samplingTime);
  dustval = analogRead(DUST); //먼지센서 값 읽기
  delayMicroseconds(deltaTime);
  digitalWrite(LED, HIGH); // 적외선 LED OFF
  delayMicroseconds(sleepTime);
  voltage = dustval * (5.0 / 1024.0);  // 전압 구하기
  dustug = 0.17 * voltage;      // ug 단위 변환
  dus = dustug * 1000;
  Serial.print("Dust Sensor Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.print("Dust Value: ");
  Serial.print(dus); // unit: mg/m3
  Serial.println("ug");

  //dht11 온습도센서 코드
  int h = dht.readHumidity();     // 습도 값 구하기
  int t = dht.readTemperature();  // 온도 값 구하기
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  //1602 LCD 모듈 코드
  lcd.clear();
  analogWrite(6, 120);
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(h);
  lcd.print(" %  ");
  lcd.print("T: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Dust: ");
  if (dus > 0) {
    a = dus;
    lcd.print(a);
  }
  else {
    lcd.print(a);
  }
  lcd.print("ug");

  //RGB LED 모듈 코드   // 판단하는 형태를 LCD화면으로 
  if (a < 35) {                 // 좋음
    analogWrite(9, 0);
    analogWrite(10, 20);
  }
  if (a > 35 & a < 75) {    // 나쁨
    analogWrite(9, 30);
    analogWrite(10, 3);
  }
  if (a > 75) {                // 매우나쁨
    analogWrite(9, 20);
    analogWrite(10, 0);
  }
  delay(1000);
}
