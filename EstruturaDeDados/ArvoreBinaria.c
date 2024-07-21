#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int chave;
    struct No* esquerda;
    struct No* direita;
    int altura;
} No;

No* criarNo(int chave) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->chave = chave;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->altura = 1;
    return novoNo;
}

int altura(No* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

int maximo(int a, int b) {
    return (a > b) ? a : b;
}

No* rotacionarDireita(No* y) {
    No* x = y->esquerda;
    No* T2 = x->direita;
    x->direita = y;
    y->esquerda = T2;
    y->altura = maximo(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = maximo(altura(x->esquerda), altura(x->direita)) + 1;
    return x;
}

No* rotacionarEsquerda(No* x) {
    No* y = x->direita;
    No* T2 = y->esquerda;
    y->esquerda = x;
    x->direita = T2;
    x->altura = maximo(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = maximo(altura(y->esquerda), altura(y->direita)) + 1;
    return y;
}

int obterBalanceamento(No* no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

No* inserir(No* no, int chave) {
    if (no == NULL)
        return criarNo(chave);
    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave);
    else
        return no;
    no->altura = 1 + maximo(altura(no->esquerda), altura(no->direita));
    int balanceamento = obterBalanceamento(no);
    if (balanceamento > 1 && chave < no->esquerda->chave)
        return rotacionarDireita(no);
    if (balanceamento < -1 && chave > no->direita->chave)
        return rotacionarEsquerda(no);
    if (balanceamento > 1 && chave > no->esquerda->chave) {
        no->esquerda = rotacionarEsquerda(no->esquerda);
        return rotacionarDireita(no);
    }
    if (balanceamento < -1 && chave < no->direita->chave) {
        no->direita = rotacionarDireita(no->direita);
        return rotacionarEsquerda(no);
    }
    return no;
}

No* buscar(No* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;
    if (raiz->chave < chave)
        return buscar(raiz->direita, chave);
    return buscar(raiz->esquerda, chave);
}

void imprimirArvoreAux(No* raiz, int espaco) {
    if (raiz == NULL)
        return;
    espaco += 5;
    imprimirArvoreAux(raiz->direita, espaco);
    printf("\n");
    int i;
    for (i = 5; i < espaco; i++)
        printf(" ");
    printf("%d\n", raiz->chave);
    imprimirArvoreAux(raiz->esquerda, espaco);
}

void imprimirArvore(No* raiz) {
    imprimirArvoreAux(raiz, 0);
}

int main() {
    No* raiz = NULL;
    int opcao, valor;
    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Inserir\n");
        printf("2. Buscar\n");
        printf("3. Imprimir árvore\n");
        printf("4. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                break;
            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                if (buscar(raiz, valor) != NULL)
                    printf("Valor %d encontrado na árvore.\n", valor);
                else
                    printf("Valor %d não encontrado na árvore.\n", valor);
                break;
            case 3:
                printf("Estrutura da árvore:\n");
                imprimirArvore(raiz);
                break;
            case 4:
                exit(0);
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    return 0;
}
