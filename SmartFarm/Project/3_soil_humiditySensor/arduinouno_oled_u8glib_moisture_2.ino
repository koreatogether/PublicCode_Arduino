/*
 보드별 토양습도센서 붙이기(Attaching Soil Humidity Sensors by Board)

 특정 보드  아두이노 우노 , 나노 , seeeduino xiao 보드에서
 3개의 토양습도센서(캐페시터방식)를 oled에 습도 수치로 표시하는 겁니다.

 부품
  토양습도센서 : 캐페시터방식(저항식과 다름 )
  oled : I2C
  아두이노 : 우노 , 나노 , seeeduino xiao 보드

  코드 시작일 : 22.07.21
  저작권자 : iot발걸음(iot_Step)
  편집날짜 : 22.07.24
  편집자 : iot발걸음(iot_Step)


*/

/*
코드는 아두이노 보드 용  과 esp32 , esp8266 , xiao 분류되어 있습니다.
그리고 oled를 쓸것인지 , 그것도 어떤 라이브러리를 쓸것인지도 구별되어 있습니다.
좀더 간편한 방법을 몰라서 현재 코드이며 , 추후  경우의 수에 따른 주석만 없애면 보드에 맞게 동작하는 걸 보고 싶네요

현재 코드는 아두이노 uno , nano  와 oled display  , u8glib을 쓰는 걸로 조정했습니다.

elecCrow_Moisture_sensor
 - Operating Voltage: 3.3V ~ 5V
 - Output type: Analog
 - Output Votage:  max 3V(제가 직접 측정) 고로 3.3V 개발보드에 쓰일수있다.
센서 공식 WIKI : https://www.elecrow.com/wiki/index.php?title=Crowtail-Capacitive_Moisture_V2.0

- 전기저항식이 아닌 캐패시터 방식 센서
센서 설명 유튜브 : https://youtu.be/pFQaFnqpOtQ

변수들을 5V 와 3.3V 사용을 가능하게 할것
변수들을 마른상태 와 젖은 상태의 값을 제어 할수있게 할것

기타 볼만한 센서
검색어 soil watch 10
판매처 : https://pino-tech.eu/product/soilwatch-10/
가격은 높지만 유튜브 설명보기란에 이에 대한 답변을 단것이 있습니다.
대충 이야기하자면 저항기준 토양수분센서들은 도금이 벗겨지고
단기 및 학습프로젝트에 어울립니다.
soil watch10은 이러함점을 보완 했고 방수 및 좀더 강한 표면 처리를 했습니다.
그래서 가격이 비쌉니다.  (최소 21달러 )

해외 비슷한 센서 설명
https://how2electronics.com/interface-capacitive-soil-moisture-sensor-arduino/

국내 TDR 방식 센서 업체
https://blog.naver.com/wkdwns9/222680728988

절대 구입하지 말아야 할 센서는 구리선 또는 면이 노출된
아주 저렴한 센서 , 이건 그냥 하루 테스트하고 버려도 될 정도의
조악한 센서로 절대 토양에 꽂아서 사용하지 말것 , 물과 산소,  구리가 닿고 작동
하는 순간 구리 분해가 시작되기에 1000원미만이면 구매하고 아니면 4000원급 , 방수 기능이
 있는 센서를 구매할것.  구리면 분해시 독성 나옴 즉 토양 오염시작합니다.

 Xiao 보드 설명 블로그
 https://blog.naver.com/PostView.nhn?blogId=hankrah&logNo=222209639905

 아두이노 코딩시 기본 규칙
 https://tagilog.tistory.com/338

*/

#include <U8glib.h>  // for oled display on arduino uno , nano
//#include <U8g2lib.h> // for oled display on not arduino uno , nano , 메모리차이로인하여 우노, 나노사용불가

//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);  // for u8glib and arduino uno , nano board
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  // for u8g2lib and not arduino , for esp32 ,esp8266 , xiao etc

const float choice_Voltage = 5.0;  // if 3.3V , choice_Voltage = 3.3;
//const float choice_Voltage = 3.3;
// 만약 우노 또는 나노 3.3V 기준으로할것이면 3.3V , Aref를 쇼트시킨다.
// 그리고 void setup() 안에서 analogReference(3.3); 해준다. 기본 3.3V 보드들은 상관없다.

const int choice_analogStep = 1024; // base 10bit , if 12bit , choice_analogStep = 4096;
//const int choice_analogStep = 4096;

const int sensor_value_inAir = 564; // 센서를 건조하게 공기에  놨을때 아날로그값 (you need to change this value that you had recorded in the air)
const int sensor_value_inWater = 282; // 센서를 물에 한계선까지 넣었을때  아날로그값 (you need to change this value that you had recorded in the water) )
// my data inAir = ~ 564(0%)  , inWater = 268 ~ 285(97%) , in Drysoil = 500 ~ 530 ( 13%)
// my data inAir = ~ 897(0%) , inWater = 464(100%)

int measure_loop_Time = 10000; //   1 Second = 1000 , 10s = 10000 , 1min = 60000 , 10min = 600000

int sensor_analogPin1 = A0;  // sensor analog pin
int sensor_analogPin2 = A1; // sensor analog pin
int sensor_analogPin3 = A2;  // sensor analog pin

float sensor_analogVal1; // read analog value from sensor
float sensor_analogVal2; // read analog value from sensor
float sensor_analogVal3; // read analog value from sensor

int percent_sensor_Val1;  // analogVal -> percent
int percent_sensor_Val2;  // analogVal -> percent
int percent_sensor_Val3;  // analogVal -> percent

float sensor_voltage1;  //  센서 전압 계산용
float sensor_voltage2; //  센서 전압 계산용
float sensor_voltage3; //  센서 전압 계산용

int high_box_Val;  // oled box그릴때 쓰일 값
int middle_box_Val; // oled box그릴때 쓰일 값
int low_box_Val;  // oled box 그릴때 쓰일 값


void setup()
{
  Serial.begin(115200); // open serial port, set the baud rate to 115200 bps
  //u8g2.begin();  if use u8g2lib
  //analogReadResolution(12);  if board's analog resolution 12bit  ex>> xiao board
  //analogReference(3.3); // if want set the reference voltage(3.3) for the analog inputs
}



void loop() // Use functions as possible
{
  read_sensor_value();  // read board's analogpin value
  calculate_sensor_voltage(); //
  calculate_sensor_percent();  // sensorValue change to percent
  calculate_drawBox_width();  //
  serial_printing(); // print the sensor value to serial port
  u8glib_display_mainLoop(); // if use u8glib and for arduino uno , nano board , oled display
  //u8g2_display_mainLoop() // if use u8g2lib and for not arduino , for esp32 ,esp8266 , xiao etc.
  delay(measure_loop_Time);
}

void read_sensor_value() {  // read sensor value
  sensor_analogVal1 = analogRead(sensor_analogPin1);
  sensor_analogVal2 = analogRead(sensor_analogPin2);
  sensor_analogVal3 = analogRead(sensor_analogPin3);
}

void calculate_sensor_voltage() { // calculate sensor voltage
  sensor_voltage1 = sensor_analogVal1 * (choice_Voltage / choice_analogStep);
  sensor_voltage2 = sensor_analogVal2 * (choice_Voltage / choice_analogStep);
  sensor_voltage3 = sensor_analogVal3 * (choice_Voltage / choice_analogStep);
}

void calculate_sensor_percent() {  // calculate sensor percent , use map()
  percent_sensor_Val1 = map(sensor_analogVal1, sensor_value_inAir, sensor_value_inWater, 0, 100);
  percent_sensor_Val2 = map(sensor_analogVal2, sensor_value_inAir, sensor_value_inWater, 0, 100);
  percent_sensor_Val3 = map(sensor_analogVal3, sensor_value_inAir, sensor_value_inWater, 0, 100);
}

void calculate_drawBox_width()
{
  high_box_Val = map(percent_sensor_Val1,0,100,0,56);
    middle_box_Val = map(percent_sensor_Val2,0,100,0,56);
    low_box_Val = map(percent_sensor_Val3,0,100,0,56);
}


void serial_printing ()  // for serial print
{
  Serial.print("sensor_analogVal1 = "); Serial.print(sensor_analogVal1);Serial.print("   ");Serial.print("sensor_voltage1 = "); Serial.print(sensor_voltage1);Serial.print("   "); Serial.print("percent_sensor_Val1 = "); Serial.println(percent_sensor_Val1);
  Serial.print("sensor_analogVal2 = "); Serial.print(sensor_analogVal2);Serial.print("   ");Serial.print("sensor_voltage2 = "); Serial.print(sensor_voltage2);Serial.print("   "); Serial.print("percent_sensor_Val2 = "); Serial.println(percent_sensor_Val2);
  Serial.print("sensor_analogVal3 = "); Serial.print(sensor_analogVal3);Serial.print("   ");Serial.print("sensor_voltage3 = "); Serial.print(sensor_voltage3);Serial.print("   "); Serial.print("percent_sensor_Val3 = "); Serial.println(percent_sensor_Val3);
}

// if use u8glib ( version 1) example code  for arduino uno , nano board
void u8glib_display_mainLoop() //for SSD1306 DRIVER , 128x64 Oled display
{
  u8g.firstPage();
  do{  // display code
     u8g.setFont(u8g_font_6x10);
     draw_Title();
     draw_number_sensor();  // number or korean font
     draw_percent_box(); // draw  bar boxs
     draw_sensor_percentVal();
     draw_symbol();

  }while(u8g.nextPage());
}

void draw_Title() // draw title
{
  u8g.drawStr(1,10,"Moisture Humidity");
}

void draw_number_sensor() // draw number sensor
{
  u8g.drawStr(2,30,"01");
  u8g.drawStr(2,47,"02");
  u8g.drawStr(2,64,"03");
}

void draw_percent_box() // draw percent box
{

  u8g.drawFrame(20,19,60,12); //
  u8g.drawFrame(20,36,60,12); //
  u8g.drawFrame(20,52,60,12); //
  u8g.drawBox(22,21,high_box_Val,8); //
  u8g.drawBox(22,38,middle_box_Val,8); //
  u8g.drawBox(22,54,low_box_Val,8); //
}

void draw_sensor_percentVal() // draw sensor percent value
{
  u8g.setPrintPos(90,30);
  u8g.print(percent_sensor_Val1);
  u8g.setPrintPos(90,47);
  u8g.print(percent_sensor_Val2);
  u8g.setPrintPos(90,64);
  u8g.print(percent_sensor_Val3);
}

void draw_symbol() // draw symbol
{
  u8g.drawStr(110,30,"%");
  u8g.drawStr(110,47,"%");
  u8g.drawStr(110,64,"%");
}  //  22-07-23 테스트 끝냄




// ***************************
// ********* U8g2lib *********
//****************************
/*
void u8g2lib_display_mainLoop() //for U8G2lib ( version 2) example code  for esp32, esp8266 , xiao board etc
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2_draw_Title();
    u8g2_draw_number_sensor();  // number or korean font
    u8g2_draw_frame_and_barbox();// draw  bar boxs , and frame
    u8g2_draw_sensor_percentVal();  // draw sensor percent value
    u8g2_draw_symbol(); // draw symbol
    u8g2.sendBuffer();
}

void u8g2_draw_Title() // draw title
{

  u8g2.drawStr(1, 10, "Moisture Humidity");
}

void u8g2_draw_number_sensor() // draw number sensor
{
      //u8g2.setFont(u8g2_font_unifont_t_korean2);
    //한글 표현은 setCursor , print 함수를 써야 한다. drawStr() 안됨
    u8g2.setCursor(2, 30);
    u8g2.print("01");
    u8g2.setCursor(2, 47);
    u8g2.print("02");
    u8g2.setCursor(2, 64);
    u8g2.print("03");
}

void u8g2_draw_frame_and_barbox() // draw percent box
{
  u8g2.drawFrame(20, 19, 60, 12);
    u8g2.drawFrame(20, 36, 60, 12);
    u8g2.drawFrame(20, 52, 60, 12);
    u8g2.drawBox(22, 21, high_box_Val, 8);
    u8g2.drawBox(22, 38, middle_box_Val, 8);
    u8g2.drawBox(22, 54, low_box_Val, 8);

}

void u8g2_draw_sensor_percentVal() // draw sensor percent value

{
  u8g2.setCursor(90, 30);
  u8g2.print(percent_sensor_Val1);
  u8g2.setCursor(90, 47);
  u8g2.print(percent_sensor_Val2);
  u8g2.setCursor(90, 64);
  u8g2.print(percent_sensor_Val3);
}

void u8g2_draw_symbol() // draw symbol
{
  u8g2.drawUTF8(110, 30, "%");
    u8g2.drawUTF8(110, 47, "%");
    u8g2.drawUTF8(110, 64, "%");
}  // 22-07-24 테스트 끝냄


*/