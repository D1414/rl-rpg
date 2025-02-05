#include "drawrl.h"

void highlightButton(Rectangle *rect) {
  float scaleWidth = rect->width / 8;
  float scaleHeight = rect->height / 8;
  int scaleX = scaleWidth / 2;
  int scaleY = scaleHeight / 2;
  if (isHovered(*rect)) {
    rect->width = rect->width + scaleWidth;
    rect->height = rect->height + scaleHeight;
    rect->x = rect->x - scaleX;
    rect->y = rect->y - scaleY;
  }
}

void drawTextOnRec(Rectangle rec, const char *text, int size) {

  // diferent size when hovered
  int fontsize = !isHovered(rec) ? size : size + size / 20;
  int textWidth = MeasureText(text, fontsize);
  int textX = rec.x + (rec.width / 2) - (textWidth / 2.0);
  int textY = rec.y + (rec.height / 2) - (fontsize / 2.0 - 10);
  DrawText(text, textX, textY - 5, fontsize, LIGHTGRAY);
}
void drawMinimap(MinimapMode mode, Vector2 playerposition, int mapWidth,
                 int mapHeight) {
  if (mode == MINIMAP_OFF)
    return;

  int minimapWidth, minimapHeight;
  Vector2 minimapPosition;

  if (mode == MINIMAP_SMALL) {
    minimapWidth = RIGHT_WIDTH(400);
    minimapHeight = RIGHT_HEIGHT(400);
    minimapPosition = (Vector2){30, 30};
  } else if (mode == MINIMAP_BIG) {
    minimapWidth = GetScreenWidth() - 100;
    minimapHeight = GetScreenHeight() - 100;
    minimapPosition = (Vector2){50, 50};
  }
  DrawRectangle(minimapPosition.x - 10, minimapPosition.y - 10,
                minimapWidth + 20, minimapHeight + 20, BLACK);
  float scaleX = (float)minimapWidth / mapWidth;
  float scaleY = (float)minimapHeight / mapHeight;

  float playerMapX = (playerposition.x + (float)PLAYER_RECTW / 2) * scaleX;
  float playerMapY = (playerposition.y + (float)PLAYER_RECTH / 2) * scaleY;

  DrawRectangleLines(minimapPosition.x - 10, minimapPosition.y - 10,
                     minimapWidth + 20, minimapHeight + 20, WHITE);
  DrawCircle(minimapPosition.x + playerMapX, minimapPosition.y + playerMapY, 15,
             RED);
}
void drawShopRec(Rectangle shop) { DrawRectangleRec(shop, DARKBROWN); }

void drawShopItems(bool shopOpen) {
  int wX = SHOPWINDOWW + RIGHT_WIDTH(115);
  int wY = RIGHT_HEIGHT(100);

  int width = RIGHT_WIDTH(300);
  int height = RIGHT_HEIGHT(300 / 2);

  ShopItem items[] = {
      (ShopItem){(Rectangle){wX, wY + RIGHT_HEIGHT(200), width, height},
                 DARKGRAY, "Döner", 50 * FONT_FACTOR},
      (ShopItem){(Rectangle){wX, wY + RIGHT_HEIGHT(400), width, height},
                 DARKGRAY, "Shawarma", 34 * FONT_FACTOR},
      (ShopItem){(Rectangle){wX, wY + RIGHT_HEIGHT(600), width, height},
                 DARKGRAY, "Pizza", 60 * FONT_FACTOR},
      (ShopItem){(Rectangle){wX, wY + RIGHT_HEIGHT(800), width, height},
                 DARKGRAY, "Justin", 50 * FONT_FACTOR},
      (ShopItem){(Rectangle){wX, wY + RIGHT_HEIGHT(1000), width, height},
                 DARKGRAY, "ZafoG", 50 * FONT_FACTOR},
  };

  size_t itemCount = sizeof(items) / sizeof(items[0]);
  if (shopOpen) {
    for (size_t i = 0; i < itemCount; i++) {
      ShopItem item = items[i];
      highlightButton(&item.rec);
      DrawRectangleRec(item.rec, item.color);
      DrawRectangleLinesEx(item.rec, 5, isHovered(item.rec) ? WHITE : BLACK);
      drawTextOnRec(item.rec, item.text, item.fontsize);
    }
  }
}

void drawShop(bool shopOpen) {
  int windowWidth = SHOPWINDOWW;
  int windowHeight = SHOPWINDOWH;
  int windowX = GetScreenWidth() / 2 - windowWidth / 2;
  int windowY = GetScreenHeight() / 2 - windowHeight / 2;

  if (shopOpen) {
    Rectangle textField =
        (Rectangle){windowX, windowY, windowWidth, windowHeight};
    DrawRectangleRec(textField, LIGHTGRAY);
    DrawRectangleLinesEx(textField, 10, BLACK);
    DrawText("Shop:", windowX + 20, windowY + 20, 100 * FONT_FACTOR, BLACK);
    drawShopItems(shopOpen);
  }
}

// Zeichnen und Funktion des Mutebuttons
void drawMuteButton(Rectangle muteButton, bool *isMuted, float *bgMusicVolume) {
  if (*bgMusicVolume == 0) {
    *isMuted = true;
  }
  highlightButton(&muteButton);
  DrawRectangleRec(muteButton, *isMuted ? RED : GREEN);
  DrawRectangleLinesEx(muteButton, 7, BLACK);
  const char *buttonText = *isMuted ? "MUTED" : "MUSIC";
  int textWidth = MeasureText(buttonText, FONTSIZE);
  int textX = muteButton.x + (muteButton.width / 2) - (textWidth / 2.0);
  int textY = muteButton.y + (muteButton.height / 2) - (FONTSIZE / 2.0);
  DrawText(buttonText, textX, textY, FONTSIZE, BLACK);
}

// Zeichnen und Funktion des Lautstaerke Reglers
void drawSlider(Rectangle volumeSlider, float bgMusicVolume) {
  DrawRectangleRec(volumeSlider, DARKGRAY);
  DrawRectangle(volumeSlider.x, volumeSlider.y,
                volumeSlider.width * bgMusicVolume, volumeSlider.height,
                LIGHTGRAY);
  DrawCircle(volumeSlider.x + volumeSlider.width * bgMusicVolume,
             volumeSlider.y + volumeSlider.height / 2, 20, BLUE);
}

// Zeichnen des Fullscreen button und Modi switch in den Fullscreen
void drawFullscreen(Texture2D fullscreen1, Texture2D fullscreen2,
                    bool *isFull) {

  int fullscreenTexWidth = 32 * 3 + 20;
  int fullscreenTexHeight = 32 * 3 + 20;
  int fullscreenTexX = GetScreenWidth() - fullscreenTexWidth;
  int fullscreenTexY = 20;
  Rectangle hitBox = {fullscreenTexX, fullscreenTexY, fullscreenTexWidth,
                      fullscreenTexHeight};

  if (!*isFull) {
    if (isHovered(hitBox)) {
      DrawTextureEx(fullscreen1, (Vector2){fullscreenTexX, fullscreenTexY}, 0,
                    3.1, BLACK);
    } else {
      DrawTextureEx(fullscreen1, (Vector2){fullscreenTexX, fullscreenTexY}, 0,
                    3, BLACK);
    }
  } else {
    if (isHovered(hitBox)) {
      DrawTextureEx(fullscreen2, (Vector2){fullscreenTexX, fullscreenTexY}, 0,
                    3.1, BLACK);
    } else {
      DrawTextureEx(fullscreen2, (Vector2){fullscreenTexX, fullscreenTexY}, 0,
                    3, BLACK);
    }
  }
}
void drawExit(Rectangle exitButton) {
  // enable highlighting for button
  highlightButton(&exitButton);

  DrawRectangleRec(exitButton, DARKGRAY);

  // different color when hovered
  DrawRectangleLinesEx(exitButton, 5, isHovered(exitButton) ? WHITE : BLACK);
  const char *exitText = "Exit";

  // diferent size when hovered
  int fontsize = !isHovered(exitButton) ? 115 : 115 + 10;
  int textWidth = MeasureText(exitText, fontsize);
  int textX = exitButton.x + (exitButton.width / 2) - (textWidth / 2.0);
  int textY = exitButton.y + (exitButton.height / 2) - (fontsize / 2.0 - 10);
  DrawText(exitText, textX, textY - 5, fontsize, LIGHTGRAY);
  if (isHovered(exitButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    CloseWindow();
    exit(0);
  }
}
