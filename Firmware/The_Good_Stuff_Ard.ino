#include "FS.h"
#include "SD.h"
#include "SPI.h"

const int upBut = 5;
const int downBut = 6;
const int rightBut = 7;
const int leftBut = 0;
const int hBut = 1;
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

  listDir(SD, "/", 0);
  writeFile(SD, "/hello.txt", "Hello World\n");
  readFile(SD, "/hello.txt");
  listDir(SD, "/", 0);
  renameFile(SD, "/hello.txt", "/foo.txt");
  deleteFile(SD, "/foo.txt");
  listDir(SD, "/", 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
