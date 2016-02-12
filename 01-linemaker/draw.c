#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

//plotting method
void plot(int x, int y, screen s){
  screen[x][y].red = 255;
}

//Insert your line algorithm here
void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  int A = y1 - y0;
  int B = -(x1 - x0);
  int d;
  int slope = A/ -B;
  //Octant I(0 < slope < 1)
  if(0 < slope && slope < 1){
    d = 2A + B;
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
  //Octant II
  if(slope > 1){
    d = A + 2B;
    while(y0 <= y1){
      plot(x,y);
      if(d < 0){
	x0 +=1;
	d += 2A;
      }
      y0 += 1;
      d +=2B;
    }
  }
  //Octant VII
  if(slope < -1){
    d = -2A + B;
    while(yo >= y1){
      plot(x,y);
      if(d < 0){
	x0 += 1;
	d += 2A;
      }
      y0 += 1;
      d += 2B;
    }
  }
  //Octant VIII 
  if(slope < 0 && slope > -1){
    d = -A + 2B;
    while(x0 <= x1){
      plot(x,y);
      if(d < 0){
	y0 += 1;
	d += 2A;
      }
      x0 += 1;
      d += 2B;
    }
  }

}

