#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 500  // maximum number of items for each monkey
#define NUM_MONKEYS 8  // number of monkeys
#define ROUNDS 20      // number of rounds

#define TASK_PART 1    


typedef enum {
    ADD,
    MULTIPLY,
    SQUARE
} Operation_type;


typedef struct {
    long long starting_items[MAX_ITEMS];           // list of start objects
    int num_items;                                 // number of objects in the monkey
    int inspect_num;                               // number of inspects
    Operation_type op_type;                        // operation type
    long long (*operation)(long long, long long);  // A reference to a operation function
    int op_value;                                  // this can be a value to multiply or add
    int test_divisor;                              // number by which we divide
    int throw_true;                                // the number of the monkey to which we throw, if divided
    int throw_false;                               // the number of the monkey to which we throw, if not divided
} Monkey;




long long add(long long a, long long b) {
    return a + b;
}

long long multiply(long long a, long long b) {
    return a * b;
}

long long square(long long a, long long b) {
    return a * a; 
}

void fill_monkeys_from_file(Monkey monkeys[]) {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char buffer[100];

    for (int i = 0; i < NUM_MONKEYS; i++) {

        fgets(buffer, sizeof(buffer), file);

        // read the initial subjects
        fscanf(file, " Starting items:");
        monkeys[i].num_items = 0;
        int temp = 0;
        while (fscanf(file, " %d,", &temp) == 1) {
            monkeys[i].starting_items[(monkeys[i].num_items)] = temp;
            //if(((long long) monkeys[i].starting_items[(monkeys[i].num_items)]) >= INT_MAX){
            //    monkeys[i].starting_items[(monkeys[i].num_items)] %= INT_MAX;
            //}
            monkeys[i].num_items++;
            
        }
        monkeys[i].inspect_num = 0;

        // read the operation
        char operation[10];
        char next_value[10];
        fscanf(file, " Operation: new = old %s %s", operation, next_value);

        // define the type of operation and a link to the corresponding function
        if (strcmp(operation, "*") == 0) {
            if (strcmp(next_value, "old") == 0) {
                monkeys[i].op_type = SQUARE;
                monkeys[i].operation = square;  
                monkeys[i].op_value = -1; // value that will not be used
            } else {
                monkeys[i].op_type = MULTIPLY;
                monkeys[i].operation = multiply;  
                monkeys[i].op_value = atoi(next_value);
            }
        } else {
            monkeys[i].op_type = ADD;
            monkeys[i].operation = add;  
            monkeys[i].op_value = atoi(next_value);
        }




        // read the divisible test
        fscanf(file, " Test: divisible by %d", &monkeys[i].test_divisor);

        // read where to throw if the value is true/false
        fscanf(file, " If true: throw to monkey %d", &monkeys[i].throw_true);

        fscanf(file, " If false: throw to monkey %d", &monkeys[i].throw_false);

        fgets(buffer, sizeof(buffer), file);  // read the rest of the line, if any

        // skip an empty line between monkey entries
        fgets(buffer, sizeof(buffer), file);  
    }

    fclose(file);
}




void display_monkeys_info(const Monkey monkeys[]) {
    for (int i = 0; i < NUM_MONKEYS; i++) {
        printf("Monkey %d:\n", i);
        printf("  Starting items: ");
        for (int j = 0; j < monkeys[i].num_items; j++) {
            printf("%lld", (long long) monkeys[i].starting_items[j]);
            if (j < monkeys[i].num_items - 1) {
                printf(", ");
            }
        }
        printf("\n");

        // Display operation
        if (monkeys[i].op_type == SQUARE) {
            printf("  Operation: new = old * old\n");
        } else {
            printf("  Operation: new = old %s %d\n", 
                   (monkeys[i].op_type == MULTIPLY) ? "*" : "+", 
                   monkeys[i].op_value);
        }

        printf("  Test: divisible by %d\n", monkeys[i].test_divisor);
        printf("    If true: throw to monkey %d\n", monkeys[i].throw_true);
        printf("    If false: throw to monkey %d\n", monkeys[i].throw_false);
        printf("\n"); 
    }
}
void process_monkey_item(Monkey *monkey, Monkey all_monkeys[], int item_index) {
    if (item_index < 0 || item_index >= monkey->num_items) {
        return;
    }

    // apply the given operation to the subject's current anxiety level
    long long new_value = monkey->operation(monkey->starting_items[item_index], monkey->op_value);
    // Divide the obtained value by 3 and round down
    #if TASK_PART == 1
        new_value /= 3;
    #elif TASK_PART == 2
        int devisor = 1;
        for(int i = 0; i < NUM_MONKEYS; i++){
            devisor *= all_monkeys[i].test_divisor;
        }
        new_value = new_value % devisor;
    #else
        #error "Invalid TASK_PART value"
    #endif
   
    // Perform an activity check
    int throw_to_index;
    if(monkey->op_value == -1){
        throw_to_index = 0;
        }
    if (new_value % monkey->test_divisor == 0) {
        throw_to_index = monkey->throw_true;
    } else {
        throw_to_index = monkey->throw_false;
    }
    monkey->inspect_num++;


    Monkey* throw_to_monkey = &all_monkeys[throw_to_index];
    throw_to_monkey->starting_items[throw_to_monkey->num_items++] = new_value;

    // Shift the items in the current monkey's list to remove the processed item
    for (int j = item_index; j < monkey->num_items - 1; j++) {
        monkey->starting_items[j] = monkey->starting_items[j+1];
    }
    monkey->num_items--;
}

void process_monkey(Monkey *monkey, Monkey all_monkeys[]) {
    int temp = monkey->num_items;
    for (int i = 0; i < temp; i++) {
        process_monkey_item(monkey, all_monkeys, 0);
    }
}

void total_rounds(Monkey monkeys[], int num_monkeys) {
    for (int round = 0; round < ROUNDS; round++) {
        for (int i = 0; i < num_monkeys; i++) {
            process_monkey(&monkeys[i], monkeys);
            
        }
    }

    // find the two largest number of checks
    int max1 = 0, max2 = 0;
    for (int i = 0; i < num_monkeys; i++) {
        if (monkeys[i].inspect_num > max1) {
            max2 = max1;
            max1 = monkeys[i].inspect_num;
        } else if (monkeys[i].inspect_num > max2) {
            max2 = monkeys[i].inspect_num;
        }
    }

    printf("Level of monkey business: %lld\n", (long long)max1 * max2);
}


int main() {
    Monkey monkeys[NUM_MONKEYS];
    fill_monkeys_from_file(monkeys);

    display_monkeys_info(monkeys);



    total_rounds(monkeys, NUM_MONKEYS);
    //display_monkeys_info(monkeys);


    return 0;
}
