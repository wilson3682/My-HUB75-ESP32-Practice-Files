/*******************************************************************

    An Example to show how to center text.

    Parts Used:
      ESP32 Trinity - https://github.com/witnessmenow/ESP32-Trinity

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Written by Brian Lough
      YouTube: https://www.youtube.com/brianlough
      Tindie: https://www.tindie.com/stores/brianlough/
      Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

/// Interesting library to check for text samples: https://github.com/Nca78/Matrix_GFX


// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
// This is the library for interfacing with the display

// Can be installed from the library manager (Search for "ESP32 MATRIX DMA")
// https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA


// ----------------------------
// Dependency Libraries - each one of these will need to be installed.
// ----------------------------

// Adafruit GFX library is a dependency for the matrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

// -------------------------------------
// -------   Matrix Config   ------
// -------------------------------------

const int panelResX = 64;   // Number of pixels wide of each INDIVIDUAL panel module.
const int panelResY = 32;   // Number of pixels tall of each INDIVIDUAL panel module.
const int panel_chain = 1;  // Total number of panels chained one to another

#define ENABLE_DOUBLE_BUFFER 1  // This is a good example to show the difference the
// double buffer makes, it doesn't flash as much
// comment this out to test without it
// See the "displaySetup" method for more display config options

//------------------------------------------------------------------------------------------------------------------

MatrixPanel_I2S_DMA *dma_display = nullptr;

//====================== Variables For scrolling Text=====================================================
unsigned long isAnimationDue;
int delayBetweeenAnimations = 23;             // Smaller == faster
int textXPosition = panelResX * panel_chain;  // Will start off screen
int textYPosition = panelResY / 2 - 7;        // center of screen - 8 (half of the text height)
//====================== Variables For scrolling Text=====================================================


uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);
uint16_t myYELLOW = dma_display->color565(255, 255, 0);
uint16_t myMAGENTA = dma_display->color565(255, 0, 255);
uint16_t myCYAN = dma_display->color565(0, 255, 255);


// Pointers to this variable will be passed into getTextBounds,
// they will be updated from inside the method
int16_t xOne, yOne;
uint16_t w, h;


void displaySetup() {
  HUB75_I2S_CFG mxconfig(
    panelResX,   // module width
    panelResY,   // module height
    panel_chain  // Chain length
  );

  // If you are using a 64x64 matrix you need to pass a value for the E pin
  // The trinity connects GPIO 18 to E.
  // This can be commented out for any smaller displays (but should work fine with it)
  mxconfig.gpio.e = -1;

#ifdef ENABLE_DOUBLE_BUFFER
  // This is how you enable the double buffer.
  // Double buffer can help with animation heavy projects
  mxconfig.double_buff = true;
  //mxconfig.double_buff = false;
#endif

  // May or may not be needed depending on your matrix
  // Example of what needing it looks like:
  // https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA/issues/134#issuecomment-866367216
  mxconfig.clkphase = false;

  // Some matrix panels use different ICs for driving them and some of them have strange quirks.
  // If the display is not working right, try this.
  //mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(120);  //0-255
  dma_display->setTextWrap(false);
  dma_display->clearScreen();
  //dma_display->flipDMABuffer();
}

void setup() {
  Serial.begin(115200);

  displaySetup();
}

uint8_t wheelval = 0;
uint8_t colorWheelBorderOffset = 0;

void loop() {
  //drawBorder(myBLACK);
  //drawBorder(myCYAN);
  drawBorderRainbow(2, 64);

  printTextRainbow(wheelval, "WELCOME", 11, 1);         //Prints text to chosen x y position with rainbow color
  //printTextRainbowCentered(wheelval, "WELCOME", 1);    //Prints text X-Centered to chosen Y position with rainbow color
  //printText(myGREEN, "WELCOME", 11, 1);                //Prints text to chosen x y position and desired color
  //printTextCentered(myGREEN, "WELCOME", 1);            //Prints text X-Centered to chosen Y position and desired color

  scrollText(wheelval, "AND HAVE A WONDERFUL DAY!");    //Prints Scrolling text with a rainbow color

  printTextRainbow(wheelval, "ENJOY IT", 8, 24);        //Prints text to chosen x y position with rainbow color
  //printTextRainbowCentered(wheelval, "ENJOY IT", 24);  //Prints text X-Centered to chosen Y position with rainbow color
  //printText(myMAGENTA, "ENJOY IT!", 8, 24);            //Prints text to chosen x y position and desired color
  //printTextCentered(myMAGENTA, "ENJOY IT!", 24);       //Prints text X-Centered to chosen Y position and desired color
  
  
  //drawText(wheelval, "WELCOME", 11, 1);                //Prints text to chosen x y position and variable color
  //drawTextCentered(wheelval, "WELCOME", 1);            //Prints text X-Centered to chosen Y position and variable color
  
  
  wheelval += 1;
  //colorWheelOffset += 5;
  //scrollString("Hello");
  delay(20);

  //scrollString("I Wish you have a Wonderful day full of Joy and Blessings");
  //scrollString("I Wish you have a Wonderful day!");
}

void drawBorderRainbow(int sPeed, int colors) {  // draw a box in Rainbow color
  dma_display->drawRect(0, 0, dma_display->width(), dma_display->height(), colorWheel(colors + colorWheelBorderOffset));
  colorWheelBorderOffset += sPeed;
}

void drawBorder(uint16_t color) {  // draw a box in a chosen color
  int w = dma_display->width();
  int h;
  dma_display->drawLine(w, 0, 0, 0, color);
  //dma_display->drawChar(1, 1, 'A', myMAGENTA, myMAGENTA, 1);
}

void printTextRainbow(int colorWheelOffset, const char *text, int xPos, int yPos) {
  uint8_t w = 0;
  // draw text with a rotating colour
  dma_display->setTextSize(1);  // size 1 == 8 pixels high

  dma_display->setCursor(xPos, yPos);  // start at top left, with 8 pixel of spacing
  const char *str = text;
  for (w = 0; w < strlen(text); w++) {
    dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
    dma_display->print(text[w]);
  }
}

void printTextRainbowCentered(int colorWheelOffset, const char *text, int yPos) {
  dma_display->setTextSize(1);  // size 1 == 8 pixels high

  // Calculate the width of the text in pixels
  int textWidth = strlen(text) * 6;  // Assuming 6 pixels per character for size 1

  // Center the text horizontally
  int xPos = (dma_display->width() - textWidth) / 2;

  dma_display->setCursor(xPos, yPos);  // Set cursor position for centered text

  // Draw text with a rotating color
  for (uint8_t w = 0; w < strlen(text); w++) {
    dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
    dma_display->print(text[w]);
  }
}

void printTextCentered(uint16_t color, const char *text, int yPos) {
  //uint8_t w = 0;
  dma_display->setTextSize(1);  // size 1 == 8 pixels high
  // Calculate the width of the text in pixels
  int textWidth = strlen(text) * 6;  // Assuming 6 pixels per character for size 1

  // Center the text horizontally
  int xPos = (dma_display->width() - textWidth) / 2;


  dma_display->setCursor(xPos, yPos);  // start at top left, with 8 pixel of spacing
  const char *str = text;
  dma_display->setTextColor(color);
  dma_display->print(str);
}


void printText2(uint16_t color, const char *text, int xPos, int yPos) {
  uint8_t w = 0;
  dma_display->setTextSize(1);         // size 1 == 8 pixels high
  dma_display->setCursor(xPos, yPos);  // start at top left, with 8 pixel of spacing
  const char *str = text;
  dma_display->setTextColor(color);
  dma_display->print(str);
}


//===Code taken from: https://github.com/witnessmenow/ESP32-Trinity/blob/master/examples/BuildingBlocks/Text/ScrollingText/ScrollingText.ino===========================
//
void scrollText(int colorWheelOffset, const char *text) {
  //dma_display->flipDMABuffer(); // not used if double buffering isn't enabled
  //  delay(25);
  //  dma_display->clearScreen();

  //String text = "AND HAVE A WONDERFUL DAY!";
  //const char *str = "AND HAVE A WONDERFUL DAY!";
  const char *str = text;
  byte offSet = 25;
  unsigned long now = millis();
  if (now > isAnimationDue) {

    dma_display->setTextSize(2);  // size 2 == 16 pixels high
    //dma_display->setTextColor(myBLUE);
    isAnimationDue = now + delayBetweeenAnimations;

    textXPosition -= 1;

    // Checking is the very right of the text off screen to the left
    dma_display->getTextBounds(str, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
    if (textXPosition + w <= 0) {
      textXPosition = dma_display->width() + offSet;
    }

    dma_display->setCursor(textXPosition, textYPosition);
    dma_display->drawRect(1, textYPosition, dma_display->width() - 2, 14, myBLACK);
    dma_display->fillRect(1, textYPosition, dma_display->width() - 2, 14, myBLACK);
    uint8_t w = 0;
    for (w = 0; w < strlen(str); w++) {
      dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
      dma_display->print(str[w]);
    }

    dma_display->flipDMABuffer();  // not used if double buffering isn't enabled
  }
}

void scrollString(String str) {
  dma_display->getTextBounds(str, textXPosition, textYPosition, &xOne, &yOne, &w, &h);
  int yoff = 1;

  dma_display->setTextSize(2);

  int charWidth = 10;  // textsize 2 @todo auto calculate charwidth from font

  int pxwidth = w;
  for (int16_t x = dma_display->width(); x >= 0 - w; x--) {
    dma_display->clearScreen();
    dma_display->setTextColor(myMAGENTA);
    dma_display->setCursor(x, yoff);

    dma_display->print(str);
    delay(25);
  }
  dma_display->flipDMABuffer();  // not used if double buffering isn't enabled
}


void drawText(int colorWheelOffset, const char *text, int xPos, int yPos) {
  // dma_display->flipDMABuffer(); // not used if double buffering isn't enabled
  // draw text with a rotating colour
  dma_display->setTextSize(1);  // size 1 == 8 pixels high

  dma_display->setCursor(xPos, yPos);  // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  //const char *str = "ESP32 DMA";
  const char *str = text;
  for (w = 0; w < strlen(str); w++) {
    dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
    dma_display->print(str[w]);
  }
}

void drawTextCentered(int colorWheelOffset, const char *text, int yPos) {
  // dma_display->flipDMABuffer(); // not used if double buffering isn't enabled
  // draw text with a rotating colour
  dma_display->setTextSize(1);       // size 1 == 8 pixels high
                                     // Calculate the width of the text in pixels
  int textWidth = strlen(text) * 6;  // Assuming 6 pixels per character for size 1

  // Center the text horizontally
  int xPos = (dma_display->width() - textWidth) / 2;


  dma_display->setCursor(xPos, yPos);  // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  //const char *str = "ESP32 DMA";
  const char *str = text;
  for (w = 0; w < strlen(str); w++) {
    dma_display->setTextColor(colorWheel((w * 32) + colorWheelOffset));
    dma_display->print(str[w]);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
  if (pos < 85) {
    return dma_display->color565(pos * 3, 255 - pos * 3, 0);
  } else if (pos < 170) {
    pos -= 85;
    return dma_display->color565(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return dma_display->color565(0, pos * 3, 255 - pos * 3);
  }
}
