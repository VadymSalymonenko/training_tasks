#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLUEPRINTS 30
#define MAX_LINE_LENGTH 256

typedef struct {
    int ore;
    int clay;
    int obsidian;
} RobotCost;

typedef struct {
    RobotCost ore_robot;
    RobotCost clay_robot;
    RobotCost obsidian_robot;
    RobotCost geode_robot;
} Blueprint;

typedef struct {
    Blueprint blueprints[MAX_BLUEPRINTS];
    int size;
} BlueprintList;

typedef struct {
    unsigned int ore_robot;
    unsigned int clay_robot;
    unsigned int obsidian_robot;
    unsigned int geode_robot;
} DroneCounts;

typedef enum {
    EMPTY_ROBOT = -1,
    ORE_ROBOT = 1,
    CLAY_ROBOT,
    OBSIDIAN_ROBOT,
    GEODE_ROBOT
} RobotType;

typedef struct {
    unsigned int ore;
    unsigned int clay;
    unsigned int obsidian;
    unsigned int geode;
} Warehouse;

void parse_line(char *line, Blueprint *bp) {
    // Parsing logic
    sscanf(line, "Blueprint %*d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
           &bp->ore_robot.ore, &bp->clay_robot.ore, &bp->obsidian_robot.ore, &bp->obsidian_robot.clay,
           &bp->geode_robot.ore, &bp->geode_robot.obsidian);

    // Other robot types will have their other resources set to 0
    bp->ore_robot.clay = bp->ore_robot.obsidian = 0;
    bp->clay_robot.clay = bp->clay_robot.obsidian = 0;
    bp->geode_robot.clay = 0;
}

BlueprintList read_blueprints(const char *filename) {
    BlueprintList bp_list;
    bp_list.size = 0;
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];

    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), file) != NULL && bp_list.size < MAX_BLUEPRINTS) {
        parse_line(line, &bp_list.blueprints[bp_list.size]);
        bp_list.size++;
    }

    fclose(file);
    return bp_list;
}
void print_blueprints(const BlueprintList *bp_list) {
    printf("Blueprints:\n");
    for (int i = 0; i < bp_list->size; i++) {
        printf("Blueprint %d:\n", i + 1);
        printf("  Ore Robot: %d ore\n", bp_list->blueprints[i].ore_robot.ore);
        printf("  Clay Robot: %d ore\n", bp_list->blueprints[i].clay_robot.ore);
        printf("  Obsidian Robot: %d ore, %d clay\n", bp_list->blueprints[i].obsidian_robot.ore, bp_list->blueprints[i].obsidian_robot.clay);
        printf("  Geode Robot: %d ore, %d obsidian\n", bp_list->blueprints[i].geode_robot.ore, bp_list->blueprints[i].geode_robot.obsidian);
        printf("\n");
    }
}
DroneCounts calculate_drones(const Blueprint *bp) {
    DroneCounts counts;

    counts.ore_robot = bp->obsidian_robot.ore;
    counts.clay_robot = bp->obsidian_robot.clay;
    counts.obsidian_robot = 1;
    counts.geode_robot = 0; 

    return counts;
}
void add_robot(DroneCounts *counts, RobotType robot_type) {
    switch (robot_type) {
        case ORE_ROBOT:
            counts->ore_robot++;
            break;
        case CLAY_ROBOT:
            counts->clay_robot++;
            break;
        case OBSIDIAN_ROBOT:
            counts->obsidian_robot++;
            break;
        case GEODE_ROBOT:
            counts->geode_robot++;
            break;
    }
}
void consume_resources(const Blueprint *bp, RobotType robot, Warehouse *warehouse) {
    switch (robot) {
        case ORE_ROBOT:
            warehouse->ore -= bp->ore_robot.ore;
            break;
        case CLAY_ROBOT:
            warehouse->ore -= bp->clay_robot.ore;
            break;
        case OBSIDIAN_ROBOT:
            warehouse->ore -= bp->obsidian_robot.ore;
            warehouse->clay -= bp->obsidian_robot.clay;
            break;
        case GEODE_ROBOT:
            warehouse->ore -= bp->geode_robot.ore;
            warehouse->obsidian -= bp->geode_robot.obsidian;
            break;
        default:
            break;
    }
}

char is_enough_resources(const Blueprint *bp, const Warehouse *warehouse, RobotType robot) {
    switch (robot) {
        case EMPTY_ROBOT:
            return 1;
        case ORE_ROBOT:
            return warehouse->ore >= bp->ore_robot.ore;
        case CLAY_ROBOT:
            return warehouse->ore >= bp->clay_robot.ore;
        case OBSIDIAN_ROBOT:
            return warehouse->ore >= bp->obsidian_robot.ore && warehouse->clay >= bp->obsidian_robot.clay;
        case GEODE_ROBOT:
            return warehouse->ore >= bp->geode_robot.ore && warehouse->obsidian >= bp->geode_robot.obsidian;
        default:
            return 0;
    }
}


unsigned int simulate_production(const Blueprint *bp, int minutes, const DroneCounts *current_counts, const Warehouse *warehouse) {
    if (minutes == 0) {
        return warehouse->geode;
    }
    if(minutes <= 10 && current_counts->obsidian_robot == 0) return 0;
    if(minutes <= 5 && current_counts->obsidian_robot == 1) return 0;
    if(minutes <= 15 && current_counts->clay_robot < 2) return 0;

    unsigned int max_geode = 0;

    for (RobotType robot_to_create = GEODE_ROBOT; robot_to_create >= EMPTY_ROBOT; robot_to_create--) {
        if (is_enough_resources(bp, warehouse, robot_to_create)) {
            Warehouse new_warehouse = *warehouse;
            consume_resources(bp, robot_to_create, &new_warehouse);
            new_warehouse.ore += current_counts->ore_robot;
            new_warehouse.clay += current_counts->clay_robot;
            new_warehouse.obsidian += current_counts->obsidian_robot;
            new_warehouse.geode += current_counts->geode_robot;

            DroneCounts new_counts = *current_counts;
            add_robot(&new_counts, robot_to_create);

            unsigned int geode_result = simulate_production(bp, minutes - 1, &new_counts, &new_warehouse);
            if (geode_result > max_geode) {
                max_geode = geode_result;
            }
            if(robot_to_create == GEODE_ROBOT || robot_to_create == OBSIDIAN_ROBOT)break;
        }
    }

    return max_geode;
}
unsigned int start_simulation(const Blueprint *bp, int minutes) {

    DroneCounts current_counts = {1, 0, 0, 0};
    Warehouse warehouse = {0, 0, 0, 0};

    return simulate_production(bp, minutes, &current_counts, &warehouse);
}

unsigned int calculate_total_quality(BlueprintList *bp_list, int minutes) {
    unsigned int total_quality = 0;

    for (int i = 0; i < bp_list->size; i++) {
        unsigned int geode_count = start_simulation(&bp_list->blueprints[i], minutes);
        unsigned int quality = (i + 1) * geode_count;

        total_quality += quality;

        printf("Blueprint ID %d: Geodes %u, Quality %u\n", i + 1, geode_count, quality);
    }

    return total_quality;
}

unsigned int calculate_geode_product(BlueprintList *bp_list, int minutes) {
    unsigned int product = 1;

    for (int i = 0; i < bp_list->size; i++) {
        unsigned int geode_count = start_simulation(&bp_list->blueprints[i], minutes);
        product *= geode_count;
        printf("Blueprint ID %d: Geodes %u\n", i + 1, geode_count);
    }
    return product;
}


int main() {
    const char *filename = "../data.txt";
    BlueprintList bp_list = read_blueprints(filename);

    print_blueprints(&bp_list);

    //unsigned int total_quality = calculate_total_quality(&bp_list, 24);
    //printf("Total quality task 1: %u\n", total_quality);

    bp_list.size = 3;
    unsigned int total_quality = calculate_geode_product(&bp_list, 32);
    printf("Total quality task 2: %u\n", total_quality);

    return 0;
}

