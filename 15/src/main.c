#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Y_ROW 2000000
#define MAX_COORDINATES_X_Y 4000000

typedef struct {
    int a;
    int b;
} Coeff;


// Define the Point structure
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int start;
    int end;
} ExclusionZone;

// Define the Sensor structure which will have two Points: one for the sensor and one for the beacon
typedef struct {
    Point sensor;
    Point beacon;
} Sensor;

// Parse one line from the input file and fill the Sensor structure
Sensor parse_line(const char *line) {
    Sensor s;
    sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", 
           &s.sensor.x, &s.sensor.y, &s.beacon.x, &s.beacon.y);
    return s;
}

int read_sensors_from_file(const char* filename, Sensor* sensors, int max_sensors) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        exit(1);
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && count < max_sensors) {
        sensors[count] = parse_line(line);
        count++;
    }

    fclose(file);
    return count;
}

void print_sensors(const Sensor* sensors, int count) {
    for (int i = 0; i < count; i++) {
        printf("Sensor: (%d, %d), Beacon: (%d, %d)\n", 
               sensors[i].sensor.x, sensors[i].sensor.y, 
               sensors[i].beacon.x, sensors[i].beacon.y);
    }
}

int manhattan_distance(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int positions_without_beacon(Sensor* sensors, int count, int target_y) {
    int x_min = INT_MAX;
    int x_max = INT_MIN;

    for (int i = 0; i < count; i++) {
        int distance = manhattan_distance(sensors[i].sensor, sensors[i].beacon);

        // Calculate the start and end of exclusion zone for this sensor on the target y
        int x_start = sensors[i].sensor.x - (distance - abs(sensors[i].sensor.y - target_y));
        int x_end = sensors[i].sensor.x + (distance - abs(sensors[i].sensor.y - target_y));

        // Update the overall exclusion zone
        if (x_start < x_min) x_min = x_start;
        if (x_end > x_max) x_max = x_end;
    }

    if (x_min <= x_max) {
        return x_max - x_min; 
    } else {
        return 0; // No exclusion zone found
    }
}

Point find_gap(Sensor* sensors, int count) {
    Coeff lines[10000];
    int line_count = 0;
    Point gap = {-1, -1};

    for (int i = 0; i < count; i++) {
        int d = manhattan_distance(sensors[i].sensor, sensors[i].beacon) + 1;

        int sx = sensors[i].sensor.x;
        int sy = sensors[i].sensor.y;

        Coeff line_coeffs[4];
        line_coeffs[0] = (Coeff) {-1, sy - d + sx};
        line_coeffs[1] = (Coeff) {1, sy - d - sx};
        line_coeffs[2] = (Coeff) {-1, sy + d + sx};
        line_coeffs[3] = (Coeff) {1, sy + d - sx};

        for (int j = 0; j < 4; j++) {
            lines[line_count++] = line_coeffs[j];
        }
    }

    // Find intersection of lines
    for (int i = 0; i < line_count; i++) {
        for (int j = 0; j < line_count; j++) {
            if (i != j && lines[i].a != lines[j].a) {
                int x = (lines[j].b - lines[i].b) / (lines[i].a - lines[j].a);
                int y = lines[i].a * x + lines[i].b;

                // Check if the point is within the valid range
                if (x < 0 || x > 4000000 || y < 0 || y > 4000000) {
                    continue;
                }

                // Check if intersection point is within exclusion zones
                bool valid = true;
                for (int k = 0; k < count; k++) {
                    int distance = manhattan_distance(sensors[k].sensor, (Point){x, y});
                    if (distance <= manhattan_distance(sensors[k].sensor, sensors[k].beacon)) {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    gap.x = x;
                    gap.y = y;
                    return gap;
                }
            }
        }
    }

    return gap;
}



int main() {
    // Assuming a maximum of 100 sensors for now
    Sensor sensors[100];

    int count = read_sensors_from_file("../data.txt", sensors, 100);

    print_sensors(sensors, count);

    int result = positions_without_beacon(sensors, count, Y_ROW);
    printf("Positions without beacon at y=%d: %d\n",Y_ROW,  result);

    
    Point gap = find_gap(sensors, count);
    long long frequency = (long long) gap.x * 4000000 + gap.y;
    if (gap.x != -1 && gap.y != -1) {
        printf("Gap found at position: (%d, %d), frequency = %lli\n", gap.x, gap.y, frequency);
    }else{
        printf("No gap\n");
    }


    return 0;
}
