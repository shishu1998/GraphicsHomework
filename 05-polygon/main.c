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

  struct matrix *edges;
  struct matrix *transform;
  struct matrix *polygons;
  
  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  polygons = new_matrix(4,4);


  add_sphere(polygons,250,250,50,100);
  draw_polygons(polygons,s,c);
  save_extension(s,"pic.png");
  clear_screen(s);
  ident(transform);
  transform = make_rotX(5);
  matrix_mult(transform,polygons);
  draw_polygons(polygons,s,c);
  save_extension(s,"pic2.png");
  
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  free_matrix( transform );
  free_matrix( edges );
}  
