#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

//Insert your line algorithm here
void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  int A = y1 - y0;
  int B = -(x1 - x0);
  //Quadrant I
  int d = 2A + B;
  while(x0 <= x1){
    plot(x,y);
    if(d < 0){
      y0 += 1;
      d += 2B;
    }
    x0 +=1;
    d += 2A;
  }
  
}

