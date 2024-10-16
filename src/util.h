#ifndef UTIL_H
#define UTIL_H

/**
 * Simple lerp function that
 * takes delta time into account.
 * DO NOT USE FOR ROTATIONS!
 */
float lerpf(float a, float b, float scale, float dt);

void printArt(char* file_path);

void truncate_str_after_directory_separator(char *path);

void parse_asset_path(char* buffer, char* local_path, const char* asset_name);

#endif // UTIL_H

