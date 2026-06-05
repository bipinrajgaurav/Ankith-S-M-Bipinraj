#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_ATTEMPTS 6
#define WORD_LENGTH 5

// ANSI Color Codes for Terminal Output
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET  "\x1b[0m"

// Function to convert a string to uppercase
void string_to_uppercase(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Function to process a single guess and print it in color
void print_colored_guess(const char *secret, const char *guess) {
    bool secret_matched[WORD_LENGTH] = {false};
    int status[WORD_LENGTH] = {0}; // 2 = Green, 1 = Yellow, 0 = Gray

    // Pass 1: Exact matches (Green)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            status[i] = 2;
            secret_matched[i] = true;
        }
    }

    // Pass 2: Misplaced matches (Yellow)
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (status[i] == 2) continue;

        for (int j = 0; j < WORD_LENGTH; j++) {
            if (!secret_matched[j] && guess[i] == secret[j]) {
                status[i] = 1;
                secret_matched[j] = true;
                break;
            }
        }
    }

    // Print the row cleanly
    printf("\t[ ");
    for (int i = 0; i < WORD_LENGTH; i++) {
        if (status[i] == 2) {
            printf("%s%c%s ", COLOR_GREEN, guess[i], COLOR_RESET);
        } else if (status[i] == 1) {
            printf("%s%c%s ", COLOR_YELLOW, guess[i], COLOR_RESET);
        } else {
            printf("%c ", guess[i]);
        }
    }
    printf("]\n");
}

int main() {
    char play_again;

    do {
        char secret_word[WORD_LENGTH + 1] = "TRACK"; // Set your secret word here!
        char current_guess[100]; 
        int attempt = 0;
        bool won = false;

        // 2D Array to store previous guesses (History Stack)
        char guess_history[MAX_ATTEMPTS][WORD_LENGTH + 1];

        while (attempt < MAX_ATTEMPTS) {
            // 1. Refresh the clean header grid
            system("cls");
            printf("=========================================\n");
            printf("       WELCOME TO C-LANGUAGE WORDLE      \n");
            printf("         by Bipinraj Gaurav HB & Ankith SM        \n")
            printf("=========================================\n");
            printf("Rules: Guess the %d-letter word in %d tries.\n\n", WORD_LENGTH, MAX_ATTEMPTS);

            // 2. Reprint ALL previous guesses in order (Traditional Wordle stack!)
            for (int i = 0; i < attempt; i++) {
                print_colored_guess(secret_word, guess_history[i]);
            }
            printf("\n");

            // 3. Prompt user for their current turn input
            printf("Attempt %d/%d. Enter a 5-letter word: ", attempt + 1, MAX_ATTEMPTS);
            
            if (scanf("%99s", current_guess) != 1) break;
            string_to_uppercase(current_guess);

            if (strlen(current_guess) != WORD_LENGTH) {
                printf("\nError! Input must be exactly %d letters. Press Enter to try again...", WORD_LENGTH);
                fflush(stdin); // Flush inputs
                getchar();     // Wait for user to read the error
                continue;
            }

            // Save the valid guess into our history stack
            strcpy(guess_history[attempt], current_guess);

            // Check if they won this turn
            if (strcmp(current_guess, secret_word) == 0) {
                won = true;
                attempt++; // Count the winning attempt
                break;
            }

            attempt++;
        }

        // Final screen render to show the full complete grid at the end of the game
        system("cls");
        printf("=========================================\n");
        printf("       WELCOME TO C-LANGUAGE WORDLE      \n");
        printf("         by Bipinraj Gaurav HB & Ankith SM        \n")
        printf("=========================================\n\n");
        
        for (int i = 0; i < attempt; i++) {
            print_colored_guess(secret_word, guess_history[i]);
        }
        printf("\n");

        // End Game Messages
        if (won) {
            printf("Congratulations! You guessed the word in %d attempts!\n\n", attempt);
        } else {
            printf("Game Over! Out of attempts. The secret word was: %s\n\n", secret_word);
        }

        // Ask for a reset
        printf("Would you like to play again? (Y/N): ");
        scanf(" %c", &play_again); 
        play_again = toupper((unsigned char)play_again);

    } while (play_again == 'Y');

    system("cls");
    printf("Thanks for playing! Goodbye!\n");
    return 0;
}