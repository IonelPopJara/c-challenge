#include "raymath.h"
#include "stdio.h"

#include "util.h"

float lerpf(float a, float b, float scale, float dt) {
  // raymath.h provides `EPSILON`
  if (a > b - EPSILON && a < b + EPSILON) return b;
  else return a + (b - a) * (1 - exp2f(-scale * dt));
}

void printArt(char* file_path) {
    FILE *file_ptr;
    file_ptr = fopen(file_path, "r");

    char character;

    while((character = fgetc(file_ptr)) != EOF) {
        printf("%c", character);
    }

    fclose(file_ptr);
}
