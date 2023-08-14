#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

// Define the PacketItem structure to hold both lists and integers.
typedef struct PacketItem {
    int is_list;   // Indicates if the item is a list
    int value;     // Value of the item if it's an integer
    int count;     // Number of items if it's a list
    struct PacketItem** items; // Pointer to the list items if it's a list
} PacketItem;

// Function prototypes
PacketItem* parse_packet(char* str, int* idx);
int compare_packets(PacketItem* left, PacketItem* right);

// Function to parse a packet from a string representation
PacketItem* parse_packet(char* str, int* idx) {
    PacketItem* item = malloc(sizeof(PacketItem));
    item->is_list = str[*idx] == '[';
    item->count = 0;
    
    // If it's a list, parse the list elements recursively
    if (item->is_list) {
        (*idx)++;
        item->items = malloc(sizeof(PacketItem*) * MAX_SIZE);
        while (str[*idx] != ']') {
            if (str[*idx] != ',') {
                item->items[item->count++] = (struct PacketItem*)parse_packet(str, idx);
            } else {
                (*idx)++;
            }
        }
        (*idx)++;
    } else {
        sscanf(str + (*idx), "%d", &item->value);
        while (str[*idx] != ',' && str[*idx] != ']' && str[*idx] != '\0') {
            (*idx)++;
        }
    }
    return item;
}

int compare_packets(PacketItem* left, PacketItem* right) {
    // If both are numbers
    if (!left->is_list && !right->is_list) {
        if (left->value < right->value) return -1;
        if (left->value > right->value) return 1;
        return 0;
    }

    // If both are lists
    if (left->is_list && right->is_list) {
        int min_count = left->count < right->count ? left->count : right->count;
        for (int i = 0; i < min_count; i++) {
            int result = compare_packets((PacketItem*) left->items[i],(PacketItem*) right->items[i]);
            if (result != 0) {
                return result;
            }
        }
        if (left->count < right->count) return -1;
        if (left->count > right->count) return 1;
        return 0;
    }

    // If only one of them is a list
    if (left->is_list) {
        PacketItem temp = {.is_list = 1, .value = 0, .count = 1};
        PacketItem sub_item = {.is_list = 0, .value = right->value, .count = 0, .items = NULL};
        temp.items = malloc(sizeof(PacketItem*));
        temp.items[0] = (struct PacketItem*) &sub_item;
        int result = compare_packets(left, &temp);
        free(temp.items);
        return result;
    } else {
        PacketItem temp = {.is_list = 1, .value = 0, .count = 1};
        PacketItem sub_item = {.is_list = 0, .value = left->value, .count = 0, .items = NULL};
        temp.items = malloc(sizeof(PacketItem*));
        temp.items[0] = (struct PacketItem*) &sub_item;
        int result = compare_packets(&temp, right);
        free(temp.items);
        return result;
    }
}

void print_packet(PacketItem* item) {
    if (item->is_list) {
        printf("[");
        for (int i = 0; i < item->count; i++) {
            print_packet((PacketItem*)item->items[i]);
            if (i < item->count - 1) {
                printf(", ");
            }
        }
        printf("]");
    } else {
        printf("%d", item->value);
    }
}
void bubble_sort(PacketItem** packets, int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (compare_packets(packets[j], packets[j+1]) > 0) {
                PacketItem* temp = packets[j];
                packets[j] = packets[j+1];
                packets[j+1] = temp;
            }
        }
    }
}
int is_single_value_packet(PacketItem* packet, int value) {
    if (!packet->is_list) return 0;
    if (packet->count != 1) return 0;
    if (!packet->items[0]->is_list) return 0;
    if (packet->items[0]->count != 1) return 0;
    return packet->items[0]->items[0]->value == value;
}


int part_two_solution(PacketItem** packets, int packet_count) {
    // Add the two new packets [[2]] and [[6]]
    int idx = packet_count;
    packets[idx++] = parse_packet("[[2]]", &(int){0});
    packets[idx++] = parse_packet("[[6]]", &(int){0});

    // Sort the packets
    bubble_sort(packets, idx);

    // Find the positions of [[2]] and [[6]]
    int index_2 = -1, index_6 = -1;
    for (int i = 0; i < idx; i++) {
        if (is_single_value_packet(packets[i], 2)) {
            index_2 = i + 1;
        }
        if (is_single_value_packet(packets[i], 6)) {
            index_6 = i + 1;
        }
    }

    return index_2 * index_6;

}


int read_packets_from_file(const char* filename, PacketItem* packets[], int* packet_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("\033[1;31m");
        perror("Failed to open data.txt");
        printf("\033[0m"); 
        exit(1);
    }

    char line[MAX_SIZE];
    int packet_idx = 0;

    while (fgets(line, sizeof(line), file)) {
        if(strcmp(line,"\n") == 0){
            continue;
        }
        int idx = 0;
        packets[packet_idx] = parse_packet(line, &idx);
        packet_idx++;
    }

    fclose(file);
    *packet_count = packet_idx;

    return 0; 
}
void process_task1(PacketItem* packets[], int packet_count) {
    int sum_indices = 0;
    for (int i = 0; i < packet_count; i += 2) {
        if (compare_packets(packets[i], packets[i + 1]) <= 0) {
            sum_indices += (i / 2) + 1;
            //printf("index add: %d\n", (i / 2) + 1);
        }
    }
    printf("The sum of indices is: %d\n", sum_indices);
}

void process_task2(PacketItem* packets[], int packet_count) {
    int decoder_key = part_two_solution(packets, packet_count);
    printf("The decoder key is: %d\n", decoder_key);
}



int main() {
    PacketItem* packets[MAX_SIZE];
    int packet_count;

    read_packets_from_file("../data.txt", packets, &packet_count);

    process_task1(packets, packet_count);
    process_task2(packets, packet_count);

    return 0;
}