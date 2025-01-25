#ifndef DRAWRL_H

#define DRAWRL_H

#include <raylib.h>

#define RECT_SIZE 200
#define FONTSIZE 50

#define MAPMIN 0
#define MAPMAX 10000

// Minimap Modes zum auswaehlen welche minimap angezeigt werden soll
typedef enum { MINIMAP_OFF, MINIMAP_SMALL, MINIMAP_BIG } MinimapMode;

// Minimap Funktionen
void drawMinimap(MinimapMode mode, Vector2 playerposition, int mapWidth,
                 int mapHeight);

#endif // !DRAWRL_H
