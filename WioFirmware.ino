#define BUZZER_PIN BUZZER_CTR /* sig pin of the buzzer */

#include "background.h"
#include "victory.h"
#include <TFT_eSPI.h>
#undef min
#include "Ticker.h"

TFT_eSPI tft = TFT_eSPI(); 

const int c = 1915;
const int d = 1700;
const int e = 1519;
const int f = 1432;
const int g = 1275;
const int a = 1136;
const int b = 1014;
const int C = 956;


struct MF{
  int Pause;
  int Note;
  int Tempo;
};



MF melody[] = {
  MF{Pause: 50, Note: d, Tempo: 400},
  MF{Pause: 50, Note: b, Tempo: 200},
  MF{Pause: 50, Note: a, Tempo: 400},
  MF{Pause: 50, Note: d, Tempo: 200},
  MF{Pause: 50, Note: b, Tempo: 400},
  MF{Pause: 50, Note: f, Tempo: 200},
  MF{Pause: 50, Note: C, Tempo: 100},
  MF{Pause: 50, Note: d, Tempo: 100},
  MF{Pause: 50, Note: d, Tempo: 100},
  MF{Pause: 50, Note: e, Tempo: 400},
  MF{Pause: 50, Note: g, Tempo: 200},
  MF{Pause: 50, Note: c, Tempo: 200},
  MF{Pause: 50, Note: d, Tempo: 200},
  MF{Pause: 50, Note: b, Tempo: 400},
  MF{Pause: 50, Note: a, Tempo: 200},
  MF{Pause: 50, Note: d, Tempo: 400},
  MF{Pause: 50, Note: b, Tempo: 200},
  MF{Pause: 50, Note: f, Tempo: 400},
  MF{Pause: 50, Note: c, Tempo: 200},
  MF{Pause: 50, Note: d, Tempo: 400},
  MF{Pause: 50, Note: d, Tempo: 100},
  MF{Pause: 50, Note: e, Tempo: 100},
  MF{Pause: 50, Note: g, Tempo: 100},
  MF{Pause: 50, Note: C, Tempo: 400},
  MF{Pause: 0, Note: 0, Tempo: 200},
};

int len = 0;

#define keyLen 20


const char *softwareNames[8] = {
  "Windows ",
  "Adobe ",
  "Nero ",
  "Sony ",
  "Fargus ",
  "EA ",
  "Bungie ",
  "Shaker ",
};

const char *softwareSurNames[9] = {
  "XP",
  "Photoshop",
  "Burning ROM",
  "Office",
  "Quiz",
  "RedAlert",
  "mIRC",
  "ImageEdging",
  "ICQ",
};

void renderKeygen() {
    const char *Key = "KEY: ";
    const char *Name = "NAME: ";
    const char *VER = "VER: ";
    const char *space = "                                   ";
    Serial.println("Start render");
    tft.setTextColor(TFT_BLACK, TFT_RED);
    tft.setTextSize(2);
    char pkey[100];
    strcpy(pkey, Key);
    char str[keyLen];
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-";
    for (int i=0;i<keyLen;i++) {
        Serial.printf("I: %d\n",i);
        int j = random(0, sizeof charset -1);
        Serial.printf("J: %d\n", j);
        str[i]=charset[j];
        str[i+1] = '\0';
        Serial.printf("Generated: %s\n", str);
     };
    Serial.printf("Generated key: %s\n",str);
    strcat(pkey, str);
    strcat(pkey, space);
    tft.drawString(pkey, 10,108);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    char name[100];
    strcpy(name, Name);
    strcat(name, softwareNames[random(0,7)]);
    strcat(name, softwareSurNames[random(0,8)]);
    strcat(name, space);
    tft.drawString(name,10, 165 );
    tft.drawString("VER: 20.032.123", 10, 185);
    tft.setTextSize(1);
    tft.drawString("(c) Drunked team", 210, 230);
//    tft.drawString(1, 210, 230);
}

Ticker tickerObject(renderKeygen, 1000);

void setup() {
    Serial.begin(115200);
    //while (!Serial);
    Serial.println("initialize");
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);


  
    tft.begin();               // Initialise the display
    tft.fillScreen(TFT_BLACK); // Black screen fill
    tft.setSwapBytes(true);
    tft.setRotation(1);
    tft.drawXBitmap(0,0,bg_bits,bg_width,bg_height,TFT_BLACK, TFT_RED);
    renderKeygen();
    tickerObject.start();
    //set buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);

    for(len = 0; true; len++){
      if (melody[len].Note == 0){
        len--;
        break;
      }
    }
}





int playMusic = 0;
int curIndex = 0;
int paused = 0;
int victory = 0;
int aPressed = 0;
int bPressed = 0;

void loop() {
    if (victory == 0) {
      tickerObject.update();
    } 
    
    if (curIndex == len){
        curIndex=random(0,len-1);
    }
    if (playMusic) {
      for(; curIndex < len; curIndex++) {
          if (melody[curIndex].Pause > 0 && paused == 0) {
              paused = 1;
              break;
          }
          paused=0;
          delay(melody[curIndex].Pause);
          playTone(melody[curIndex].Note, melody[curIndex].Tempo);
      }
    }
    if (digitalRead(WIO_KEY_A) == LOW && aPressed == 0){
      if (playMusic){
        playMusic = 0;
      } else {
        playMusic = 1;
      }
      aPressed = 1;
    }
    if (digitalRead(WIO_KEY_A) == HIGH && aPressed == 1) {
      aPressed = 0;
    }
    if (digitalRead(WIO_KEY_B) == LOW && bPressed == 0){
      if (victory){
        victory = 0;
        tft.drawXBitmap(0,0,bg_bits,bg_width,bg_height,TFT_BLACK, TFT_RED);
      } else {
        victory = 1;
        tft.drawXBitmap(0,0,victory_bits,victory_width,victory_height,TFT_BLACK, TFT_RED);
      }
      bPressed = 1;
    }
    if (digitalRead(WIO_KEY_B) == HIGH && bPressed == 1) {
      bPressed = 0;
    }
}

//Play tone
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(tone);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}
