#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SPELL_LENGTH 30
#define MAX_SPELLS 78

int spellExists(const char *spell, const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    for (int i = 0; i < numAvailableSpells; i++) {
        if (strcmp(spell, availableSpells[i]) == 0) {
            return 1; // Spell exists in the list
        }
    }
    return 0; // Spell doesn't exist in the list
}

int spellIsValid(const char *spell1, const char *spell2, const char spellsDone[][MAX_SPELL_LENGTH], int counter) {
    if (counter > 0 && spell1[0] != spell2[strlen(spell2) - 1])
        return 3; // Spells don't match
    
    for (int i = 0; i < counter; i++) {
        if (strcmp(spellsDone[i], spell1) == 0)
            return 2; // Spell is already used
    }

    return 1; // Valid spell
}

void printAvailableSpells(const char availableSpells[][MAX_SPELL_LENGTH], int numAvailableSpells) {
    printf("Available spells:\n");
    for (int i = 0; i < numAvailableSpells; i++) {
        printf("%s\n", availableSpells[i]);
    }
}

int main() {
    char p1[MAX_SPELL_LENGTH], p2[MAX_SPELL_LENGTH];
    char total_spells_used[MAX_SPELLS][MAX_SPELL_LENGTH];
    int counter = 0;
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

    printf("Please enter player 1 name: ");
    scanf("%29s", p1);
    printf("Please enter player 2 name: ");
    scanf("%29s", p2);
    printf("Welcome %s and %s\n", p1, p2);

    srand(time(NULL));

    printf("%s goes first\n", (rand() % 2 == 1) ? p1 : p2);

    printAvailableSpells(availableSpells, numAvailableSpells);

    char spell1[MAX_SPELL_LENGTH], spell2[MAX_SPELL_LENGTH];

    printf("%s please enter a spell: ", p1);
    scanf("%29s", spell1);

    if (!spellExists(spell1, availableSpells, numAvailableSpells)) {
        printf("Invalid spell, %s wins because %s entered a spell that doesn't exist\n", p2, p1);
        return 0;
    }

    strcpy(total_spells_used[counter], spell1);
    counter++;

    while (1) {
        printf("%s please enter a spell: ", p2);
        scanf("%29s", spell2);

        int spellResult = spellIsValid(spell2, spell1, total_spells_used, counter);
        if (spellResult == 0) {
            printf("Invalid spell, %s wins because %s entered a spell that doesn't exist\n", p1, p2);
            break;
        } else if (spellResult == 2) {
            printf("Invalid spell, %s wins because %s's spell is already used\n", p1, p2);
            break;
        } else if (spellResult == 3) {
            printf("Invalid spell, %s wins because %s's spell does not match\n", p1, p2);
            break;
        }

        strcpy(total_spells_used[counter], spell2);
        counter++;

        printf("%s please enter a spell: ", p1);
        scanf("%29s", spell1);

        spellResult = spellIsValid(spell1, spell2, total_spells_used, counter);
        if (spellResult == 0) {
            printf("Invalid spell, %s wins because %s entered a spell that doesn't exist\n", p2, p1);
            break;
        } else if (spellResult == 2) {
            printf("Invalid spell, %s wins because %s's spell is already used\n", p2, p1);
            break;
        } else if (spellResult == 3) {
            printf("Invalid spell, %s wins because %s's spell does not match\n", p2, p1);
            break;
        }

        strcpy(total_spells_used[counter], spell1);
        counter++;
    }

    return 0;
}
