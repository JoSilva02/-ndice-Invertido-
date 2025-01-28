#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 1000
#define MAX_PALAVRAS 1000

// Estrutura de um nó da lista encadeada
typedef struct No {
    int linha;
    int frequencia;
    struct No *prox;
} No;

// Estrutura de armazenamento da lista encadeada
typedef struct Encadeada {
    No *head;
} Encadeada;

// Estrutura do índice invertido
typedef struct InInvertido {
    char palavra[MAX_PALAVRAS];
    Encadeada *lista;
    struct InInvertido *prox;
} InInvertido;

// Função para criar um novo nó
No* criarNo(int linha) {
    No *novo = (No*)malloc(sizeof(No));
    novo->linha = linha;
    novo->frequencia = 1;
    novo->prox = NULL;
    return novo;
}

// Função para criar uma nova lista
Encadeada* criarLista() {
    Encadeada *nova = (Encadeada*)malloc(sizeof(Encadeada));
    nova->head = NULL;
    return nova;
}

// Função para criar uma nova entrada no índice invertido
InInvertido* criarEntradaIn(const char *palavra) {
    InInvertido *nova = (InInvertido *)malloc(sizeof(InInvertido));
    strcpy(nova->palavra, palavra);
    nova->lista = criarLista();
    nova->prox = NULL;
    return nova;
}

// Função para inserir um nó na lista encadeada
void inserirNaLista(Encadeada *lista, int linha) {
    No *atual = lista->head;

    // Verificar se a linha já existe
    while (atual != NULL) {
        if (atual->linha == linha) {
            atual->frequencia++;
            return;
        }
        atual = atual->prox;
    }

    // Caso não exista, criar um novo nó
    No *novo = criarNo(linha);
    novo->prox = lista->head;
    lista->head = novo;
}

// Função para encontrar ou criar uma entrada no índice
InInvertido* encontrarOuCriar(InInvertido **indice, const char *palavra) {
    InInvertido *atual = *indice;
    InInvertido *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            return atual;
        }
        anterior = atual;
        atual = atual->prox;
    }

    // Caso não encontre, criar uma nova entrada
    InInvertido *novaEntrada = criarEntradaIn(palavra);
    if (anterior == NULL) {
        *indice = novaEntrada;
    } else {
        anterior->prox = novaEntrada;
    }
    return novaEntrada;
}

// Função para processar uma linha e atualizar o índice
void processarLinha(InInvertido **indice, const char *linhaTexto, int numeroLinha) {
    char copiaLinha[MAX_LINHA];
    strcpy(copiaLinha, linhaTexto);

    char *palavra = strtok(copiaLinha, " \t\n");
    while (palavra != NULL) {
        // Converter a palavra para minúsculas
        for (int i = 0; palavra[i]; i++) {
            palavra[i] = tolower(palavra[i]);
        }

        // Inserir no índice
        InInvertido *entrada = encontrarOuCriar(indice, palavra);
        inserirNaLista(entrada->lista, numeroLinha);

        palavra = strtok(NULL, " \t\n");
    }
}

// Função para exibir o índice invertido
void exibirIndice(InInvertido *indice) {
    while (indice != NULL) {
        printf("%s: ", indice->palavra);
        No *atual = indice->lista->head;
        while (atual != NULL) {
            printf("(%d, %d)", atual->linha, atual->frequencia);
            if (atual->prox != NULL) {
                printf(", ");
            }
            atual = atual->prox;
        }
        printf("\n");
        indice = indice->prox;
    }
}

// Função principal
int main() {
    char nomeArquivo[MAX_LINHA];
    printf("Digite o nome do arquivo .txt: ");
    scanf("%s", nomeArquivo);

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    InInvertido *indice = NULL;
    char linhaTexto[MAX_LINHA];
    int numeroLinha = 0;

    while (fgets(linhaTexto, MAX_LINHA, arquivo)) {
        numeroLinha++;
        processarLinha(&indice, linhaTexto, numeroLinha);
    }
    fclose(arquivo);

    printf("\nÍndice Invertido:\n");
    exibirIndice(indice);

    return 0;
}
