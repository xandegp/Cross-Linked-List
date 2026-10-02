# Cross-Linked-List
Cross-Linked List developed in C

This project is a Cross-Linked List developed in C

## Introduction:
This data structure is a better way to store data in a spreadsheet, using  Linked Lists to link rows and cell together to use less memory comparing to a dynamic or static allocated matrix. Also, it uses a stack system where it store the last edit the user made to make it possible to restore the last value of the cell, and it have a function to transpose a piece of the spreadsheet.

## Functions:
1. start_spreadsheet
2. get_value
3. sum_range
4. count_non-null
5. define_cell
6. remove_cell
7. transpose
8. undo
9. show_spreadsheet
10. show_history
11. free_all

## Pre-Requisits:
.GCC or any C compiler
.Git

## How to use:
.Download the Input files in this repository (you can also download the Output files to compare the output that the program make in your local envirement)

.Download the repository on your local environment copying and pasting the following code on your terminal:

    git clone https://github.com/xandegp/Cross-Linked-List.git

.Locate which directory the file was saved

.At the terminal, put the following code until you find the Cross-Linked-List file:

    cd (directory where the file was saved)


.Still at the terminal, past the following code:

    gcc -Wall -o Cross-Linked-List Cross-Linked-List.c
    
    ./Cross-Linked-List (name of the input file).txt (name of a file that the program will make).txt

## Input File:
.The input file contains one command per line in the following format:
    .DEF line col value – Equivalent to calling define_cell with the provided parameters;
    .REM line col – Equivalent to calling remove_cell;
    .GET line col – Queries the value of a cell (get_value);
    .SUM start_line end_line start_col end_col – Sums the range [start_line, end_line] × [start_col, end_col] (sum_range);
    .COUNT – Queries the number of non-null cells (count_non_null);
    .TRANSPOSE line col size – Transposes the square submatrix with top-left corner (line, col) and dimension size;
    .UNDO – Reverts the last operation (undo);
    .SHOW – Displays the current state of the spreadsheet (show_spreadsheet);
    .HISTORY – Displays the history of changes (show_history).
    
.Commands are processed in the order they appear in the input file.

.Example Input:

    Plaintext
    DEF 0 1 12
    DEF 0 3 5
    DEF 2 1 7
    GET 0 1
    SUM 0 2 0 3
    COUNT
    REM 0 3
    UNDO
    SHOW
    HISTORY
## Output File:
.The commands specified in the previous section produce the following outputs when processed:
    •GET line col: Prints a single line with GET line col value (uses get_value)
    •SUM start_line end_line start_col end_col: Prints a single line with SUM start_line end_line start_col end_col followed by the value returned by sum_range
    •COUNT: Prints a single line with COUNT followed by the value returned by count_non_null
    •UNDO: If the history stack is empty, prints "EMPTY HISTORY"; otherwise, produces no output
    •SHOW: Prints SPREADSHEET followed by a line line col value for each non-null cell, sorted by row and column; if the spreadsheet is empty, prints EMPTY SPREADSHEET
    •HISTORY: Prints HISTORY followed by a line for each operation on the stack from top to bottom; if the history is empty, prints EMPTY HISTORY

.The commands DEF, REM, and TRANSPOSE produce no output.

.Standard output is redirected to the output file. Therefore, calls to printf directly write to the output file.

.Example Output corresponding to the Example Input:

    Plaintext
    GET 0 1 12
    SUM 0 2 0 3 24
    COUNT 3
    SPREADSHEET
    0 1 12
    0 3 5
    2 1 7
    HISTORY
    2 1 0
    0 3 0
    0 1 0
