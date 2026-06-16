#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//
// DATA
//

typedef char figure;

figure chart[15];
figure *motherdaughters = chart;
figure *mothers = chart;
figure *daughters = chart + 4;
figure *nieces = chart + 8;
figure *witnesses = chart + 12;
figure *judge = chart + 14;

//
// FUNCTIONS
//

// Math utils
int iexp(const int num, const int pow) {
  int result = 1;
  for (int i=0; i<pow; i++) {
    result *= num;
  }
  return result;
}
#define tento(i) iexp(10, (i))

int itobase(int i, const int base) {
  int places = 1;
  int placeCheck = base;
  while (i > placeCheck - 1) {
    placeCheck *= base;
    ++places;
  }
  int result = 0;
  for (int j=0; j<places; j++) {
    result += (i%base) * tento(j);
    i /= base;
  }
  return result;
}
#define itofig(i) itobase((i), 2)

int get_dot(figure fig, int dot) {
  int shifts = 3-dot;
  fig = (fig >> shifts) & 0b1;
  return fig;
}

// Printing
#include "print_chart.h"

// User flow
void dummyInput(void) {
  printf("\nAsk a question. Press ENTER/RETURN when ready.\n\n");
  while(getchar() != '\n') {}
  putchar('\n');
}

void userExit(void) {
  printf("\n\n\nPress ENTER/RETURN to exit.\n");
  while(getchar() != '\n') {}
}

//
// MAIN
//

int main(void) {
  dummyInput();
  srand(time(NULL));

  // Mothers
  for (int i=0; i<4; i++) {
    mothers[i] = rand() % 15;
  }
  // Daughters
  for (int i=0; i<4; i++) {
    daughters[i] = 0;
    for (int j=0; j<4; j++) {
      daughters[i] += get_dot(mothers[j], i) * iexp(2, 3-j);
    }
  }
  // Nieces
  for (int i=0; i<4; i++) {
    nieces[i] = motherdaughters[i*2] ^ motherdaughters[i*2 + 1];
  }
  // Witnesses + Judge
  witnesses[0] = nieces[0] ^ nieces[1];
  witnesses[1] = nieces[2] ^ nieces[3];
  *judge = witnesses[0] ^ witnesses[1];

  printf("SHIELD CHART:\n\n");
  print_shield(chart);
  printf("\n\nHOUSE CHART:\n\n");
  print_houses(chart);
  userExit();

  return 0;
}
