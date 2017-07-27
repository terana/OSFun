#include "config.h"

#if (WORD_LEN == 64)
typedef double num_t;
#endif

#if(WORD_LEN == 32)
typedef float num_t;
#endif

num_t sum(num_t a, num_t b) {
	return a + b;
}