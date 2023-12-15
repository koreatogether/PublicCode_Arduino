#include <Versatile_RotaryEncoder.h>

/* SET READING PINS ACCORDINGLY TO YOUR ENCODER TO BOARD CONNECTIONS
    ( 엔코더 읽기 핀을 보드 연결에 맞게 설정하세요 )
   Set here your encoder reading pins (Ex.: EC11 with breakout board)
    여기에 엔코더 읽기 핀을 설정하세요(예: EC11 브레이크아웃 보드와의 연결)
*/
#define clk 17  // (A3)   ==> 저는 나노 보드 기준   2
#define dt 16   // (A2)                                 3
#define sw 18   // (A4)                                4

/* Functions prototyping to be handled on each Encoder Event
   (각 엔코더 이벤트에서 처리할 함수 프로토타이핑)
   의미를 모르겠지만 쓸때 그냥 두세요 , 잘아시는 분있으면 필요한것만 살려도 되는건지 커멘트 부탁드립니다.
*/
void handleRotate(int8_t rotation);
void handlePressRotate(int8_t rotation);
void handleHeldRotate(int8_t rotation);
void handlePress();
void handleDoublePress();
void handlePressRelease();
void handleLongPress();
void handleLongPressRelease();
void handlePressRotateRelease();
void handleHeldRotateRelease();

// Create a global pointer for the encoder object ( 엔코더 객체를 위한 전역 포인터 생성  , 삭제 하면 안됩니다.)
Versatile_RotaryEncoder *versatile_encoder;

void setup() {

    Serial.begin(9600);
	versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);  // 초급자분들은 위치만 변경하고 지우시면 안됩니다.

    /* Load to the encoder all nedded handle functions here (up to 9 functions)
       ( 여기에 필요한 모든 처리 함수를 엔코더에 로드하기(최대 9개 함수 )
       아래 개별 함수 중에 필요한것만 살려두시면 됩니다. 주석처리하면 사용중지됩니다.
      보통은  ### 표시된 함수를 많이 씁니다.
    */
    versatile_encoder->setHandleRotate(handleRotate);  // ### , 좌우 회전에 관련된 함수입니다.
    versatile_encoder->setHandlePressRotate(handlePressRotate);  // 누르면서 회전 감지 ???
    versatile_encoder->setHandleHeldRotate(handleHeldRotate);  // 의미를 모르겠네요
    versatile_encoder->setHandlePress(handlePress);     // ### , sw(switch) 핀의 누르기만 감지하는 겁니다.
    versatile_encoder->setHandleDoublePress(handleDoublePress); // 빠르게 두번 누르면 감지하는 겁니다.
    //versatile_encoder->setHandleDoublePress(nullptr); // Disables Double Press  , 빠르게 누름 2번을 안쓰게 할때 활성화하면됩니다.
    versatile_encoder->setHandlePressRelease(handlePressRelease); // ### , sw(switch) 핀의 뗄때 감지하는 겁니다.
    versatile_encoder->setHandleLongPress(handleLongPress);  // 엔코더 스위치를 기본설정 1초이상 길게 누르고 감지할때 씁니다.
    versatile_encoder->setHandleLongPressRelease(handleLongPressRelease);  // 길게 누름(1초이상) 뗄때 감지합니다.
    versatile_encoder->setHandlePressRotateRelease(handlePressRotateRelease); // ????
    versatile_encoder->setHandleHeldRotateRelease(handleHeldRotateRelease); // ????

    Serial.println("Ready!");

    // set your own defualt values (optional) , 감지 간격 , 감지 방법을 변경하는 곳입니다.
    // versatile_encoder->setInvertedSwitch(true); // inverts the switch behaviour from HIGH to LOW to LOW to HIGH
    // versatile_encoder->setReadIntervalDuration(1); // set 2ms as long press duration (default is 1ms)
    // versatile_encoder->setShortPressDuration(35); // set 35ms as short press duration (default is 50ms)
    // versatile_encoder->setLongPressDuration(550); // set 550ms as long press duration (default is 1000ms)
    // versatile_encoder->setDoublePressDuration(350); // set 350ms as double press duration (default is 250ms)

}

void loop() {

    // Do the encoder reading and processing
    if (versatile_encoder->ReadEncoder()) {
        // Do something here whenever an encoder action is read ( 엔코더 동작이 읽힐 때마다 할 동작 코딩을 넣으세요 )
    }

}

/* Implement your functions here accordingly to your needs( 필요에 따라 여기에 함수를 구현하세요.)
아래 함수 10개가 보시면 아시겠지만 동작할 코드를 넣으셔도 됩니다. 코드가 길어서 ,지연시간이 길어서 나오는 부작용은
테스트 못해 봤습니다.

*/

void handleRotate(int8_t rotation) {  // ### , 회전에 관련된 함수로 rotatin 값이 0 보다 크면 오른쪽감지한것이고 작으면 왼쪽감지한겁니다.
	Serial.print("#1 Rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePressRotate(int8_t rotation) {
	Serial.print("#2 Pressed and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handleHeldRotate(int8_t rotation) {
	Serial.print("#3 Held and rotated: ");
    if (rotation > 0)
	    Serial.println("Right");
    else
	    Serial.println("Left");
}

void handlePress() {   // ### , sw 버튼 누르고 있을때 동작 함수 입니다.
	Serial.println("#4.1 Pressed");
}

void handleDoublePress() {
	Serial.println("#4.2 Double Pressed");
}

void handlePressRelease() {  // ### , , sw 버튼 누르고 뗄때 동작함수입니다.
	Serial.println("#5 Press released");
}

void handleLongPress() {
	Serial.println("#6 Long pressed");
}

void handleLongPressRelease() {
	Serial.println("#7 Long press released");
}

void handlePressRotateRelease() {
	Serial.println("#8 Press rotate released");
}

void handleHeldRotateRelease() {
	Serial.println("#9 Held rotate released");
}