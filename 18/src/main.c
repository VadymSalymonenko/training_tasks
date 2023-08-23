#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int x, y, z;
} Cube;

typedef struct {
    Cube* cubes;
    int size;
    int capacity;
} CubeArray;

void initialize(CubeArray* array) {
    array->capacity = 100;
    array->size = 0;
    array->cubes = (Cube*)malloc(array->capacity * sizeof(Cube));
}

void add_cube(CubeArray* array, Cube cube) {
    if (array->size == array->capacity) {
        array->capacity += 100;
        array->cubes = (Cube*)realloc(array->cubes, array->capacity * sizeof(Cube));
    }
    array->cubes[array->size++] = cube;
}

void parse_file(CubeArray* array, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(-1);
    }
    int x, y, z;
    while (fscanf(file, "%d,%d,%d", &x, &y, &z) == 3) {
        Cube cube = {x, y, z};
        add_cube(array, cube);
    }
    fclose(file);
}

void free_memory(CubeArray* array) {
    free(array->cubes);
}

void print_cubes(const CubeArray* array) {
    printf("Cubes (%d):\n", array->size);
    for (int i = 0; i < array->size; i++) {
        printf("Cube %d: x = %d, y = %d, z = %d\n", i, array->cubes[i].x, array->cubes[i].y, array->cubes[i].z);
    }
}

int is_adjacent(Cube a, Cube b) {
    int diff = 0;
    if (a.x == b.x + 1 || a.x == b.x - 1) diff++;
    if (a.y == b.y + 1 || a.y == b.y - 1) diff++;
    if (a.z == b.z + 1 || a.z == b.z - 1) diff++;
    return diff == 1;
}

int calculate_sides(const CubeArray* array) {
    int total_sides = 0;

    for (int i = 0; i < array->size; i++) {
        Cube cube = array->cubes[i];
        int sides_exposed = 6;

        // check all six possible neighboring coordinates for this cube
        Cube neighbors[6] = {
            {cube.x + 1, cube.y, cube.z},
            {cube.x - 1, cube.y, cube.z},
            {cube.x, cube.y + 1, cube.z},
            {cube.x, cube.y - 1, cube.z},
            {cube.x, cube.y, cube.z + 1},
            {cube.x, cube.y, cube.z - 1}
        };

        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < array->size; k++) {
                if (array->cubes[k].x == neighbors[j].x &&
                    array->cubes[k].y == neighbors[j].y &&
                    array->cubes[k].z == neighbors[j].z) {
                    sides_exposed--; // If a neighbor is found, we reduce the number of open sides
                    break;
                }
            }
        }

        total_sides += sides_exposed;
    }

    return total_sides;
}




int main() {
    CubeArray array;
    initialize(&array);
    parse_file(&array, "../data.txt");

    int sides = calculate_sides(&array);
    printf("Total exposed sides: %d\n", sides);

    free_memory(&array);
    return 0;
}
