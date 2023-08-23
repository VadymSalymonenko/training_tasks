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

typedef struct {
    int minX, maxX;
    int minY, maxY;
    int minZ, maxZ;
} Bounds;


void initialize(CubeArray* array) {
    array->capacity = 100;
    array->size = 0;
    array->cubes = (Cube*)malloc(array->capacity * sizeof(Cube));
}

void determine_bounds(const CubeArray* array, Bounds* bounds) {
    if (array->size == 0) return;

    // Initialization of boundaries with the first cube
    bounds->minX = bounds->maxX = array->cubes[0].x;
    bounds->minY = bounds->maxY = array->cubes[0].y;
    bounds->minZ = bounds->maxZ = array->cubes[0].z;

    // Sorting cubes to find boundaries
    for (int i = 1; i < array->size; i++) {
        Cube cube = array->cubes[i];
        if (cube.x < bounds->minX) bounds->minX = cube.x;
        if (cube.x > bounds->maxX) bounds->maxX = cube.x;
        if (cube.y < bounds->minY) bounds->minY = cube.y;
        if (cube.y > bounds->maxY) bounds->maxY = cube.y;
        if (cube.z < bounds->minZ) bounds->minZ = cube.z;
        if (cube.z > bounds->maxZ) bounds->maxZ = cube.z;
    }
}


void add_cube(CubeArray* array, Cube cube) {
    if (array->size == array->capacity) {
        array->capacity += 100;
        array->cubes = (Cube*)realloc(array->cubes, array->capacity * sizeof(Cube));
    }
    array->cubes[array->size++] = cube;
}
Cube get_and_remove_first_cube(CubeArray* array) {
    if (array->size == 0) {
        fprintf(stderr, "Queue is empty, cannot retrieve cube.\n");
        exit(EXIT_FAILURE);
    }

    Cube first_cube = array->cubes[0];

    // Let's move all the cubes one position to the left
    for (int i = 0; i < array->size - 1; i++) {
        array->cubes[i] = array->cubes[i + 1];
    }

    array->size--;

    return first_cube;
}
Cube get_last_cube(const CubeArray* array) {
    if (array->size == 0) {
        fprintf(stderr, "Array is empty, cannot retrieve last cube.\n");
        exit(EXIT_FAILURE);
    }

    return array->cubes[array->size - 1];
}
int contains_cube(const CubeArray* array, Cube target_cube) {
    for (int i = 0; i < array->size; i++) {
        if (array->cubes[i].x == target_cube.x &&
            array->cubes[i].y == target_cube.y &&
            array->cubes[i].z == target_cube.z) {
            return 1; 
        }
    }
    return 0;
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

        // Let's check all six possible neighboring coordinates for this cube
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
int is_within_bounds(const Bounds* bounds, Cube cube) {
    return (cube.x >= bounds->minX && cube.x <= bounds->maxX) &&
           (cube.y >= bounds->minY && cube.y <= bounds->maxY) &&
           (cube.z >= bounds->minZ && cube.z <= bounds->maxZ);
}

int calculate_surface_area(const CubeArray* array) {
    CubeArray toVisit; 
    CubeArray visited; 
    initialize(&toVisit);
    initialize(&visited);

    Bounds bounds;
    determine_bounds(array, &bounds); // Determining the boundaries of the region
    bounds.maxX++;
    bounds.maxY++;
    bounds.maxZ++;
    bounds.minX--;
    bounds.minY--;
    bounds.minZ--;
    // Initialize directions to check neighboring cubes
    Cube directions[6] = {
        {1, 0, 0},
        {-1, 0, 0},
        {0, 1, 0},
        {0, -1, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    Cube startCube = {0, 0, 0}; 
    add_cube(&toVisit, startCube); 

    int surfaceArea = 0; 

    // Main loop for analyzing cubes
    while (toVisit.size > 0) {
        Cube currentCube = get_and_remove_first_cube(&toVisit); // Remove the current cube from the queue
        add_cube(&visited, currentCube); // Adding the current cube to visited

        for (int i = 0; i < 6; i++) {
            Cube neighbor = {currentCube.x + directions[i].x,
                             currentCube.y + directions[i].y,
                             currentCube.z + directions[i].z}; 

            if (is_within_bounds(&bounds, neighbor)) { 
                if (contains_cube(array, neighbor)) {
                    surfaceArea++;
                } else if (!contains_cube(&visited, neighbor) && !contains_cube(&toVisit, neighbor)) {
                    add_cube(&toVisit, neighbor); // If the cube has not yet been visited, add to the queue
                }
            }
        }
    }

    free_memory(&toVisit); 
    free_memory(&visited); 

    return surfaceArea; 
}




int main() {
    CubeArray array;
    initialize(&array);
    parse_file(&array, "../data.txt");

    int sides = calculate_sides(&array);
    printf("Total exposed sides: %d\n", sides);

    int sides1 = calculate_surface_area(&array);
    printf("Total calculate_surface_area sides: %d\n", sides1);
    free_memory(&array);
    return 0;
}
