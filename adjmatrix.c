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
int add_peer(int32_t, int32_t (*)[], int);
void row_bfs(int32_t, int *);

int main(void) {
  int maxd;

  /* init the edge list */
  for (int n = 0; n < NODES; n++) {
    for (int p = 0; p < MAXPEERS; p++) {
      edges[n][p] = -1;
    }
  }

  read_edges();

  maxd = 0;
  for (int i = 0; i < NODES; i++) {
    row_bfs(i, &maxd);
  }

  return 0;
}


void row_bfs(int32_t row, int *maxd) {
  int d = 1; /* depth */
  int n; /* working node */
  int nn; /* new node */
  int clptr; /* pointer into cl list */
  int8_t dist_r[NODES]; /* the row of distances */
  int32_t cl[NODES], nl[NODES];

  dist_r[row] = 0; /* this node is adjacent to itself (distance 0) */

  for (int i = 0; i < NODES; i++) {
    dist_r[i] = -1;
    cl[i] = -1;
  }

  for (int i = 0; i < MAXPEERS; i++) {
    cl[i] = edges[row][i];
  }

  while (cl[0] != -1) { /* until there is nothing left in the current list */

    /*fprintf(stderr, "Doing depth %d\n", d);*/
    if (d > *maxd) {
      *maxd = d;
      fprintf(stderr, "Found new maximum depth: %d\n", d);
    }

    /* init the new list */
    for (int i = 0; i < NODES; i++) {
      nl[i] = -1;
    }

    /* all the current nodes are at distance d */
    for (int i = 0; i < NODES; i++) {
      n = cl[i];

      if (n != -1) {
	dist_r[n] = d;
      }
      else {
	break;
      }
    }

    /* now we need to find the new nodes reachable at the next depth */
    for (int i = 0; i < NODES; i++) {
      n = cl[i];

      if (n == -1) {
	break;
      }

      for (int j = 0; j < MAXPEERS; j++) {
	nn = edges[n][j];

	if (nn == -1) {
	  break;
	}

	if (dist_r[nn] == -1) {
	  /* add_peer(nn, &nl, NODES); */
	  nl[nn] = nn;
	}
      }
    }

    /*memcpy(cl, nl, sizeof(nl));*/
    clptr = 0;
    for (int i = 0; i < NODES; i++) {
      nn = nl[i];

      if (nn == -1) {
	continue;
      }

      cl[clptr] = nn;
      clptr++;
    }
    if (clptr < NODES) {
      cl[clptr] = -1;
    }

    d++;
  } /* end while the current list isn't empty */

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

  return add_peer(p, &(edges[n]), MAXPEERS);
}


int add_peer(int32_t p, int32_t (*plist)[], int len) {

  for (int i = 0; i < len; i++) {
    if ((*plist)[i] == -1) {
      (*plist)[i] = p;
      return 1;
    }
    else if ((*plist)[i] == p) {
      return 0;
    }
  }

  fprintf(stderr, "Maximum peer list exceeded %d!\n", len);
  abort();

  return -1; /* not reachable with above abort */

}
