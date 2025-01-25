#ifndef DRAWRL_H

#define DRAWRL_H

#include <raylib.h>

#define WIDTH_FACTOR 0.078125f
#define HEIGHT_FACTOR 0.138889f
#define PLAYER_RECTW GetScreenWidth() * WIDTH_FACTOR 
#define PLAYER_RECTH GetScreenHeight() * HEIGHT_FACTOR
#define FONTSIZE 50

#define MAPMIN 0
#define MAPMAX 10000

// GetScreenWidth() * 0.15f

// Minimap Modes zum auswaehlen welche minimap angezeigt werden soll
typedef enum { MINIMAP_OFF, MINIMAP_SMALL, MINIMAP_BIG } MinimapMode;

// Minimap Funktionen
void drawMinimap(MinimapMode mode, Vector2 playerposition, int mapWidth,
                 int mapHeight);

#endif // !DRAWRL_H
