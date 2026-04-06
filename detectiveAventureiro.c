#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================
// ESTRUTURAS
// ==========================

// Estrutura da Sala (árvore da mansão)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;


// ==========================
// FUNÇÃO: criarSala
// Cria uma sala com nome e pista
// ==========================
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (nova == NULL) {
        printf("Erro de memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}


// ==========================
// FUNÇÃO: inserirPista (BST)
// ==========================
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    // Ordenação alfabética
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }

    return raiz;
}


// ==========================
// FUNÇÃO: exibirPistas (em ordem)
// ==========================
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}


// ==========================
// FUNÇÃO: explorarSalasComPistas
// ==========================
void explorarSalasComPistas(Sala *atual, PistaNode **pistasColetadas) {

    char opcao;

    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        // Se tiver pista, coleta automaticamente
        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("\nEscolha:\n");

        if (atual->esquerda != NULL)
            printf("e - esquerda\n");

        if (atual->direita != NULL)
            printf("d - direita\n");

        printf("s - sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (opcao == 's') {
            printf("\nExploração encerrada.\n");
            break;
        } 
        else {
            printf("Opção inválida!\n");
        }
    }
}


// ==========================
// MAIN
// ==========================
int main() {

    // ===== CRIANDO MANSÃO =====
    Sala *hall = criarSala("Hall de Entrada", "Pegadas sujas no tapete");

    hall->esquerda = criarSala("Sala de Estar", "Controle remoto quebrado");
    hall->direita = criarSala("Cozinha", "Faca fora do lugar");

    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro rasgado");
    hall->esquerda->direita = criarSala("Escritorio", NULL);

    hall->direita->esquerda = criarSala("Despensa", "Comida mexida");
    hall->direita->direita = criarSala("Jardim", "Pegadas na terra");

    // BST de pistas
    PistaNode *pistas = NULL;

    // ===== EXPLORAÇÃO =====
    explorarSalasComPistas(hall, &pistas);

    // ===== RESULTADO FINAL =====
    printf("\n=== PISTAS COLETADAS (ORDEM ALFABÉTICA) ===\n");
    exibirPistas(pistas);

    return 0;
}