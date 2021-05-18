/// Analog mouse by Edward Gibson


#include <AbsMouse.h>
#include <Keyboard.h>

uint32_t lastMillis;
int CurrentState = 1;
int CurrentStateD;

typedef struct {
  const int pin;
  const char key;
  uint8_t isButtonUp;
  void (*pressAction)(const char key);
  void (*releaseAction)(const char key);
} ButtonState_t;

void pressMouse(const char key)
{
  AbsMouse.press(key);
//  Serial.print("Press Mouse ");
//  Serial.println((uint8_t)key);
}

void releaseMouse(const char key)
{
  AbsMouse.release(key);
//  Serial.print("Release Mouse ");
//  Serial.println((uint8_t)key);
}

void pressButton(const char key)
{
  Keyboard.press(key);
//  Serial.print("Press Button ");
//  Serial.println(key);
}

void releaseButton(const char key)
{
  Keyboard.release(key);
//  Serial.print("Release Button ");
//  Serial.println(key);
}

void releaseButtonAll(const char key)
{
  Keyboard.releaseAll();
}

void resetMousePosition(void)
{
  AbsMouse.move(0, 0);
 // Serial.println("Reset mouse");
}

void pressBombbaydoors(const char key)
{
  #if 1
  //Serial.print(CurrentState);
  if (CurrentState == 1){
  for (int i = 1; i<5;i++){
     Keyboard.press('c');
     //Keyboard.press('1');
     Serial.println(i);
     Serial.print(" C's");
     Keyboard.release('c');
     delay(100);
   }
    CurrentStateD = 2;
  }
  #endif
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_F1);
  Keyboard.releaseAll();
  //pressButton(key);
  #if 1
  if (CurrentState == 2){
    for (int i = 0;i < 1; i++){
    Keyboard.press('c');
    //Serial.println("Pressed C");
    CurrentStateD = 1;
    //Serial.println(CurrentState);
    Keyboard.release('c');
    }
  }
  #endif
 CurrentState = CurrentStateD;
 //Serial.println(CurrentState);
} 
void dropBomb(const char key)
{
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.print(' ');
//  Keyboard.releaseAll();
}

void resetState(const char key)
{
  CurrentState = 1;
}

void pressReset(const char key)
{
  resetMousePosition();
}

void releaseReset(char const key)
{
  // nothing
}

ButtonState_t stateReset = 
{
  .pin = 7,
  .key = 'a',
  .isButtonUp = true,
  .pressAction = resetState,
  .releaseAction = releaseButton,
};

ButtonState_t LClick =
{
  .pin = A2,
  .key = MOUSE_LEFT,
  .isButtonUp = true,
  .pressAction = pressMouse,
  .releaseAction = releaseMouse,
};

ButtonState_t bombDrop =
{
  .pin = A3,
  .key = 'b',
  .isButtonUp = true,
  .pressAction = dropBomb,
  .releaseAction = releaseButtonAll,
};

ButtonState_t bombBaydoors =
{
  .pin = 15,
  .key = 'd',
  .isButtonUp = true,
  .pressAction = pressBombbaydoors,
  .releaseAction = releaseButton,
};

ButtonState_t gunReset =
{
  .pin = 16,
  .key = 0,
  .isButtonUp = true,
  .pressAction = pressReset,
  .releaseAction = releaseReset,
};

void checkButtonState(ButtonState_t *pButton)
{
  uint8_t buttonValue;
 
  buttonValue = digitalRead(pButton->pin);
  if (buttonValue != pButton->isButtonUp)
  {
    pButton->isButtonUp = buttonValue;
    if (pButton->isButtonUp)
      pButton->releaseAction(pButton->key);
    else
      pButton->pressAction(pButton->key);
  }
}
void setup() {  
AbsMouse.init(80,45); //Originally 1920 x 1080
Keyboard.begin();
pinMode(A0,INPUT);
pinMode(A1, INPUT);
Serial.begin(9600);
pinMode (gunReset.pin, INPUT);
pinMode (LClick.pin, INPUT);
pinMode (bombDrop.pin, INPUT);
pinMode (bombBaydoors.pin, INPUT);
pinMode(stateReset.pin,INPUT);
pinMode (14, INPUT);//KillSwitch
lastMillis = 0;
}


int aState;
int aState2;
int aLastState = 0;
int aLastState2 = 0;
int mousex = 0;
int mousey = 0;
int lastMousex = 0;
int lastMousey = 0;
int deter = 0;

void loop() {
//if (digitalRead(14) == HIGH){
aState = analogRead(A0);
if (aState != aLastState){
  mousex = map(aState, 0, 1023, 80, 0); 
}
aState2 = analogRead(A1);
if (aState2 != aLastState2){
  mousey = map(aState2, 0, 1023, 0, 45);
}
aState = aLastState;
aState2 = aLastState2;

#if 0
Serial.print("x: ");
Serial.println(mousex);
Serial.print("y: ");
Serial.println(mousey);
#endif


if  (mousex != lastMousex || mousey != lastMousey){
  #if 0
  Serial.print("x: ");
  Serial.print(mousex);
  Serial.print("     y:");
  Serial.println(mousey);
  #endif
  AbsMouse.move(mousex, mousey);

  lastMousex = mousex;
  lastMousey = mousey;
}
    if (millis() - lastMillis > 25)
    {
      checkButtonState(&LClick);
      checkButtonState(&bombDrop);
      checkButtonState(&bombBaydoors);
      checkButtonState(&gunReset);
      lastMillis = millis();
    }    
  //}
  //else{
    //Do Nothing
  //}
}
