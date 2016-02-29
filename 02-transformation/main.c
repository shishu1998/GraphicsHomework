#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"

int main() {

  screen s;
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  
  print_matrix(edges);
  ident(edges);
  print_matrix(edges);
  scalar_mult(0.5,edges);
  print_matrix(edges);

  free_matrix( transform );
  free_matrix( edges );
}  
