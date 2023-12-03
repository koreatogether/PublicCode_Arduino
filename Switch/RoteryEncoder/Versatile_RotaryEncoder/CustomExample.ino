/*
koreatogetherfactory 의해서 편집됨
*/

#include <Versatile_RotaryEncoder.h>

// SET READING PINS ACCORDINGLY TO YOUR ENCODER TO BOARD CONNECTIONS
// Set here your encoder reading pins (Ex.: EC11 with breakout board)
#define clk 2  // (A3)
#define dt 3   // (A2)
#define sw 4   // (A4)

// Functions prototyping to be handled on each Encoder Event
void handleRotate(int8_t rotation);
void handlePress();
void handlePressRelease();

// Create a global pointer for the encoder object
Versatile_RotaryEncoder *versatile_encoder;

int count = 0;

void setup()
{
  Serial.begin(9600);
  initRotoryEncoder();
}

void loop()
{

  // Do the encoder reading and processing
  if (versatile_encoder->ReadEncoder())
  {
    // Do something here whenever an encoder action is read
  }
}


void initRotoryEncoder()  // setup이 복잡해서 따로 빼놨습니다.
{
  versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);
  versatile_encoder->setHandleRotate(handleRotate);
  versatile_encoder->setHandlePress(handlePress);
  versatile_encoder->setHandlePressRelease(handlePressRelease);
  Serial.println("RotoryEncoder is Ready!");
}

// Implement your functions here accordingly to your needs

void handleRotate(int8_t rotation)
{
  Serial.print("#1 Rotated: ");
  if (rotation > 0)
  {
    Serial.println("Right");
    count++;
    Serial.println(count);
  }
  else
  {
    Serial.println("Left");
    count--;
    Serial.println(count);
  }
}

void handlePress()
{
  Serial.println("#4.1 Pressed");
}

void handlePressRelease()
{
  Serial.println("#5 Press released");
}