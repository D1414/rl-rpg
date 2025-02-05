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


// Funktionsprototypen

// Eingabeverarbeitung
bool isHovered(Rectangle rect);

// Eingabeüberprüfung
void mouseIn(Rectangle muteButton, bool *isMuted, Music *bgMusic,
             float *bgMusicVolume, Rectangle *volumeSlider, bool *isFull);
void kbIn(float *playerSpeed, float deltaTime, Vector2 *playerPosition,
          bool *isMuted, float *bgMusicVolume, Music *bgMusic, Rectangle shop,
          bool *shopOpen);

void drawPause(Rectangle exitButton, Rectangle muteButton, bool isMuted,
               Rectangle volumeSlider, float bgMusicVolume,
               Texture2D fullscreen1, Texture2D fullscreen2, bool *isFull);
#endif // !GAME_H
