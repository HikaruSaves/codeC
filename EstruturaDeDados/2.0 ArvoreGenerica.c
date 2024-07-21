#include <stdio.h>
#include <stdlib.h>

#define MAX_FILHOS 3
#define TAM_PILHA_FILA 100

struct no {
    char letra;
    struct no* filhos[MAX_FILHOS];
    struct no* prox;
};

struct no* raiz = NULL;

struct no* nos[100];
int indice_no = 0;

struct no* criar_no(char letra) {
    struct no* novo = (struct no*)malloc(sizeof(struct no));
    novo->letra = letra;
    int i;
    for (i = 0; i < MAX_FILHOS; i++) {
        novo->filhos[i] = NULL;
    }
    novo->prox = NULL;
    return novo;
}

void inserir_no(char letra) {
    struct no* novo = criar_no(letra);
    nos[indice_no++] = novo;
    if (raiz == NULL) {
        raiz = novo;
    } else {
        struct no* temp = raiz;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novo;
    }
}

void adicionar_filho(struct no* pai, struct no* filho, int pos) {
    if (pai != NULL && pos >= 0 && pos < MAX_FILHOS) {
        pai->filhos[pos] = filho;
    }
}

void remover_no(struct no* pai, struct no* remover) {
    if (remover == NULL) return;
    int i;
    for (i = 0; i < MAX_FILHOS; i++) {
        if (remover->filhos[i] != NULL) {
            remover_no(remover, remover->filhos[i]);
            remover->filhos[i] = NULL;
        }
    }

    if (pai != NULL) {
        for (i = 0; i < MAX_FILHOS; i++) {
            if (pai->filhos[i] == remover) {
                pai->filhos[i] = NULL;
            }
        }
    }

    if (raiz == remover) {
        raiz = remover->prox;
    } else {
        struct no* temp = raiz;
        while (temp->prox != NULL && temp->prox != remover) {
            temp = temp->prox;
        }
        if (temp->prox == remover) {
            temp->prox = remover->prox;
        }
    }

    free(remover);
}

void imprimir_arvore(struct no* atual) {
    if (atual == NULL) return;

    printf("Pai: %c\n", atual->letra);
    int i;
    for (i = 0; i < MAX_FILHOS; i++) {
        if (atual->filhos[i] != NULL) {
            printf("  Filho %d: %c\n", i, atual->filhos[i]->letra);
        } else {
            printf("  Filho %d: -\n", i);
        }
    }

    if (atual->prox != NULL) {
        imprimir_arvore(atual->prox);
    }
}

void busca_em_profundidade(struct no* raiz) {
    if (raiz == NULL) return;

    struct no* pilha[TAM_PILHA_FILA];
    int topo = -1;

    pilha[++topo] = raiz;

    while (topo != -1) {
        struct no* atual = pilha[topo--];
        if (atual != NULL) {
            printf("%c ", atual->letra);
            int i;
            for (i = MAX_FILHOS - 1; i >= 0; i--) {
                if (atual->filhos[i] != NULL) {
                    pilha[++topo] = atual->filhos[i];
                }
            }
        }
    }
}

void busca_em_largura(struct no* raiz) {
    if (raiz == NULL) return;

    struct no* fila[TAM_PILHA_FILA];
    int inicio = 0, fim = 0;

    fila[fim++] = raiz;

    while (inicio < fim) {
        struct no* atual = fila[inicio++];
        if (atual != NULL) {
            printf("%c ", atual->letra);
            int i;
            for (i = 0; i < MAX_FILHOS; i++) {
                if (atual->filhos[i] != NULL) {
                    fila[fim++] = atual->filhos[i];
                }
            }
        }
    }
}

int main() {
    int opcao, pos;
    char letra, pai_letra;
    int i;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Inserir no\n");
        printf("2. Imprimir arvore\n");
        printf("3. Remover no\n");
        printf("4. Busca em profundidade\n");
        printf("5. Busca em largura\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        struct no* pai;
        struct no* remover;
        struct no* raiz_no;

        switch (opcao) {
            case 1:
                printf("Digite o novo no (letra): ");
                scanf(" %c", &letra);
                inserir_no(letra);
                if (indice_no > 1) {
                    printf("Este no e filho de qual no (letra)? ");
                    scanf(" %c", &pai_letra);
                    printf("Qual a posicao deste filho (0, 1 ou 2)? ");
                    scanf("%d", &pos);
                    pai = raiz;
                    while (pai != NULL && pai->letra != pai_letra) {
                        pai = pai->prox;
                    }
                    if (pai != NULL) {
                        adicionar_filho(pai, nos[indice_no - 1], pos);
                    }
                }
                break;

            case 2:
                imprimir_arvore(raiz);
                break;

            case 3:
                printf("Digite o no a ser removido (letra): ");
                scanf(" %c", &letra);
                pai = NULL;
                remover = raiz;
                while (remover != NULL && remover->letra != letra) {
                    pai = remover;
                    remover = remover->prox;
                }
                if (remover != NULL) {
                    remover_no(pai, remover);
                }
                break;

            case 4:
                printf("Digite a letra do no raiz para iniciar a busca: ");
                scanf(" %c", &letra);
                raiz_no = raiz;
                while (raiz_no != NULL && raiz_no->letra != letra) {
                    raiz_no = raiz_no->prox;
                }
                if (raiz_no != NULL) {
                    printf("Resultado da busca em profundidade: ");
                    busca_em_profundidade(raiz_no);
                    printf("\n");
                }
                break;

            case 5:
                printf("Digite a letra do no raiz para iniciar a busca: ");
                scanf(" %c", &letra);
                raiz_no = raiz;
                while (raiz_no != NULL && raiz_no->letra != letra) {
                    raiz_no = raiz_no->prox;
                }
                if (raiz_no != NULL) {
                    printf("Resultado da busca em largura: ");
                    busca_em_largura(raiz_no);
                    printf("\n");
                }
                break;

            case 6:
                exit(0);

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }

    return 0;
}
