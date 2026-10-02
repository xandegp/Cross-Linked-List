# Cross-Linked-List
Cross-Linked List developed in C

This project is a Cross-Linked List developed in C

### Introduction:

This data structure is a better way to store data in a spreadsheet, using  Linked Lists to link rows and cell together to use less memory comparing to a dynamic or static allocated matrix. Also, it uses a stack system where it store the last edit the user made to make it possible to restore the last value of the cell, and it have a function to transpose a piece of the spreadsheet.

### Functions:
.start_spreadsheet

.get_value

.sum_range

.count_non-null

.define_cell

.remove_cell

.transpose

.undo

.show_spreadsheet

.show_history

.free_all


### Pre-Requisits:
. GCC or any C compiler
. Git

### How to use:
.Download the Input files in this repository (you can also download the Output files to compare the output that the program make in your local envirement)

.Download the repository on your local environment copying and pasting the following code on your terminal:

    git clone https://github.com/xandegp/Cross-Linked-List.git

.Locate which directory the file was saved

.At the terminal, put the following code until you find the Cross-Linked-List file:

    cd (directory where the file was saved)


.Still at the terminal, past the following code:

    gcc -Wall -o Cross-Linked-List Cross-Linked-List.c
    ./Cross-Linked-List (name of the input file).txt (name of a file that the program will make).txt



