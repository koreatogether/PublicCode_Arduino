/*
수중펌프 ap201

아두이노
L9110 모듈

pwm 제어

Example testing sketch for various DHT humidity/temperature sensors
DHT CODE Written by ladyada, public domain

밀리함수 컨트롤 코드는 https://blog.naver.com/damtaja/220992246602
by 해바우


*/


#include <U8glib.h>
#include <DHT.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  

#define DHTTYPE DHT22 
const int DHTPIN = 12;     // what pin we're connected to
DHT dht(DHTPIN, DHTTYPE);
float Humidity;
float Temperature;

const int pump_A1 = 5;
const int pump_A2 = 6;
const int pump_B1 = 10;
const int pump_B2 = 11;
unsigned long previousMillis;
unsigned long previousMillis2;
unsigned long currentMillis;
unsigned long currentMillis2;
long intervalOn = 28000;//28000;  // 28초 는 28000
long intervalOff = 900000;//900000; // 15분은 900000
int motorState = LOW;
int motorSpeed1;
int motorSpeed2;

void setup()
{
    Serial.begin(115200);
    dht.begin();
    pinMode(pump_A1, OUTPUT);
    pinMode(pump_A2, OUTPUT);
    pinMode(pump_B1, OUTPUT);
    pinMode(pump_B2, OUTPUT);
}

void loop()
{
    currentMillis = millis();
    currentMillis2 = millis();   
    temp_humidity();
    water_motor_control();
    oled_Display();
}

void temp_humidity()
{
    if ( currentMillis2 - previousMillis2 >= 2500 )
    {
        Humidity = dht.readHumidity();
        Temperature = dht.readTemperature();
        if (isnan(Temperature) || isnan(Humidity))
        {
            Serial.println("Failed to read from DHT sensor!");
        }
        else
        {
            Serial.print("Humidity: ");
            Serial.print(Humidity);
            Serial.print(" %\t");
            Serial.print("Temperature: ");
            Serial.print(Temperature);
            Serial.println(" *C ");
        }
        previousMillis2 = currentMillis2;
    }
}

void water_motor_control()
{
    currentMillis = millis();
    if ((motorState == HIGH && currentMillis - previousMillis >= intervalOn) ||

        (motorState == LOW && currentMillis - previousMillis >= intervalOff))
    { // 수정한 부분
        previousMillis = currentMillis;
        if (motorState == LOW)
        {            
            motorState = HIGH;
            motorSpeed1 = 255;            
            motorSpeed2 = 0;
        }
        else
        {
            motorState = LOW;
            motorSpeed1 = 0;
            motorSpeed2 = 0;
        }

        analogWrite(pump_A1, motorSpeed1);
        analogWrite(pump_A2, motorSpeed2);
        analogWrite(pump_B1, motorSpeed1);
        analogWrite(pump_B2, motorSpeed2);
        Serial.print("motorState = ");Serial.print(motorState);Serial.println("    ");
        Serial.print("motorSpeed1 = ");Serial.print(motorSpeed1);Serial.println("    ");
        Serial.print("motorSpeed2 = ");Serial.print(motorSpeed2);Serial.println("    ");
    }

}

void oled_Display()
{
    u8g.firstPage();
    do
    {
        u8g.setFont(u8g_font_unifont);                
        u8g.drawStr(25, 10, "Water Pump");
        u8g.drawStr(10, 30, "ON");
        u8g.drawStr(90, 30, "OFF");
        u8g.setFont(u8g_font_courB14);                
        u8g.setPrintPos(5, 56);
        u8g.print(Humidity,0);
        u8g.print("%");        
        u8g.setPrintPos(70, 56);
        u8g.print(Temperature,0);
        u8g.print("C");
        u8g.setFont(u8g_font_unifont);    
        if (motorState == HIGH)
        {
            u8g.drawFrame(5,13,25,25);
        }
        else
        {
             u8g.drawFrame(85,13,35,25);
        }
    } while (u8g.nextPage());
}
