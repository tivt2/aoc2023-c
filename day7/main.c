#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "day7/input"
#define TEST_INPUT "day7/test_input"

#define BUFFER_SIZE 1024

#define max(x, y) (x > y ? x : y)

enum HandType {
  FIVE_OF_A_KIND,
  FOUR_OF_A_KIND,
  FULL_HOUSE,
  THREE_OF_A_KIND,
  TWO_PAIR,
  ONE_PAIR,
  HIGH_CARD
};

const char *typeString[] = {
    "Five of a kind", "Four of a kind", "Full House", "Three of a kind",
    "Two pair",       "One pair",       "High card",
};

typedef struct {
  enum HandType type;
  int bid;
  char cards[5];
} Hand;

const char card_lut[] = {'2', '3', '4', '5', '6', '7', '8',
                         '9', 'T', 'J', 'Q', 'K', 'A'};

int card_value(char card) {
  for (int i = 0; i < sizeof(card_lut); i++) {
    if (card == card_lut[i]) {
      return i;
    }
  }
  return -1;
}

// 0 if equal or -1 if card1 > card2 or 1 if card1 < card2
int card_cmp(char card1, char card2) {
  int val1 = card_value(card1);
  int val2 = card_value(card2);
  if (val1 > val2) {
    return 1;
  } else if (val1 < val2) {
    return -1;
  } else {
    return 0;
  }
}

// 0 if equal or -1 if hand1 > hand2 or 1 if hand1 < hand
int rank_cmp(Hand hand1, Hand hand2) {
  if (hand1.type < hand2.type) {
    return 1;
  } else if (hand1.type > hand2.type) {
    return -1;
  } else {
    int cmp = 0;
    for (int i = 0; i < 5; i++) {
      if (hand1.cards[i] == hand2.cards[i]) {
        continue;
      }

      cmp = card_cmp(hand1.cards[i], hand2.cards[i]);
      if (cmp != 0) {
        break;
      }
    }

    return cmp;
  }
}

enum HandType hand_type_get(char cards[5]) {
  char count[5][2] = {{0, 0}};
  int max = 0;
  for (int i = 0; i < 5; i++) {
    char card = cards[i];
    for (int j = 0; j <= i + 1; j++) {
      char cmp = count[j][0];
      if (cmp == card || cmp == 0) {
        count[j][0] = card;
        count[j][1]++;
        max = max(max, count[j][1]);
        break;
      }
    }
  }

  int d = -1;
  switch (max) {
  case 5:
    return FIVE_OF_A_KIND;
    break;
  case 4:
    return FOUR_OF_A_KIND;
    break;
  case 3:
    if (count[0][1] == 2 || count[1][1] == 2) {
      return FULL_HOUSE;
    } else {
      return THREE_OF_A_KIND;
    }
    break;
  case 2:
    for (int i = 0; i < 4; i++) {
      if (count[i][1] == 2) {
        d++;
      }
    }
    if (d == 1) {
      return TWO_PAIR;
    } else {
      return ONE_PAIR;
    }
    break;
  default:
    return HIGH_CARD;
    break;
  }
}

void hand_merge(Hand *hands, size_t m, size_t len) {
  size_t i = 0, j = m;

  while (i < j && j < len) {
    if (rank_cmp(hands[i], hands[j]) == 1) {
      Hand tmp = hands[j];
      for (size_t k = j; k > i; k--) {
        hands[k] = hands[k - 1];
      }
      hands[i] = tmp;
      j++;
    }
    i++;
  }
}

void hand_sort(Hand *hands, size_t len) {
  if (len <= 1) {
    return;
  }

  size_t m = len / 2;
  hand_sort(hands, m);
  hand_sort(hands + m, len - m);

  hand_merge(hands, m, len);
}

int main(void) {
  FILE *file = fopen(INPUT, "r");
  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  Hand hands_arr[1024];
  size_t hands_len = 0;

  char line[BUFFER_SIZE];
  while (fgets(line, BUFFER_SIZE, file) != NULL) {
    Hand hand;
    char *hand_str = strtok(line, " ");

    memcpy(hand.cards, hand_str, 5);
    hand.bid = atoi(strtok(NULL, ""));
    hand.type = hand_type_get(hand.cards);
    hands_arr[hands_len] = hand;
    hands_len++;
  }

  hand_sort(hands_arr, hands_len);

  uint64_t result = 0;

  for (int i = 0; i < hands_len; i++) {
    Hand hand = hands_arr[i];
    result += hand.bid * (i + 1);
  }

  printf("answer: %lu\n", result);

  return 0;
}
