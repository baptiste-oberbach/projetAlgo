#ifndef go_h
#define go_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dessine.h"
#include <math.h>

void draw_win();
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void game();

#endif /* go_h */
