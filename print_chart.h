#include <stdio.h>
#include <stdlib.h>

//
// Data
//

typedef struct Canvas {
  int W, H, length;
  char c[];
} Canvas;

const char *dotchar[2] = {" O O ", "  O  "};

// 
// Functions
//

Canvas *new_Canvas(const int w, const int h) {
  Canvas *out = malloc(sizeof(Canvas) + sizeof(char) * w*h);
  out->W = w;
  out->H = h;
  out->length = w*h;
  for (int i=0; i<w*h; i++) {
    out->c[i] = ' ';
  }
  return out;
}

void printfig_num(figure fig) {
  fig = fig & 0b1111;
  printf("%04d", itofig(fig));
}

void print_houses(figure *chart) {
  Canvas *cv = new_Canvas(80, 40);
  const char inner = '*';
  
  // Update 1 (outer square and lines)
  for (int y=0; y < cv->H; y++) {
    for (int x=0; x < cv->W; x++) {
      int loc = x + y*cv->W;

      if (
        y == x/2 || 
        y == (x - cv->W)/-2 ||
        y == (x - cv->W/2)/2 ||
        y == (x - cv->W/2)/-2 ||
        y == (x + cv->W/2)/2 ||
        y == (x - cv->W/2*3)/-2
      ) {
        cv->c[loc] = inner;
      }

      if (
        y == 0 ||
        y == cv->H-1
      ) {
        cv->c[loc] = '-';
      }

      if ( 
        x == 0 ||
        x == cv->W-1
      ) {
        cv->c[loc] = '|';
      }
    }
  }

  // Update 2 (center square)
  for (int y=cv->H/4; y<cv->H/4*3; y++) {
    for (int x=cv->W/4; x<cv->W/4*3; x++) {
      int loc = x + y*cv->W;

      if (
        x == cv->W/4 || 
        x == cv->W/4*3-1 ||
        y == cv->H/4 ||
        y == cv->H/4*3-1
      ) {
        cv->c[loc] = inner;
      } else {
        cv->c[loc] = ' ';
      }
    }
  }

  // Update 3 (geomantic figures)
  const int COORDS[][2] = {
    {6,16},
    {3,24},
    {8,29},
    {16,27},
    {24,29},
    {29,24},
    {26,16},
    {29,8},
    {24,4},
    {16,6},
    {8,4},
    {3,8},

    {18,15},
    {14,15},
    {16,19}
  };
  for (int n=0; n<sizeof(COORDS)/sizeof(COORDS[0]); n++) {
    for (int y=0; y<4; y++) {
      for (int x=0; x<4; x++) {
        int xloc = (cv->W * COORDS[n][0])/32-2;
        int yloc = (cv->H * COORDS[n][1])/32-2;
        int loc = (xloc + x) + (yloc + y) * cv->W;
        int dotval = get_dot(chart[n], y);
        cv->c[loc] = dotchar[dotval][x]; 
      }
    }
  }

  // Draw
  for (int y=0; y < cv->H; y++) {
    for (int x=0; x < cv->W; x++) {
      putchar(cv->c[x + y*cv->W]);
    }
    putchar('\n');
  }

  free(cv);
}

void print_shield(figure *chart) {
  Canvas *cv = new_Canvas(76, 20);

  // Update
  for (int fig=0; fig<15; fig++) {
    int xloc = 0;                      // Calculate x offset for each figure
    if (fig < 8) {                     // Mothers and daughters
      xloc = cv->W * fig/9;
      if (fig > 3) {xloc += cv->W/9;}
    } else if (fig < 12) {             // Nieces
      int nfig = fig-8;
      xloc = cv->W * 2 * nfig/9 + cv->W/18;
      if (fig-8 > 1) {xloc += cv->W/9;}
    } else if (fig < 14) {            // Witnesses
      int nfig = fig-12;
      xloc = cv->W * nfig/2 + cv->W * nfig/18 + cv->W/9 + cv->W/18;
    } else {                          // Judge
      xloc = cv->W/2 - 5;
    }

    for (int line=0; line<4; line++) {
      int yloc = cv->W * line;       // y offset
      if (fig >= 8 && fig < 12) {
        yloc += 5 * cv->W;
      } else if (fig >= 12 && fig < 14) {
        yloc += 10 * cv->W;
      } else if (fig == 14) {
        yloc += 15 * cv->W;
      }

      for (int c=0; c<4; c++) {
        int loc = xloc + yloc + c;
        int dotval = get_dot(chart[fig], line);
        cv->c[loc] = dotchar[dotval][c];
      }
    }
  }

  // Print (RTL)
  for (int y=0; y<cv->H; y++) {
    for (int x=cv->W-1; x>=0; x--) {
      int loc = x + y*cv->W;
      putchar(cv->c[loc]);
    }
    putchar('\n');
  }

  free(cv);
}
