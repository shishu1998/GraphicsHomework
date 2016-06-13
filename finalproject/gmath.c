#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "symtab.h"
#include "matrix.h"
#include "gmath.h"

/*======== double * calculate_normal() ==========
  Inputs:   double ax
            double ay
	    double az
	    double bx
	    double by
	    double bz  
  Returns: A double arry of size 3 representing the 
           cross product of <ax, ay, az> and <bx, by, bz>

  04/17/12 16:46:30
  jonalf
  ====================*/
double * calculate_normal( double ax, double ay, double az,	
			   double bx, double by, double bz ) {
  
  double *normal;
  normal = (double *)malloc(3 * sizeof(double));

  normal[0] = ay*bz - az*by;
  normal[1] = az*bx - ax*bz;
  normal[2] = ax*by - ay*bx;

  return normal;
}

/*======== double calculate_dot() ==========
  Inputs:   struct matrix *points
            int i  
  Returns: The dot product of a surface normal and
           a view vector
  
  calculates the dot product of the surface normal to
  triangle points[i], points[i+1], points[i+2] and a 
  view vector (use <0, 0, -1> to start.

  04/17/12 16:38:34
  jonalf
  ====================*/
double calculate_dot( struct matrix *points, int i, double vx, double vy, double vz ) {

  double ax, ay, az, bx, by, bz;
  double *normal;
  double dot;

  //calculate A and B vectors
  ax = points->m[0][i+1] - points->m[0][i];
  ay = points->m[1][i+1] - points->m[1][i];
  az = points->m[2][i+1] - points->m[2][i];

  bx = points->m[0][i+2] - points->m[0][i];
  by = points->m[1][i+2] - points->m[1][i];
  bz = points->m[2][i+2] - points->m[2][i];

  //get the surface normal
  normal = calculate_normal( ax, ay, az, bx, by, bz );

  //calculate dot product
  dot = normal[0] * vx + normal[1] * vy + normal[2] * vz;

  free(normal);  
  return dot;
}

double * normalize(double x, double y, double z){
  double magnitude = sqrt(x*x + y*y + z*z);
  double * normalized = (double *)malloc(3 * sizeof(double));
  normalized[0] = x / magnitude;
  normalized[1] = y / magnitude;
  normalized[2] = z / magnitude;
  return normalized;
}

color calculate_diffuse(struct light* l,color light, struct constants* constants,
			double pointx, double pointy, double pointz,
			double normx, double normy, double normz){
  double* normN = normalize(normx,normy,normz);
  double* L = (double *)malloc(3 * sizeof(double));
  L[0] = l->l[0]-pointx;
  L[1] = l->l[1]-pointy;
  L[2] = l->l[2]-pointz;
  double* normL = normalize(L[0],L[1],L[2]);
  double scalar = normN[0] * normL[0] + normN[1] * normL[1] + normN[2] * normL[2];
  light.red = ((double)(l->c[0]))* scalar * constants->red + light.red * constants->red;
  light.green = ((double)(l->c[1]))* scalar * constants->green + light.green * constants->green;
  light.blue = ((double)(l->c[2]))* scalar * constants->blue + light.blue * constants->blue;
  light.red = light.red<255? light.red : 255;
  light.red = light.red>0? light.red : 0;
  light.green = light.green<255? light.green : 255;
  light.green = light.green>0? light.green : 0;
  light.blue = light.blue<255? light.blue : 255;
  light.blue = light.blue>0? light.blue : 0;
  free(L);
  free(normL);
  free(normN);
  return light;
}
