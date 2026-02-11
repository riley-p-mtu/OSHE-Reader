
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

// Create a group and add widgets to it so they can be selected by the keypad
  lv_group_t *g = lv_group_create();

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
  lv_indev_set_read_cb(indev, keyboard_read);
  lv_indev_set_group(indev, g);
}

void keyboard_read(lv_indev_t *indev, lv_indev_data_t *data) {
  if(key_pressed()) {
    data->key = my_last_key();            /* Get the last pressed or released key */
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void loop(){


  uint8_t my_input_read(void)
  {
    if (upBtnState == LOW && digitalRead(upBtn)) {
    lv_obj_t * btn = lv_button_create(lv_screen_active());
    lv_obj_add_event_cb(btn, my_event_cb, LV_EVENT_CLICKED, user_data);
    }
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