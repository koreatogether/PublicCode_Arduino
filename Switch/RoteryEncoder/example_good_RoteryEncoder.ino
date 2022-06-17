/*
Rotery Encoder and Arduino Uno Code

** Part

1. Rotery Encoder
    - https://www.arduino.cc/en/Reference/RotaryEncoder
    - https://www.arduino.cc/en/Tutorial/RotaryEncoder
    - https://www.arduino.cc/en/Tutorial/RotaryEncoderPulse

    국내 기준 사이트
    - 네이버 검색어 : "아두이노 로터리 엔코더"
    - 가격대 900원 부터 시작 ~
2. Arduino Uno

** Pinout

Rotary Encoder
    - GND <-> Arduino GND
    - VCC <-> Arduino 5V  ( 5V or 3.3v)
    - CLK <-> Arduino D2 ( 위의 엔코더에 대한 설명을 기준으로 접촉부 A의 역할을 하는 핀으로서 최소 회전단위마다 한사이클의 신호(HIGH에서 LOW로) 변환을 출력함)
    - DT <-> Arduino D3  ( 위의 엔코더에 대한 설명을 기준으로 접촉부 B의 역할을 하는 핀으로서 CLK핀과 90도만큼의 위상차이를 가짐)
    - SW <-> Arduino D4  (푸시버튼 스위치로 노브를 누르면 딸각소리가 나며 눌립니다. 버튼이 눌렸을 때 LOW와 연결됩니다.)

*/

// 로터리 엔코더 모듈 예제(https://www.devicemart.co.k)
// 로터리 엔코더 만 남겨놓고 변
// 이 코드가 가장 정확함
// 모듈은  CLK , DT , SW 이 실크된 모듈을 씀
#define ENOTS_PIN 4
#define ENOTB_PIN 3
#define ENOTA_PIN 2
// #define LED_PIN 5        //핀번호 지정

int ENOTS;
int ENOTB;
int ENOTA;
int n = LOW;
int Last = LOW;
int encoder0Pos = -10; // 변수 초기값 설정

void setup()
{
    Serial.begin(9600); //시리얼 통신
    pinMode(ENOTS_PIN, INPUT_PULLUP);
    pinMode(ENOTB_PIN, INPUT);
    pinMode(ENOTA_PIN, INPUT);
}

void loop()
{
    ENOTS = digitalRead(ENOTS_PIN);
    ENOTB = digitalRead(ENOTB_PIN);
    ENOTA = digitalRead(ENOTA_PIN);
    
    //센서 값 지정
    if ((Last == LOW) && (ENOTA == HIGH))
    { // 센서 값이 HIGH -> LOW 로 변경될때
        if (ENOTB == LOW)
        { // B 값이 변할 경우 -10  (반 시계방향 회전)
            encoder0Pos = encoder0Pos - 10;
        }
        else
        { // A 값이 변할 경우 +10   (시계방향 회전)
            encoder0Pos = encoder0Pos + 10;
        }      
        Serial.println(encoder0Pos); // 회전 시 변화하는 값
    }
    Last = ENOTA;

    if (ENOTS == LOW)
    {

        Serial.println(encoder0Pos);
    }
}
