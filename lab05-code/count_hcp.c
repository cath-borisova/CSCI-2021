#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcs.h"

#define NUM_PLAYERS 4
#define HAND_SIZE 13

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <index_file>\n", argv[0]);
        return 1;
    }

    char *file_name = argv[1];
    FILE *index_file = fopen(file_name, "r");
    if (index_file == NULL) {
        printf("Failed to open index file %s\n", file_name);
        return 1;
    }

    int indices[NUM_CARDS];
    int index;
    for (int i = 0; i < NUM_CARDS; i++) {
        fscanf(index_file, "%d", &index);
        indices[i] = index;
    }
    fclose(index_file);

    card_t *deck = generate_deck(indices);

    long seed;
    FILE* seed_file = fopen("seed.txt", "r+");
    if (seed_file == NULL) {
        printf("Failed to open file seed.txt\n");
        return 1;
    }
    int res = fscanf(seed_file, "%ld", &seed);
    if (res != 1) {
        seed = time(NULL);
        freopen("seed.txt", "w", seed_file);
        fprintf(seed_file, "%ld", seed);
    }
    fclose(seed_file);

    shuffle(deck, seed);

    card_t north[HAND_SIZE];
    card_t east[HAND_SIZE];
    card_t south[HAND_SIZE];
    card_t west[HAND_SIZE];
    card_t *hands[NUM_PLAYERS] = {north, east, south, west};
    deal(deck, NUM_PLAYERS, HAND_SIZE, hands);

    int hcp_n = count_hcp(HAND_SIZE, north);
    int hcp_s = count_hcp(HAND_SIZE, south);
    free(deck);
    if (hcp_n + hcp_s == 40) {
        printf("Wow! You've got all the high cards!\n");
    } else {
        printf("Oops! East and West have some high cards. Try again!\n");
    }
    return 0;
}
