#ifndef WILKINSO_NEXTENDED_H_
#define WILKINSO_NEXTENDED_H_

/*
An Extension of Wilkinson's Algorithm for Positioning Tick Labels on Axes
Justin Talbot, Sharon Lin, Pat Hanrahan
InfoVis 2010

This code file defines the three algorithms that we compared in the quantitative evaluation:
Heckbert's, Wilkinson's, and our Extended optimization search

It also includes code to generate the results of the quantitative evaluation (Figures 2 and 3).

Parameters (not necessarily used by all three functions):
dmin, dmax -- range of the data
m -- target number of tick label locations (not the number of intervals as with pretty!)
Q -- set of nice step sizes, elements >= 1 and < 10   (the default value is the Q recommended by the author)

The Extended-Wilkinson algorithm described in the paper.
Additional arguments:
maxdensitydelta -- the maximum change in the label density e.g. between 1/(1+maxdensitydelta) and (1+maxdensitydelta)
only.loose -- whether to constrain the search to only loose labelings
First our scoring functions
helper--define a mod function that works sensibly on non-integers

created: 2018/6/5
*/

/*Now the optimization search
*dmin*
    minimum of the data range
*dmax*
    maximum of the data range
*m*
    number of axis labels
*only_inside* : [ int ]
    controls if the first and last label include the data range.
    0  : doesn't matter
    >0 : label range must include data range
    <0 : data range must be larger than label range
*per_inch* : if per_inch=True then it specifies the number of ticks per
    inch instead of a fixed number, so the actual number of
    ticks depends on the size of the axis

*Q* : [ list of numbers ]
    numbers that are considered as 'nice'. Ticks will be
    multiples of these

*w* : [ list of numbers ]
    list of weights that control the importance of the four
    criteria: simplicity, coverage, density and legibility

out value:
  outlmin,outlmax,outlstep

return:
   -1 : value is not set
   0  : OK
*/
#ifdef __cplusplus
extern "C"
#endif
int wilk_ext(double dmin, double dmax, double m, int only_inside,
               double *Q ,unsigned int Qlen,double *w,
               double *outlmin, double *outlmax, double *outlstep);

#ifdef __cplusplus
extern "C"
#endif
int easy_wilk_ext(double dmin, double dmax, double m, int only_inside,
               double *outlmin, double *outlmax, double *outlstep);

#endif
