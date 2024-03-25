// 리팩토링 하지 않는 #1 버전가상시뮬레이터 주소 :  https://wokwi.com/projects/384152722205360129

/*
참고할 사이트
https://rottenstar.tistory.com/35
https://www.instructables.com/ARDUINO-MENU-DESIGN-With-OLED-UIUSER-INTERFACE/
https://github.com/alanesq/BasicOLEDMenu

보완 해야할 사항
1. 버튼 누를때 숫자간 한 단계씩 이동하는게 아니라 건너뛰는 현상

라이브러리 주소
1. Adafruit_SSD1306 : https://github.com/adafruit/Adafruit_SSD1306
2. Adafruit_GFX : https://github.com/adafruit/Adafruit-GFX-Library
3. ezButton : https://www.arduino.cc/reference/en/libraries/ezbutton/

하드 웨어 배선
Arduino nano Board          SSD1306_OLED_0.96inch
    VCC(5V)             <===>            5V
    GND                 <===>            GND
    A4                  <===>            SDA
    A5                  <===>            SCL
-------------------------------------------------

문서화 필요 영역
순서도 -> whimsical로 작성해 볼 것 (작성완료)

*/
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128    // OLED 디스플레이 가로폭 픽셀수
#define SCREEN_HEIGHT 64    // OLED 디스플레이 세로폭 픽셀수
#define OLED_RESET -1       // 리셋핀 설정 , -1 값은 아두이노 리셋핀과 공유
#define SCREEN_ADDRESS 0x3C /// 데이타시트와 I2C_SCANNER 예제로 주소를 찾아 적을 것
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int SELECT_BTN_PIN = 10;
const int UP_BTN_PIN = 9;
const int DOWN_BTN_PIN = 8;
const int debounceTime = 75;
ezButton selectBtn(SELECT_BTN_PIN);
ezButton upBtn(UP_BTN_PIN);
ezButton downBtn(DOWN_BTN_PIN);

int menu = 0;
int subMenu = 0;

void setup(void)
{
  Serial.begin(9600);     // 시리얼 통신 시작
  setup_displaySSD1306(); // SSD1306 초기화 및 대기화면 함수
  setup_debounceForBtn(); // 버튼 디바운스 시간 및 카운팅 모드 호출
}

void loop(void)
{
  selectBtn.loop();
  upBtn.loop();
  downBtn.loop();

  loop_mainMenuControl(); // 메인 화면 제어 함수 호출
  loop_subMenuControl();  // 서브 화면 제어 함수 호출
  loop_backToMainMenu();  // 메인화면으로 돌아가기 위한 조건문 호출
}

// SSD1306 초기화 및 대기화면 함수
void setup_displaySSD1306(void)
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // 초기화면 구성
  display.clearDisplay();
  delay(1000);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 5);
  display.setTextSize(2);
  display.print("IDLE");
  display.setCursor(0, 25);
  display.print("DISPLAY");
  display.display();
}

// 버튼 디바운싱 시간 및 카운팅 모드 설정 함수
void setup_debounceForBtn(void)
{
  selectBtn.setDebounceTime(debounceTime);
  upBtn.setDebounceTime(debounceTime);
  downBtn.setDebounceTime(debounceTime);
  selectBtn.setCountMode(COUNT_FALLING);
  upBtn.setCountMode(COUNT_FALLING);
  downBtn.setCountMode(COUNT_FALLING);
}

//=================== 메인 화면 제어 조건문 =================================
void loop_mainMenuControl(void)
{
  // IDLE 화면에서 select버튼 눌러도 메인 화면으로 넘어가지 위한 제어문
  if (selectBtn.isPressed() && selectBtn.getCount() == 1 && menu == 0)
  {
    display.clearDisplay();
    display.setCursor(5, 5);
    display.print(" up OR down ");
    display.setCursor(5, 25);
    display.print(" press please ");
    display.display();
    selectBtn.resetCount();
  }

  if (upBtn.isPressed() && selectBtn.getCount() == 0)
  {
    menu++;
    if (menu > 4)
    {
      menu = 4;
    }
    Serial.print("++menu : ");
    Serial.println(menu);
    updateMenu();
  }
  else if (downBtn.isPressed() && selectBtn.getCount() == 0)
  {
    menu--;
    if (menu < 1)
    {
      menu = 1;
    }
    updateMenu();
    Serial.print("--menu : ");
    Serial.println(menu);
  }
}

//===============서브화면 이동 위한 조건문  =====================================
void loop_subMenuControl(void)
{

  if (menu == 1 && selectBtn.isPressed())
  {
    menu = 0;
    subMenu = 11;
    Serial.print("subMenu is : ");
    Serial.println(subMenu);
    updateMenu();
  }
  else if (menu == 2 && selectBtn.isPressed())
  {
    menu = 0;
    subMenu = 21;
    Serial.print("subMenu is : ");
    Serial.println(subMenu);
    updateMenu();
  }
  else if (menu == 3 && selectBtn.isPressed())
  {
    menu = 0;
    subMenu = 31;
    Serial.print("subMenu is : ");
    Serial.println(subMenu);
    updateMenu();
  }
  else if (menu == 4 && selectBtn.isPressed())
  {
    menu = 0;
    subMenu = 41;
    Serial.print("subMenu is : ");
    Serial.println(subMenu);
    updateMenu();
  }
}

//=============== 메인화면으로 돌아가기 위한 조건문 =========================================
void loop_backToMainMenu(void)
{

  if (subMenu == 11 && selectBtn.getCount() == 2)
  {
    menu = 1;
    subMenu = 0;
    Serial.print("back to menu : ");
    Serial.println(menu);
    selectBtn.resetCount();
    updateMenu();
  }
  else if (subMenu == 21 && selectBtn.getCount() == 2)
  {
    menu = 2;
    subMenu = 0;
    Serial.print("back to menu : ");
    Serial.println(menu);
    selectBtn.resetCount();
    updateMenu();
  }
  else if (subMenu == 31 && selectBtn.getCount() == 2)
  {
    menu = 3;
    subMenu = 0;
    Serial.print("back to menu : ");
    Serial.println(menu);
    selectBtn.resetCount();
    updateMenu();
  }
  else if (subMenu == 41 && selectBtn.getCount() == 2)
  {
    menu = 4;
    subMenu = 0;
    Serial.print("back to menu : ");
    Serial.println(menu);
    selectBtn.resetCount();
    updateMenu();
  }
}

void updateMenu()
{
  //  ========== 메인 화면 표시 조건문 =======================
  if (menu == 1)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Dsn menu test"));
    display.println("Menu option 1");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu 1");
    display.display();
  }
  else if (menu == 2)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Dsn menu test"));
    display.println("Menu option 2");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu 2");
    display.display();
  }
  else if (menu == 3)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Dsn menu test"));
    display.println("Menu option 3");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu 3");
    display.display();
  }
  else if (menu == 4)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Dsn menu test"));
    display.println("Menu option 4");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu 4");
    display.display();
  }

  // =========  서브메뉴 화면 표시 조건문 ==============
  if (subMenu == 11)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("sub menu test"));
    display.println("Menu sub 1");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu #1");
    display.display();
  }
  else if (subMenu == 21)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("sub menu test"));
    display.println("Menu sub 2");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu #2");
    display.display();
  }
  else if (subMenu == 31)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("sub menu test"));
    display.println("Menu sub 3");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu #3");
    display.display();
  }
  else if (subMenu == 41)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("sub menu test"));
    display.println("Menu sub 4");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.println("Dsn Menu #4");
    display.display();
  }
}
