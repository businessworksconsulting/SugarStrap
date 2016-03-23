#include <pebble.h>
#include "randgen.h"

static long g_seed = 100;

void randgen_seed(int seed)
{
    g_seed = seed;    
}

int randgen(int max)
{
    g_seed = (((g_seed * 214013L + 2531011L) >> 16) & 32767);

    return g_seed % (max + 1);
}