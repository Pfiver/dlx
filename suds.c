// Sudoku solver.
//
// Input: nonzero digits represent themselves, and '0' or '.' represents an
// unknown digit. All other characters are ignored. Examples:
//
// .......1.
// 4........
// .2.......
// ....5.4.7
// ..8...3..
// ..1.9....
// 3..4..2..
// .5.1.....
// ...8.6...
//
//  . . . | . . . | . 1 2  
//  . . . | . . . | . . 3  
//  . . 2 | 3 . . | 4 . .  
// -------+-------+------ 
//  . . 1 | 8 . . | . . 5  
//  . 6 . | . 7 . | 8 . .  
//  . . . | . . 9 | . . .  
// -------+-------+------ 
//  . . 8 | 5 . . | . . .  
//  9 . . | . 4 . | 5 . .  
//  4 7 . | . . 6 | . . .  
//
// Shows step-by-step reasoning when run with -v option.
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "dlx.h"

#define F(i,n) for(int i = 0; i < n; i++)
#define C(i,n,dir) for(cell_t i = n->dir; i != n; i = i->dir)

// print_solution
int a[9][9] = {{0}}, c, sn = 1;
void print_solution(int row[], int n) {
	printf("# %d\n", sn++);
	F(i, n) a[row[i]/9%9][row[i]%9] = row[i]/9/9 + 1;
//	F(r, 9) F(c, 9 || (putchar('\n'), 0)) putchar('0'+a[r][c]);
	F(R, 3) {
	    F(r, 3) {
	        F(s, 3) {
	            F(c, 3) {
	                putchar(' ');
	                putchar('0'+a[3*R+r][3*s+c]);
	            }
	            printf(s < 2 ? " |" : "\n");
	        }
	    }
	    if (R<2) puts("-------+-------+-------");
	}
}

// print_reasoning
int kid[9*9], n = 0, tried[9*9] = { 0 }, indent;
void tabs() { F(i, indent) fputs("  ", stdout); }
void con(int c) {
  int k = c%(9*9);
  switch(c/9/9) {
    case 0: printf("! %d %d", k/9+1, k%9+1); break;
    case 1: printf("%d r %d", k/9+1, k%9+1); break;
    case 2: printf("%d c %d", k/9+1, k%9+1); break;
    case 3: printf("%d x %d %d", k/9+1, k%9/3+1, k%9%3+1); break;
  }
}
void cover(int c, int s, int r) {
  if (!tried[n]) {
    kid[n] = s;
    indent += s > 1;
  }
  tabs(), con(c);
  if (s == 1) printf(" =>"); else printf(" guess [%d/%d]:", tried[n]+1, s);
  printf(" %d @ %d %d\n", r/9/9+1, r/9%9+1, r%9+1);
  n++;
}
void uncover() {
  n--;
  tried[n]++;
  if (tried[n] == kid[n]) {
    indent -= kid[n] > 1;
    tried[n] = 0;
  }
}
void found() { tabs(), puts("solved!"); }
void stuck(int c) { tabs(), con(c), puts(" => stuck! backtracking..."); }

// main
  int nine(int a, int b, int c) { return 9*9*a + 9*b + c; }
int main(int argc, char *argv[]) {
  int verbose = 0, opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    if (opt == 'v') verbose++; else {
      fprintf(stderr, "Usage: %s [-v]\n", *argv);
      exit(1);
    }
  }
  F(i, 9) F(j, 9) do if (EOF == (c = getchar())) exit(1); while(
      isdigit(c) ? a[i][j] = c - '0', 0 : c != '.');

  dlx_t dlx = dlx_new();
  F(d, 9) F(r, 9) F(c, 9) {
    int i = 0;
#define con2(x, y) dlx_set(dlx, nine(d, r, c), nine(i++, x, y));
    con2(r, c);            // One digit per cell.
    con2(r, d);            // One digit per row.
    con2(c, d);            // One digit per column.
    con2(r/3*3 + c/3, d);  // One digit per 3x3 region.
  }
  // Fill in the given digits.
  F(r, 9) F(c, 9) if (a[r][c]) dlx_pick_row(dlx, nine(a[r][c]-1, r, c));

  if (verbose) {
    // Print reasoning.
    dlx_solve(dlx, cover, uncover, found, stuck);
  }
  else {
	dlx_forall_cover(dlx, print_solution);
  }
  dlx_clear(dlx);
  return 0;
}
