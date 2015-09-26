#include <stdio.h>
#include <string.h>
/* Size of the sub-squares also called "blocks", "boxes", "regions" */
#define MIN 3
/* Size of the large square also called "grid" */
#define MAX 9
/* Return code functions */
#define FALSE 0
#define TRUE 1

char grid[MAX][MAX]; /* The grid */
long int g_test_counter; /* Test counter */
int g_display_flag; /* Flag for displaying set by the option -a */
/********/
int main(int argc, char *argv[]);
int display_parameter(char t[]);
/**** INPUTS AND CHECKS ****/
int input_grid(void);
int input_a_line(int row);
int check_input_grid(void);
int check_input_rows(void);
int check_input_a_row(int row);
int check_input_columns(void);
int check_input_a_column(int col);
int check_input_sub_squares(void);
int check_input_a_sub_square(int sub_square);
void trans_sub_square(char buffer[], int sub_square);
/**** RESOLUTION ****/
void resolve(void);
int free_cell_for_number(int number, int row, int col);
void trans_sub_square_of_cell(char buffer[], int row, int col);
/**** MISCELLANEOUS ****/
void display_grid_waiting(void);
void display_grid(void);
void display_string(int number);
int finished_grid(void);
void waiting(void);

/******/
int main(int argc, char *argv[])
/******/
{
/* Display option in resolve() function. */
g_display_flag = ((argc >= 2) && (display_parameter(argv[1])));
/**********/
printf("\n******* SUDOKU *******\n\n");
printf("The option sudoku -a displays the solution step by step.\n\n");
printf("Enter %d lines of %d digits.\n", MAX, MAX);
printf("Numbers may be separated by any character.\n");
printf("Tuck a 0 for each blank.\n");
printf("To exit the sudoku program, confirm with the letter Q.\n\n");
while (input_grid()) {
g_test_counter = 0;
resolve();
display_grid();
if (! finished_grid()) printf("Wrong problem...\n");
printf("Tests: %ld\n\n", g_test_counter);
}/* end while */
printf("\n");
return 0;
}

/*******************/
int display_parameter(char t[])
/*******************/
{
/* Return TRUE if the user has typed an application with a display option
 Â in the form: a, A, or /a, /A, or -a, -A, etc ... otherwise return FALSE. */
char param;
if (strlen(t) == 1) param = t[0];
else if (strlen(t) == 2) param = t[1];
else param = 0;
return ((param == 'A') || (param == 'a'));
}/* end display_parameter */

/***************************/
/**** INPUTS AND CHECKS ****/
/***************************/

/************/
int input_grid(void)
/************/
{/* User inputs 9 lines of 9 digits. */
int row;
printf("Enter your Sudoku puzzle: \n");
while (TRUE){
for (row = 0; row < MAX; row++){
if (! input_a_line(row)) return (FALSE);
}/* end for */
printf("\nSudoku problem: \n");
display_grid();
if (check_input_grid()) return(TRUE);
else printf("\n");
}/* end while */
}/* input_grid */

/**************/
int input_a_line(int row)
/**************/
{
/* The user must enter a line from 0 to 9 digits
separated or not by one or more characters.
Only the first 9 digits are retained.
The most practical: Numerical keypad and
put a point after every 3rd number.
Avoid using gets() because of overflows.
In this case, remember to completely empty the buffer. */
int c, i;
int col = 0;
int result = TRUE;
/* Reset to zero of the cells in the row. */
for (i = 0; i < MAX; i++) {
grid[row][i] = 0;
}/* for */
/*******/
printf("row %d : ", row+1);
while(TRUE){
c = fgetc(stdin);
if (c == feof(stdin) || c == 0x0A) return result;
else if ((c == 'Q') || (c == 'q')) result = FALSE;
/* Transfer the keyboard input line into the row of the grid. */
else if ( (result) && (col < MAX) && (c >= '0') && (c <= '9')) {
grid[row][col++] = c-48;
}/*end if */
}/* end while */
}/*end input_a_line */

/******************/
int check_input_grid(void)
/******************/
{/* Check whether any duplicate throughout the grid. */
int result = TRUE;
if (! check_input_rows()) result = FALSE;
if (! check_input_columns()) result = FALSE;
if (! check_input_sub_squares()) result = FALSE;
return result;
}/* end check_input_grid */

/******************/
int check_input_rows(void)
/******************/
{/* Checks if no duplicate into the rows. */
int row;
int result = TRUE;
for (row = 0; row < MAX; row++) {
if (! check_input_a_row(row)) result = FALSE;
}/* end for */
return result;
}/* end check_input_rows */

/*******************/
int check_input_a_row(int row)
/*******************/
{/* Checks if no duplicate into the row. */
int i,j;
for (i = 0; i < MAX-1; i++) {
if ( grid[row][i] == 0 ) continue;
for (j = i+1; j < MAX; j++) {
if (grid[row][i] == grid[row][j]) {
printf("Error row %d: equal numbers\n", row+1);
return FALSE;
}/* end if */
}/* end for j */
}/* end for i */
return TRUE;
}/* end check_input_a_row */

/*********************/
int check_input_columns(void)
/*********************/
{/* Checks if no duplicate into the columns. */
int col;
int result = TRUE;
for (col = 0; col < MAX; col++) {
if (! check_input_a_column(col)) result = FALSE;
}/* end for */
return result;
}/* end check_input_columns */

/**********************/
int check_input_a_column(int col)
/**********************/
{/* Checks if no duplicate into the column. */
int i, j;
for (i = 0; i < MAX-1; i++) {
if ( grid[i][col] == 0 ) continue;
for (j = i+1; j < MAX; j++) {
if (grid[i][col] == grid[j][col]) {
printf("Error column %d: equal numbers\n", col+1);
return FALSE;
}/* end if */
}/* end for j */
}/* end for i */
return TRUE;
}/* end check_input_a_column */

/*************************/
int check_input_sub_squares(void)
/*************************/
{/* Checks if no duplicate into the sub-squares of 3 x 3. */
int sub_square;
int result = TRUE;
for (sub_square = 0; sub_square < MAX; sub_square++) {
if (! check_input_a_sub_square(sub_square)) result = FALSE;
}/* end for */
return result;
}/* end check_input_sub_squares */

/**************************/
int check_input_a_sub_square(int sub_square)
/**************************/
{/* Checks if no duplicate into the sub-square of 3 x 3. */
char buffer[MAX+10];
int i, j;
trans_sub_square(buffer, sub_square);
for (i = 0; i < MAX-1; i++) {
if (buffer[i] == 0 ) continue;
for (j = i+1; j < MAX; j++) {
if (buffer[i] == buffer[j]) {
printf("Error sub-square %d: equal numbers\n", sub_square+1);
return FALSE;
}/* end if */
}/* end for j */
}/* end for i */
return TRUE;
}/* end check_input_a_sub_square */

/*******************/
void trans_sub_square(char buffer[], int sub_square)
/*******************/
{/* Transfer the sub-square of 3 x 3 into a one-dimensional array. */
int i,j, x,y,z;
x = (((sub_square) % MIN)*MIN);
y = (((sub_square+MIN) / MIN)*MIN)-MIN;
z = 0;
for (j = y; j < y+MIN; j++) {
for (i = x; i < x+MIN; i++) {
buffer[z++] = grid[j][i];
}/* end for i */
}/* end for j */
}/* end trans_sub_square */

/********************/
/**** RESOLUTION ****/
/********************/

/**********/
void resolve(void)
/**********/
{/* Attention, recursive function... */
int row, col, number, buffer_number;
for (row = 0; row < MAX; row++) {
for (col = 0; col < MAX; col++) {
if (grid[row][col]) continue;
for (number = 1; number <= MAX; number++) {
if (! free_cell_for_number(number, row, col)) continue;
buffer_number = grid[row][col];
grid[row][col] = number;
g_test_counter++;
if (g_display_flag) display_grid_waiting();
resolve();
if (finished_grid()) return;
/* To see all the solutions of the grids
Â Â Â Â Â Â Â Â Â Â Â Â with multiple solutions,
Â Â Â Â Â Â Â Â Â Â Â Â switch the line above as a comment and
Â Â Â Â Â Â Â Â Â Â Â Â remove the comment command of the line below. */
/* if (finished_grid()) display_grid_waiting(); */
grid[row][col] = buffer_number;
}/* end for number */
return;
}/* end for col */
}/* end for row */
return;
}/* end resolve */

/**********************/
int free_cell_for_number(int number, int row, int col)
/**********************/
{
/* Test if the number is already in
the line, or the column, or the sub-square of 3 X 3
afferent at the cell pointed to by line and column. */
char buffer[MAX+10];
int i;
/* If the number is used in the line return FALSE */
for (i = 0; i < MAX; i++) if (grid[row][i] == number) return(FALSE);
/* If the number is used in the column return FALSE */
for (i = 0; i < MAX; i++) if (grid[i][col] == number) return(FALSE);
/* If the number is used in the sub-square of 3 X 3 return FALSE */
trans_sub_square_of_cell(buffer, row, col);
for (i = 0; i < MAX; i++) if (buffer[i] == number) return (FALSE);
/* whereby the number is available for the cell, return TRUE. */
return(TRUE);
}/* end free_cell_for_number */

/***************************/
void trans_sub_square_of_cell(char buffer[], int row, int col)
/***************************/
/* Transfer the sub-square of 3 x 3 of
the cell into a one-dimensional array. */
{
int i, j, k;
while ((row % MIN) != 0) row--;
while ((col % MIN) != 0) col--;
k = 0;
for (j = row; j < row+MIN; j++) {
for (i = col; i < col+MIN; i++) {
buffer[++k] = grid[j][i];
}/* end for i */
}/* end for j */
}/* end trans_sub_square_of_cell */

/***********************/
/**** MISCELLANEOUS ****/
/***********************/

/***********************/
void display_grid_waiting(void)
/***********************/
{/* Display the grid with wait for the enter key on the keybord. */
display_grid();
printf("%ld, hit the enter key for continuation.", g_test_counter);
waiting();
}/* display_grid_waiting */

/***************/
void display_grid(void)
/***************/
{/* Display the grid. */
int row, col;
for (row = 0; row < MAX; row++) {
for (col = 0; col < MAX; col++) {
printf(" %1d ", grid[row][col]);
if ( ((col % MIN) == MIN-1) && (col < MAX-1) ) {
printf(" | ");
}
}/* end for col */
printf("\n");
if ( ((row % MIN) == MIN-1) && (row < MAX-1) ) {
display_string(33);
}
}/* end for row */
printf("\n");
}/* end display_grid */

/*****************/
void display_string(int number)
/*****************/
{/* Displays a sequence of asterisk character. */
int i;
for (i = 1; i <= number; i++) {
printf("*");
}
printf("\n");
}/* end display_string */

/****************/
int finished_grid(void)
/****************/
/* Test if the grid is finished.
Return TRUE if grid completed, otherwise return FALSE.
Begins with the end of the grid,
this one being completed by its beginning. */
{
int row, col;
for (row = MAX-1; row >= 0; row--) {
for (col = MAX-1; col >= 0; col--) {
if (grid[row][col] == 0 ) {
return FALSE;
}
}/* end for col */
}/* end for row */
return TRUE;
}/* end finished_grid */

/**********/
void waiting(void)
/**********/
{/* Wait for the enter key on the keybord. */
char c;
while(TRUE){
c = fgetc(stdin);
if(c == feof(stdin) || c == 0x0A) return;
}/* end while */
}/* end waiting */
