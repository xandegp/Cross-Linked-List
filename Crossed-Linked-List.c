/* ============================================================================
    EP1 - Planilha Esparsa com Histórico de Alterações
    TEMPLATE - preencha os TODOs abaixo. Não altere assinaturas de função,
    nomes ou ordem de campos de struct.
 
    Uso: ./ep_XXXX arquivo_entrada.txt arquivo_saida.txt
 ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
  Estruturas de dados (não altere nomes e ordem de campos)
 ---------------------------------------------------------------------- */

typedef struct celula {
    int linha;
    int coluna;
    int valor;
    struct celula *proxima_linha;
    struct celula *proxima_coluna;
} celula_t;

typedef struct fileira {
    int indice;
    celula_t* primeiro;
    struct fileira *proximo;
} fileira_t;

typedef struct {
    bool transposicao;
    int linha;
    int coluna;
    union {
        int valor_anterior;
        int tamanho;
    };
} operacao_t;

typedef struct elo_pilha {
    operacao_t op;
    struct elo_pilha *proximo;
} elo_pilha_t;

typedef struct {
    elo_pilha_t* topo;
} pilha_t;

typedef struct {
    fileira_t* primeira_linha;
    fileira_t* primeira_coluna;
    int total_celulas;
    pilha_t historico;
} planilha_t;

/* ---------------------------------------------------------------------- *
    Funções auxiliar para leitura das strings dos comandos
 ---------------------------------------------------------------------- */

int igual(char* a, char* b) {
    int i = 0;
    while (a[i] == b[i] && a[i] != '\0') i++;

    return (a[i] == b[i]);
}

/* ---------------------------------------------------------------------- *
    Funções obrigatórias (não altere as assinaturas)
 ---------------------------------------------------------------------- */

void inicializar_planilha(planilha_t *p) {
    // TODO: inicialize primeiraLinha, primeiraColuna, total_celulas e historico.topo
    p->primeira_linha = NULL;
    p->primeira_coluna = NULL;
    p->total_celulas = 0;
    p->historico.topo = NULL;
}

celula_t* buscar_celula(planilha_t *p, int lin, int col,
                      celula_t** cel_ant_linha, celula_t** cel_ant_coluna,
                      fileira_t** fil_ant_linha, fileira_t** fil_ant_coluna) {
    /* TODO: localize a celula (lin,col), preencha os quatro antecessores 
        por referencia, retorne NULL se a celula nao existir */

    celula_t* atual = NULL;
    celula_t* temp_linha = NULL;
    celula_t* temp_col = NULL;

    if(cel_ant_linha != NULL) *cel_ant_linha = NULL;
    if(cel_ant_coluna != NULL) *cel_ant_coluna = NULL;
    if(fil_ant_linha != NULL) *fil_ant_linha = NULL;
    if(fil_ant_coluna != NULL) *fil_ant_coluna = NULL;

    if(lin < 0 || col < 0 || p == NULL) return NULL;

    
    fileira_t * fil_atual_linha = p->primeira_linha;
    fileira_t * fil_atual_coluna = p->primeira_coluna;


    while(fil_atual_linha != NULL && fil_atual_linha->indice < lin){
        if (fil_ant_linha)*fil_ant_linha = fil_atual_linha;
        fil_atual_linha = fil_atual_linha->proximo;
    }
    while(fil_atual_coluna != NULL && fil_atual_coluna->indice < col){
        if (fil_ant_coluna)*fil_ant_coluna = fil_atual_coluna;
        fil_atual_coluna = fil_atual_coluna->proximo;
    }

    if(fil_atual_linha != NULL && fil_atual_linha->indice == lin){
        temp_linha = fil_atual_linha->primeiro;
        while(temp_linha != NULL && temp_linha->coluna < col){
            if (cel_ant_linha)*cel_ant_linha = temp_linha;
            temp_linha = temp_linha->proxima_coluna;
        }
    }

    if(temp_linha != NULL && temp_linha->coluna == col) atual = temp_linha;

    if(fil_atual_coluna != NULL && fil_atual_coluna->indice == col){
        temp_col = fil_atual_coluna->primeiro;
        while(temp_col != NULL && temp_col->linha < lin){
            if (cel_ant_coluna)*cel_ant_coluna = temp_col;
            temp_col = temp_col->proxima_linha;
        }
    }
    
    return atual;
}

int obter_valor(planilha_t *p, int linha, int coluna) { 
    // TODO: use buscarCelula; retorne 0 se a celula nao existir
    if(p == NULL) return 0;

    celula_t*atual = buscar_celula(p,linha,coluna,NULL,NULL,NULL,NULL);

    if(atual != NULL) return atual->valor;

    return 0;
}

int somar_intervalo(planilha_t* p, int linha_ini, int linha_fim, int coluna_ini, int coluna_fim) {
    // TODO: some os valores das celulas nao nulas no intervalo dado
    if(p == NULL || linha_ini < 0 || linha_fim < linha_ini || coluna_ini < 0 || coluna_fim < coluna_ini) return 0;
    
    int soma = 0;
    fileira_t*fileira_linha_atual = p->primeira_linha;
    
    while(fileira_linha_atual != NULL && fileira_linha_atual->indice < linha_ini){
        fileira_linha_atual = fileira_linha_atual->proximo;
    }

    while(fileira_linha_atual != NULL && fileira_linha_atual->indice <= linha_fim){
        celula_t*atual = fileira_linha_atual->primeiro;

        while(atual != NULL && atual->coluna < coluna_ini){
            atual = atual->proxima_coluna;
        }

        while(atual != NULL && atual->coluna <= coluna_fim){
            soma += atual->valor;
            atual = atual->proxima_coluna;
        }
        fileira_linha_atual = fileira_linha_atual->proximo;
    }
    return soma;
}

int contar_nao_nulas(planilha_t* p) {
    // TODO: retorne a quantidade de celulas nao nulas
    if(p == NULL) return 0;

    int count = 0;
    fileira_t*fileira_linha_atual;
    fileira_linha_atual = p->primeira_linha;


    while(fileira_linha_atual != NULL){
        celula_t*atual = fileira_linha_atual->primeiro;

        while(atual != NULL){
            count++;
            atual = atual->proxima_coluna;
        }

        fileira_linha_atual = fileira_linha_atual->proximo;
    } 
    return count;
}

bool definir_celula(planilha_t* p, int lin, int col, int valor) {
    /* TODO: implemente os 4 casos (atualizar/criar/remover/nulo),
       empilhando em p->historico quando houver alteracao efetiva.
       Retorna true se houve alteracao, false se foi operacao nula. */
    if(p == NULL || lin < 0 || col < 0) return false;

    celula_t*cel_ant_linha, *cel_ant_coluna;
    fileira_t*fil_ant_linha, *fil_ant_coluna;

    celula_t*atual = buscar_celula(p,lin,col,&cel_ant_linha, &cel_ant_coluna,
                                     &fil_ant_linha, &fil_ant_coluna);
    int valor_ant;
    
    if(atual) valor_ant = atual->valor;
    else valor_ant = 0;

    if(valor_ant == valor) return false;
    //se valor da celula antiga = valor a ser adicionado, nao faz nada(nulo)

    elo_pilha_t*novo = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
    novo->op.linha = lin;
    novo->op.coluna = col;
    novo->op.transposicao = false;
    novo->op.valor_anterior = valor_ant;
    novo->proximo = p->historico.topo;
    p->historico.topo = novo;

    if(atual != NULL && valor != 0){
    //celula existe e valor != 0
        atual->valor = valor;
    }

    else if(atual == NULL && valor != 0){
    //celula nao existe e valor != 0
        celula_t*nova_celula = (celula_t*) malloc(sizeof(celula_t));
        nova_celula->coluna = col;
        nova_celula->linha = lin;
        nova_celula->valor = valor;
        
        fileira_t*nova_fil_lin;
        fileira_t*nova_fil_col;

        //nova fileira de linha
        if(fil_ant_linha) nova_fil_lin = fil_ant_linha->proximo;
        else nova_fil_lin = p->primeira_linha;

        if(nova_fil_lin == NULL || nova_fil_lin->indice != lin){
        //nova fileira diferente da esperada -> nova fileira nao existe
            nova_fil_lin = (fileira_t*) malloc(sizeof(fileira_t));
            nova_fil_lin->indice = lin;
            nova_fil_lin->primeiro = NULL;
            
            
            if(fil_ant_linha == NULL){
                nova_fil_lin->proximo = p->primeira_linha;
                p->primeira_linha = nova_fil_lin;                
            }
            else{
                nova_fil_lin->proximo = fil_ant_linha->proximo;
                fil_ant_linha->proximo = nova_fil_lin;
            }


        }

        //nova fileira de coluna
        if(fil_ant_coluna) nova_fil_col = fil_ant_coluna->proximo;
        else nova_fil_col = p->primeira_coluna;

        if(nova_fil_col == NULL || nova_fil_col->indice != col){
        //nova fileira diferente da esperada -> nova fileira nao existe
            nova_fil_col = (fileira_t*) malloc(sizeof(fileira_t));
            nova_fil_col->indice = col;
            nova_fil_col->primeiro = NULL;
            
            if(fil_ant_coluna == NULL){
                nova_fil_col->proximo = p->primeira_coluna;
                p->primeira_coluna = nova_fil_col;                
            }
            else{
                nova_fil_col->proximo = fil_ant_coluna->proximo;
                fil_ant_coluna->proximo = nova_fil_col;
            }
        }

        if(cel_ant_linha){
            nova_celula->proxima_coluna = cel_ant_linha->proxima_coluna;
            cel_ant_linha->proxima_coluna = nova_celula;
        }
        else{
            nova_celula->proxima_coluna = nova_fil_lin->primeiro;
            nova_fil_lin->primeiro = nova_celula;
        }


        if(cel_ant_coluna){
            nova_celula->proxima_linha = cel_ant_coluna->proxima_linha;
            cel_ant_coluna->proxima_linha = nova_celula;
        }
        else{
            nova_celula->proxima_linha = nova_fil_col->primeiro;
            nova_fil_col->primeiro = nova_celula;
        }

        p->total_celulas++;
    }
    else{
    // celula existe e valor = 0 -> remover
        fileira_t*fil_atual_lin;
        fileira_t*fil_atual_col;
        
        //achar fileira de linhas atual
        if(fil_ant_linha) fil_atual_lin = fil_ant_linha->proximo;
        else fil_atual_lin = p->primeira_linha;

        //achar fileira de colunas atual
        if(fil_ant_coluna) fil_atual_col = fil_ant_coluna->proximo;
        else fil_atual_col = p->primeira_coluna;

        //remover a celula da linha
        if(cel_ant_linha) cel_ant_linha->proxima_coluna = atual->proxima_coluna;
        else fil_atual_lin->primeiro = atual->proxima_coluna;

        //remover a celula da coluna
        if(cel_ant_coluna) cel_ant_coluna->proxima_linha = atual->proxima_linha;
        else fil_atual_col->primeiro = atual->proxima_linha;

        if(fil_atual_lin->primeiro == NULL){
            if(fil_ant_linha) fil_ant_linha->proximo = fil_atual_lin->proximo;
            else p->primeira_linha = fil_atual_lin->proximo;
            free(fil_atual_lin);
        }

        if(fil_atual_col->primeiro == NULL){
            
            if(fil_ant_coluna) fil_ant_coluna->proximo = fil_atual_col->proximo;
            else p->primeira_coluna = fil_atual_col->proximo;
            free(fil_atual_col);
        }

        free(atual);
        p->total_celulas--;
    }
    return true;
}

bool remover_celula(planilha_t* p, int lin, int col) {
    // TODO: remova a celula (lin,col)
    if(p == NULL || lin < 0 || col < 0) return false;
    
    celula_t*cel_ant_linha, *cel_ant_coluna;
    fileira_t*fil_ant_linha, *fil_ant_coluna, *fil_atual_linha, *fil_atual_coluna;



    celula_t*atual = buscar_celula(p,lin,col,&cel_ant_linha,&cel_ant_coluna,
                                            &fil_ant_linha,&fil_ant_coluna);

    if(atual == NULL) return false;

    elo_pilha_t*novo = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
    novo->op.coluna = col;
    novo->op.linha = lin;
    novo->op.transposicao = false;
    novo->op.valor_anterior = atual->valor;
    novo->proximo = p->historico.topo;
    p->historico.topo = novo;
    
    //achar a fileira das linhas
    if(fil_ant_linha) fil_atual_linha = fil_ant_linha->proximo;
    else fil_atual_linha = p->primeira_linha;
    
    //achar a fileira das colunas
    if(fil_ant_coluna) fil_atual_coluna = fil_ant_coluna->proximo;
    else fil_atual_coluna = p->primeira_coluna;

    //remover a celula da linha
    if(cel_ant_linha) cel_ant_linha->proxima_coluna = atual->proxima_coluna;
    else fil_atual_linha->primeiro = atual->proxima_coluna;

    //remover a celula da coluna
    if(cel_ant_coluna) cel_ant_coluna->proxima_linha = atual->proxima_linha;
    else fil_atual_coluna->primeiro = atual->proxima_linha;

    //checar se as fileiras vao estar vazias ou n
    if(fil_atual_linha->primeiro == NULL){
        if(fil_ant_linha) fil_ant_linha->proximo = fil_atual_linha->proximo;
        else p->primeira_linha = fil_atual_linha->proximo;
        free(fil_atual_linha);
    }

    if(fil_atual_coluna->primeiro == NULL){
        if(fil_ant_coluna) fil_ant_coluna->proximo = fil_atual_coluna->proximo;
        else p->primeira_coluna = fil_atual_coluna->proximo;
        free(fil_atual_coluna);
    }
    free(atual);
    p->total_celulas--;

    return true;
}

bool transpor(planilha_t* p, int lin, int col, int tamanho) {
    /* TODO: transpoe uma matriz quadrada que está localizada entre
    as linhas [lin, lin + tamanho) e colunas [col, col + tamanho). */    
    if (p == NULL || lin < 0 || col < 0 || tamanho <= 0) return false;

    int trocas = 0;

    for (int i = 0; i < tamanho; i++) {
        for (int j = i + 1; j < tamanho; j++) {
            int l1 = lin + i, c1 = col + j;
            int l2 = lin + j, c2 = col + i;

            int val1 = obter_valor(p, l1, c1);
            int val2 = obter_valor(p, l2, c2);

            if (val1 != val2) {
                definir_celula(p, l1, c1, val2);
                definir_celula(p, l2, c2, val1);
                trocas++;
            }
        }
    }


    if (trocas == 0) return false;

    int remover = 2 * trocas;

    for (int k = 0; k < remover; k++) {
        if (p->historico.topo != NULL) {
            elo_pilha_t* temp = p->historico.topo;
            p->historico.topo = temp->proximo;
            free(temp);
        }
    }

    elo_pilha_t* novo = (elo_pilha_t*) malloc(sizeof(elo_pilha_t));
    if (!novo) return false;

    novo->op.linha = lin;
    novo->op.coluna = col;
    novo->op.transposicao = true;
    novo->op.tamanho = tamanho;
    novo->proximo = p->historico.topo;
    p->historico.topo = novo;

    return true;
}

bool desfazer(planilha_t* p) {
    /* TODO: desempilhe de p->historico e restaure o valor anterior.
       Retorna false se o historico estiver vazio, true caso contrario. */

    if(p == NULL || !p->historico.topo) return false;     

    elo_pilha_t* remover_topo = p->historico.topo;
    operacao_t op = remover_topo->op;

    p->historico.topo = remover_topo->proximo;
    free(remover_topo);

    int lin = op.linha;
    int col = op.coluna;
    int val = op.valor_anterior;

    if(op.transposicao){
        int tamanho = op.tamanho;
        transpor(p,lin,col,tamanho);

        elo_pilha_t*temp = p->historico.topo;
        if(temp){
            p->historico.topo = temp->proximo;
            free(temp);
        }
    }
    else{
        definir_celula(p, lin, col, val);
        elo_pilha_t*temp = p->historico.topo;
        p->historico.topo = temp->proximo;
        free(temp);
    }
    return true;
}

void exibir_planilha(planilha_t *p) {
    /* TODO: imprima "linha coluna valor" por linha, em ordem crescente
       de linha e, dentro de cada linha, de coluna. Se vazia, imprima
       "PLANILHA VAZIA" */
    if(p == NULL){
        printf("PLANILHA VAZIA\n");
        return; 
    }

    fileira_t*fileira_linha_atual;
    fileira_linha_atual = p->primeira_linha;

    while(fileira_linha_atual != NULL){
        celula_t*atual = fileira_linha_atual->primeiro;
        while(atual != NULL){
            printf("%d %d %d\n",atual->linha,atual->coluna,atual->valor);
            atual = atual->proxima_coluna;
        }
        fileira_linha_atual = fileira_linha_atual->proximo;
    }

}

void exibir_historico(planilha_t* p) {
    /* TODO: imprima "linha coluna valor_anterior" por linha, do topo
       para a base. Se vazio, imprima "HISTORICO VAZIO" */

    if(p == NULL || p->historico.topo == NULL) {
        printf("HISTORICO VAZIO\n");
        return;
    }

    elo_pilha_t* atual = p->historico.topo;
    while(atual){
        if(!atual->op.transposicao){
            printf("%d %d %d\n", atual->op.linha, atual->op.coluna, atual->op.valor_anterior);
        } else {
            printf("T %d %d %d\n", atual->op.linha, atual->op.coluna, atual->op.tamanho);
        }
        atual = atual->proximo;
    }
}

void liberar_tudo(planilha_t* p) {
    // TODO: libere toda a memória alocada por fileira, celula, e pilha.
    if(p == NULL) return;
    fileira_t*fileira_linha = p->primeira_linha;

    while(fileira_linha != NULL){
        celula_t*apagar;
        celula_t*atual = fileira_linha->primeiro;

        while(atual != NULL){
            apagar = atual;
            atual = atual->proxima_coluna;
            free(apagar);
        }

        fileira_linha = fileira_linha->proximo;
    }

    fileira_t*fileira_linha_apagar;
    fileira_t*fileira_linha_atual = p->primeira_linha;
    while(fileira_linha_atual != NULL){
        fileira_linha_apagar = fileira_linha_atual;
        fileira_linha_atual = fileira_linha_atual->proximo;
        free(fileira_linha_apagar);
    }
    
    fileira_t*fileira_coluna_apagar;
    fileira_t*fileira_coluna_atual = p->primeira_coluna;
    while(fileira_coluna_atual != NULL){
        fileira_coluna_apagar = fileira_coluna_atual;
        fileira_coluna_atual = fileira_coluna_atual->proximo;
        free(fileira_coluna_apagar);
    }

    elo_pilha_t*apagar;
    elo_pilha_t*atual = p->historico.topo;
    while(atual != NULL){
        apagar = atual;
        atual = atual->proximo;
        free(apagar);
    }
    p->historico.topo = NULL;
    p->primeira_coluna = NULL;
    p->primeira_linha = NULL;
    p->total_celulas = 0;

}

/* ---------------------------------------------------------------------- *
    Main para leitura de arquivos (já pronta no caso, pode ser que na versão final deixemos sem)
 ---------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso do comando eh: %s arquivo_entrada.txt arquivo_saida.txt\n", argv[0]);
        return 1;
    }
    
    FILE* entrada = fopen(argv[1], "r");
    FILE* saida = freopen(argv[2], "w", stdout);

    if (!entrada || !saida) {
        fprintf(stderr, "Erro ao tentar abrir os arquivos.\n");
        return 1;
    }

    planilha_t p;
    inicializar_planilha(&p);

    int n;
    fscanf(entrada, "%d", &n);

    char cmd[20];

    while (fscanf(entrada, "%s", cmd) != EOF) {
 
        if (igual(cmd, "DEF")) {
            int lin, col, valor;
            fscanf(entrada, "%d %d %d", &lin, &col, &valor);
            definir_celula(&p, lin, col, valor);
        } else if (igual(cmd, "REM")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            remover_celula(&p, lin, col);
        } else if (igual(cmd, "GET")) {
            int lin, col;
            fscanf(entrada, "%d %d", &lin, &col);
            fprintf(saida, "GET %d %d %d\n", lin, col, obter_valor(&p, lin, col));
        } else if (igual(cmd, "SOMA")) {
            int li, lf, ci, cf;
            fscanf(entrada, "%d %d %d %d", &li, &lf, &ci, &cf);
            fprintf(saida, "SOMA %d %d %d %d %d\n", li, lf, ci, cf, somar_intervalo(&p, li, lf, ci, cf));
        } else if (igual(cmd, "CONT")) {
            fprintf(saida, "CONT %d\n", contar_nao_nulas(&p));
        } else if (igual(cmd, "DESFAZER")) {
            if (!desfazer(&p)) {
                fprintf(saida, "HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "EXIBIR")) {
            if (contar_nao_nulas(&p)) {
                printf("PLANILHA\n");
                exibir_planilha(&p);
            } else {
                printf("PLANILHA VAZIA\n");
            }
        } else if (igual(cmd, "HIST")) {
            if (p.historico.topo) {
                printf("HISTORICO\n");
                exibir_historico(&p);
            }
            else {
                printf("HISTORICO VAZIO\n");
            }
        } else if (igual(cmd, "TRANS")) {
            int lin, col, tam;
            fscanf(entrada, "%d %d %d", &lin, &col, &tam);
            transpor(&p, lin, col, tam);
        }
    }

    fclose(entrada);
    fclose(saida);

    liberar_tudo(&p);
    return 0;
}