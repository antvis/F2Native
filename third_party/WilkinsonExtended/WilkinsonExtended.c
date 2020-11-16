#include "WilkinsonExtended.h"
#include <float.h>
#include <math.h>
#include <stdio.h>

double floored_mod(double a, double n) { return a - n * floor(a / n); }

double simplicity(size_t qpos, size_t Qlen, double j, double lmin, double lmax, double lstep) {
    int v = 0;
    if(floored_mod(lmin, lstep) < 1e-10 && lmin <= 0 && lmax >= 0)
        v = 1;

    return 1. - (qpos + 1 - 1.) / (Qlen - 1.) + v - j;
}

double simplicity_max(size_t qpos, size_t Qlen, unsigned int qlen, double j) {
    return 1. - (qpos + 1 - 1.) / (Qlen - 1.) - j + 1.;
}

double coverage(double dmin, double dmax, double lmin, double lmax) {
    return 1. - 0.5 * (pow((dmax - lmax), 2.) + pow((dmin - lmin), 2.)) / pow((0.1 * (dmax - dmin)), 2.);
}

double coverage_max(double dmin, double dmax, double span) {
    double trange = dmax - dmin;
    if(span > trange) {
        double half = (span - trange) / 2.;
        return 1. - 0.5 * (pow(half, 2.) + pow(half, 2.)) / pow((0.1 * trange), 2.);
    }

    return 1.;
}

double density(double k, double m, double dmin, double dmax, double lmin, double lmax) {
    double r = (k - 1.) / (lmax - lmin);
    double rt = (m - 1.) / (fmax(lmax, dmax) - fmin(lmin, dmin));
    return 2. - fmax(r / rt, rt / r);
}

double density_max(double k, double m) {
    if(k >= m)
        return 2. - (k - 1.0) / (m - 1.0);

    return 1.;
}

double legibility(double lmin, double lmax, double lstep) { return 1.; }

double score(double *weights, double simplicityVal, double coverageVal, double densityVal, double legibilityVal) {
    return weights[0] * simplicityVal + weights[1] * coverageVal + weights[2] * densityVal + weights[3] * legibilityVal;
}

int wilk_ext(double dmin, double dmax, double m, int only_inside, double *Q, unsigned int Qlen, double *w, double *outlmin, double *outlmax, double *outlstep) {
    //    if (dmin >= dmax) or (m < 1):
    //        return (dmin, dmax, dmax - dmin, 1, 0, 2, 0);
    int rst = -1;

    double best_score = -2.0;
    double j = 1.0;
    while(j < DBL_MAX) {
        for(size_t pos = 0; pos < Qlen; pos++) {
            double q = Q[pos];

            double sm = 1. - (pos) / (Qlen - 1.) - j + 1.;

            if(score(w, sm, 1., 1., 1.) < best_score) {
                j = DBL_MAX;
                break;
            }

            double k = 2.;
            while(k < DBL_MAX) {
                double dm = density_max(k, m);

                if(score(w, sm, 1, dm, 1) < best_score)
                    break;

                double delta = (dmax - dmin) / (k + 1.) / j / q;
                double z = ceil(log10(delta));

                while(z < DBL_MAX) {
                    double step = j * q * pow(10., z);
                    double cm = coverage_max(dmin, dmax, step * (k - 1.));

                    if(score(w, sm, cm, dm, 1) < best_score)
                        break;

                    double min_start = floor(dmax / step) * j - (k - 1.) * j;
                    double max_start = ceil(dmin / step) * j;

                    if(min_start > max_start) {
                        z += 1;
                        break;
                    }

                    for(double start = min_start; start <= max_start; start += 1.) {
                        double lmin = start * (step / j);
                        double lmax = lmin + step * (k - 1.0);
                        double lstep = step;

                        double s = simplicity(pos, Qlen, j, lmin, lmax, lstep);
                        double c = coverage(dmin, dmax, lmin, lmax);
                        double g = density(k, m, dmin, dmax, lmin, lmax);
                        double l = legibility(lmin, lmax, lstep);
                        double scr = score(w, s, c, g, l);

                        if((scr > best_score) && ((only_inside <= 0) || ((lmin >= dmin) && (lmax <= dmax))) &&
                           ((only_inside >= 0) || ((lmin <= dmin) && (lmax >= dmax)))) {
                            best_score = scr;

                            *outlmin = lmin;
                            *outlmax = lmax;
                            *outlstep = lstep;

                            rst = 0;
                        }
                    }

                    z += 1.;
                } // end of z-while-loop
                k += 1.;
            } // end of k-while-loop
        }
        j += 1.;
    }

    return rst;
}

int easy_wilk_ext(double dmin, double dmax, double m, int only_inside, double *outlmin, double *outlmax, double *outlstep) {
    static double Q[] = { 1., 5., 2., 2.5, 4., 3. };
    static double w[] = { 0.25, 0.2, 0.5, 0.05 };

    return wilk_ext(dmin, dmax, m, only_inside, Q, 6, w, outlmin, outlmax, outlstep);
}
