/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.

  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

void my_main( int polygons ) {

  int i;
  double step;
  double xval, yval, zval,width,height,depth,r,cx,cy,cz,r1,r2;
  struct matrix *transform;
  struct matrix *tmp;
  struct stack *s = new_stack();
  screen t;
  color g;
  g.red = 255;
  
  s = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );

  for (i=0;i<lastop;i++) {  
    switch (op[i].opcode) {
    case PUSH:
      push(s);
      printf("%s\n", "push");
      break;
    case POP:
      pop(s);
      printf("%s\n","pop");
      break;
    case MOVE:
      printf("%s\n","move");
      xval = op[i].op.move.d[0];
      yval = op[i].op.move.d[1];
      zval = op[i].op.move.d[2];
      transform = make_translate(xval,yval,zval);
      matrix_mult(s->data[s->top],transform);
      copy_matrix(transform,s->data[s->top]);
      break;
    case ROTATE:
      printf("%s\n","rotate");
      if(op[i].op.rotate.axis < 1.1){
	transform = make_rotX(op[i].op.rotate.degrees * (M_PI/180));
      }
      else if(op[i].op.rotate.axis < 2.1){
	transform = make_rotY(op[i].op.rotate.degrees * (M_PI/180));
      }
      else{
	transform = make_rotZ(op[i].op.rotate.degrees * (M_PI/180));
      }
      matrix_mult(s->data[s->top],transform);
      copy_matrix(transform,s->data[s->top]);
      break;
    case SCALE:
      printf("%s\n","scale");
      xval = op[i].op.scale.d[0];
      yval = op[i].op.scale.d[1];
      zval = op[i].op.scale.d[2];
      transform = make_scale(xval,yval,zval);
      matrix_mult(s->data[s->top],transform);
      copy_matrix(transform,s->data[s->top]);
      break;
    case BOX:
      printf("%s\n","box");
      xval = op[i].op.box.d0[0];
      yval = op[i].op.box.d0[1];
      zval = op[i].op.box.d0[2];
      width = op[i].op.box.d1[0];
      height = op[i].op.box.d1[1];
      depth = op[i].op.box.d1[2];
      add_box(tmp,xval,yval,zval,width,depth,height);
      matrix_mult(s->data[s->top],tmp);
      draw_polygons(tmp,t,g);
      ident(tmp);
      break;
    case SPHERE:
      printf("%s\n","sphere");
      cx = op[i].op.sphere.d[0];
      cy = op[i].op.sphere.d[1];
      cz = op[i].op.sphere.d[2];
      r = op[i].op.sphere.r;
      add_sphere(tmp,cx,cy,cz,r,100);
      matrix_mult(s->data[s->top],tmp);
      draw_polygons(tmp,t,g);
      ident(tmp);
      break;
    case TORUS:
      printf("%s\n","torus");
      cx = op[i].op.torus.d[0];
      cy = op[i].op.torus.d[1];
      cz = op[i].op.torus.d[2];
      r1 = op[i].op.torus.r0;
      r2 = op[i].op.torus.r1;
      add_torus(tmp,cx,cy,cz,r1,r2,100);
      matrix_mult(s->data[s->top],tmp);
      draw_polygons(tmp,t,g);
      ident(tmp);
      break;
    case LINE:
      printf("%s\n","line");
      add_edge(tmp,
	       op[i].op.line.p0[0],
	       op[i].op.line.p0[1],
	       op[i].op.line.p0[2],
	       op[i].op.line.p1[0],
	       op[i].op.line.p1[1],
	       op[i].op.line.p1[2]);
      matrix_mult(s->data[s->top],tmp);
      draw_lines(tmp,t,g);
      break;
    case SAVE:
      printf("%s\n","saving");
      save_extension(t,op[i].op.save.p->name);
      break;
    case DISPLAY:
      printf("%s\n","displaying");
      display(t);
      break;
    }
  }
}
