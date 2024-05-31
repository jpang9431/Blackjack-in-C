#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define maxBlackJackHand 22
#define RAND(minimum_number, max_number) \
    rand() % (max_number + 1 - minimum_number) + minimum_number
#define SIZEOF(x) (sizeof(x) / sizeof((x)[0]))
#define blackJackHandMapSize 10
#define maxDigits 1

int totalPointsGained = 0;

const char names[13] = {'A', '2', '3', '4', '5', '6', '7',
                        '8', '9', 'T', 'J', 'Q', 'K'};
const char suits[4][9] = {"Hearts", "Diamonds", "Clubs", "Spades"};

const char* seperator = "----------------------------\n";

struct Card {
    char name;
    int suit;
};

// Function prototypes
int getIntegerInput();
int getValidNum(char**, int);
int rand_lim(int);
struct Card generateCard();
void printCard(struct Card*);
void printHand(struct Card[], int);
int calcBlackJackHandValue(int[]);
int getCardIndex(char);
int playBlackJack();
void printList(int[]);
void testing();
void clearConsole();

int getIntegerInput() {
    char a[maxDigits];
    int ret = 0;
    int num;
    for (int i = 0; i < maxDigits; i++) {
        a[i] = getchar();
    }
    getchar();
    ret = sscanf(a, "%d", &num);
    // printf("%d\n", ret);
    if (ret != 1) {
        return -1;
    } else {
        return num;
    }
}

int getValidNum(char** choices, int length) {
    int value;
    printf("Choices:\n");
    for (int i = 0; i < length; i++) {
        printf("%d.%s\n", i + 1, choices[i]);
    }
    printf("Please enter in the number of the choice you want to do: ");
    value = getIntegerInput();
    while (value == -1 || value < 1 || value > length) {
        if (value == -1) {
            printf(
                "Error, please enter in an INTEGER number of the choice you "
                "want to do: ");
        } else if (value < 1 || value > length) {
            printf(
                "Error, please enter in a number between %d and %d inclusive: ",
                1, length);
        }
        value = getIntegerInput();
    }
    return value;
}

int rand_lim(int limit) {
    int divisor = RAND_MAX / (limit + 1);
    int retval;
    do {
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;
}

struct Card generateCard() {
    struct Card returnCard = {.name = names[rand_lim(SIZEOF(names) - 1)],
                              .suit = rand_lim(SIZEOF(suits) - 1)};
    return returnCard;
}

void printCard(struct Card* card) {
    printf("%c of %s\n", card->name, suits[card->suit]);
}

void printHand(struct Card hand[maxBlackJackHand], int handSize) {
    for (int i = 0; i < handSize; i++) {
        printCard(&hand[i]);
    }
}

int calcBlackJackHandValue(int handMap[]) {
    int handValue = 0;
    for (int i = 1; i < blackJackHandMapSize; i++) {
        handValue = handValue + handMap[i] * (i + 1);
    }
    if (handValue < 11 && handMap[0] < 2) {
        handValue = handValue + handMap[0] * 11;
    } else {
        handValue = handValue + handMap[0];
    }
    return handValue;
}

int getCardIndex(char cardName) {
    int num = (int)cardName - (int)'0';
    if (num > 0 && num < 10) {
        return num - 1;
    } else if (cardName == 'A') {
        return 0;
    } else {
        return blackJackHandMapSize - 1;
    }
}

void printBlackJack(struct Card dealerHand[maxBlackJackHand],
                    struct Card playerHand[maxBlackJackHand],
                    int dealerHandMap[blackJackHandMapSize],
                    int playerHandMap[blackJackHandMapSize], int dealerHandSize,
                    int playerHandSize) {
    clearConsole();
    printf("Dealer Hand (%d):\n", calcBlackJackHandValue(dealerHandMap));
    printHand(dealerHand, dealerHandSize);
    printf("%s", seperator);
    printf("Your hand (%d):\n", calcBlackJackHandValue(playerHandMap));
    printHand(playerHand, playerHandSize);
    printf("%s", seperator);
}

int playBlackJack() {
    struct Card dealerHand[maxBlackJackHand];
    struct Card playerHand[maxBlackJackHand];
    int dealerHandMap[blackJackHandMapSize] = {0};
    int playerHandMap[blackJackHandMapSize] = {0};
    int dealerHandSize = 2;
    int playerHandSize = 2;
    for (int i = 0; i < 2; i++) {
        dealerHand[i] = generateCard();
        playerHand[i] = generateCard();
        dealerHandMap[getCardIndex(dealerHand[i].name)]++;
        playerHandMap[getCardIndex(playerHand[i].name)]++;
    }
    int countinue = 1;  // seperator
    char* choices[] = {"Hit", "Stand"};
    while (countinue) {
        int choice = -1;
        printBlackJack(dealerHand, playerHand, dealerHandMap, playerHandMap,
                       dealerHandSize, playerHandSize);
        printf("Choices:\n");
        for (int i = 0; i < 2; i++) {
            printf("%d.%s\n", i + 1, choices[i]);
        }
        printf("Please enter in the number of the choice you want to do: ");
        choice = getIntegerInput();
        while (choice == -1 || choice < 1 || choice > 2) {
            printBlackJack(dealerHand, playerHand, dealerHandMap, playerHandMap,
                           dealerHandSize, playerHandSize);
            printf("Choices:\n");
            for (int i = 0; i < 2; i++) {
                printf("%d.%s\n", i + 1, choices[i]);
            }
            if (choice == -1) {
                printf(
                    "Please enter in the INTEGER number of the choice you "
                    "want to do: ");
            } else if (choice < 1 || choice > 2) {
                printf(
                    "Please enter in the number of the choice between %d "
                    "and %d inclusive: ",
                    1, 2);
            } else {
                printf("ERROR Unexpected condition");
            }
            choice = getIntegerInput();
        }
        if (choice == 1) {
            playerHand[playerHandSize] = generateCard();
            playerHandMap[getCardIndex(playerHand[playerHandSize].name)]++;
            playerHandSize++;
            if (calcBlackJackHandValue(playerHandMap) > 21) {
                countinue = 0;
                printBlackJack(dealerHand, playerHand, dealerHandMap,
                               playerHandMap, dealerHandSize, playerHandSize);
                printf("You busted (went over 21), you lose.\n");
                printf("%s", seperator);
                return 0;
            }
        } else if (choice == 2) {
            clearConsole();
            countinue = 0;
        }
    }
    int dealerHandValue = calcBlackJackHandValue(dealerHandMap);
    int playerHandValue = calcBlackJackHandValue(playerHandMap);
    while (dealerHandValue <= playerHandValue) {
        dealerHand[dealerHandSize] = generateCard();
        dealerHandMap[getCardIndex(dealerHand[dealerHandSize].name)]++;
        dealerHandSize++;
        dealerHandValue = calcBlackJackHandValue(dealerHandMap);
        if (dealerHandValue > 21) {
            countinue = 0;
            printBlackJack(dealerHand, playerHand, dealerHandMap, playerHandMap,
                           dealerHandSize, playerHandSize);
            printf("The dealer busted (went over 21), you win!\n");
            printf("%s", seperator);
            return 1;
        }
    }
    if (dealerHandValue < 21 && dealerHandValue > playerHandValue) {
        printBlackJack(dealerHand, playerHand, dealerHandMap, playerHandMap,
                       dealerHandSize, playerHandSize);
        printf(
            "The delear hand value is %d which is greater than the player hand "
            "value of %d, you lose.",
            dealerHandValue, playerHandValue);
        return 0;
    } else if (playerHandValue > dealerHandValue && playerHandValue < 21) {
        printBlackJack(dealerHand, playerHand, dealerHandMap, playerHandMap,
                       dealerHandSize, playerHandSize);
        printf(
            "The player hand value is %d which is greater than delear hand "
            "value of %d, you lose.",
            playerHandValue, dealerHandValue);
        return 1;
    }
    printf("Unexpected error has occured");
    return -1;
}

void printList(int nums[]) {
    for (int i = 0; i < blackJackHandMapSize; i++) {
        printf("%d,", nums[i]);
    }
    printf("\n");
}

void testing() {
    /*for (int i = 0; i < 10; i++) {
        struct Card card = generateCard();
        printCard(&card);
        printf("%d\n", getCardIndex(card.name));
    }*/
    // playBlackJack(0);
    /*int testingHand[blackJackHandMapSize] = {0};
    printList(testingHand);
    printf("%d\n",calcBlackJackHandValue(testingHand));
    testingHand[0] = 1;
    printList(testingHand);
    printf("%d\n",calcBlackJackHandValue(testingHand));
    testingHand[0] = 2;
    printList(testingHand);
    printf("%d\n",calcBlackJackHandValue(testingHand));
    testingHand[blackJackHandMapSize-1] = 1;
    printList(testingHand);
    printf("%d\n",calcBlackJackHandValue(testingHand));*/
}

void clearConsole() { printf("\e[1;1H\e[2J"); }

int main() {
    srand(time(NULL));
    printf(
        "---------------------------------------------------------------------"
        "\n");
    printf(
        ":__        __   _                                                   "
        ":\n");
    printf(
        ":\\ \\      / /__| | ___ ___  _ __ ___   ___                          "
        ":\n");
    printf(
        ": \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\                    "
        "     :\n");
    printf(
        ":  \\ V  V /  __/ | (_| (_) | | | | | |  __/_                        "
        ":\n");
    printf(
        ":   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___( )      _            "
        "    :\n");
    printf(
        ": _ __  _ __ ___ _ __   __ _ _ __ ___  | ||/___   | | ___  ___  ___ "
        ":\n");
    printf(
        ":| '_ \\| '__/ _ \\ '_ \\ / _` | '__/ _ \\ | __/ _ \\  | |/ _ \\/ "
        "__|/ _ \\:\n");
    printf(
        ":| |_) | | |  __/ |_) | (_| | | |  __/ | || (_) | | | (_) \\__ \\  "
        "__/:\n");
    printf(
        ":| .__/|_|  \\___| .__/ \\__,_|_| _\\___|_ \\__\\___/  "
        "|_|\\___/|___/\\___|:\n");
    printf(
        ":|_|____   _____|_|__ _   _| |_| |__ (_)_ __   __ _                 "
        ":\n");
    printf(
        ": / _ \\ \\ / / _ \\ '__| | | | __| '_ \\| | '_ \\ / _` |             "
        "   :\n");
    printf(
        ":|  __/\\ V /  __/ |  | |_| | |_| | | | | | | | (_| |                "
        ":\n");
    printf(
        ": \\___| \\_/ \\___|_|   \\__, |\\__|_| |_|_|_| |_|\\__, |            "
        "    :\n");
    printf(
        ":                     |___/                   |___/                 "
        ":\n");
    printf(
        "---------------------------------------------------------------------"
        "\n");

    // testing();
    char* choices[11] = {"Blackjack", "Exit"};
    int continueToPlay = 1;
    int choice;
    while (continueToPlay) {
        choice = getValidNum(choices, 2);
        if (choice == 1) {
            clearConsole();
            playBlackJack();
        } else if (choice == 2) {
            clearConsole();
            continueToPlay = 0;
            printf("Thank you for playing!");
        }
    }
    return (0);
}