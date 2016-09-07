#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memcpy */
#include <stdint.h> /* gives fixed sized ints */
#include <assert.h> /* basic failure reporting */

#define NODES    14098 /* compile time limit for fixed memory allocation */
#define MAXPEERS 12    /* compile time limit on max edges out of a node */


int main(void)
