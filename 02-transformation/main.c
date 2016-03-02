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

  /*
  print_matrix(edges);
  ident(edges);
  printf("Edges - Lastcol = %d\n",edges->lastcol);
  print_matrix(edges);
  scalar_mult(0.5,edges);
  print_matrix(edges);
  edges = make_translate(0.5,0.5,0.5);
  print_matrix(edges);
  printf("Transform:\n");
  copy_matrix(edges,transform);
  print_matrix(transform);
  printf("Transform - Lastcol = %d\n",transform->lastcol);
  grow_matrix(transform,8);
  print_matrix(transform);
  printf("%g\n",transform->m[0][transform->lastcol]);
  printf("\n");*/
  //ident(edges);
  //ident(transform);
  //matrix_mult(transform,edges);
  print_matrix(edges);
  add_point(edges,1,1,1);
  print_matrix(edges);
  
  
  free_matrix( transform );
  free_matrix( edges );
}  
