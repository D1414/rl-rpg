#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RECT_SIZE 200
#define FONTSIZE 50

#define MAPMIN 0
#define MAPMAX 10000

/* Unterschiedliche gamestates die entscheiden was angezeigt wird, und welche
 * inputs ausgefuehrt werden duerfen.
 */
typedef enum { GAME_RUNNING, GAME_PAUSED, GAME_SHOP } GameState;

// Minimap Modes zum auswaehlen welche minimap angezeigt werden soll
typedef enum { MINIMAP_OFF, MINIMAP_SMALL, MINIMAP_BIG } MinimapMode;

typedef struct {
  Rectangle rec;
  Color color;
  const char *text;
  int fontsize;
} ShopItem;

MinimapMode minimapMode = MINIMAP_SMALL;

// Minimap standart ist auf small gestellt
GameState gameState = GAME_RUNNING;

// zwischen speicher fuer Musiklautstaerke damit sie
// zuruek auf den Wert geht bei dem sie gemutet wurde
float tempMusic;
bool isHovered(Rectangle rect) {
  Vector2 mousePosition = GetMousePosition();
  return CheckCollisionPointRec(mousePosition, rect);
}
void drawTextOnRec(Rectangle rec, const char *text, int size) {

  // diferent size when hovered
  int fontsize = !isHovered(rec) ? size : size + size / 20;
  int textWidth = MeasureText(text, fontsize);
  int textX = rec.x + (rec.width / 2) - (textWidth / 2.0);
  int textY = rec.y + (rec.height / 2) - (fontsize / 2.0 - 10);
  DrawText(text, textX, textY - 5, fontsize, LIGHTGRAY);
}

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

void drawShopRec(Rectangle shop) { DrawRectangleRec(shop, DARKBROWN); }

void drawShopItems(bool shopOpen) {
  int wX = GetScreenWidth() / 2 - 1000 / 2 + 40,
      wY = GetScreenHeight() / 2 - 1500 / 2;

  int width = 400;
  int height = 200;

  ShopItem items[] = {
      (ShopItem){(Rectangle){wX, wY + 150, width, height}, DARKGRAY, "Döner",
                 120},
      (ShopItem){(Rectangle){wX, wY + 400, width, height}, DARKGRAY, "Shawarma",
                 80},
      (ShopItem){(Rectangle){wX, wY + 650, width, height}, DARKGRAY, "Pizza",
                 120},
      (ShopItem){(Rectangle){wX, wY + 900, width, height}, DARKGRAY, "Justin",
                 110},
      (ShopItem){(Rectangle){wX, wY + 1150, width, height}, DARKGRAY, "Zafer",
                 120},
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
  int windowWidth = 1000, windowHeight = 1500;
  int windowX = GetScreenWidth() / 2 - windowWidth / 2;
  int windowY = GetScreenHeight() / 2 - windowHeight / 2;

  if (shopOpen) {
    Rectangle textField =
        (Rectangle){windowX, windowY, windowWidth, windowHeight};
    DrawRectangleRec(textField, LIGHTGRAY);
    DrawRectangleLinesEx(textField, 10, BLACK);
    DrawText("Shop:", windowX + 20, windowY + 20, 100, BLACK);
    drawShopItems(shopOpen);
  }
}

void drawMinimap(MinimapMode mode, Vector2 playerposition, int mapWidth,
                 int mapHeight) {
  if (mode == MINIMAP_OFF)
    return;

  int minimapWidth, minimapHeight;
  Vector2 minimapPosition;

  if (mode == MINIMAP_SMALL) {
    minimapWidth = 400;
    minimapHeight = 400;
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

  float playerMapX = (playerposition.x + (float)RECT_SIZE / 2) * scaleX;
  float playerMapY = (playerposition.y + (float)RECT_SIZE / 2) * scaleY;

  DrawRectangleLines(minimapPosition.x - 10, minimapPosition.y - 10,
                     minimapWidth + 20, minimapHeight + 20, WHITE);
  DrawCircle(minimapPosition.x + playerMapX, minimapPosition.y + playerMapY, 15,
             RED);
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
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (isHovered(hitBox)) {
      *isFull = !*isFull;
      if (*isFull) {
        ToggleBorderlessWindowed();
        printf("turned on Fullscreen\n");
      }
      if (!*isFull) {
        ToggleBorderlessWindowed();
        printf("disabled fullscreen\n");
      }
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

// Zeichnen des Pause Menues und Anwendung der Funktionen fuer funktionen die
// im Menue Verwendbar sein sollen
void drawPause(Rectangle exitButton, Rectangle muteButton, bool isMuted,
               Rectangle volumeSlider, float bgMusicVolume,
               Texture2D fullscreen1, Texture2D fullscreen2, bool *isFull) {
  if (gameState == GAME_PAUSED) {
    int screenwidth = GetScreenWidth();
    int screenheight = GetScreenHeight();
    DrawText("PAUSED", screenwidth / 2 - MeasureText("PAUSED", 100) / 2,
             screenheight / 2 - 30, 100, WHITE);
    // DrawText(TextFormat("isMuted: %s", isMuted ? "true" : "false"), 0, 50,
    // 50, YELLOW);
    drawMuteButton(muteButton, &isMuted, &bgMusicVolume);
    drawSlider(volumeSlider, bgMusicVolume);
    drawFullscreen(fullscreen1, fullscreen2, isFull);
    drawExit(exitButton);
  }
}

// mouse inputs Verarbeitung
void mouseIn(Rectangle muteButton, bool *isMuted, Music *bgMusic,
             float *bgMusicVolume, Rectangle *volumeSlider) {
  Rectangle sliderHitbox = {volumeSlider->x, volumeSlider->y,
                            volumeSlider->width, volumeSlider->height + 30};
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (isHovered(muteButton)) {
      if (*isMuted) {
        printf("isMuted tempMusic: %f\n", tempMusic);
        *bgMusicVolume = tempMusic;
        *isMuted = false;
      } else if (!*isMuted) {
        tempMusic = *bgMusicVolume;
        *bgMusicVolume = 0;
        *isMuted = true;
        printf("!isMuted tempMusic: %f\n", tempMusic);
      }
      SetMusicVolume(*bgMusic, *bgMusicVolume);
      printf("bgMusicVolume: %f\ntempMusic: %f\n", *bgMusicVolume, tempMusic);
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (isHovered(sliderHitbox)) {
      Vector2 mousePosition = GetMousePosition();
      if (*isMuted) {
        *isMuted = false;
        *bgMusicVolume =
            (mousePosition.x - volumeSlider->x) / volumeSlider->width;
        SetMusicVolume(*bgMusic, *bgMusicVolume);
        printf("Music volume update: %f\n", *bgMusicVolume);
      }
      if (!*isMuted) {
        *bgMusicVolume =
            (mousePosition.x - volumeSlider->x) / volumeSlider->width;
        printf("Music volume update: %f\n", *bgMusicVolume);
        if (*bgMusicVolume < 0.05f) {
          *isMuted = true;
          *bgMusicVolume = 0;
          SetMusicVolume(*bgMusic, *bgMusicVolume);
          printf("Sollte sich muten\n");
        }
        SetMusicVolume(*bgMusic, *bgMusicVolume);
        printf("Music volume update: %f\n", *bgMusicVolume);
      }
    }
  }
}

// keyboard inputs Verarbeitung
void kbIn(float *playerSpeed, float deltaTime, Vector2 *playerPosition,
          bool *isMuted, float *bgMusicVolume, Music *bgMusic, Rectangle shop,
          bool *shopOpen) {
  if (gameState == GAME_RUNNING) {
    float playerMove = *playerSpeed * deltaTime;

    // New potential position
    Vector2 newPosition = *playerPosition;

    // Check the movement direction and calculate the new position
    if (IsKeyDown(KEY_W)) {
      newPosition.y -= playerMove;
    }
    if (IsKeyDown(KEY_A)) {
      newPosition.x -= playerMove;
    }
    if (IsKeyDown(KEY_S)) {
      newPosition.y += playerMove;
    }
    if (IsKeyDown(KEY_D)) {
      newPosition.x += playerMove;
    }

    // Ensure the player does not move outside the map boundaries
    if (newPosition.x < MAPMIN)
      newPosition.x = MAPMIN;
    if (newPosition.y < MAPMIN)
      newPosition.y = MAPMIN;
    if (newPosition.x > MAPMAX - RECT_SIZE)
      newPosition.x = MAPMAX - RECT_SIZE;
    if (newPosition.y > MAPMAX - RECT_SIZE)
      newPosition.y = MAPMAX - RECT_SIZE;

    // Collision check: make sure the player does not walk through the shop
    Rectangle playerRect = {newPosition.x, newPosition.y, RECT_SIZE, RECT_SIZE};
    if (!CheckCollisionRecs(
            playerRect,
            (Rectangle){shop.x, shop.y, shop.width - 15, shop.height - 15})) {
      *playerPosition =
          newPosition; // Update the player's position if no collision
    }

    *playerSpeed = IsKeyDown(KEY_LEFT_SHIFT) ? 1800.0f : 600.0f;
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    if (gameState == GAME_RUNNING) {
      gameState = GAME_PAUSED;
      printf("gamestate: %d\n", gameState);
    } else if (gameState == GAME_PAUSED) {
      gameState = GAME_RUNNING;
      printf("gamestate: %d\n", gameState);
    } else if (gameState == GAME_SHOP) {
      *shopOpen = !*shopOpen;
      gameState = GAME_RUNNING;
    }
  }
  if (IsKeyPressed(KEY_V)) {
    if (*isMuted) {
      *isMuted = !*isMuted;
      *bgMusicVolume = tempMusic;
    } else {
      *isMuted = !*isMuted;
      tempMusic = *bgMusicVolume;
      *bgMusicVolume = 0;
    }
    SetMusicVolume(*bgMusic, *bgMusicVolume);
  }
  if (IsKeyPressed(KEY_M) && gameState == GAME_RUNNING) {
    minimapMode = (MinimapMode)((minimapMode + 1) % 3);
    printf("minimapMode: %d\n", minimapMode);
  }
  if (CheckCollisionPointRec((Vector2){playerPosition->x, playerPosition->y},
                             shop) &&
      IsKeyPressed(KEY_F)) {
    if (gameState == GAME_RUNNING) {
      gameState = GAME_SHOP;
      *shopOpen = !*shopOpen;
      printf("shop: %s\n", *shopOpen ? "open" : "close");
    } else if (gameState == GAME_SHOP) {

      gameState = GAME_RUNNING;
      *shopOpen = !*shopOpen;
      printf("shop: %s\n", *shopOpen ? "open" : "close");
    }
  }
}

int main(void) {
  Image icon = LoadImage("ressources/icon.png");
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "rlrpg");
  SetWindowIcon(icon);
  MaximizeWindow();
  InitAudioDevice();
  SetTargetFPS(144);

  Music bgMusic = LoadMusicStream("ressources/bleach.mp3");
  PlayMusicStream(bgMusic);

  float bgMusicVolume = 0.0f;
  SetMusicVolume(bgMusic, bgMusicVolume);

  // variable zum speicher ob das game gemutet ist.
  bool isMuted = true;

  // variable zum speicher ob das game pausiert ist.
  bool isPaused = false;

  // variable zum speichern ob das game im Fullscreen ist.
  bool isFull = false;

  bool shopOpen = false;

  Vector2 playerPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  float playerSpeed = 600.0f;

  Camera2D camera = {0};

  Texture2D mapTexture = LoadTexture("ressources/grass_dark.png");
  Texture2D fullscreen1 = LoadTexture("ressources/fullscreen_1.png");
  Texture2D fullscreen2 = LoadTexture("ressources/fullscreen_2.png");

  while (!WindowShouldClose()) {

    SetExitKey(KEY_NULL);
    int fps = GetFPS();
    float deltaTime = GetFrameTime();
    UpdateMusicStream(bgMusic);
    // kamera
    camera.offset = (Vector2){GetScreenWidth() / 2.0f - 100,
                              GetScreenHeight() / 2.0f - 100};
    camera.zoom = 1.0f;
    camera.target = playerPosition;

    Rectangle volumeSlider = {GetScreenWidth() - 320, GetScreenHeight() - 60,
                              300, 20};
    Rectangle muteButton = {GetScreenWidth() - volumeSlider.width / 2 - 100,
                            GetScreenHeight() - 200, 200, 100};
    Rectangle exitButton = {(float)GetScreenWidth() / 2 - (float)350 / 2,
                            (float)GetScreenHeight() / 2 + 80, 350, 130};

    Rectangle shop = {0, 0, 300, 300};
    BeginDrawing();
    ClearBackground(BLACK);

    kbIn(&playerSpeed, deltaTime, &playerPosition, &isMuted, &bgMusicVolume,
         &bgMusic, shop, &shopOpen);
    mouseIn(muteButton, &isMuted, &bgMusic, &bgMusicVolume, &volumeSlider);
    BeginMode2D(camera);

    int startX = (int)(camera.target.x - camera.offset.x) / mapTexture.width *
                 mapTexture.width;
    int startY = (int)(camera.target.y - camera.offset.y) / mapTexture.height *
                 mapTexture.height;

    for (int y = startY - mapTexture.height;
         y < startY + GetScreenHeight() + mapTexture.height;
         y += mapTexture.height) {
      for (int x = startX - mapTexture.width;
           x < startX + GetScreenWidth() + mapTexture.width;
           x += mapTexture.width) {
        DrawTexture(mapTexture, x, y, WHITE);
      }
    }
    DrawRectangleLinesEx(
        (Rectangle){MAPMIN - 20, MAPMIN - 20, MAPMAX + 40, MAPMAX + 40}, 20,
        BROWN);

    DrawRectangleV(playerPosition, (Vector2){200, 200},
                   (Color){99, 149, 238, 255});
    DrawRectangleLinesEx(
        (Rectangle){playerPosition.x, playerPosition.y, 200, 200}, 10, BLACK);

    drawShopRec(shop);

    EndMode2D();
    drawShop(shopOpen);
    drawPause(exitButton, muteButton, isMuted, volumeSlider, bgMusicVolume,
              fullscreen1, fullscreen2, &isFull);
    drawMinimap(minimapMode, playerPosition, MAPMAX, MAPMAX);

    EndDrawing();
  }
  UnloadMusicStream(bgMusic);
  CloseAudioDevice();
  UnloadTexture(mapTexture);
  UnloadTexture(fullscreen1);
  UnloadTexture(fullscreen2);
  CloseWindow();

  return 0;
}
