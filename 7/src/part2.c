// Link: https://adventofcode.com/2022/day/7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 5
#define SPACE_TO_FREE 5349983

typedef struct file_or_dir {
    char name[256];
    int is_directory;                            // 1 if directory, 0 if file
    int size;                                    // size of the directory or file
    int content_count;
    struct file_or_dir** contents;                // children if it's a directory
    struct file_or_dir* parent;                  // pointer to the parent directory
} file_or_dir;

file_or_dir* root;


file_or_dir* create_directory(const char* name) {
    file_or_dir* new_dir = malloc(sizeof(file_or_dir));
    new_dir->contents = malloc(sizeof(file_or_dir)*START_CAPACITY);
    strcpy(new_dir->name, name);
    new_dir->is_directory = 1;
    new_dir->size = 0;
    new_dir->parent = NULL;
    new_dir->content_count = 0;
    return new_dir;
}

file_or_dir* create_file(const char* name, int size) {
    file_or_dir* new_file = malloc(sizeof(file_or_dir));
    new_file->contents = malloc(sizeof(file_or_dir)*START_CAPACITY);
    strcpy(new_file->name, name);
    new_file->is_directory = 0;
    new_file->size = size;
    new_file->parent = NULL;
    new_file->content_count = 0;
    return new_file;
}


void add_to_directory(file_or_dir* parent, file_or_dir* child) {
    if(parent->content_count % 10 == 0) {
        parent->contents = realloc(parent->contents, sizeof(file_or_dir*) * (parent->content_count + 10));
    }
    parent->contents[parent->content_count++] = child;
    child->parent = parent;
    if (child->is_directory == 0) { // If it's a file, update the parent directory size
        parent->size += child->size;
    }
}

void traverse_tree(file_or_dir* root, int depth) {
    if (!root) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    if (root->is_directory) {
        printf("Directory: %s\n", root->name);
        // If it is a directory, iterate over its contents
        for (int i = 0; i < root->content_count; i++) {
            traverse_tree(root->contents[i], depth + 1);
        }
    } else {
        printf("File: %s, Size: %d bytes\n", root->name, root->size);
    }
}


void parse_filesystem() {
    char line[512];
    FILE* file = fopen("../data.txt", "r");

    root = create_directory("/");
    file_or_dir* current_dir = root;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "$ cd ", 5) == 0) {
            char dir_name[256];
            sscanf(line, "$ cd %s", dir_name);
            
        if (strcmp(dir_name, "..") == 0) {
            if (current_dir->parent) {
                current_dir = current_dir->parent;
            }
        } else if (strcmp(dir_name, "/") == 0) {
            current_dir = root;
        } else {
        file_or_dir* next_dir = NULL;

        for (int i = 0; i < current_dir->content_count; i++) {
            if (strcmp(current_dir->contents[i]->name, dir_name) == 0) {
                next_dir = current_dir->contents[i];
                break;
            }
        }

        if (next_dir) {
            current_dir = next_dir;
        } else {
            // If the directory is not found, create a new one and go to it
            file_or_dir* new_dir = create_directory(dir_name);
            add_to_directory(current_dir, new_dir);
            current_dir = new_dir;
        }
    }


        } else if (strstr(line, "dir ")) {
            char dir_name[256];
            sscanf(line, "dir %s", dir_name);
            if (strcmp(current_dir->name, dir_name) != 0) { // Adding this check
                file_or_dir* new_dir = create_directory(dir_name);
                add_to_directory(current_dir, new_dir);
            }
        } else {
            int file_size;
            char file_name[256];
            sscanf(line, "%d %s", &file_size, file_name);
            if (strcmp(current_dir->name, file_name) != 0) { // Adding this check
                file_or_dir* new_file = create_file(file_name, file_size);
                add_to_directory(current_dir, new_file);
            }
        }
    }
    
    fclose(file);
}


int get_directory_size(file_or_dir* directory, int* total_sum) {
    int size = 0;

    for (int i = 0; i < directory->content_count; ++i) {
        if (directory->contents[i]->is_directory) {
            size += get_directory_size(directory->contents[i], total_sum);
        } else {
            size += directory->contents[i]->size;
        }
    }

    if (size <= 100000) {
        *total_sum += size;
    }

    return size;
}

void calculate_directory_sizes() {
    int total_sum = 0;
    get_directory_size(root, &total_sum);
    printf("The sum of directories with total size of at most 100000 is: %d\n", total_sum);
}

file_or_dir* smallest_over_limit_directory = NULL;
int smallest_size_over_limit = INT_MAX;

int get_min_directory_size(file_or_dir* directory) {
    int size = 0;
    for (int i = 0; i < directory->content_count; ++i) {
        if (directory->contents[i]->is_directory) {
            size += get_min_directory_size(directory->contents[i]);
        } else {
            size += directory->contents[i]->size;
        }
    }
    if (size > SPACE_TO_FREE && size <= smallest_size_over_limit) {
        smallest_over_limit_directory = directory;
        smallest_size_over_limit = size;

    }
    return size;
}

int main() {
    parse_filesystem();
    
    //printf("Final tree:\n\n\n");
    //traverse_tree(root, 0);
    
    get_min_directory_size(root);
    if (smallest_over_limit_directory) {
        printf("The smallest directory with size over %d is: %s, Size: %d bytes\n",SPACE_TO_FREE, smallest_over_limit_directory->name, smallest_size_over_limit);
    } else {
        printf("No directory found with size over 8381165 bytes.\n");
    }
    return 0;
}