#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  color c;
  c.red = 255;
  c.green = 0;
  c.blue = 0;

  clear_screen(s);
  
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4,4);
  add_circle(transform,250,250,100,4);
  print_matrix(transform);
  draw_lines(transform,s,c);
  display(s);
  
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  
  free_matrix( transform );
  free_matrix( edges );
}  
