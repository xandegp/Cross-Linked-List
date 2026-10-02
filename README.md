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

 ##Input file:
 .The Input file have a command per line, in the following format:

    .DEF lin col 


O arquivo de entrada possui um comando por linha, no seguinte formato:
• DEF lin col valor – equivalente a chamar definir_celula com os parâmetros
informados;
• REM lin col – equivalente a chamar remover_celula;
• GET lin col – consulta o valor da célula (obter_valor);
• SOMA li lf ci cf – soma o intervalo [li, lf] × [ci, cf] (somar_intervalo);
• CONT – consulta o número de células não nulas (contar_nao_nulas);
• TRANS lin col tam – transpõe a matriz quadrada com canto superior esquerdo
(lin, col) e dimensão tam;
• DESFAZER – desfaz a última operação (desfazer);
• EXIBIR – exibe o estado atual da planilha (exibir_planilha);
• HIST – exibe o histórico de alterações (exibir_historico).
Os comandos são processados na ordem em que aparecem no arquivo de entrada.
Exemplo de entrada:
DEF 0 1 12
DEF 0 3 5
DEF 2 1 7
GET 0 1
SOMA 0 2 0 3
9
CONT
REM 0 3
DESFAZER
EXIBIR
HIST
5.2 Arquivo de saída
Os comandos especificados na seção anterior produzem as seguintes saídas ao serem processados:
• GET lin col: imprime uma linha com GET linha coluna valor (usa
obter_valor);
• SOMA li lf ci cf: imprime uma linha com SOMA li lf ci cf e o valor retornado
por somar_intervalo;
• CONT: imprime uma linha com CONT e o valor retornado por contar_nao_nulas;
• DESFAZER: se a pilha de histórico estiver vazia, imprime "HISTORICO VAZIO"; caso
contrário, não produz saída;
• EXIBIR: imprime PLANILHA seguido de uma linha linha coluna valor para cada
célula não nula, na ordem descrita na Seção 4; se a planilha estiver vazia, imprime
apenas "PLANILHA VAZIA";
• HIST: imprime HISTORICO seguido de uma linha para cada operação na pilha no
formato definido na Seção 4.11, do topo para a base; se o histórico estiver vazio,
imprime apenas "HISTORICO VAZIO".
• Os comandos DEF, REM e TRANS não geram saída.
A saída padrão do programa é redirecionada para o arquivo de saída. Então, o uso de
printf irá interferir no arquivo de saída.
Exemplo de saída referente ao exemplo de entrada da Seção 5.1:
GET 0 1 12
SOMA 0 2 0 3 24
CONT 3
PLANILHA
0 1 12
0 3 5
2 1 7
HISTORICO
2 1 0
0 3 0
0 1 0
