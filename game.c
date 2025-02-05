#include "game.h"
#include "drawrl.h"
#include <raylib.h>

// globale variablen
MinimapMode minimapMode =
    MINIMAP_SMALL;                  // Minimap standart ist auf small gestellt
GameState gameState = GAME_RUNNING; // game faengt running an
float tempMusic; // zwischen speicher fuer Musiklautstaerke damit sie, zuruek
                 // auf den Wert geht bei dem sie gemutet wurde
bool isHovered(Rectangle rect) {
  Vector2 mousePosition = GetMousePosition();
  return CheckCollisionPointRec(mousePosition, rect);
}
void drawPlayer(Rectangle playerRect, Vector2 *playerPosition) {}

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
             float *bgMusicVolume, Rectangle *volumeSlider, bool *isFull) {
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

  int fullscreenTexWidth = 32 * 3 + 20;
  int fullscreenTexHeight = 32 * 3 + 20;
  int fullscreenTexX = GetScreenWidth() - fullscreenTexWidth;
  int fullscreenTexY = 20;
  Rectangle hitBox = {fullscreenTexX, fullscreenTexY, fullscreenTexWidth,
                      fullscreenTexHeight};
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

void kbIn(float *playerSpeed, float deltaTime, Vector2 *playerPosition,
          bool *isMuted, float *bgMusicVolume, Music *bgMusic, Rectangle shop,
          bool *shopOpen) {
  if (gameState == GAME_RUNNING) {
    float playerMove = *playerSpeed * deltaTime;

    Vector2 newPosition = *playerPosition;

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

    if (newPosition.x < MAPMIN)
      newPosition.x = MAPMIN;
    if (newPosition.y < MAPMIN)
      newPosition.y = MAPMIN;
    if (newPosition.x > MAPMAXW - PLAYER_RECTW)
      newPosition.x = MAPMAXW - PLAYER_RECTW;
    if (newPosition.y > MAPMAXH - PLAYER_RECTH)
      newPosition.y = MAPMAXH - PLAYER_RECTH;

    Rectangle playerRect = {newPosition.x, newPosition.y, PLAYER_RECTW,
                            PLAYER_RECTH};
    if (!CheckCollisionRecs(
            playerRect,
            (Rectangle){shop.x, shop.y, shop.width - 15, shop.height - 15})) {
      *playerPosition = newPosition;
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

  Texture2D mapTexture = LoadTexture("ressources/grass.png");
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

    Rectangle shop = {0, 0, RIGHT_WIDTH(300), RIGHT_HEIGHT(300)};
    BeginDrawing();
    ClearBackground(BLACK);

    kbIn(&playerSpeed, deltaTime, &playerPosition, &isMuted, &bgMusicVolume,
         &bgMusic, shop, &shopOpen);
    mouseIn(muteButton, &isMuted, &bgMusic, &bgMusicVolume, &volumeSlider,
            &isFull);
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
        (Rectangle){MAPMIN - 20, MAPMIN - 20, MAPMAXW + 40, MAPMAXH + 40}, 20,
        BROWN);

    DrawRectangleV(playerPosition, (Vector2){PLAYER_RECTW, PLAYER_RECTH},
                   (Color){99, 149, 238, 255});
    DrawRectangleLinesEx((Rectangle){playerPosition.x, playerPosition.y,
                                     PLAYER_RECTW, PLAYER_RECTH},
                         10, BLACK);

    drawShopRec(shop);

    EndMode2D();
    drawShop(shopOpen);
    drawPause(exitButton, muteButton, isMuted, volumeSlider, bgMusicVolume,
              fullscreen1, fullscreen2, &isFull);
    drawMinimap(minimapMode, playerPosition, MAPMAXW, MAPMAXH);

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
