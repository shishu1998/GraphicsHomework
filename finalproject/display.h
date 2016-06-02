#ifndef DISPLAY_H
#define DISPLAY_H

void plot( screen s, color c, int x, int y);
void zplot( screen s, color c,int row, int column, double x, double y, double z, struct matrix * zbuffer);
void clear_screen( screen s);
void save_ppm( screen s, char *file);
void save_extension( screen s, char *file);
void display( screen s);

color change_color( int i );
#endif