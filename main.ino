/**
 * author @solaris0616
 * Need to clone these libraries.
 * $ cd ~/Arduino/libraries
 * $ git clone https://github.com/earlephilhower/ESP8266Audio
 * $ git clone https://github.com/Gianbacchio/ESP8266_Spiram
 */

#include <M5Stack.h>
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"

#define MAX_FILE_NAME 80
char filename[MAX_FILE_NAME];
String s;

AudioGeneratorMP3 *mp3;
AudioGeneratorWAV *wav;
AudioFileSourceSD *file;
AudioFileSourceID3 *id3;
AudioOutputI2S *out;

String randomString(int min, int max) {
  long rnd = random(min, max);
  return String(rnd);
}

void setup(){
  M5.begin();

  // random generator
  randomSeed(analogRead(0));

  // LCD display (1.jpg, 2.jpg, ..., 9.jpg)
  s = String("/img/" + randomString(1, 10) + ".jpg");
  s.toCharArray(filename, MAX_FILE_NAME);
  M5.Lcd.clear();
  M5.Lcd.drawJpgFile(SD, filename);

  // player initialize
  out = new AudioOutputI2S(0, 1);
  out->SetOutputModeMono(true);
  out->SetGain(0.7);
  wav = new AudioGeneratorWAV();
  mp3 = new AudioGeneratorMP3();

  delay(100);
  // ohayo play (0.mp3, 1.mp3, 2.mp3, ... 18.mp3)
  s = String("/ohayo/" + randomString(0, 19) + ".mp3");
  s.toCharArray(filename, MAX_FILE_NAME);
  file = new AudioFileSourceSD(filename);
  id3 = new AudioFileSourceID3(file);
  mp3->begin(id3, out);
  while(mp3->isRunning()) {
    if(!mp3->loop()) mp3->stop();
  }
  delay(500);
}

void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()){
    file = new AudioFileSourceSD("/alexa.wav");
    wav->begin(file, out);
    while(wav->isRunning()) {
      if(!wav->loop()) wav->stop();    
    }
    delay(500);
  } else if(M5.BtnB.wasPressed()) {
    s = String("/voice/" + randomString(0, 109) + ".mp3");
    s.toCharArray(filename, MAX_FILE_NAME);
    file = new AudioFileSourceSD(filename);
    id3 = new AudioFileSourceID3(file);
    mp3->begin(id3, out);
    while(mp3->isRunning()) {
      if(!mp3->loop()) mp3->stop();
    }
    delay(500);
  } else if(M5.BtnC.wasPressed()) {
    s = String("/img/" + randomString(1, 10) + ".jpg");
    s.toCharArray(filename, MAX_FILE_NAME);
    M5.Lcd.clear();
    M5.Lcd.drawJpgFile(SD, filename);
    delay(500);
  }
}
