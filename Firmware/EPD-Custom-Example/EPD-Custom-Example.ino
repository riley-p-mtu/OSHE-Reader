#include "DEV_Config.h"
#include "EPD.h"
#include "OSHE_Logo.h"
#include "OSHE_Logo_Inverted.h"

void setup()
{
  Serial.begin(115200);

  DEV_Module_Init();
  
  EPD_5IN83_V2_Init();
  EPD_5IN83_V2_Clear();

  delay(500);

  for(int i = 0; i < 3; i++)
  {
    Serial.println("Displaying Original");
    EPD_5IN83_V2_Display(OSHE_Logo_Bitmap);

    Serial.println("Waiting");
    delay(5000);

    Serial.println("Clearing");
    EPD_5IN83_V2_Clear();

    Serial.println("Waiting");
    delay(5000);

    Serial.println("Displaying Inverted");
    EPD_5IN83_V2_Display(OSHE_Logo_Inverted_Bitmap);

    Serial.println("Waiting");
    delay(5000);

    Serial.println("Clearing");
    EPD_5IN83_V2_Clear();

    Serial.println("Waiting");
    delay(5000);
  }

  Serial.println("Sleeping");
  EPD_5IN83_V2_Sleep();
}

void loop()
{
  
}
