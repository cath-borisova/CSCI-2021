// Author: Davis Everson-Rose

#define NUM_CARDS 52
#define NUM_SUITS 4
#define NUM_RANKS 13

typedef enum { CLUB, DIAMOND, HEART, SPADE } suit_t;
typedef enum {
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
} rank_t;

typedef struct {
    suit_t suit;
    rank_t rank;
} card_t;

card_t *generate_deck(const int *indices);

card_t *shuffle(card_t *deck, long seed);

int deal(card_t *deck, int nplayers, int ncards, card_t **hands);

int count_hcp(int ncards, card_t *hand);
