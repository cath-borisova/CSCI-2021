// Author: Davis Everson-Rose

#include <stdlib.h>
#include "funcs.h"

card_t *generate_deck(const int *indices) {
    card_t *deck = malloc(sizeof(card_t) * NUM_CARDS);
    for (int s = 0; s < NUM_SUITS; ++s) {
        for (int r = 0; r < NUM_RANKS; ++r) {
            int idx = s * 13 + r;
            card_t card = { (suit_t) s, (rank_t) r };
            deck[indices[idx]] = card;
        }
    }
    return deck;
}

card_t *shuffle(card_t *deck, long seed) {
    srand(seed);
    for (int index = NUM_CARDS - 1; index >= 1; index--) {
        int new_index = rand() % index;
        card_t temp = deck[new_index];
        deck[new_index] = deck[index];
        deck[index] = temp;
    }
    return deck;
}

int deal(card_t *deck, int nplayers, int hand_size, card_t **hands) {
    if (nplayers * hand_size > NUM_CARDS) {
        return 1;
    }
    for (int i = 0; i < hand_size; i++) {
        for (int j = 0; j < nplayers; j++) {
            int index = i * nplayers + j;
            hands[j][i] = deck[index];
        }
    }
    return 0;
}

int count_hcp(int ncards, card_t *hand) {
    int hcp = 0;
    for (int i = 0; i < ncards; i++) {
        rank_t rank = hand[i].rank;
        switch (rank) {
            case JACK:
                hcp += 1;
                break;
            case QUEEN:
                hcp += 2;
                break;
            case KING:
                hcp += 3;
                break;
            case ACE:
                hcp += 4;
                break;
            default:
                break;
        }
    }
    return hcp;
}
