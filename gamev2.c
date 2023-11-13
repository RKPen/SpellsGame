#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_SPELL_LENGTH 30
#define MAX_SPELLS 78
#define ALPHABET_SIZE 26

int counter = 0;

// Function declarations
int spellExists(const char *spell, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

int spellIsValid(const char *spell1, const char *spell2, const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

void printAvailableSpells(const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

const char *botEasy(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

const char *botMedium(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

unsigned int simpleHash(const char *str);

const char *botHard(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells);

void playerTurn(const char *player, const char opponent[][MAX_SPELL_LENGTH], const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells, char total_spells_used[][MAX_SPELL_LENGTH], int *counter);

void botTurn(const char *bot, const char opponent[][MAX_SPELL_LENGTH], const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells, char total_spells_used[][MAX_SPELL_LENGTH], int *counter, int difficulty);

int main() {
    char p1[MAX_SPELL_LENGTH], p2[] = "Bot"; // Bot as player 2
    char total_spells_used[MAX_SPELLS][MAX_SPELL_LENGTH];
    srand(time(NULL));

    // Read available spells from "spells.txt"
    FILE *spellsFile = fopen("spells.txt", "r");
    if (spellsFile == NULL) {
        perror("Unable to open spells.txt");
        exit(EXIT_FAILURE);
    }

    char availableSpells[MAX_SPELLS][MAX_SPELL_LENGTH];
    int numAvailableSpells = 0;

    // Skip the first line that contains the number of available spells
    fscanf(spellsFile, "%*d");

    while (fscanf(spellsFile, "%29s", availableSpells[numAvailableSpells]) == 1) {
        numAvailableSpells++;
    }

    fclose(spellsFile);

    printf("Please enter your name: ");
    scanf("%29s", p1);
    printf("Welcome %s and %s\n", p1, p2);

    srand(time(NULL));

    const char *startingPlayer = (rand() % 2 == 0) ? p1 : p2;
    printf("%s goes first\n", startingPlayer);

    int difficulty;
    if (strcmp(startingPlayer, p1) == 0) {
        // Player goes first
        printf("%s please enter a spell: ", p1);
        scanf("%29s", total_spells_used[counter]);
        counter++;

        printf("Choose bot difficulty level (1: Easy, 2: Medium, 3: Hard): ");
        scanf("%d", &difficulty);
        botTurn(p2, total_spells_used, availableSpells, numAvailableSpells, total_spells_used, &counter, difficulty);
    } else {
        // Bot goes first
        printf("Choose bot difficulty level (1: Easy, 2: Medium, 3: Hard): ");
        scanf("%d", &difficulty);
        botTurn(p2, total_spells_used, availableSpells, numAvailableSpells, total_spells_used, &counter, difficulty);
    }

    // Continue the game
    while (1) {
        playerTurn(p1, total_spells_used, availableSpells, numAvailableSpells, total_spells_used, &counter);
        botTurn(p2, total_spells_used, availableSpells, numAvailableSpells, total_spells_used, &counter, difficulty);
    }

    return 0;
}

int spellExists(const char *spell, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    for (int i = 0; i < numAvailableSpells; i++) {
        if (strcmp(spell, availableSpells[i]) == 0) {
            return 1; // Spell exists in the list
        }
    }
    return 0; // Spell doesn't exist in the list
}

int spellIsValid(const char *spell1, const char *spell2, const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    if (counter > 0 && spell1[0] != spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1]) {
        return 3; // Spells don't match
    }

    for (int i = 0; i < counter; i++) {
        if (strcmp(spellsDone[i], spell1) == 0) {
            return 2; // Spell is already used
        }
    }

    if (!spellExists(spell1, availableSpells, numAvailableSpells)) {
        return 4; // Spell doesn't exist in the available spells
    }

    return 1; // Valid spell
}

void printAvailableSpells(const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    printf("Available spells:\n");
    for (int i = 1; i < numAvailableSpells; i++) {
        printf("%s\n", availableSpells[i]);
    }
}

const char *botEasy(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Easy bot chooses a totally random available spell
    int randomIndex = rand() % numAvailableSpells;
    const char *chosenSpell = availableSpells[randomIndex];
    char lastLetter = spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1];
    for (int i = 0; i < 3; i++) {
        if (availableSpells[i][0] == lastLetter && spellIsValid(availableSpells[i], spellsDone[counter - 1], spellsDone, counter, availableSpells, numAvailableSpells) == 1) {
            return availableSpells[i];
        }
    }

    // Check if the chosen spell is valid
    if (spellIsValid(chosenSpell, spellsDone[counter - 1], spellsDone, counter, availableSpells, numAvailableSpells) == 1) {
        return chosenSpell;
    }

    return NULL; // Chosen spell is not valid
}

const char *botMedium(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Medium bot tries to find a spell that matches the last letter of the opponent's spell
    char lastLetter = spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1];
    for (int i = 0; i < numAvailableSpells; i++) {
        if (availableSpells[i][0] == lastLetter && spellIsValid(availableSpells[i], spellsDone[counter - 1], spellsDone, counter, availableSpells, numAvailableSpells) == 1) {
            return availableSpells[i];
        }
    }
    // If no matching letter is found, fall back to the Easy strategy
    return botEasy(spellsDone, counter, availableSpells, numAvailableSpells);
}

int letterFrequency[ALPHABET_SIZE] = {0};

unsigned int simpleHash(const char *str) {
    unsigned int hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

const char *botHard(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Reset the frequency of each letter
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        letterFrequency[i] = 0;
    }

    // Count the frequency of each letter in the first position of available spells
    for (int i = 0; i < numAvailableSpells; i++) {
        char firstLetter = availableSpells[i][0];
        if ('a' <= firstLetter && firstLetter <= 'z') {
            letterFrequency[firstLetter - 'a']++;
        } else if ('A' <= firstLetter && firstLetter <= 'Z') {
            letterFrequency[firstLetter - 'A']++;
        }
    }

    // Initialize variables for finding the minimum hash and least frequent letter
    unsigned int minHash = UINT_MAX;
    int leastFrequentLetter = INT_MAX;
    const char *minHashSpell = NULL;

    // Iterate over available spells and find the one with the lowest hash and least frequent starting letter
    for (int i = 0; i < numAvailableSpells; i++) {
        const char *currentSpell = availableSpells[i];

        // Check if the spell is valid and not used before
        if (spellIsValid(currentSpell, spellsDone[counter - 1], spellsDone, counter, availableSpells , numAvailableSpells) == 1) {
            // Calculate a modified hash based on the frequency of the first letter
            unsigned int currentHash = simpleHash(currentSpell) + 10 * letterFrequency[currentSpell[0] - 'a'];

            // Update the minimum hash and least frequent letter if the current spell is better
            if (currentHash < minHash || minHashSpell == NULL) {
                minHash = currentHash;
                minHashSpell = currentSpell;
            }
        }
    }

    // Return the spell with the lowest hash and least frequent starting letter (or NULL if no valid spell found)
    return minHashSpell;
}


void playerTurn(const char *player, const char opponent[][MAX_SPELL_LENGTH], const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells, char total_spells_used[][MAX_SPELL_LENGTH], int *counter) {
    char spell[MAX_SPELL_LENGTH];
    printf("%s please enter a spell: ", player);
    scanf("%29s", spell);

    int spellResult = spellIsValid(spell, opponent[*counter - 1], total_spells_used, *counter, availableSpells, numAvailableSpells);
    if (spellResult != 1) {
        printf("Invalid spell, %s wins because of an invalid spell\n", (strcmp(player, "Bot") == 0) ? "You" : "Bot");
        exit(EXIT_SUCCESS);
    }

    strcpy(total_spells_used[*counter], spell);
    (*counter)++;
}

void botTurn(const char *bot, const char opponent[][MAX_SPELL_LENGTH], const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells, char total_spells_used[][MAX_SPELL_LENGTH], int *counter, int difficulty) {
    const char *botSpell;

    if (difficulty == 1)
        botSpell = botEasy(total_spells_used, *counter, availableSpells, numAvailableSpells);
    else if (difficulty == 2)
        botSpell = botMedium(total_spells_used, *counter, availableSpells, numAvailableSpells);
    else if (difficulty == 3)
        botSpell = botHard(total_spells_used, *counter, availableSpells, numAvailableSpells);

    if (botSpell == NULL) {
        printf("No valid spells for the bot. You win!\n");
        exit(EXIT_SUCCESS);
    }

    printf("%s's turn: %s\n", bot, botSpell);
    strcpy(total_spells_used[*counter], botSpell);
    (*counter)++;
}
