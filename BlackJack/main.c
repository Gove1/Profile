#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define DECK_SIZE 52

void shuffle_deck(int *deck) {
    for (int i = 0; i < DECK_SIZE; i++) deck[i] = i;
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

int get_card_value(int card_num) {
    int rank = (card_num % 13) + 1;
    if (rank > 10) return 10;
    return rank;
}

void print_card(int card_num) {
    char *suits[] = {"S", "D", "H", "C"};
    char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    printf("[%s %s] ", suits[card_num / 13], ranks[card_num % 13]);
    fflush(stdout);
}

int calculate_score(int *hand, int card_count) {
    int score = 0;
    int ace_count = 0;
    for (int i = 0; i < card_count; i++) {
        int value = get_card_value(hand[i]);
        score += value;
        if (value == 1) ace_count++;
    }
    while (ace_count > 0 && score + 10 <= 21) {
        score += 10;
        ace_count--;
    }
    return score;
}

int main() {
    srand((unsigned int)time(NULL));

    int deck[DECK_SIZE];
    shuffle_deck(deck);

    int deck_index = 0;
    int player_hand[10], dealer_hand[10];
    int player_count = 0, dealer_count = 0;

    printf("Blackjack Game Start\n\n");
    sleep(1);

    printf("Player Card 1: ");
    player_hand[player_count++] = deck[deck_index++];
    print_card(player_hand[player_count-1]);
    printf("\n");
    sleep(1);

    printf("Dealer Card 1: ");
    dealer_hand[dealer_count++] = deck[deck_index++];
    print_card(dealer_hand[dealer_count-1]);
    printf("\n");
    sleep(1);

    printf("Player Card 2: ");
    player_hand[player_count++] = deck[deck_index++];
    print_card(player_hand[player_count-1]);
    printf("\n");
    sleep(1);

    printf("Dealer Card 2: [ Hidden Card ]\n");
    dealer_hand[dealer_count++] = deck[deck_index++];
    sleep(1);

    printf("\nCard Distribution Complete\n\n");

    bool player_bust = false;

    while (true) {
        printf("--- Player Turn ---\n");
        printf("Your Cards: ");
        for (int i = 0; i < player_count; i++) print_card(player_hand[i]);
        
        int player_score = calculate_score(player_hand, player_count);
        printf("\nYour Score: %d\n", player_score);

        if (player_score > 21) {
            printf("Bust! Over 21 Points.\n");
            player_bust = true;
            break;
        }

        printf("1: Hit, 2: Stand -> ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            break;
        }

        if (choice == 1) {
            printf("\nPlayer Hit: ");
            sleep(1);
            player_hand[player_count++] = deck[deck_index++];
            print_card(player_hand[player_count-1]);
            printf("\n\n");
        } else if (choice == 2) {
            printf("\nPlayer Stand.\n\n");
            break;
        } else {
            printf("Invalid Input.\n\n");
        }
    }

    if (!player_bust) {
        printf("--- Dealer Turn ---\n");
        sleep(1);
        
        while (true) {
            printf("Dealer Cards: ");
            for (int i = 0; i < dealer_count; i++) print_card(dealer_hand[i]);
            
            int dealer_score = calculate_score(dealer_hand, dealer_count);
            printf("\nDealer Score: %d\n\n", dealer_score);
            sleep(1);
            
            if (dealer_score >= 17) {
                printf("Dealer Stand.\n");
                break;
            }
            
            printf("Dealer Hit...\n");
            dealer_hand[dealer_count++] = deck[deck_index++];
            sleep(1);
        }
    }

    int final_dealer = calculate_score(dealer_hand, dealer_count);
    int final_player = calculate_score(player_hand, player_count);

    printf("\n=== Final Result ===\n");
    printf("Player Score: %d\n", final_player);
    printf("Dealer Score: %d\n", final_dealer);
    printf("--------------------\n");

    if (final_player > 21) {
        printf("Player Bust. Dealer Wins.\n");
    } else if (final_dealer > 21) {
        printf("Dealer Bust. Player Wins.\n");
    } else if (final_player > final_dealer) {
        printf("Player Wins.\n");
    } else if (final_player < final_dealer) {
        printf("Dealer Wins.\n");
    } else {
        printf("Push.\n");
    }

    return 0;
}