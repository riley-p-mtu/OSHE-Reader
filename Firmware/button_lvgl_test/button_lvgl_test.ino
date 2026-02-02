// Number for button pins
const int upBtn = 5;
const int downBtn = 11;
const int rightBtn = 7; 
const int leftBtn = 4;
const int homeBtn = 10;

// Variables for reading pushbutton status
int upBtnState = 0;
int downBtnState = 0;
int rightBtnState = 0;
int leftBtnState = 0;
int homeBtnState = 0;

void setup(){
// Initialize button pins as input
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(rightBtn, INPUT);
  pinMode(leftBtn, INPUT);
  pinMode(homeBtn, INPUT);
}

void loop(){
  if (upBtnState == LOW && digitalRead(upBtn)) {
    
  }

  if (downBtnState == LOW && digitalRead(downBtn)) {

  }

  if (rightBtnState == LOW && digitalRead(rightBtn)) {
    
  }

  if (leftBtnState == LOW && digitalRead(leftBtn)) {
    
  }

  if (homeBtnState == LOW && digitalRead(homeBtn)) {
    
  }

  upBtnState = digitalRead(upBtn);
  downBtnState = digitalRead(downBtn);
  rightBtnState = digitalRead(rightBtn);
  leftBtnState = digitalRead(leftBtn);
  homeBtnState = digitalRead(homeBtn);
}