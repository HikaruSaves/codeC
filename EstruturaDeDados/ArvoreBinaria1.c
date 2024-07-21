#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int dado;
    struct No* esquerda;
    struct No* direita;
} No;

No* raiz = NULL;

No* criarNo(int dado) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->dado = dado;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

No* inserir(No* raiz, int dado) {
    if (raiz == NULL) {
        return criarNo(dado);
    }
    if (dado <= raiz->dado) {
        raiz->esquerda = inserir(raiz->esquerda, dado);
    } else {
        raiz->direita = inserir(raiz->direita, dado);
    }
    return raiz;
}

No* buscar(No* raiz, int dado, int indice, int *altura) {
    if (raiz == NULL || raiz->dado == dado) {
        return raiz;
    }
    *altura += 1;
    if (dado < raiz->dado) {
        printf("Indice: %d, Valor: %d, Altura: %d (Esquerda)\n", indice, raiz->dado, *altura);
        return buscar(raiz->esquerda, dado, indice * 2 + 1, altura);
    } else {
        printf("Indice: %d, Valor: %d, Altura: %d (Direita)\n", indice, raiz->dado, *altura);
        return buscar(raiz->direita, dado, indice * 2 + 2, altura);
    }
}

No* encontrarMinimo(No* raiz) {
    while (raiz->esquerda != NULL) raiz = raiz->esquerda;
    return raiz;
}

No* remover(No* raiz, int dado) {
    if (raiz == NULL) return raiz;
    if (dado < raiz->dado) {
        raiz->esquerda = remover(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = remover(raiz->direita, dado);
    } else {
        if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        No* temp = encontrarMinimo(raiz->direita);
        raiz->dado = temp->dado;
        raiz->direita = remover(raiz->direita, temp->dado);
    }
    return raiz;
}

void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->dado);
        emOrdem(raiz->direita);
    }
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
    printf("%d\n", raiz->dado);
    imprimirArvoreAux(raiz->esquerda, espaco);
}

void imprimirArvore(No* raiz) {
    imprimirArvoreAux(raiz, 0);
}

int main() {
    int opcao, valor;
    while (1) {
        printf("\nEscolha uma opção:\n");
        printf("1. Inserir\n");
        printf("2. Remover\n");
        printf("3. Buscar\n");
        printf("4. Exibir itens da árvore\n");
        printf("5. Imprimir estrutura da árvore\n");
        printf("6. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                if (raiz == NULL) {
                    raiz = criarNo(valor);
                } else {
                    raiz = inserir(raiz, valor);
                }
                break;
            case 2:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                raiz = remover(raiz, valor);
                break;
            case 3:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                int altura = 0;
                No* resultado = buscar(raiz, valor, 0, &altura);
                if (resultado != NULL) {
                    printf("Valor %d encontrado na árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
            case 4:
                printf("Árvore binária em ordem: ");
                emOrdem(raiz);
                printf("\n");
                break;
            case 5:
                printf("Estrutura da árvore:\n");
                imprimirArvore(raiz);
                break;
            case 6:
                exit(0);
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    }
    return 0;
}
