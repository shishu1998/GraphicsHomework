#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 
  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  circle: add a circle to the edge matrix - 
  takes 3 arguments (cx, cy, r)
  hermite: add a hermite curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  bezier: add a bezier curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  xrotate: create an x-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  yrotate: create an y-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  zrotate: create an z-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing
  See the file script for an example of the file format
  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  char nextline[256];
  char *next;
  double args[10];

  color c;
  c.red = 255;
  c.blue = 100;
  c.green = 0;
  
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 )
    f = stdin;
  else
    f = fopen(filename, "r");
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    if(!(strcmp(line,"apply"))){
      matrix_mult(transform,pm);
    }
    else if(!(strcmp(line,"display"))){
      draw_lines(pm,s,c);
      display(s);
    }
    else if(!(strcmp(line,"save"))){
      fgets(nextline,255,f);
      draw_lines(pm,s,c);
      save_extension(s,nextline);
    }
    else{
      fgets(nextline,255,f);
      next = nextline;
      int i = 0;
      while(next){
	args[i] = strtod(strsep(&next," "),NULL);
	i ++;
      }
    }
    if(!(strcmp(line,"line"))){
      add_edge(pm,args[0],args[1],args[2],args[3],args[4],args[5]);
    }
    if(!(strcmp(line,"circle"))){
      add_circle(pm,args[0],args[1],args[2],360);
    }
    if (!(strcmp(line,"hermite"))){
      add_curve(pm,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],360,HERMITE_MODE);
    }
    if (!(strcmp(line,"bezier"))){
      add_curve(pm,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],360,BEZIER_MODE);
    }
    if (!(strcmp(line,"ident"))){
      ident(transform);
    }
    if (!(strcmp(line,"scale"))){
      struct matrix* scale = make_scale(args[0],args[1],args[2]);
      matrix_mult(scale,transform);
    }
    if (!(strcmp(line,"translate"))){
      struct matrix* translate = make_translate(args[0],args[1],args[2]);
      matrix_mult(translate,transform);
    }
    if (!(strcmp(line,"xrotate"))){
      struct matrix* xrotate = make_rotX(args[0]);
      matrix_mult(xrotate,transform);
    }
    if (!(strcmp(line,"yrotate"))){
      struct matrix* yrotate = make_rotY(args[0]);
      matrix_mult(yrotate,transform);
    }
    if (!(strcmp(line,"zrotate"))){
      struct matrix* zrotate = make_rotZ(args[0]);
      matrix_mult(zrotate,transform);
    }
    
    printf(":%s:\n",line);  
  }
}

  
