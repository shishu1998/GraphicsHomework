#ifndef GMATH_H
#define GMATH_H

#include "matrix.h"

double * calculate_normal( double a1, double a2, double a3,
			   double b1, double b2, double b3 );
double calculate_dot( struct matrix *points,int i,double vx, double vy, double vz);
double * normalized(double x, double y, double z);
color calculate_diffuse(struct light* l, color light, struct constants* constants,
			double pointx, double pointy, double pointz,
			double normx, double normy, double normz);
#endif
