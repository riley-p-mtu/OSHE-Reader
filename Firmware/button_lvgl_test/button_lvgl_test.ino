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

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEX_TYPE_BUTTON);
  lv_indev_set_read_cb(indev, my_input_read);
}

static void btn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target_obj(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}

/**
 * Create a button with a label and react on click event.
 */
void lv_example_get_started_2(void)
{
    lv_obj_t * btn = lv_button_create(lv_screen_active());     /*Add a button the current screen*/
    lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
    lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

    lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Button");                     /*Set the labels text*/
    lv_obj_center(label);
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