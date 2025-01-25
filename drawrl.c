#include "drawrl.h"

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
