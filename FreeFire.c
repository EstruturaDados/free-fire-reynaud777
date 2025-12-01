/*
 * mochila_comparativa.c
 *
 * Implementa duas versões de um sistema de inventário (mochila):
 *  - Versão com vetor (lista sequencial) — capacidade fixa (MAX_ITENS)
 *  - Versão com lista encadeada (estrutura dinâmica)
 *
 * Funcionalidades:
 *  - inserir, remover, listar, buscar (busca sequencial) em ambas
 *  - ordenar vetor (Bubble Sort)
 *  - buscar binária no vetor (após ordenação)
 *  - contadores de comparações para buscas (sequencial e binária)
 *  - medição de tempo de cada operação (com time.h)
 *
 * Requisitos adotados:
 *  - Usabilidade: menus claros e listagens após operações
 *  - Documentação: comentários explicativos em cada função
 *  - Nomes de funções/variáveis autoexplicativos
 *
 * Compile: gcc -O2 mochila_comparativa.c -o mochila
 * Execute: ./mochila
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

/* ----------------------------
   Definição das estruturas
   ---------------------------- */

/* Struct Item: representa cada objeto do inventário */
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

/* Nó para lista encadeada */
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

/* ----------------------------
   Contadores globais de comparações
   ---------------------------- */

/* Vetor */
int comparacoesSeqVetor = 0;
int comparacoesBinVetor = 0;
int comparacoesOrdenacaoVetor = 0;

/* Lista encadeada */
int comparacoesSeqLista = 0;

/* ----------------------------
   Funções utilitárias
   ---------------------------- */

/* Limpa o buffer de entrada até nova linha (evita problemas com fgets/scanf) */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Copia string com limite e remove \n caso exista (para fgets) */
void copiaStringLimite(char *dest, const char *src, int limite) {
    strncpy(dest, src, limite - 1);
    dest[limite - 1] = '\0';
    /* remover newline se presente */
    size_t len = strlen(dest);
    if (len > 0 && dest[len - 1] == '\n') dest[len - 1] = '\0';
}

/* Mostra um item (usado por vetor e lista) */
void mostrarItem(const Item *it) {
    printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
           it->nome, it->tipo, it->quantidade);
}

/* ----------------------------
   Implementação: Vetor (lista sequencial)
   ---------------------------- */

/* Insere item no vetor se houver espaço */
void inserirItemVetor(Item vetor[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n❌ Mochila (vetor) cheia! Remova um item antes de inserir.\n");
        return;
    }

    Item novo;
    printf("\n--- Inserir item (Vetor) ---\n");
    printf("Nome: ");
    scanf(" %29[^\n]", novo.nome);
    limparBuffer();

    printf("Tipo (ex: arma, municao, cura, ferramenta): ");
    scanf(" %19[^\n]", novo.tipo);
    limparBuffer();

    printf("Quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada inválida para quantidade.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    vetor[*total] = novo;
    (*total)++;
    printf("✅ Item inserido no vetor com sucesso.\n");
}

/* Remove item do vetor por nome; retorna 1 se removido, 0 se não encontrado */
int removerItemVetor(Item vetor[], int *total, const char *nomeRemover) {
    if (*total == 0) {
        printf("A mochila (vetor) está vazia.\n");
        return 0;
    }

    int i, j;
    for (i = 0; i < *total; i++) {
        if (strcmp(vetor[i].nome, nomeRemover) == 0) {
            /* deslocar itens à esquerda para preencher o buraco */
            for (j = i; j < *total - 1; j++) {
                vetor[j] = vetor[j + 1];
            }
            (*total)--;
            return 1;
        }
    }
    return 0;
}

/* Lista todos os itens do vetor */
void listarVetor(const Item vetor[], int total) {
    printf("\n--- Itens na Mochila (Vetor) ---\n");
    if (total == 0) {
        printf("Vetor vazio.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("%d) ", i + 1);
        mostrarItem(&vetor[i]);
    }
}

/* Busca sequencial no vetor: retorna índice ou -1; incrementa comparacoesSeqVetor */
int buscarSequencialVetor(const Item vetor[], int total, const char *nomeBusca) {
    comparacoesSeqVetor = 0;
    for (int i = 0; i < total; i++) {
        comparacoesSeqVetor++;
        if (strcmp(vetor[i].nome, nomeBusca) == 0) {
            return i;
        }
    }
    return -1;
}

/* Ordena o vetor por nome (Bubble Sort) e conta comparacoesOrdenacaoVetor */
void ordenarVetor(Item vetor[], int total) {
    comparacoesOrdenacaoVetor = 0;
    if (total <= 1) return;
    int trocou;
    for (int i = 0; i < total - 1; i++) {
        trocou = 0;
        for (int j = 0; j < total - 1 - i; j++) {
            comparacoesOrdenacaoVetor++;
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                Item tmp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/* Busca binária no vetor ordenado por nome; retorna índice ou -1; conta comparacoesBinVetor */
int buscarBinariaVetor(const Item vetor[], int total, const char *nomeBusca) {
    comparacoesBinVetor = 0;
    int esquerda = 0;
    int direita = total - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        comparacoesBinVetor++;
        int cmp = strcmp(vetor[meio].nome, nomeBusca);
        if (cmp == 0) return meio;
        else if (cmp < 0) esquerda = meio + 1;
        else direita = meio - 1;
    }
    return -1;
}

/* ----------------------------
   Implementação: Lista Encadeada
   ---------------------------- */

/* Insere no final da lista encadeada */
void inserirItemLista(No **cabeca) {
    Item novo;
    printf("\n--- Inserir item (Lista Encadeada) ---\n");
    printf("Nome: ");
    scanf(" %29[^\n]", novo.nome);
    limparBuffer();

    printf("Tipo (ex: arma, municao, cura, ferramenta): ");
    scanf(" %19[^\n]", novo.tipo);
    limparBuffer();

    printf("Quantidade: ");
    if (scanf("%d", &novo.quantidade) != 1) {
        printf("Entrada inválida para quantidade.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    No *novoNo = (No *) malloc(sizeof(No));
    if (!novoNo) {
        printf("Erro de alocação!\n");
        return;
    }
    novoNo->dados = novo;
    novoNo->proximo = NULL;

    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        No *p = *cabeca;
        while (p->proximo != NULL) p = p->proximo;
        p->proximo = novoNo;
    }
    printf("✅ Item inserido na lista encadeada com sucesso.\n");
}

/* Remove item da lista por nome; retorna 1 se removido, 0 se não encontrado */
int removerItemLista(No **cabeca, const char *nomeRemover) {
    if (*cabeca == NULL) {
        printf("A mochila (lista) está vazia.\n");
        return 0;
    }
    No *atual = *cabeca;
    No *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nomeRemover) == 0) {
            if (anterior == NULL) {
                /* remover o primeiro nó */
                *cabeca = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return 0;
}

/* Lista todos os itens da lista encadeada */
void listarLista(const No *cabeca) {
    printf("\n--- Itens na Mochila (Lista Encadeada) ---\n");
    if (cabeca == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    const No *p = cabeca;
    int idx = 1;
    while (p != NULL) {
        printf("%d) ", idx++);
        mostrarItem(&p->dados);
        p = p->proximo;
    }
}

/* Busca sequencial na lista encadeada por nome; retorna ponteiro para o nó ou NULL; conta comparacoesSeqLista */
No *buscarSequencialLista(No *cabeca, const char *nomeBusca) {
    comparacoesSeqLista = 0;
    No *p = cabeca;
    while (p != NULL) {
        comparacoesSeqLista++;
        if (strcmp(p->dados.nome, nomeBusca) == 0) {
            return p;
        }
        p = p->proximo;
    }
    return NULL;
}

/* Libera toda a memória da lista encadeada */
void liberarLista(No **cabeca) {
    No *p = *cabeca;
    while (p != NULL) {
        No *tmp = p;
        p = p->proximo;
        free(tmp);
    }
    *cabeca = NULL;
}

/* Conta elementos da lista (útil para comparações) */
int contarLista(const No *cabeca) {
    int c = 0;
    const No *p = cabeca;
    while (p != NULL) {
        c++;
        p = p->proximo;
    }
    return c;
}

/* ----------------------------
   Menus e fluxo principal
   ---------------------------- */

/* Submenu para operações do vetor */
void menuVetor(Item vetor[], int *total) {
    int opc;
    char nomeTemp[30];
    clock_t inicio, fim;
    double duracao;

    do {
        printf("\n==== MOCHILA (VETOR) ====\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("5 - Ordenar itens (Bubble Sort)\n");
        printf("6 - Buscar (binária) - exige vetor ordenado\n");
        printf("7 - Voltar ao menu principal\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBuffer(); opc = -1; }
        limparBuffer();

        switch (opc) {
            case 1:
                inicio = clock();
                inserirItemVetor(vetor, total);
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                listarVetor(vetor, *total);
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 2:
                printf("Nome do item a remover: ");
                scanf(" %29[^\n]", nomeTemp);
                limparBuffer();
                inicio = clock();
                if (removerItemVetor(vetor, total, nomeTemp))
                    printf("✅ Item removido do vetor.\n");
                else
                    printf("❌ Item não encontrado no vetor.\n");
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                listarVetor(vetor, *total);
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 3:
                inicio = clock();
                listarVetor(vetor, *total);
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 4:
                printf("Nome do item a buscar (sequencial): ");
                scanf(" %29[^\n]", nomeTemp);
                limparBuffer();
                inicio = clock();
                {
                    int idx = buscarSequencialVetor(vetor, *total, nomeTemp);
                    fim = clock();
                    duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                    if (idx != -1) {
                        printf("🔎 Item encontrado no vetor (índice %d):\n", idx);
                        mostrarItem(&vetor[idx]);
                    } else {
                        printf("🔍 Item NÃO encontrado no vetor.\n");
                    }
                    printf("Comparações (sequencial): %d | tempo: %.6f s\n",
                           comparacoesSeqVetor, duracao);
                }
                break;

            case 5:
                inicio = clock();
                ordenarVetor(vetor, *total);
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("✅ Vetor ordenado por nome.\n");
                printf("Comparações durante ordenação (bubble): %d | tempo: %.6f s\n",
                       comparacoesOrdenacaoVetor, duracao);
                listarVetor(vetor, *total);
                break;

            case 6:
                if (*total == 0) {
                    printf("Vetor vazio.\n");
                    break;
                }
                printf("Obs: a busca binária requer vetor ordenado. Deseja ordenar agora? (s/n): ");
                char opcaoOrdenar;
                scanf(" %c", &opcaoOrdenar);
                limparBuffer();
                if (opcaoOrdenar == 's' || opcaoOrdenar == 'S') {
                    ordenarVetor(vetor, *total);
                    printf("Vetor ordenado.\n");
                }
                printf("Nome do item a buscar (binária): ");
                scanf(" %29[^\n]", nomeTemp);
                limparBuffer();
                inicio = clock();
                {
                    int idx = buscarBinariaVetor(vetor, *total, nomeTemp);
                    fim = clock();
                    duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                    if (idx != -1) {
                        printf("🔎 Item encontrado no vetor (índice %d):\n", idx);
                        mostrarItem(&vetor[idx]);
                    } else {
                        printf("🔍 Item NÃO encontrado no vetor.\n");
                    }
                    printf("Comparações (binária): %d | tempo: %.6f s\n",
                           comparacoesBinVetor, duracao);
                }
                break;

            case 7:
                printf("Retornando ao menu principal...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opc != 7);
}

/* Submenu para operações da lista encadeada */
void menuLista(No **cabeca) {
    int opc;
    char nomeTemp[30];
    clock_t inicio, fim;
    double duracao;

    do {
        printf("\n==== MOCHILA (LISTA ENCADEADA) ====\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item (por nome)\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("5 - Voltar ao menu principal\n");
        printf("Escolha: ");
        if (scanf("%d", &opc) != 1) { limparBuffer(); opc = -1; }
        limparBuffer();

        switch (opc) {
            case 1:
                inicio = clock();
                inserirItemLista(cabeca);
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                listarLista(*cabeca);
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 2:
                printf("Nome do item a remover: ");
                scanf(" %29[^\n]", nomeTemp);
                limparBuffer();
                inicio = clock();
                if (removerItemLista(cabeca, nomeTemp))
                    printf("✅ Item removido da lista.\n");
                else
                    printf("❌ Item não encontrado na lista.\n");
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                listarLista(*cabeca);
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 3:
                inicio = clock();
                listarLista(*cabeca);
                fim = clock();
                duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                printf("(tempo: %.6f s)\n", duracao);
                break;

            case 4:
                printf("Nome do item a buscar (sequencial): ");
                scanf(" %29[^\n]", nomeTemp);
                limparBuffer();
                inicio = clock();
                {
                    No *res = buscarSequencialLista(*cabeca, nomeTemp);
                    fim = clock();
                    duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;
                    if (res != NULL) {
                        printf("🔎 Item encontrado na lista:\n");
                        mostrarItem(&res->dados);
                    } else {
                        printf("🔍 Item NÃO encontrado na lista.\n");
                    }
                    printf("Comparações (sequencial – lista): %d | tempo: %.6f s\n",
                           comparacoesSeqLista, duracao);
                }
                break;

            case 5:
                printf("Retornando ao menu principal...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opc != 5);
}

/* Menu principal que permite escolher a implementação */
int main() {
    Item vetor[MAX_ITENS];
    int totalVetor = 0;
    No *cabeca = NULL;

    int opcao;
    printf("=== SISTEMA DE MOCHILA (Comparação: Vetor vs Lista) ===\n");
    do {
        printf("\nMenu Principal:\n");
        printf("1 - Usar mochila (Vetor)\n");
        printf("2 - Usar mochila (Lista Encadeada)\n");
        printf("3 - Comparar estado atual (contagens e tamanhos)\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limparBuffer(); opcao = -1; }
        limparBuffer();

        switch (opcao) {
            case 1:
                menuVetor(vetor, &totalVetor);
                break;

            case 2:
                menuLista(&cabeca);
                break;

            case 3:
                printf("\n--- Comparação rápida ---\n");
                printf("Vetor: %d itens\n", totalVetor);
                printf("Lista: %d itens\n", contarLista(cabeca));
                printf("Últimas contagens de comparações:\n");
                printf("  Busca sequencial (vetor): %d\n", comparacoesSeqVetor);
                printf("  Busca binária (vetor): %d\n", comparacoesBinVetor);
                printf("  Ordenação (comparações bubble): %d\n", comparacoesOrdenacaoVetor);
                printf("  Busca sequencial (lista): %d\n", comparacoesSeqLista);
                break;

            case 4:
                printf("Encerrando o sistema. Liberando memória e saindo...\n");
                liberarLista(&cabeca);
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 4);

    return 0;
}
