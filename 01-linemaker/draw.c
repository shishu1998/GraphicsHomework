#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

//Insert your line algorithm here
void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
  if(x1 < x0){
    draw_line(x1,y1,x0,y0,s,c);
  }
  else if (x0 == x1){
    if(y0 < y1){
      while(y0 <= y1){
	plot(s,c,x0,y0);
	y0 ++;
      }
    }
    else{
      draw_line(x1,y1,x0,y0,s,c);
    }
  }
  else{
    int A = y1 - y0;
    int B = -(x1 - x0);
    int d;
    float slope = (float)A/ -(float)B;
    //Octant I
    if(0 <= slope && slope <= 1){
      d = 2*A + B;
      while(x0 <= x1){
	plot(s,c,x0,y0);
	if(d > 0){
	  y0 += 1;
	  d += 2*B;
	}
	x0 +=1;
	d += 2*A;
      }
    }
    //Octant II
    if(slope > 1){
      d = A + 2*B;
      while(y0 <= y1){
	plot(s,c,x0,y0);
	if(d < 0){
	  x0 +=1;
	  d += 2*A;
	}
	y0 += 1;
	d +=2*B;
      }
    }
    //Octant VII
    if(slope <= -1){
      d = A - 2*B;
      while(y0 >= y1){
	plot(s,c,x0,y0);
	if(d > 0){
	  x0 += 1;
	  d += 2*A;
	}
	y0 -= 1;
	d -= 2*B;
      }
    }
    //Octant VIII 
    if(slope < 0 && slope > -1){
      d = 2*A - B;
      while(x0 <= x1){
	plot(s,c,x0,y0);
	if(d < 0){
	  y0 -= 1;
	  d -= 2*B;
	}
	x0 += 1;
	d += 2*A;
      }
    }
  }
  
}

