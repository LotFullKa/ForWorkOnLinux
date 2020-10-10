#include <stdio.h>
#include <stdint.h>

enum {
    LOGIC_AND = 38,
    LOGIC_OR = 124,
    LOGIC_XOR = 94,
    LOGIC_NOT = 126,

    NUM_INTERVAL_END = 58,
    NUM_SHIFT = 48,
    UP_CASE_END = 91,
    UP_CASE_SHIFT = 55,
    LOW_CASE_SHIFT = 61,

    NUM_COUNT = 10,
    UP_CASE_COUNT = 36,
};


uint8_t
char_position(const uint8_t *symbol){
    if (*symbol < NUM_INTERVAL_END)
        return *symbol - NUM_SHIFT;
    else if (*symbol < UP_CASE_END)
        return *symbol - UP_CASE_SHIFT;
    else
        return *symbol - LOW_CASE_SHIFT;
}

uint8_t
get_char(const uint8_t *i) {
    if (*i < NUM_COUNT)
        return *i + NUM_SHIFT;
    else if (*i < UP_CASE_COUNT)
        return *i + UP_CASE_SHIFT;
    else
        return *i + LOW_CASE_SHIFT;
}

void
action(const uint8_t *symbol, uint64_t *my_set, uint64_t *stack){
    uint8_t pos;
    switch(*symbol) {
        case LOGIC_AND:
            *my_set = *my_set & *stack;
            *stack = 0;
            break;
        case LOGIC_OR:
            *my_set = *my_set | *stack;
            *stack = 0;
            break;
        case LOGIC_XOR:
            *my_set = *my_set ^ *stack;
            *stack = 0;
            break;
        case LOGIC_NOT:
            *my_set = ~*my_set;
            break;
        default:
            pos = char_position(symbol);
            *stack = *stack | ((uint64_t)1 << pos);
    }
}

int
main(){
    uint64_t my_set = 0;
    uint64_t stack = 0;

    int symbol;
    while ((symbol = getchar()) != EOF) {
        action((const uint8_t *) &symbol, &my_set, &stack);
    }

    for (uint8_t i = 0; i < 62; i++) {
        if (my_set & ((uint64_t)1 << i))
            putchar(get_char(&i));
    }
    return 0;
}