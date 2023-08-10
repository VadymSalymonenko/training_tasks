#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_SIZE 200

int heightmap[MAX_SIZE][MAX_SIZE];
int gScore[MAX_SIZE][MAX_SIZE];
int fScore[MAX_SIZE][MAX_SIZE];

typedef struct {
    int x, y;
} Point;


Point directions[4] = {
    {0, -1},
    {0, 1},
    {-1, 0},
    {1, 0}
};
typedef struct {
    Point position;
    Point parent;  // Only this field is required for now
} Node;

Point cameFrom[MAX_SIZE][MAX_SIZE];  // To store parent of each point

Point start, end;
int row = 0, col = 0;



char is_valid_move(int curr_height, int next_height) {
    if(curr_height > next_height) return 1;
    return (abs(curr_height - next_height) <= 1);
}

int get_neighbors(Point p, Point neighbors[], int row, int col) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        Point next = {p.x + directions[i].x, p.y + directions[i].y};
        if (next.x >= 0 && next.x < col && next.y >= 0 && next.y < row) {
            if (is_valid_move(heightmap[p.y][p.x], heightmap[next.y][next.x])) {
                neighbors[count++] = next;
            }
        }
    }
    return count;
}

void print_heightmap() {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%3d", heightmap[i][j]);
        }
        printf("\n");
    }
}

void read_file(char* name){
    FILE *file = fopen(name, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    char ch;
    
    while (1) {
        ch = fgetc(file);
        if(ch == EOF){
            row++;
            break;  
        }
        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }

        // Convert characters to their integer values
        if (ch == 'S') {
            start.x = col;
            start.y = row;
            heightmap[row][col] = 1;
        } else if (ch == 'E') {
            end.x = col;
            end.y = row;
            heightmap[row][col] = 26; // Assuming z is the highest point
        } else {
            heightmap[row][col] = ch - 'a' + 1;
        }

        gScore[row][col] = INT_MAX;
        fScore[row][col] = INT_MAX;
        
        col++;
    }

    fclose(file);
}

int get_heuristic(Point a, Point b) {
    int manhattan_distance = abs(a.x - b.x) + abs(a.y - b.y);
    int height_difference = abs(heightmap[a.y][a.x] - heightmap[b.y][b.x]);
    
    return manhattan_distance + height_difference;
}

bool is_in_closed_list(Point p, Point closedList[], int closedListCount) {
    for (int i = 0; i < closedListCount; i++) {
        if (closedList[i].x == p.x && closedList[i].y == p.y) return true;
    }
    return false;
}

bool is_in_open_list(Point p, Point openList[], int openListCount) {
    for (int i = 0; i < openListCount; i++) {
        if (openList[i].x == p.x && openList[i].y == p.y) return true;
    }
    return false;
}

int find_index_of_lowest_f(Point openList[], int openListCount) {
    int lowestIndex = 0;
    for (int i = 1; i < openListCount; i++) {
        if (fScore[openList[i].y][openList[i].x] < fScore[openList[lowestIndex].y][openList[lowestIndex].x]) {
            lowestIndex = i;
        }
    }
    return lowestIndex;
}


void remove_point_from_open_list(Point p, Point openList[], int *openListCount) {
    for (int i = 0; i < *openListCount; i++) {
        if (openList[i].x == p.x && openList[i].y == p.y) {
            for (int j = i; j < *openListCount - 1; j++) {
                openList[j] = openList[j+1];
            }
            (*openListCount)--;
            break;
        }
    }
}



int reconstruct_path(Point end) {
    Point path[MAX_SIZE*MAX_SIZE];
    int pathLength = 0;

    Point current = end;
    while (!(current.x == start.x && current.y == start.y)) {
        path[pathLength++] = current;
        current = cameFrom[current.y][current.x];
    }
    path[pathLength++] = start;
    return pathLength-1;
}

int a_star() {
    Point openList[MAX_SIZE*MAX_SIZE];
    int openListCount = 0;

    Point closedList[MAX_SIZE*MAX_SIZE];
    int closedListCount = 0;

    openList[openListCount++] = start;

    while (openListCount > 0) {
        int currentIndex = find_index_of_lowest_f(openList, openListCount);
        Point currentPoint = openList[currentIndex];
        if (currentPoint.x == end.x && currentPoint.y == end.y) {
            return reconstruct_path(end);;
        }

        remove_point_from_open_list(currentPoint, openList, &openListCount);
        closedList[closedListCount++] = currentPoint;

        Point neighbors[4];
        int neighbors_count = get_neighbors(currentPoint, neighbors, row, col);

        for (int i = 0; i < neighbors_count; i++) {
            Point neighbor = neighbors[i];
            if (is_in_closed_list(neighbor, closedList, closedListCount)) {
                continue;
            }
            int tentative_gScore = gScore[currentPoint.y][currentPoint.x] + 1;  // assuming cost is 1 for each move
            if (!is_in_open_list(neighbor, openList, openListCount) || tentative_gScore < gScore[neighbor.y][neighbor.x]) {
                cameFrom[neighbor.y][neighbor.x] = currentPoint;  // Keep track of the parent node
                gScore[neighbor.y][neighbor.x] = tentative_gScore;
                fScore[neighbor.y][neighbor.x] = tentative_gScore + get_heuristic(neighbor, end);

                if (!is_in_open_list(neighbor, openList, openListCount)) {
                    openList[openListCount++] = neighbor;
                }
            }
        }
    }

    return -1;
}
int find_min_distanse_a_z(){
    int min = INT_MAX;
    int result  = INT_MAX;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if(heightmap[i][j] == 1){
                start.x = j;
                start.y = i;
                gScore[start.y][start.x] = 0;
                result = a_star();
                if(min > result && result >= 0){
                    min = result;
                }
            }
        }
    }
    if(min == INT_MAX){
        return -1;
    }
    return min;
}

int main() {
    int result = -1;
    int task2_result = -1;

    read_file("data.txt");
    
    gScore[start.y][start.x] = 0;
    fScore[start.y][start.x] = get_heuristic(start, end);

    //printf("start x = %d; y = %d.\n",start.x, start.y);
    //printf("end x = %d; y = %d.\n", end.x, end.y);
    //print_heightmap();

    result = a_star();
    if(result >= 0){
        printf("task 1: total length from Start to End= %d\n", result);
    }else{
        printf("No path found.\n");
    }

    
    task2_result = find_min_distanse_a_z();
    if(task2_result >= 0){
        printf("task 2: min length from height a to End = %d\n", task2_result);
    }else{
        printf("Min length no found.\n");
    }
    return 0;
}
