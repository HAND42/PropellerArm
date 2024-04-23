/* mwm: 30.05.2011
 *
 * Macros originally from fc.c, could be used elsewhere too
 */

#ifndef USEFULMACROS_H
#define USEFULMACROS_H

#define MAX(A,B) (((A)>(B))?(A):(B))
#define MIN(A,B) (((A)<(B))?(A):(B))

#define LIMIT_MIN(value, min) {if((value) < (min)) value = (min);}
#define LIMIT_MAX(value, max) {if((value) > (max)) value = (max);}
#define LIMIT_MIN_MAX(value, min, max) {if((value) <  (min)) value =  (min); else if((value) > (max)) value = (max);}
#define LIMIT_SYMMETRIC(value, lim)    {if((value) < -(lim)) value = -(lim); else if((value) > (lim)) value = (lim);}

#endif
