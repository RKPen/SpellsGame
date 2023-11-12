#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SPELL_LENGTH 30
#define MAX_SPELLS 78

int counter = 0;

// Function declarations
int spellIsValid(const char *spell1, const char *spell2, const char spellsDone[][MAX_SPELL_LENGTH], int counter) {
    if (counter > 0 && spell1[0] != spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1]) {
        return 3; // Spells don't match
    }

    for (int i = 0; i < counter; i++) {
        if (strcmp(spellsDone[i], spell1) == 0) {
            return 2; // Spell is already used
        }
    }

    return 1; // Valid spell
}


int spellExists(const char *spell, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    for (int i = 0; i < numAvailableSpells; i++) {
        if (strcmp(spell, availableSpells[i]) == 0) {
            return 1; // Spell exists in the list
        }
    }
    return 0; // Spell doesn't exist in the list
}

void printAvailableSpells(const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    printf("Available spells:\n");
    for (int i = 0; i < numAvailableSpells; i++) {
        printf("%s\n", availableSpells[i]);
    }
}

const char *botEasy(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Easy bot chooses a random available spell without checking for matching letters
    int validSpells[MAX_SPELLS];
    int numValidSpells = 0;

    for (int i = 0; i < numAvailableSpells; i++) {
        if (spellIsValid(availableSpells[i], spellsDone[counter - 1], spellsDone, counter) == 1) {
            validSpells[numValidSpells++] = i;
        }
    }

    if (numValidSpells > 0) {
        int randomIndex = rand() % numValidSpells;
        return availableSpells[validSpells[randomIndex]];
    }

    return NULL; // No valid spells for the bot
}

const char *botMedium(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Medium bot tries to find a spell that matches the last letter of the opponent's spell
    char lastLetter = spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1];
    for (int i = 0; i < numAvailableSpells; i++) {
        if (availableSpells[i][0] == lastLetter && spellIsValid(availableSpells[i], spellsDone[counter - 1], spellsDone, counter) == 1) {
            return availableSpells[i];
        }
    }
    // If no matching letter is found, fall back to the Easy strategy
    return botEasy(spellsDone, counter, availableSpells, numAvailableSpells);
}

const char *botHard(const char spellsDone[][MAX_SPELL_LENGTH], int counter, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    // Hard bot's strategy: Predict and counter the player's moves

    // Predict the player's next move
    char predictedSpell[MAX_SPELL_LENGTH];
    if (counter >= 2) {
        // Predict based on the player's last two spells
        char lastLetter1 = spellsDone[counter - 2][strlen(spellsDone[counter - 2]) - 1];
        char lastLetter2 = spellsDone[counter - 1][strlen(spellsDone[counter - 1]) - 1];
        for (int i = 0; i < numAvailableSpells; i++) {
            if (availableSpells[i][0] == lastLetter1 && availableSpells[i][strlen(availableSpells[i]) - 1] == lastLetter2) {
                // Counter the predicted move
                strcpy(predictedSpell, availableSpells[i]);
                break;
            }
        }
    } else {
        // If there are not enough player moves to predict, use the Medium strategy
        return botMedium(spellsDone, counter, availableSpells, numAvailableSpells);
    }

    // Check if the predicted spell is valid and available
    for (int i = 0; i < numAvailableSpells; i++) {
        if (spellIsValid(predictedSpell, spellsDone[counter - 1], spellsDone, counter) == 1 && strcmp(predictedSpell, availableSpells[i]) == 0) {
            // If the predicted move is valid, use it to counter the player
            return predictedSpell;
        }
    }

    // If the predicted spell is not available or valid, fall back to the Medium strategy
    return botMedium(spellsDone, counter, availableSpells, numAvailableSpells);
}
// Function definitions

void playerTurn(const char *player, const char opponent[][MAX_SPELL_LENGTH], const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells, char total_spells_used[][MAX_SPELL_LENGTH], int *counter) {
    char spell[MAX_SPELL_LENGTH];
    printf("%s please enter a spell: ", player);
    scanf("%29s", spell);

    int spellResult = spellIsValid(spell, opponent[*counter - 1], total_spells_used, *counter);
    if (spellResult != 1) {
        printf("Invalid spell, %s wins because of an invalid spell\n", (strcmp(player, "Bot") == 0) ? "You" : "Bot");
        exit(0);
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
        exit(0);
    }

    printf("%s's turn: %s\n", bot, botSpell);
    strcpy(total_spells_used[*counter], botSpell);
    (*counter)++;
}
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

    while (fscanf(spellsFile, "%s", availableSpells[numAvailableSpells]) == 1) {
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

