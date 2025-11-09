#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Button input, writing to SD card directory

// Number for button pins
const int upBtn = 5;
const int downBtn = 6;
const int rightBtn = 7; 
const int leftBtn = 8;
const int homeBtn = 10;

// Variables for reading pushbutton status
int upBtnState = 0;
int downBtnState = 0;
int rightBtnState = 0;
int leftBtnState = 0;
int homeBtnState = 0;

//Uncomment and set up if you want to use custom pins for the SPI communication
#define REASSIGN_PINS
int sck = 21;
int miso = 20;
int mosi = 19;
int cs = 18;


void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void setup() {
  // put your setup code here, to run once:
    delay(1000);

  Serial.begin(115200);

#ifdef REASSIGN_PINS
  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)) {
#else
  if (!SD.begin()) {
#endif
    Serial.printf("sck = %d, miso = %d, mosi = %d, cs = %d\n", sck, miso, mosi, cs);
    Serial.println("Card Mount Failed");
    return;
  }

  Serial.printf("Test?\n");

  // Initialize button pins as input
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(rightBtn, INPUT);
  pinMode(leftBtn, INPUT);
  pinMode(homeBtn, INPUT);

  listDir(SD, "/", 0);
  createDir(SD, "/inputDir");
  writeFile(SD, "/inputDir/InputTest.txt", "Input Test:\n");
  listDir(SD, "/", 0);
  readFile(SD, "/inputDir/InputTest.txt");

  // listDir(SD, "/", 0);
  // writeFile(SD, "/hello.txt", "Hello World\n");
  // readFile(SD, "/hello.txt");
  // listDir(SD, "/", 0);
  // renameFile(SD, "/hello.txt", "/foo.txt");
  // deleteFile(SD, "/foo.txt");
  // listDir(SD, "/", 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  upBtnState = digitalRead(upBtn);
  downBtnState = digitalRead(downBtn);
  rightBtnState = digitalRead(rightBtn);
  leftBtnState = digitalRead(leftBtn);
  homeBtnState = digitalRead(homeBtn);
  
  if (upBtnState == HIGH) {
    appendFile(SD, "/inputDir/InputTest.txt", "Up\n");
  }

  if (downBtnState == HIGH) {
    appendFile(SD, "/inputDir/InputTest.txt", "Down\n");
  }

  if (rightBtnState == HIGH) {
    appendFile(SD, "/inputDir/InputTest.txt", "Right\n");
  }

  if (leftBtnState == HIGH) {
    appendFile(SD, "/inputDir/InputTest.txt", "Left\n");
  }

  if (homeBtnState == HIGH) {
    appendFile(SD, "/inputDir/InputTest.txt", "Home\n");
    readFile(SD, "/inputDir/InputTest.txt");
  }
}
