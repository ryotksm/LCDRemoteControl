#include <SPI.h>
#include "Small_GFX.h"
#include "Small_TFTLCD.h"

#define LCD_CS A0 // Chip Select goes to Analog 3
#define LCD_CD A1 // Command/Data goes to Analog 2
#define LCD_WR A2 // LCD Write goes to Analog 1
#define LCD_RD A3 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

//   D0 connects to digital pin 8
//   D1 connects to digital pin 9
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define PROMPT ">"

typedef enum {
  CMDID_drawFastVLine = 1,
  CMDID_drawFastHLine = 2,
  CMDID_fillRect = 3,
  CMDID_fillScreen = 4,
  CMDID_drawLine = 5,
  CMDID_drawRect = 6,
  CMDID_drawCircle = 7,
  CMDID_fillCircle = 8,
  CMDID_drawTriangle = 9,
  CMDID_fillTriangle = 10,
  CMDID_drawRoundRect = 11,
  CMDID_fillRoundRect = 12,
  CMDID_drawBitmap = 13,
  CMDID_drawXBitmap = 14,
  CMDID_drawGrayscaleBitmap = 15,
  CMDID_drawRGBBitmap = 16,
  CMDID_drawChar = 17,
  CMDID_setCursor = 18,
  CMDID_setTextColor = 19,
  CMDID_setTextSize = 20,
  CMDID_setTextWrap = 21,
  CMDID_cp437 = 22,
  CMDID_setFont = 23,
  CMDID_getTextBounds = 24,
  CMDID_setRotation = 25,
  CMDID_invertDisplay = 26,
  CMDID_print = 27,
  CMDID_println = 28,
  CMDID_drawPixel = 29,
  CMDID_drawCircleHelper = 30,
  CMDID_UNKNOWN = 0
} CMDID;

int lf = 10;    // ASCIIの改行
String cmds[10] = {"\0"}; // 分割された文字列を格納する配列

Small_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int split(String data, char delimiter, String *dst) {
  int index = 0;
  int arraySize = (sizeof(data) / sizeof((data)[0]));
  int datalength = data.length();
  for (int i = 0; i < datalength; i++) {
    char tmp = data.charAt(i);
    if ( tmp == delimiter ) {
      index++;
      if ( index > (arraySize - 1)) return -1;
    }
    else dst[index] += tmp;
  }
  return (index + 1);
}

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("LCD Remote Control"));

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);

  pinMode(MISO, OUTPUT);            //送信ポートを出力にする

  SPI.setBitOrder(MSBFIRST);        //LSBFIRST / MSBFIRST
  SPI.setDataMode(SPI_MODE3 );      //立ち上がりでラッチ
  //SPCR |= _BV(SPE);                 //SPI Enable
  SPI.attachInterrupt();            //SPI割り込み開始

  Serial.println(F(PROMPT));
}

// SPI割り込み処理

ISR (SPI_STC_vect) {
  byte cc = SPDR;    //SPIの受信バッファから取得
  cc++;
  SPDR = cc;      //送信データ格納
  Serial.println(cc);//シリアル出力
}

void loop(void) {
  if (Serial.available() > 0)
  {
    String str = Serial.readStringUntil(';');
    // 分割数 = 分割処理(文字列, 区切り文字, 配列)
    int index = split(str, ',', cmds);
    // 結果表示
    int cmdid = cmds[0].toInt();
    switch (cmdid) {
      case CMDID_fillRect:
        // fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
        tft.fillRect(cmds[1].toInt(), cmds[2].toInt(),cmds[3].toInt(), cmds[4].toInt(),cmds[5].toInt());
        break;
      case CMDID_fillScreen:
        // fillScreen(uint16_t color)
        tft.fillScreen(cmds[1].toInt());
        break;
      case CMDID_drawLine:
        // drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
        tft.drawLine(cmds[1].toInt(), cmds[2].toInt(),cmds[3].toInt(), cmds[4].toInt(),cmds[5].toInt());
        break;
      case CMDID_drawRect:
        // drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
        tft.drawRect(cmds[1].toInt(), cmds[2].toInt(),cmds[3].toInt(), cmds[4].toInt(),cmds[5].toInt());
        break;
      case CMDID_drawCircle:
        // drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
        tft.drawCircle(cmds[1].toInt(), cmds[2].toInt(),cmds[3].toInt(), cmds[4].toInt());
        break;
      case CMDID_fillCircle:
        // fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
        tft.fillCircle(cmds[1].toInt(), cmds[2].toInt(),cmds[3].toInt(), cmds[4].toInt());
        break;
      case CMDID_setCursor:
        // setCursor(int16_t x, int16_t y)
        tft.setCursor(cmds[1].toInt(), cmds[2].toInt());
        break;
      case CMDID_setTextColor:
        // setTextColor(uint16_t c)
        tft.setTextColor(cmds[1].toInt());
        break;
      case CMDID_setTextSize:
        // setTextSize(uint8_t s)
        tft.setTextSize(cmds[1].toInt());
        break;
      case CMDID_setRotation:
        // setRotation(uint8_t r)
        tft.setRotation(cmds[1].toInt());
        break;
      case CMDID_invertDisplay:
        // invertDisplay(boolean i)
        cmds[1].toUpperCase();
        tft.setTextSize(cmds[1].equals("TRUE"));
        break;
      case CMDID_print:
        // print(const String &str)
        tft.print(cmds[1]);
        break;
      case CMDID_println:
        // println(const String &str)
        tft.println(cmds[1]);
        break;
      case CMDID_drawPixel:
        // drawPixel(int16_t x, int16_t y, uint16_t color)
        tft.drawPixel(cmds[1].toInt(), cmds[2].toInt(), cmds[3].toInt());
        break;
      case CMDID_drawCircleHelper:
        // drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color)
        tft.drawCircleHelper(cmds[1].toInt(), cmds[2].toInt(), cmds[3].toInt(), cmds[4].toInt(), cmds[5].toInt());
        break;
      default:
        Serial.println(F("???"));
    }
    for (int i = 0; i < index; i++) cmds[i] = "\0";
    Serial.println(F(PROMPT));
  }
}
