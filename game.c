#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

#define RECT_SIZE 200
#define FONTSIZE 50

typedef enum { GAME_RUNNING, GAME_PAUSED } GameState;

GameState gameState = GAME_RUNNING;
bool isFull = false;

void drawMuteButton(Rectangle muteButton, bool *isMuted, float bgMusicVolume) {
  if (bgMusicVolume == 0) {
    *isMuted = true;
  }
  DrawRectangleRec(muteButton, *isMuted ? RED : GREEN);
  DrawRectangleLinesEx(muteButton, 7, BLACK);
  const char *buttonText = *isMuted ? "MUTED" : "MUSIC";
  int textWidth = MeasureText(buttonText, FONTSIZE);
  int textX = muteButton.x + (muteButton.width / 2) - (textWidth / 2.0);
  int textY = muteButton.y + (muteButton.height / 2) - (FONTSIZE / 2.0);
  DrawText(buttonText, textX, textY, FONTSIZE, BLACK);
}

void drawSlider(Rectangle volumeSlider, float bgMusicVolume) {
  DrawRectangleRec(volumeSlider, DARKGRAY);
  DrawRectangle(volumeSlider.x, volumeSlider.y,
                volumeSlider.width * bgMusicVolume, volumeSlider.height,
                LIGHTGRAY);
  DrawCircle(volumeSlider.x + volumeSlider.width * bgMusicVolume,
             volumeSlider.y + volumeSlider.height / 2, 20, BLUE);
}
void drawFullscreen(Texture2D fullscreen1, Texture2D fullscreen2,
                    bool *isFull) {

  int fullscreenWidth = 32 * 3 + 20;
  int fullscreenHeight = 32 * 3 + 20;
  int fullscreenX = GetScreenWidth() - fullscreenWidth;
  int fullscreenY = 20;
  Rectangle hitBox = {fullscreenX, fullscreenY, fullscreenWidth,
                      fullscreenHeight};
  Vector2 mousePosition = GetMousePosition();
  if (!IsWindowFullscreen()) {
    if (CheckCollisionPointRec(mousePosition, hitBox)) {
      DrawTextureEx(fullscreen1, (Vector2){fullscreenX, fullscreenY}, 0, 3.1,
                    BLACK);
    } else {
      DrawTextureEx(fullscreen1, (Vector2){fullscreenX, fullscreenY}, 0, 3,
                    BLACK);
    }
  } else {
    if (CheckCollisionPointRec(mousePosition, hitBox)) {
      DrawTextureEx(fullscreen2, (Vector2){fullscreenX, fullscreenY}, 0, 3.1,
                    BLACK);
    } else {
      DrawTextureEx(fullscreen2, (Vector2){fullscreenX, fullscreenY}, 0, 3,
                    BLACK);
    }
  }
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (CheckCollisionPointRec(mousePosition, hitBox)) {
      ToggleFullscreen();
      *isFull = !isFull;
    }
  }
}

void drawPause(Rectangle muteButton, bool isMuted, Rectangle volumeSlider,
               float bgMusicVolume, Texture2D fullscreen1,
               Texture2D fullscreen2, bool *isFull) {
  if (gameState == GAME_PAUSED) {
    int screenwidth = GetScreenWidth();
    int screenheight = GetScreenHeight();
    DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 60) / 2,
             GetScreenHeight() / 2 - 30, 90, WHITE);
    drawMuteButton(muteButton, &isMuted, bgMusicVolume);
    drawSlider(volumeSlider, bgMusicVolume);
    drawFullscreen(fullscreen1, fullscreen2, isFull);
  }
}

void mouseIn(Rectangle muteButton, bool *isMuted, Music *bgMusic,
             float *bgMusicVolume, Rectangle *volumeSlider) {
  Vector2 mousePosition = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (CheckCollisionPointRec(mousePosition, muteButton)) {
      *isMuted = !*isMuted;
      SetMusicVolume(*bgMusic, *isMuted ? 0.0f : *bgMusicVolume);
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && gameState == GAME_PAUSED) {
    if (CheckCollisionPointRec(mousePosition, *volumeSlider)) {
      if (!*isMuted) {
        *bgMusicVolume =
            (mousePosition.x - volumeSlider->x) / volumeSlider->width;
        SetMusicVolume(*bgMusic, *bgMusicVolume);
      }
    }
  }
}

void kbIn(float *playerSpeed, float deltaTime, Vector2 *playerPosition) {
  if (gameState == GAME_RUNNING) {
    if (IsKeyDown(KEY_W)) {
      playerPosition->y -= *playerSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
      playerPosition->x -= *playerSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
      playerPosition->y += *playerSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
      playerPosition->x += *playerSpeed * deltaTime;
    }
    if (IsKeyReleased(KEY_B)) {
      printf("Screenwidth: %d/n", GetScreenWidth());
    }
    *playerSpeed = IsKeyDown(KEY_LEFT_SHIFT) ? 1500.0f : 600.0f;
  }
  if (IsKeyPressed(KEY_P)) {
    if (gameState == GAME_RUNNING) {
      gameState = GAME_PAUSED;
      printf("gamestate: %d", gameState);
    } else {
      gameState = GAME_RUNNING;
      printf("gamestate: %d", gameState);
    }
  }
}

int main(void) {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "rlrpg");
  InitAudioDevice();
  SetTargetFPS(300);

  Music bgMusic = LoadMusicStream("ressources/bleach.mp3");
  PlayMusicStream(bgMusic);

  float bgMusicVolume = 1 / 3.0f;
  SetMusicVolume(bgMusic, bgMusicVolume);

  bool isMuted = false;
  bool isPaused = false;

  Vector2 playerPosition = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  float playerSpeed = 350.0f;

  Camera2D camera = {0};

  Texture2D mapTexture = LoadTexture("ressources/grass.png");
  Texture2D fullscreen1 = LoadTexture("ressources/fullscreen_1.png");
  Texture2D fullscreen2 = LoadTexture("ressources/fullscreen_2.png");

  while (!WindowShouldClose()) {
    int fps = GetFPS();
    float deltaTime = GetFrameTime();
    UpdateMusicStream(bgMusic);
    // kamera
    camera.offset =
        (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.zoom = 1.0f;
    camera.target = playerPosition;

    Rectangle volumeSlider = {GetScreenWidth() - 320, GetScreenHeight() - 60,
                              300, 20};
    Rectangle muteButton = {GetScreenWidth() - volumeSlider.width / 2 - 100,
                            GetScreenHeight() - 200, 200, 100};

    BeginDrawing();
    ClearBackground(BLACK);

    kbIn(&playerSpeed, deltaTime, &playerPosition);
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
    int mapMin = 0;
    int mapMax = 10000;

    if (playerPosition.x < mapMin)
      playerPosition.x = mapMin;
    if (playerPosition.y < mapMin)
      playerPosition.y = mapMin;
    if (playerPosition.x > mapMax - RECT_SIZE)
      playerPosition.x = mapMax - RECT_SIZE;
    if (playerPosition.y > mapMax - RECT_SIZE)
      playerPosition.y = mapMax - RECT_SIZE;
    DrawRectangleLinesEx(
        (Rectangle){mapMin - 20, mapMin - 20, mapMax + 40, mapMax + 40}, 20,
        BROWN);
    // Spieler
    DrawRectangleV(playerPosition, (Vector2){200, 200}, BLUE);
    DrawRectangleLinesEx(
        (Rectangle){playerPosition.x, playerPosition.y, 200, 200}, 10, BLACK);
    DrawText(TextFormat("FPS: %d", fps),
             camera.target.x - (int)(GetScreenWidth() / 2),
             camera.target.y - (int)(GetScreenHeight() / 2), 60, YELLOW);
    EndMode2D();

    // Zeichnen des Mute buttons und des sliders
    // drawSlider(volumeSlider, bgMusicVolume);
    // drawMuteButton(muteButton, isMuted);
    drawPause(muteButton, isMuted, volumeSlider, bgMusicVolume, fullscreen1,
              fullscreen2, &isFull);

    EndDrawing();
  }
  UnloadMusicStream(bgMusic);
  CloseAudioDevice();
  UnloadTexture(mapTexture);
  CloseWindow();

  return 0;
}
