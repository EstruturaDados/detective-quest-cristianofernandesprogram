#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10

// ==========================
// ESTRUTURAS
// ==========================

// Árvore da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

// Hash (encadeamento)
typedef struct NodoHash {
    char pista[100];
    char suspeito[50];
    struct NodoHash *prox;
} NodoHash;

NodoHash* tabela[TAM];


// ==========================
// HASH
// ==========================

// Função hash simples
int funcaoHash(char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM;
}

// Inserir na hash (pista -> suspeito)
void inserirNaHash(char *pista, char *suspeito) {
    int indice = funcaoHash(pista);

    NodoHash *novo = (NodoHash*) malloc(sizeof(NodoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

// Buscar suspeito pela pista
char* encontrarSuspeito(char *pista) {
    int indice = funcaoHash(pista);

    NodoHash *aux = tabela[indice];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }

    return NULL;
}


// ==========================
// SALAS
// ==========================

Sala* criarSala(char *nome, char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");

    nova->esq = NULL;
    nova->dir = NULL;

    return nova;
}


// ==========================
// BST DE PISTAS
// ==========================

PistaNode* inserirPista(PistaNode *raiz, char *pista) {

    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

// Exibir pistas ordenadas
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}


// ==========================
// EXPLORAÇÃO
// ==========================

void explorarSalas(Sala *atual, PistaNode **pistasColetadas) {
    char op;

    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("\nEscolha:\n");
        if (atual->esq) printf("e - esquerda\n");
        if (atual->dir) printf("d - direita\n");
        printf("s - sair\n");

        scanf(" %c", &op);

        if (op == 'e' && atual->esq)
            atual = atual->esq;
        else if (op == 'd' && atual->dir)
            atual = atual->dir;
        else if (op == 's')
            break;
        else
            printf("Opção inválida!\n");
    }
}


// ==========================
// CONTAGEM DE PISTAS POR SUSPEITO
// ==========================

int contarPistas(PistaNode *raiz, char *suspeito) {
    if (raiz == NULL)
        return 0;

    int count = 0;

    char *resp = encontrarSuspeito(raiz->pista);

    if (resp != NULL && strcmp(resp, suspeito) == 0)
        count = 1;

    return count + contarPistas(raiz->esq, suspeito)
                 + contarPistas(raiz->dir, suspeito);
}


// ==========================
// JULGAMENTO FINAL
// ==========================

void verificarSuspeitoFinal(PistaNode *pistas) {
    char suspeito[50];

    printf("\nDigite o nome do suspeito: ");
    scanf(" %[^\n]", suspeito);

    int total = contarPistas(pistas, suspeito);

    printf("\nPistas contra %s: %d\n", suspeito, total);

    if (total >= 2)
        printf("✅ Evidências suficientes! %s é o culpado!\n", suspeito);
    else
        printf("❌ Evidências insuficientes! %s pode ser inocente.\n", suspeito);
}


// ==========================
// MAIN
// ==========================

int main() {

    // Inicializar hash
    for (int i = 0; i < TAM; i++)
        tabela[i] = NULL;

    // ===== MAPA =====
    Sala *hall = criarSala("Hall", "Pegadas");

    hall->esq = criarSala("Sala de Estar", "Controle quebrado");
    hall->dir = criarSala("Cozinha", "Faca sumida");

    hall->esq->esq = criarSala("Biblioteca", "Livro rasgado");
    hall->esq->dir = criarSala("Escritorio", "Documento suspeito");

    hall->dir->esq = criarSala("Despensa", "Comida mexida");
    hall->dir->dir = criarSala("Jardim", "Pegadas na terra");

    // ===== HASH (pista -> suspeito) =====
    inserirNaHash("Pegadas", "Joao");
    inserirNaHash("Controle quebrado", "Maria");
    inserirNaHash("Faca sumida", "Carlos");
    inserirNaHash("Livro rasgado", "Maria");
    inserirNaHash("Documento suspeito", "Joao");
    inserirNaHash("Comida mexida", "Carlos");
    inserirNaHash("Pegadas na terra", "Joao");

    // ===== BST =====
    PistaNode *pistas = NULL;

    // ===== EXPLORAÇÃO =====
    explorarSalas(hall, &pistas);

    // ===== RESULTADO =====
    printf("\n=== PISTAS COLETADAS ===\n");
    exibirPistas(pistas);

    // ===== JULGAMENTO =====
    verificarSuspeitoFinal(pistas);

    return 0;
}