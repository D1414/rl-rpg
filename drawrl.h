#ifndef DRAWRL_H

#define DRAWRL_H

#include "game.h"
#include <raylib.h>

#define RIGHT_WIDTH(x) (100.f / 2560 * x / 100.f * GetScreenWidth())
#define RIGHT_HEIGHT(x) (100.f / 1440 * x / 100.f * GetScreenHeight())

#define PLAYER_RECTW RIGHT_WIDTH(200)
#define PLAYER_RECTH RIGHT_HEIGHT(200)

#define FONT_FACTOR GetScreenWidth() / 1440.f
#define FONTSIZE 50

#define MAPMIN 0
#define MAPMAXW RIGHT_WIDTH(10000)
#define MAPMAXH RIGHT_HEIGHT(10000)
#define SHOPWINDOWW RIGHT_WIDTH(800)
#define SHOPWINDOWH RIGHT_HEIGHT(1200)
// GetScreenWidth() * 0.15f

// Minimap Modes zum auswaehlen welche minimap angezeigt werden soll
typedef enum { MINIMAP_OFF, MINIMAP_SMALL, MINIMAP_BIG } MinimapMode;

// ShopItem struct zum einfachen einfuegen von items in den shop
typedef struct {
  Rectangle rec;
  Color color;
  const char *text;
  int fontsize;
} ShopItem;
// Minimap Funktionen
void drawMinimap(MinimapMode mode, Vector2 playerposition, int mapWidth,
                 int mapHeight);

// Shop Funktionen
void drawShopRec(Rectangle shop);
void drawShopItems(bool shopOpen);
void drawShop(bool shopOpen);

// Button Funktionen
void highlightButton(Rectangle *rect);
void drawTextOnRec(Rectangle rec, const char *text, int size);

// Pause-Menü Funktionen
void drawMuteButton(Rectangle muteButton, bool *isMuted, float *bgMusicVolume);
void drawSlider(Rectangle volumeSlider, float bgMusicVolume);
void drawFullscreen(Texture2D fullscreen1, Texture2D fullscreen2, bool *isFull);
void drawExit(Rectangle exitButton);

#endif // !DRAWRL_H
