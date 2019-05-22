#ifndef _SMALL_GFX_H
#define _SMALL_GFX_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include "gfxfont.h"

/// A generic graphics superclass that can handle all sorts of drawing. At a minimum you can subclass and provide drawPixel(). At a maximum you can do a ton of overriding to optimize. Used for any/all Adafruit displays!
class Small_GFX : public Print {

  public:

    Small_GFX(int16_t w, int16_t h); // Constructor

    // This MUST be defined by the subclass:
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;    ///< Virtual drawPixel() function to draw to the screen/framebuffer/etc, must be overridden in subclass. @param x X coordinate.  @param y Y coordinate. @param color 16-bit pixel color.

    // TRANSACTION API / CORE DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void startWrite(void);
    virtual void writePixel(int16_t x, int16_t y, uint16_t color);
    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void endWrite(void);

    // CONTROL API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(boolean i);

    // BASIC DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void
    // It's good to implement those, even if using transaction API
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
                  drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
                  fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
                  fillScreen(uint16_t color),
                  // Optional and probably not necessary to change
                  drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
                  drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    // These exist only with Small_GFX (no subclass overrides)
    void
    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
               drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color),
               fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
               fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color),
               drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
               fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color),
               drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
               fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color),
               drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color),
               drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size),
               setCursor(int16_t x, int16_t y),
               setTextColor(uint16_t c),
               setTextColor(uint16_t c, uint16_t bg),
               setTextSize(uint8_t s),
               setTextWrap(boolean w),
               cp437(boolean x = true),
               setFont(const GFXfont *f = NULL),
               getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h),
               getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h),
               getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);


#if ARDUINO >= 100
    virtual size_t write(uint8_t);
#else
    virtual void   write(uint8_t);
#endif

    int16_t height(void) const;
    int16_t width(void) const;

    uint8_t getRotation(void) const;

    // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
    int16_t getCursorX(void) const;
    int16_t getCursorY(void) const;

  protected:
    void
    charBounds(char c, int16_t *x, int16_t *y,
               int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
    const int16_t
    WIDTH,          ///< This is the 'raw' display width - never changes
    HEIGHT;         ///< This is the 'raw' display height - never changes
    int16_t
    _width,         ///< Display width as modified by current rotation
    _height,        ///< Display height as modified by current rotation
    cursor_x,       ///< x location to start print()ing text
    cursor_y;       ///< y location to start print()ing text
    uint16_t
    textcolor,      ///< 16-bit background color for print()
    textbgcolor;    ///< 16-bit text color for print()
    uint8_t
    textsize,       ///< Desired magnification of text to print()
    rotation;       ///< Display rotation (0 thru 3)
    boolean
    wrap,           ///< If set, 'wrap' text at right edge of display
    _cp437;         ///< If set, use correct CP437 charset (default is off)
    GFXfont
    *gfxFont;       ///< Pointer to special font
};


#endif // _SMALL_GFX_H
