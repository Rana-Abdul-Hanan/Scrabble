#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

char letters[27]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
int points[27]= {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1,1, 4, 4, 8, 4, 10, 0};
char last_word[8];  
int last_row_indices[7], last_col_indices[7]; 
int last_word_length = 0;

typedef struct {
    int num;
    char letter;
} num_state;

typedef struct {
    char letter;
    int points;
} tile;

char tos();
void initializeBag(tile bag[], int *bagsize);
char newChar(tile bag[100], int *bagsize);
void shuffleBag(tile bag[100], int bagsize);
void displayGrid(char arr[15][15], tile bag[100]);
void placeTile(char grid[15][15], char letter, int row_index, int col_index);
void InitializeGrid(char arr[15][15]);
int calculateScore(tile bag[100], char letter, int row, int col);
int getTilePoints(tile bag[100], char letter);
int hasTiles(char rack[7]);
int isValidWord(const char *word);
int main() {
    tile bag[100];
    int turn, n, bagsize = 0;
    char rack_p1[7], rack_p2[7];
    char grid[15][15], letter[7];
    int column, col_index, row_index;
    char row, c;
    int score_p1, score_p2;
    int row_indices[7], col_indices[7];
    int total_p1 = 0, total_p2 = 0;

    printf("First we have to toss who is going to move first\nPress enter to proceed.....\n");
    getchar();
    srand(time(NULL));

    char p1 = tos();
    printf("For toss player 1 has drawn tile %c from the bag\n", p1);
    char p2 = tos();
    printf("For toss player 2 has drawn tile %c from the bag\n", p2);

    while (p1 == p2) {
        printf("It's a tie! Drawing again...\n");
        p1 = tos();
        p2 = tos();
        printf("Player 1 drew %c, Player 2 drew %c\n", p1, p2);
    }

    int diff_1 = p1 - 'A';
    int diff_2 = p2 - 'A';

    if (diff_1 > diff_2) {
        printf("It's player 2 turn.\n");
        turn = 2;
    } else if (diff_2 > diff_1) {
        printf("It's player 1 turn.\n");
        turn = 1;
    }

    initializeBag(bag, &bagsize);
    shuffleBag(bag, bagsize);

    for (int i = 0; i < 7; i++) {
        rack_p1[i] = newChar(bag, &bagsize);
        
    }

    for (int i = 0; i < 7; i++) {
        rack_p2[i] = newChar(bag, &bagsize);
        
    }

    InitializeGrid(grid);

    while (bagsize > 0 || hasTiles(rack_p1) || hasTiles(rack_p2)) {
        char objection;
        if (last_word_length > 0) {
            char objection;
            printf("Do you want to object to the last word \"%s\"? (y/n): ", last_word);
            scanf(" %c", &objection);
        
            if (objection == 'y' || objection == 'Y') {
                if (!isValidWord(last_word)) {
                    printf("Word \"%s\" is invalid! Removing from grid.\n", last_word);
                    for (int i = 0; i < last_word_length; i++) {
                        grid[last_row_indices[i]][last_col_indices[i]] = ' ';
                    }

                } else {
                    printf("Word \"%s\" is valid.\n", last_word);
                }
            }
        }
        

        score_p1 = 0;
        if (turn == 1) {
            displayGrid(grid, bag);
            printf("Player 1, here is your rack:\n");

            printf("|\n");
            printf("-----------------------------\n");
            for (int i = 0; i < 7; i++) {
                printf("| %c ", rack_p1[i]);
            }
            printf("|\n");
            printf("-----------------------------\n");
            do {
                printf("How many letters from the rack do you want to place (1-7): ");
                scanf("%d", &n);
                if (n > 7 || n < 0) {
                    printf("\nTry again. Invalid number\n");
                }
            } while (n > 7 || n < 0);

            for (int i = 0; i < n; i++) {
                int valid = 0;
                while (!valid) {
                    printf("Enter letter %d you want to place from your rack: ", i + 1);
                    scanf(" %c", &letter[i]);

                    int found_in_rack = 0;
                    for (int j = 0; j < 7; j++) {
                        if (letter[i] == rack_p1[j]) {
                            found_in_rack = 1;

                            if (letter[i] == ' ') {
                                printf("Which letter do you want to place here (A-Z): ");
                                scanf(" %c", &c);
                                letter[i] = c;
                            }
                            valid = 1;
                            break;
                        }
                    }
                    if (!valid) {
                        printf("Invalid choice! The letter is not in your rack.\n");
                    }
                }

                do {
                    printf("In which column (1 to 15) do you want to place your letter %d: ", i + 1);
                    scanf("%d", &column);
                    col_index = column - 1;

                    printf("In which row (A to O) do you want to place your letter %d: ", i + 1);
                    scanf(" %c", &row);
                    row_index = row - 'A';

                    if (row_index < 0 || row_index > 14 || col_index < 0 || col_index > 14) {
                        printf("Invalid cell coordinate. Try again.\n");
                    } else if (grid[row_index][col_index] != ' ') {
                        printf("Cell already occupied. Try again.\n");
                    }
                } while (row_index < 0 || row_index > 14 || col_index < 0 || col_index > 14 || grid[row_index][col_index] != ' ');

                row_indices[i] = row_index;
                col_indices[i] = col_index;
            }

            for (int i = 0; i < n; i++) {
                score_p1 += calculateScore(bag, letter[i], row_indices[i], col_indices[i]);
            }

            int same_row = 1, same_col = 1;
            for (int i = 1; i < n; i++) {
                if (row_indices[i] != row_indices[0]) 
                    same_row = 0;
                if (col_indices[i] != col_indices[0]) 
                    same_col = 0;
            }

            if (!same_row && !same_col) {
                printf("Error: Letters must be placed in the same row or the same column!\n");
                return 1;
            }

            if (same_row) {
                for (int i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) {
                        if (col_indices[i] > col_indices[j]) {
                            int temp = col_indices[i];
                            col_indices[i] = col_indices[j];
                            col_indices[j] = temp;

                            char t = letter[i];
                            letter[i] = letter[j];
                            letter[j] = t;
                        }
                    }
                }
                for (int i = 0; i < n - 1; i++) {
                    if (col_indices[i + 1] != col_indices[i] + 1) {
                        printf("Error: Letters must be in continuous columns.\n");
                        return 1;
                    }
                }
            } else if (same_col) {
                for (int i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) {
                        if (row_indices[i] > row_indices[j]) {
                            int temp = row_indices[i];
                            row_indices[i] = row_indices[j];
                            row_indices[j] = temp;

                            char t = letter[i];
                            letter[i] = letter[j];
                            letter[j] = t;
                        }
                    }
                }
                for (int i = 0; i < n - 1; i++) {
                    if (row_indices[i + 1] != row_indices[i] + 1) {
                        printf("Error: Letters must be in continuous rows.\n");
                        return 1;
                    }
                }
            }

            for (int i = 0; i < n; i++) {
                placeTile(grid, letter[i], row_indices[i], col_indices[i]);
                displayGrid(grid, bag);
                strcpy(last_word, "");  
                for (int i = 0; i < n; i++) {
                    last_word[i] = letter[i];
                    last_row_indices[i] = row_indices[i];
                    last_col_indices[i] = col_indices[i];
                }
                last_word[n] = '\0';
                last_word_length = n;

                for (int k = 0; k < 7; k++) {
                    if (rack_p1[k] == letter[i]) {
                        if (bagsize > 0) {
                            rack_p1[k] = newChar(bag, &bagsize);
                        } else {
                            rack_p1[k] = ' ';
                        }

                        break;
                    }
                }
            }

            printf("\nPlayer 1 scored: %d points in this turn.\n", score_p1);
            total_p1 += score_p1;
            turn = (turn == 1) ? 2 : 1;
        } else {
            char objection;
            if (last_word_length > 0) {
                char objection;
                printf("Do you want to object to the last word \"%s\"? (y/n): ", last_word);
                scanf(" %c", &objection);
            
                if (objection == 'y' || objection == 'Y') {
                    if (!isValidWord(last_word)) {
                        printf("Word \"%s\" is invalid! Removing from grid.\n", last_word);
                        for (int i = 0; i < last_word_length; i++) {
                            grid[last_row_indices[i]][last_col_indices[i]] = ' ';
                        }
                        
                    } else {
                        printf("Word \"%s\" is valid.\n", last_word);
                    }
                }
            }
            

            score_p2 = 0;
            

            displayGrid(grid, bag);
            printf("Player 2, here is your rack:\n");

            printf("|\n");
            printf("-----------------------------\n");
            for (int i = 0; i < 7; i++) {
                printf("| %c ", rack_p2[i]);
            }
            printf("|\n");
            printf("-----------------------------\n");
            do {
                printf("How many letters from the rack do you want to place (1-7): ");
                scanf("%d", &n);
                if (n > 7 || n < 0) {
                    printf("\nTry again. Invalid number\n");
                }
            } while (n > 7 || n < 0);

            for (int i = 0; i < n; i++) {
                int valid = 0;
                while (!valid) {
                    printf("Enter letter %d you want to place from your rack: ", i + 1);
                    scanf(" %c", &letter[i]);

                    int found_in_rack = 0;
                    for (int j = 0; j < 7; j++) {
                        if (letter[i] == rack_p2[j]) {
                            found_in_rack = 1;

                            if (letter[i] == ' ') {
                                printf("Which letter do you want to place here (A-Z): ");
                                scanf(" %c", &c);
                                letter[i] = c;
                            }
                            valid = 1;
                            break;
                        }
                    }
                    if (!valid) {
                        printf("Invalid choice! The letter is not in your rack.\n");
                    }
                }

                do {
                    printf("In which column (1 to 15) do you want to place your letter %d: ", i + 1);
                    scanf("%d", &column);
                    col_index = column - 1;

                    printf("In which row (A to O) do you want to place your letter %d: ", i + 1);
                    scanf(" %c", &row);
                    row_index = row - 'A';

                    if (row_index < 0 || row_index > 14 || col_index < 0 || col_index > 14) {
                        printf("Invalid cell coordinate. Try again.\n");
                    } else if (grid[row_index][col_index] != ' ') {
                        printf("Cell already occupied. Try again.\n");
                    }
                } while (row_index < 0 || row_index > 14 || col_index < 0 || col_index > 14 || grid[row_index][col_index] != ' ');

                row_indices[i] = row_index;
                col_indices[i] = col_index;
            }

            for (int i = 0; i < n; i++) {
                score_p2 += calculateScore(bag, letter[i], row_indices[i], col_indices[i]);
            }

            int same_row = 1, same_col = 1;
            for (int i = 1; i < n; i++) {
                if (row_indices[i] != row_indices[0]) same_row = 0;
                if (col_indices[i] != col_indices[0]) same_col = 0;
            }

            if (!same_row && !same_col) {
                printf("Error: Letters must be placed in the same row or the same column!\n");
                return 1;
            }

            if (same_row) {
                for (int i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) {
                        if (col_indices[i] > col_indices[j]) {
                            int temp = col_indices[i];
                            col_indices[i] = col_indices[j];
                            col_indices[j] = temp;

                            char t = letter[i];
                            letter[i] = letter[j];
                            letter[j] = t;
                        }
                    }
                }
                for (int i = 0; i < n - 1; i++) {
                    if (col_indices[i + 1] != col_indices[i] + 1) {
                        printf("Error: Letters must be in continuous columns.\n");
                        return 1;
                    }
                }
            } else if (same_col) {
                for (int i = 0; i < n - 1; i++) {
                    for (int j = i + 1; j < n; j++) {
                        if (row_indices[i] > row_indices[j]) {
                            int temp = row_indices[i];
                            row_indices[i] = row_indices[j];
                            row_indices[j] = temp;

                            char t = letter[i];
                            letter[i] = letter[j];
                            letter[j] = t;
                        }
                    }
                }
                for (int i = 0; i < n - 1; i++) {
                    if (row_indices[i + 1] != row_indices[i] + 1) {
                        printf("Error: Letters must be in continuous rows.\n");
                        return 1;
                    }
                }
            }

            for (int i = 0; i < n; i++) {
                placeTile(grid, letter[i], row_indices[i], col_indices[i]);
                displayGrid(grid, bag);
                strcpy(last_word, "");  // reset
                for (int i = 0; i < n; i++) {
                    last_word[i] = letter[i];
                    last_row_indices[i] = row_indices[i];
                    last_col_indices[i] = col_indices[i];
                }
                last_word[n] = '\0';
                last_word_length = n;

                for (int k = 0; k < 7; k++) {
                    if (rack_p2[k] == letter[i]) {
                        if (bagsize > 0) {
                            rack_p2[k] = newChar(bag, &bagsize);
                        } else {
                            rack_p2[k] = ' ';
                        }

                        break;
                    }
                }
            }

            printf("\nPlayer 2 scored: %d points in this turn.\n", score_p2);
            total_p2 += score_p2;
            turn = (turn == 1) ? 2 : 1;
        }
    }

    printf("Game Over!\n");
    printf("Player 1's total score: %d\n", total_p1);
    printf("Player 2's total score: %d\n", total_p2);

    if (total_p1 > total_p2) {
        printf("Player 1 wins!\n");
    } else if (total_p2 > total_p1) {
        printf("Player 2 wins!\n");
    } else {
        printf("It's a tie!\n");
    }

    return 0;
}

char tos() {
    num_state arr[26];
    for (int i = 0; i < 26; i++) {
        arr[i].letter = letters[i];
        arr[i].num = i + 1;
    }
    int x = rand() % 26;
    return arr[x].letter;
}

void initializeBag(tile bag[], int *bagsize) {
    int index = 0;

    int repeat[27] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1, 2};
    for (int i = 0; i < 27; i++) {
        for (int j = 0; j < repeat[i]; j++) {
            bag[index].letter = letters[i];
            bag[index].points = points[i];
            index++;
        }
    }
    *bagsize = index;
}

char newChar(tile bag[100], int *bagsize) {
    int a = rand() % *bagsize;
    char new_alpha = bag[a].letter;
    tile temp = bag[a];
    bag[a] = bag[*bagsize - 1];
    bag[*bagsize - 1] = temp;
    (*bagsize)--;
    return new_alpha;
}

void shuffleBag(tile bag[100], int bagsize) {
    for (int i = (bagsize - 1); i > 0; i--) {
        int j = rand() % (i + 1);
        tile k = bag[i];
        bag[i] = bag[j];
        bag[j] = k;
    }
}

void InitializeGrid(char arr[15][15]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            arr[i][j] = ' ';
        }
    }
}

void displayGrid(char grid[15][15], tile bag[100]) {
    printf("\n\n   ");
    printf("\n   ");
    printf("|");
    for (int i = 0; i < 15; i++) {
        printf(" %2d |", i + 1);
    }
    printf("\n   ");
    for (int i = 0; i < 15; i++) {
        printf("-----");
    }
    printf("-");

    for (int i = 0; i < 15; i++) {
        printf("\n%2c |", i + 65);
        for (int j = 0; j < 15; j++) {
            if ((i == 0 && j == 0) || (i == 0 && j == 7) || (i == 0 && j == 14) || (i == 7 && j == 0) 
                || (i == 7 && j == 14) || (i == 14 && j == 0) || (i == 14 && j == 7) || (i == 14 && j == 14)
                || (i == 1 && j == 5) || (i == 1 && j == 9) || (i == 5 && j == 1) || (i == 5 && j == 5) 
                || (i == 5 && j == 9) || (i == 5 && j == 13) || (i == 9 && j == 1) || (i == 9 && j == 5) 
                || (i == 9 && j == 9) || (i == 9 && j == 13) || (i == 13 && j == 5) || (i == 13 && j == 9)) {
                printf("\x1b[1;37;44m  %c \x1b[0m|", grid[i][j]);
            } else if ((i == 1 && j == 1) || (i == 2 && j == 2) || (i == 3 && j == 3) || (i == 4 && j == 4)
                || (i == 1 && j == 13) || (i == 2 && j == 12) || (i == 3 && j == 11) || (i == 4 && j == 10)
                || (i == 13 && j == 1) || (i == 12 && j == 2) || (i == 11 && j == 3) || (i == 10 && j == 4)
                || (i == 13 && j == 13) || (i == 12 && j == 12) || (i == 11 && j == 11) || (i == 10 && j == 10)
                || (i == 7 && j == 7) || (i == 0 && j == 3) || (i == 0 && j == 11) || (i == 2 && j == 6) 
                || (i == 2 && j == 8) || (i == 3 && j == 0) || (i == 3 && j == 7) || (i == 3 && j == 14) 
                || (i == 6 && j == 2) || (i == 6 && j == 6) || (i == 6 && j == 8) || (i == 6 && j == 12) 
                || (i == 7 && j == 3) || (i == 7 && j == 11) || (i == 8 && j == 2) || (i == 8 && j == 6) 
                || (i == 8 && j == 8) || (i == 8 && j == 12) || (i == 11 && j == 0) || (i == 11 && j == 7) 
                || (i == 11 && j == 14) || (i == 12 && j == 6) || (i == 12 && j == 8) || (i == 14 && j == 3) 
                || (i == 14 && j == 11)) {
                printf("\x1b[1;30;46m  %c \x1b[0m|", grid[i][j]);
            } else {
                printf("  %c |", grid[i][j]);
            }
        }
        printf("\n   ");
        for (int k = 0; k < 15; k++) {
            printf("-----");
        }
        printf("-");
    }
    printf("\n");
}

void placeTile(char grid[15][15], char letter, int row_index, int col_index) {
    if (row_index >= 0 && row_index <= 14 && col_index >= 0 && col_index <= 14) {
        if (grid[row_index][col_index] == ' ') {
            grid[row_index][col_index] = letter;
        } else {
            printf("cell already occupied");
        }
    } else {
        printf("invalid cell coordinate");
    }
}

int getTilePoints(tile bag[100], char letter) {
    for (int i = 0; i < 100; i++) {
        if (bag[i].letter == letter) {
            return bag[i].points;
        }
    }
    return 0; 
}

int calculateScore(tile bag[100], char letter, int row, int col) {
    int basePoints = getTilePoints(bag, letter);
    int multiplier = 1;

    
    if ((row == 0 && col == 0) || (row == 0 && col == 7) || (row == 0 && col == 14) ||
        (row == 7 && col == 0) || (row == 7 && col == 14) || (row == 14 && col == 0) ||
        (row == 14 && col == 7) || (row == 14 && col == 14)) {
        multiplier = 3;
    }
    
    else if ((row == col) || (row + col == 14)) {
        multiplier = 2;
    }

    return basePoints * multiplier;
}

int hasTiles(char rack[7]) {
    for (int i = 0; i < 7; i++) {
        if (rack[i] != ' ') { 
            return 1;
        }
    }
    return 0;
}
int isValidWord(const char *word) {
    FILE *file = fopen("words.txt", "r");
    if (!file) {
        printf("Error opening dictionary.\n");
        return 1;  
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;  
        if (strcasecmp(buffer, word) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
int calculateWordScore(tile bag[100], char *word, int *rows, int *cols, int length) {
    int word_score = 0;
    int word_multiplier = 1;

    for (int i = 0; i < length; i++) {
        int row = rows[i];
        int col = cols[i];
        int letter_score = getTilePoints(bag, word[i]);
        int letter_multiplier = 1;

        
        if ((row == col) || (row + col == 14)) {
            letter_multiplier = 2;
        }

        
        if ((row == 0 && col == 0) || (row == 0 && col == 7) || (row == 0 && col == 14) ||
            (row == 7 && col == 0) || (row == 7 && col == 7) || (row == 7 && col == 14) ||
            (row == 14 && col == 0) || (row == 14 && col == 7) || (row == 14 && col == 14)) {
            word_multiplier *= 3;
        }

        word_score += letter_score * letter_multiplier;
    }

    return word_score * word_multiplier;
}

void handleObjection(char grid[15][15], char *rack, int *score,tile bag[100]) {
    char objection;
    printf("Do you want to object to the last word \"%s\"? (y/n): ", last_word);
    scanf(" %c", &objection);

    if (objection == 'y' || objection == 'Y') {
        if (!isValidWord(last_word)) {
            printf("Word \"%s\" is invalid! Removing from grid.\n", last_word);
            for (int i = 0; i < last_word_length; i++) {
                grid[last_row_indices[i]][last_col_indices[i]] = ' ';
                
                for (int j = 0; j < 7; j++) {
                    if (rack[j] == ' ') {
                        rack[j] = last_word[i];
                        break;
                    }
                }
            }
            *score -= calculateWordScore(bag, last_word, last_row_indices, last_col_indices, last_word_length);
 
        } else {
            printf("Word \"%s\" is valid.\n", last_word);
        }
    }
}