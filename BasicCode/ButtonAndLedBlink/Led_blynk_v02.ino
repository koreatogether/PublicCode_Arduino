/**
 * @file led_blink_self.ino
 * @author iot발걸음
 * @brief led blink를 연습하는 예제
 * @version 0.2
 * @date 2022-09-13
 * @copyright Mit License
 */

/*
 @brief  Led Blink 상태와 버튼 간의 상태  표를 통한 쉬운 이해
 @link : https://kikiai.tistory.com/29
 @brief 위 링크와 다른게 보드 내부 풀업 저항을 이용하므로 버튼 상태들은 true 이다.

 V01은 버튼을 눌렀을때만 led가 켜지는 순간적인 깜박임을 자세하게 표현 한것입니다.
표로 표시하자면
          현재 버튼 상태  이전 버튼 상태 현재 led 상태 이전 led 상태
초기상태      True (1)      True (1)     False(0)      False(0)
버튼 누름     False(0)      True (1)     True (1)      False(0)
버튼 뗌       True (1)      False(0)     False(0)      True (1)
버튼 누름     False(0)      True (1)     True (1)      False(0)
버튼 뗌       True (1)      False(0)     False(0)      True (1)

@brief V01 WOKKI 버전
@link :https://wokwi.com/projects/342569511526859348

 V02는 버튼을 누르고 떼면 led의 상태 변화 유지를 목표로 합니다.
 표로 표시하자면
          현재 버튼 상태  이전 버튼 상태 현재 led 상태 이전 led 상태
초기상태      True (1)       True (1)     False(0)      False(0)
버튼 누름     False(0)       True (1)     True (1)      False(0)
버튼 뗌       True (1)       False(0)     True (1)      True (1)
버튼 누름     False(0)       True (1)     False(0)      True (1)
버튼 뗌       Ture (1)       False(0)     False(0)      False(0)

@brief V02 WOKKI 버전
@link :https://wokwi.com/projects/342571994445775443

v2 - 인공지능 copilot 제시한 코드의 흐름도 추가 해보기
   - 카페회워분의 조언에 따라서 딜레이 사용자 정의 함수 내부로 이동
22-09-13 마무리 하기
*/

int ledPin = 7;
int buttonPin = 11;
boolean current_buttonState = true;  // pullup 버튼이므로 true
boolean previous_buttonState = true; // pullup 버튼이므로 true
boolean current_LedState = false;    // v1 and v2 공통 변수
boolean previous_LedState = false;   // v1 전용 변수
int delayTime = 30;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{

  v2_LedBlynk(); // LED 상태 변화 및 유지
  // v1_LedBlynkSimple();  // 버전 1 의 단순 코드
  // v1_Led_Blynk();       // 버전 1 순간적인 깜박임
  debugging_print();       // 버튼에 따라 순간적인 깜빡임_self
}

void v2_LedBlynk()
{
  current_buttonState = digitalRead(buttonPin);
  if (current_buttonState == false && previous_buttonState == true)
  {
    current_LedState = !current_LedState;
  }
  previous_buttonState = current_buttonState;
  digitalWrite(ledPin, current_LedState);
  // delay(delayTime);  // 가상 시뮬레이터에서  동작시 주석 없앨것
}

void v1_LedBlynkSimple()
{
  current_buttonState = digitalRead(buttonPin);
  if(current_buttonState == false)
  {
    digitalWrite(ledPin, true);
  }
  else
  {
    digitalWrite(ledPin, false);
  }
  // delay(delayTime);  // 가상 시뮬레이터에서  동작시 주석 없앨것
}

void v1_Led_Blynk()  // 주석의 표 보고 짠 코드
{
  current_buttonState = digitalRead(buttonPin);
  if (current_buttonState == false && previous_buttonState == true)
  {
    current_LedState = !current_LedState;
    previous_buttonState = current_buttonState;
    previous_LedState = current_LedState;
    digitalWrite(ledPin, current_LedState);
  }
  else if (current_buttonState == true && previous_buttonState == false)
  {
    current_LedState = !current_LedState;
    previous_buttonState = current_buttonState;
    previous_LedState = current_LedState;
    digitalWrite(ledPin, current_LedState);
  }
  // delay(delayTime);  // 가상 시뮬레이터에서  동작시 주석 없앨것
}

void debugging_print()
{
  Serial.print("c_bttState : "), Serial.print(current_buttonState);
  Serial.print(" ");
  Serial.print("p_bttState : "), Serial.print(previous_buttonState);
  Serial.print(" ");
  Serial.print("c_LedState : "), Serial.print(current_LedState);
  Serial.print(" ");
  Serial.print("p_LedState : "), Serial.println(previous_LedState);
}