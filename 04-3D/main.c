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
  c.red = 255;
  c.blue = 0;
  c.green = 0;

  struct matrix *edges = new_matrix(4,0);
  struct matrix *transform;
  
  add_box(edges,250,250,250,50,50,50);
  draw_lines(edges,s,c);
  display(s);

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  free_matrix( transform );
  free_matrix( edges );
}  
