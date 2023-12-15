각 AI 별 Menu_#1.ino 코드에 대한 추가 제안요약


## Claude
- 메뉴 구성이 복잡해질 수록 클래스 or 상태머신 구조 전환을 고려 할것 ( 2차나 3차때 적용 예정 )
- 각 메뉴별 처리 로직을 별도의 함수로 분리 시도를 해볼 것 ( 50% 적용 완료 )
- 메뉴 데이타 자체를 별도 파일 또는 구조체를 사용 시도 해볼 것 ( 보류 )

## wrtn chatGPT 3.5
- 주석의 한글+영어 쓰지 말고 한글로 통일 해 볼 것 ( 완료 )
- 메뉴 구성이 깊어질 경우 switch case 구성을 고려 해볼 것 ( 보류 )
- 라이브러리와 하드웨어에 대한 설명 부족(완료)

## Cloava X
- menu 변수 이름을 mainMenu로 변경 고려 해볼 것 (보류 )
- switch case 문으로 간결화 해볼 것 ( 보류 )
- 변수 이름 에 대한 명확성을 확보 해 볼 것
- 적절한 들여쓰기 작성 시도( 완료 )

## original chatGPT
- 상태기반 메뉴 및 서브메뉴 관리
- 반복코드 줄이기 특히'updateMune ' , 추상화 사용 가능 ( 고려중 , 3차 이후에나 가능  )
```c++
void displayMenuScreen(const char *title, const char *option, const char *menuLabel) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(title);
  display.println(option);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.println(menuLabel);
  display.display();
}
```

## Bard
- 함수명 개선(보류)
```text
loop_mainMenuControl -> handleMainMenuControl
loop_subMenuControl -> handleSubMenuControl
loop_backToMainMenu -> handleBackToMainMenu
```
- menu == 0 대신 isInIdleScreen() 함수을 사용 해볼 수 있음
- selectBtn.getCount() == 2 대신 isSelectButtonDoublePressed() 함수를 만들 수 있음
- 복잡한 조건문 에 주석을 추가 할것
  
  

## 대략적인 순서도 

### setup() 함수
SSD1306 디스플레이 및 버튼 초기화
초기 디스플레이 출력


### loop() 함수
버튼 입력 처리 (loop_mainMenuControl, loop_subMenuControl)
메뉴 업데이트 출력 (updateMenu)


### 사용자 별도 함수들
loop_mainMenuControl()
메인 메뉴 수정 및 선택 처리
loop_subMenuControl()
서브 메뉴 진입 처리
loop_backToMainMenu()
서브메뉴에서 메인메뉴로 돌아가기 처리
updateMenu()
현재 메뉴 상태 출력
