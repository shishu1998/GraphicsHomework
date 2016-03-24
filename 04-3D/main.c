#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char** argv ) {

  screen s;
  color c;

  clear_screen(s);

  c.red = 255;
  c.blue = 0;
  c.green = 0;

  struct matrix *edges = new_matrix(4,0);
  struct matrix *transform = new_matrix(4,4);
  add_sphere(edges,250,250,100,0.01);
  draw_lines(edges,s,c);
  transform = make_rotY(45);
  matrix_mult(transform,edges);
  draw_lines(edges,s,c);
  save_extension(s,"pic.png");
  
  
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  free_matrix( transform );
  free_matrix( edges );
}  
