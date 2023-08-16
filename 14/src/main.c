#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#define TASK_PART 2

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int min_x;
    int min_y;
    int width;
    int height;
    char** map;
} GameMap;

typedef struct {
    Point* data;
    int size;
    int capacity;
} PointArray;


PointArray* create_point_array(int initial_capacity) {
    PointArray* arr = (PointArray*) malloc(sizeof(PointArray));
    arr->data = (Point*) malloc(initial_capacity * sizeof(Point));
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void add_point(PointArray* arr, int x, int y) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (Point*) realloc(arr->data, arr->capacity * sizeof(Point));
    }
    arr->data[arr->size].x = x;
    arr->data[arr->size].y = y;
    arr->size++;
}

void print_map(GameMap* gameMap) {
    for (int i = 0; i < gameMap->height; i++) {
        for (int j = 0; j < gameMap->width; j++) {
            printf("%c", gameMap->map[i][j]);
        }
        printf(" %d\n", i);
    }
}


void draw_line(GameMap* gameMap, Point p1, Point p2) {
    if (p1.x == p2.x) {
        // Vertical line
        int start_y = (p1.y < p2.y) ? p1.y : p2.y;
        int end_y = (p1.y > p2.y) ? p1.y : p2.y;

        for (int y = start_y; y <= end_y; y++) {
            gameMap->map[y][p1.x] = '#';
        }
    } else if (p1.y == p2.y) {
        // Horizontal line
        int start_x = (p1.x < p2.x) ? p1.x : p2.x;
        int end_x = (p1.x > p2.x) ? p1.x : p2.x;

        for (int x = start_x; x <= end_x; x++) {
            gameMap->map[p1.y][x] = '#';
        }
    }
    #if TASK_PART == 2
        for (int x = 0; x < gameMap->width; x++) {
            gameMap->map[gameMap->height-1][x] = '#';
        }
    #endif
}


void find_bounds(FILE* file, GameMap* gameMap) {
    int x, y;
    char line[1000];
    char* ptr;
    int min_x = INT_MAX;
    int min_y = INT_MAX;
    int max_x = INT_MIN;
    int max_y = INT_MIN;

    while (fgets(line, sizeof(line), file)) {
        ptr = line;
        while (sscanf(ptr, "%d,%d", &x, &y) == 2) {
            if (x < min_x) min_x = x;
            if (y < min_y) min_y = y;
            if (x > max_x) max_x = x;
            if (y > max_y) max_y = y;

            ptr = strchr(ptr, '>');
            if (ptr) ptr += 2;
            else break;
        }
    }
    fseek(file, 0, SEEK_SET);

    // the x coordinate in the task does not start from 0, but from a number around 500. 
    // Therefore, we need to set the origin of coordinates for our game to the minimum from "min in task-1", 
    // in order to leave empty passages for sand on the left and right of the field
    gameMap->min_x = min_x-1; 
    gameMap->min_y = 0;

    // when setting the size, the field should be 1 block more to the left and right.
    // so that the sand wakes up. And the height is greater to consider the bottom line
    // as empty and complete the task
    gameMap->width = (max_x - min_x) + 3;
    gameMap->height = max_y + 3;

    gameMap->map = (char**) malloc(gameMap->height * sizeof(char*));
    for (int i = 0; i < gameMap->height; i++) {
        gameMap->map[i] = (char*) malloc(gameMap->width * sizeof(char));
        for (int j = 0; j < gameMap->width; j++) {
            gameMap->map[i][j] = '.';
        }
    }
}



void parse_input(const char *filename, GameMap* gameMap) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        exit(1);
    }

    find_bounds(file, gameMap);


    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        PointArray* arr = create_point_array(10);
        int x, y;
        char* ptr = line;
        
        while (sscanf(ptr, "%d,%d", &x, &y) == 2) {
            add_point(arr, x - gameMap->min_x, y - gameMap->min_y);
            ptr = strchr(ptr, '>'); 
            if (ptr) ptr += 2;
            else break;
        }
        
        for (int i = 0; i < arr->size - 1; i++) {
            draw_line(gameMap, arr->data[i], arr->data[i + 1]);
        }

        free(arr->data);
        free(arr);
    }
    
    fclose(file);
}


int is_cell_empty(GameMap* gameMap, Point p) {
    if (p.y >= gameMap->height ||
        p.x < 0 || 
        p.x >= gameMap->width ||
        gameMap->map[p.y][p.x] != '.') {
            return 0;
    }
    return 1;

}

Point calculate_next_move(GameMap* gameMap, Point current) {
    if(current.y == gameMap->height-1) return current;

    Point down = {current.x, current.y + 1};
    Point down_left = {current.x - 1, current.y + 1};
    Point down_right = {current.x + 1, current.y + 1};

    if (is_cell_empty(gameMap, down)) return down;

    else if (is_cell_empty(gameMap, down_left)) return down_left;
    else if (is_cell_empty(gameMap, down_right)) return down_right;

    else return current;
}


Point move_sand_grain(GameMap* gameMap, Point current) {
    Point next = calculate_next_move(gameMap, current);
    // If the sand grain has come to a stop.
    if (next.x == current.x && next.y == current.y) return current;
    
    gameMap->map[next.y][next.x] = '*';  // Sand particle in the new position.
    gameMap->map[current.y][current.x] = '.';  // Leaving an empty position at the old spot.
    return next;
}

int simulate_sand_drop(GameMap* gameMap, Point start) {
    Point current = start;
    Point next;
    do {
        next = calculate_next_move(gameMap, current);

        // If the grain has come to a stop.
        if (next.x == current.x && next.y == current.y) {
            gameMap->map[current.y][current.x] = 'o';
            return 0;
        }

        // If the grain has fallen off the edge.
        if (next.y == gameMap->height - 1) {
            gameMap->map[next.y][next.x] = '.';
            return -1;
        }

        current = next;
    } while (1);
}
int count_out_of_field_sand(GameMap* gameMap){
    int left_part_count = 0;
    int right_part_count = 0;
    for(int i = 0; i < gameMap->height; i++){
        if(left_part_count == 0 && gameMap->map[i][0] == 'o'){
            int sand_height = gameMap->height-i-2;
            left_part_count = ((float)sand_height/2.0)*(sand_height+1);
        }
        if(right_part_count == 0 && gameMap->map[i][gameMap->width-1] == 'o'){
            int sand_height = gameMap->height-i-2;
            right_part_count = ((float)sand_height/2.0)*(sand_height+1);
        }
    }
    return left_part_count + right_part_count;
}
int run_simulation(GameMap* gameMap, Point start) {
    int count = 0;
    while (1) {
        if(gameMap->map[start.y][start.x] == 'o'){
            break;
        }
        int result = simulate_sand_drop(gameMap, start);
        if (result == -1) break;
        count++;
    }
    #if TASK_PART == 2
        count += count_out_of_field_sand(gameMap);
    #endif
    return count;
}


int main() {
    GameMap* gameMap = (GameMap*) malloc(sizeof(GameMap));
    struct timeval start_time, end_time;

    parse_input("../data.txt", gameMap);
    gettimeofday(&start_time, NULL);

    Point start = {500-gameMap->min_x, 0};
    int result = run_simulation(gameMap, start);

    gettimeofday(&end_time, NULL);
    long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1e6 + (end_time.tv_usec - start_time.tv_usec);
    
    //print_map(gameMap);
    printf("total sand count = %d;\n", result);
    printf("Time taken: %ld microseconds\n", elapsed_time);

    return 0;
}

