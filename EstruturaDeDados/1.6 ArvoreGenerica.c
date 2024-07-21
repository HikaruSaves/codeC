#include <stdio.h>
#include <stdlib.h>

#define MAX_NOS 100

typedef struct {
    char valor;
    int filhos[MAX_NOS];
    int numFilhos;
} No;

typedef struct {
    No nos[MAX_NOS];
    int disponiveis[MAX_NOS];
    int topo;
} Arvore;

typedef struct {
    int itens[MAX_NOS];
    int frente;
    int tras;
} Fila;

void inicializaArvore(Arvore *arvore) {
    arvore->topo = -1;
    int i;
    for (i = 0; i < MAX_NOS; i++) {
        arvore->nos[i].valor = '\0';
        arvore->nos[i].numFilhos = 0;
        arvore->disponiveis[i] = i;
    }
}

void inicializaFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
}

int filaVazia(Fila *fila) {
    return fila->tras < fila->frente;
}

void enfileira(Fila *fila, int valor) {
    if (fila->tras < MAX_NOS - 1) {
        fila->itens[++fila->tras] = valor;
    }
}

int desenfileira(Fila *fila) {
    if (!filaVazia(fila)) {
        return fila->itens[fila->frente++];
    }
    return -1;
}

int insereNo(Arvore *arvore, char valor) {
    if (arvore->topo >= MAX_NOS - 1) return -1;
    int indice = arvore->disponiveis[++arvore->topo];
    arvore->nos[indice].valor = valor;
    arvore->nos[indice].numFilhos = 0;
    return indice;
}

int insereFilho(Arvore *arvore, int indicePai, char valorFilho) {
    if (indicePai < 0 || indicePai >= MAX_NOS || arvore->nos[indicePai].numFilhos >= MAX_NOS) return -1;
    int indiceFilho = insereNo(arvore, valorFilho);
    if (indiceFilho != -1) {
        arvore->nos[indicePai].filhos[arvore->nos[indicePai].numFilhos++] = indiceFilho;
    }
    return indiceFilho;
}

void removeNo(Arvore *arvore, int indice) {
    int i;
    if (indice < 0 || indice >= MAX_NOS || arvore->nos[indice].valor == '\0') return;
    for (i = 0; i < arvore->nos[indice].numFilhos; i++) {
        removeNo(arvore, arvore->nos[indice].filhos[i]);
    }
    arvore->nos[indice].valor = '\0';
    arvore->nos[indice].numFilhos = 0;
    arvore->disponiveis[--arvore->topo] = indice;
}

void removeNoEspecifico(Arvore *arvore, int indice) {
    if (indice < 0 || indice >= MAX_NOS || arvore->nos[indice].valor == '\0') return;

    int indicePai = -1;
    int i, j;
    for (i = 0; i < MAX_NOS; i++) {
        for (j = 0; j < arvore->nos[i].numFilhos; j++) {
            if (arvore->nos[i].filhos[j] == indice) {
                indicePai = i;
                break;
            }
        }
        if (indicePai != -1) break;
    }

    if (indicePai == -1) {
        printf("Não é possível remover o nó raiz.\n");
        return;
    }

    No *pai = &arvore->nos[indicePai];
    No *noRemover = &arvore->nos[indice];

    for (i = 0; i < noRemover->numFilhos; i++) {
        pai->filhos[pai->numFilhos++] = noRemover->filhos[i];
    }

    for (i = 0; i < pai->numFilhos; i++) {
        if (pai->filhos[i] == indice) {
            for (j = i; j < pai->numFilhos - 1; j++) {
                pai->filhos[j] = pai->filhos[j + 1];
            }
            pai->numFilhos--;
            break;
        }
    }

    noRemover->valor = '\0';
    noRemover->numFilhos = 0;
    arvore->disponiveis[--arvore->topo] = indice;
}

void buscaProfundidade(Arvore *arvore, int indiceInicial) {
    Fila fila;
    inicializaFila(&fila);
    enfileira(&fila, indiceInicial);
    while (!filaVazia(&fila)) {
        int indiceAtual = desenfileira(&fila);
        if (indiceAtual == -1 || arvore->nos[indiceAtual].valor == '\0') continue;
        printf("Visitando nó %d com valor %c\n", indiceAtual, arvore->nos[indiceAtual].valor);
        int i;
        for (i = 0; i < arvore->nos[indiceAtual].numFilhos; i++) {
            enfileira(&fila, arvore->nos[indiceAtual].filhos[i]);
        }
    }
}

void imprimeArvore(Arvore *arvore, int indice, int nivel) {
    int i;
    if (indice < 0 || indice >= MAX_NOS || arvore->nos[indice].valor == '\0') return;
    for (i = 0; i < nivel; i++) {
        printf("  ");
    }
    printf("Nó %d (valor %c, filhos: ", indice, arvore->nos[indice].valor);

    for (i = 0; i < arvore->nos[indice].numFilhos; i++) {
        printf("%d ", arvore->nos[indice].filhos[i]);
    }
    printf(")\n");
    for (i = 0; i < arvore->nos[indice].numFilhos; i++) {
        imprimeArvore(arvore, arvore->nos[indice].filhos[i], nivel + 1);
    }
}

void menu() {
    Arvore arvore;
    inicializaArvore(&arvore);
    int opcao, indicePai, indiceInicial, indice;
    char valor;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Inserir nó\n");
        printf("2. Inserir filho\n");
        printf("3. Remover nó\n");
        printf("4. Remover nó específico\n");
        printf("5. Busca em profundidade\n");
        printf("6. Imprimir árvore\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor do nó (letra): ");
                scanf(" %c", &valor);
                if (insereNo(&arvore, valor) != -1) {
                    printf("Nó inserido com sucesso.\n");
                } else {
                    printf("Falha ao inserir nó.\n");
                }
                break;
            case 2:
                printf("Digite o índice do nó pai: ");
                scanf("%d", &indicePai);
                printf("Digite o valor do filho (letra): ");
                scanf(" %c", &valor);
                if (insereFilho(&arvore, indicePai, valor) != -1) {
                    printf("Filho inserido com sucesso.\n");
                } else {
                    printf("Falha ao inserir filho.\n");
                }
                break;
            case 3:
                printf("Digite o índice do nó a ser removido: ");
                scanf("%d", &indice);
                removeNo(&arvore, indice);
                printf("Nó removido com sucesso.\n");
                break;
            case 4:
                printf("Digite o índice do nó a ser removido: ");
                scanf("%d", &indice);
                removeNoEspecifico(&arvore, indice);
                printf("Nó removido com sucesso.\n");
                break;
            case 5:
                printf("Digite o índice inicial para a busca em profundidade: ");
                scanf("%d", &indiceInicial);
                buscaProfundidade(&arvore, indiceInicial);
                break;
            case 6:
                printf("Estrutura da árvore:\n");
                imprimeArvore(&arvore, 0, 0);
                break;
            case 7:
                exit(0);
            default:
                printf("Opção inválida!\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
