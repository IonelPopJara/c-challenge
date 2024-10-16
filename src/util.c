#include "raymath.h"
#include <string.h>
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

void truncate_str_after_directory_separator(char *path) {
    for (int i = strlen(path); i >= 0; --i) {
        if (path[i] == '\\' || path[i] == '/') {
            path[i+1] = '\0';
            return;
        }
    }
}

const char* ASSETS_FOLDER = "assets";
void parse_asset_path(char* buffer, char* local_path, const char* asset_name) {
    // Parse the relative path of an asset file
#ifdef _WIN32
    sprintf(buffer, "%s%s\\%s", local_path, ASSETS_FOLDER, asset_name);
#else
    sprintf(buffer, "%s%s/%s", local_path, ASSETS_FOLDER, asset_name);
#endif
}
