#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for memcpy */
#include <stdint.h> /* gives fixed sized ints */
#include <assert.h> /* basic failure reporting */
#include <error.h>

#define NODES    14098 /* compile time limit for fixed memory allocation */
#define MAXPEERS 32    /* compile time limit on max edges out of a node */


int32_t edges[NODES][MAXPEERS];

void read_edges();
int add_edge(int32_t, int32_t);


int main(void) {

  /* init the edge list */
  for (int n = 0; n < NODES; n++) {
    for (int p = 0; p < MAXPEERS; p++) {
      edges[n][p] = -1;
    }
  }

  read_edges();

  return 0;
}


void read_edges() {
  int scanret, lnum, ecount, byte;
  int32_t e1, e2;

  lnum = 0;
  ecount = 0;
  while (1) {
    scanret = fscanf(stdin, "%d,%d\n", &e1, &e2);
    lnum++;

    if (scanret == EOF) {
      lnum--;
      break;
    }
    else if (scanret != 2) {
      fprintf(stderr, "Got bogus edge on input line number %d\n", lnum);

      /* need to advance input past this bogus line */
      while (1) {
	byte = fgetc(stdin);
	if ((byte == '\n') || (byte == EOF)) {
	  break;
	}
      }

      continue; /* move on to the next line */
    }
    else {
      ecount += add_edge(e1, e2); /* count this edge */
      add_edge(e2, e1); /* don't count this edge (undirected graph) */
    }
  } /* end while (1) */

  fprintf(stderr, "Read %d lines and got %d distinct edges\n", lnum, ecount);
}


int add_edge(int32_t n, int32_t p) {

  assert(n >= 0);
  assert(p >= 0);
  assert(n < NODES);
  assert(p < NODES);

  for (int i = 0; i < MAXPEERS; i++) {
    if (edges[n][i] == -1) {
      edges[n][i] = p;
      return 1;
    }
    else if (edges[n][i] == p) {
      return 0;
    }
  }

  fprintf(stderr, "Unable peer list for node %d exceeded %d!\n", n, MAXPEERS);
  abort();

  return -1; /* not reachable with above abort */
}
