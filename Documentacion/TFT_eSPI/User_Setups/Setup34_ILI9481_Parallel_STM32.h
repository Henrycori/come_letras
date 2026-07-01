// See SetupX_Template.h for all options available
#define USER_SETUP_ID 34

#define TFT_PARALLEL_8_BIT

//#define ILI9488_DRIVER
#define ILI9481_DRIVER
//#define ILI9486_DRIVER

// ESP32 pins used for UNO format board

#define TFT_CS   33
#define TFT_DC   18
#define TFT_RST  32
#define TFT_WR   19
#define TFT_RD   23

#define TFT_D0   13
#define TFT_D1   12  
#define TFT_D2   14  
#define TFT_D3   27  
#define TFT_D4   25  
#define TFT_D5   15  
#define TFT_D6   2  
#define TFT_D7   4

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT
