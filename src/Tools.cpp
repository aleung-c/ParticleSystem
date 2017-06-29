#include "../includes/ParticleSystem.hpp"

double	GetRandomDouble(double min, double max)
{
    // double before = rand() % (int)max + (int)min;
    // double after = (double)rand() / RAND_MAX;
    // double result = before + after;
    // if (result < min || result > max) {
    //     result = GetRandomDouble(min, max);
    // }
    // return result;
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}
