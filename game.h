#ifndef GAME_H

#define GAME_H

#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Unterschiedliche gamestates die entscheiden was angezeigt wird, und welche
 * inputs ausgefuehrt werden duerfen.
 */
typedef enum { GAME_RUNNING, GAME_PAUSED, GAME_SHOP } GameState;

// ShopItem struct zum einfachen einfuegen von items in den shop
typedef struct {
  Rectangle rec;
  Color color;
  const char *text;
  int fontsize;
} ShopItem;

// Funktionsprototypen

// Eingabeverarbeitung
bool isHovered(Rectangle rect);
void drawTextOnRec(Rectangle rec, const char *text, int size);
void highlightButton(Rectangle *rect);

// Shop Funktionen
void drawShopRec(Rectangle shop);
void drawShopItems(bool shopOpen);
void drawShop(bool shopOpen);

// Pause-Menü Funktionen
void drawMuteButton(Rectangle muteButton, bool *isMuted, float *bgMusicVolume);
void drawSlider(Rectangle volumeSlider, float bgMusicVolume);
void drawFullscreen(Texture2D fullscreen1, Texture2D fullscreen2, bool *isFull);
void drawExit(Rectangle exitButton);
void drawPause(Rectangle exitButton, Rectangle muteButton, bool isMuted,
               Rectangle volumeSlider, float bgMusicVolume,
               Texture2D fullscreen1, Texture2D fullscreen2, bool *isFull);

// Eingabeüberprüfung
void mouseIn(Rectangle muteButton, bool *isMuted, Music *bgMusic,
             float *bgMusicVolume, Rectangle *volumeSlider);
void kbIn(float *playerSpeed, float deltaTime, Vector2 *playerPosition,
          bool *isMuted, float *bgMusicVolume, Music *bgMusic, Rectangle shop,
          bool *shopOpen);

#endif // !GAME_H
