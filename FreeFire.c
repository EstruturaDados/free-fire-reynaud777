/*
 * torre_compartilhamento.c
 *
 * Sistema para priorização e montagem de componentes da "Torre de Fuga".
 * - Permite cadastrar até 20 componentes (nome, tipo, prioridade).
 * - Ordena usando diferentes algoritmos:
 *     * Bubble Sort por nome (string)
 *     * Insertion Sort por tipo (string)
 *     * Selection Sort por prioridade (int)
 * - Conta comparações e mede tempo de execução (clock()) para cada algoritmo.
 * - Permite busca binária por nome (APENAS se o vetor estiver ordenado por nome).
 * - Interface de menu interativa com feedback numérico (comparações e tempo).
 *
 * Compile: gcc -O2 torre_compartilhamento.c -o torre
 * Execute: ./torre
 *
 * Autor: (implementação fornecida pelo assistente)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

/* -------------------------
   Struct Componente
   ------------------------- */
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade; /* 1..10 */
} Componente;

/* -------------------------
   Utilitários de entrada
   ------------------------- */

/* Lê uma linha com fgets e remove '\n' final */
void leStringSeguro(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        /* Em caso de EOF, garante string vazia */
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') buffer[len - 1] = '\0';
}

/* Limpa o restante do buffer de entrada (quando usamos scanf antes) */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Mostra um componente formatado */
void mostrarComponente(const Componente *c, int idx) {
    printf("%2d) Nome: %-28s | Tipo: %-12s | Prioridade: %2d\n",
           idx + 1, c->nome, c->tipo, c->prioridade);
}

/* Mostra vetor inteiro */
void mostrarComponentes(const Componente vet[], int n) {
    printf("\n--- Componentes (%d) ---\n", n);
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    for (int i = 0; i < n; ++i) {
        mostrarComponente(&vet[i], i);
    }
}

/* -------------------------
   Ordenações e contadores
   ------------------------- */

/*
 * Cada função de ordenação recebe:
 *   - vet: vetor de Componente
 *   - n:  número de elementos
 *   - comparacoes: ponteiro para int onde será armazenada a contagem
 *
 * A função apenas ordena o vetor in-place e escreve o número de comparações.
 */

/* Bubble Sort por nome (string) */
void bubbleSortNome(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; ++i) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++;
            if (strcmp(vet[j].nome, vet[j + 1].nome) > 0) {
                Componente tmp = vet[j];
                vet[j] = vet[j + 1];
                vet[j + 1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/* Insertion Sort por tipo (string) */
void insertionSortTipo(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = vet[i];
        int j = i - 1;
        /* desloca enquanto tipo de j > tipo da chave */
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(vet[j].tipo, chave.tipo) > 0) {
                vet[j + 1] = vet[j];
                j--;
            } else {
                break;
            }
        }
        vet[j + 1] = chave;
    }
}

/* Selection Sort por prioridade (int) - ordem decrescente (prioridade maior primeiro)
   ou ascendente? Aqui escolhemos ordem decrescente (prioridade 10 -> 1) para montagem prioritária. */
void selectionSortPrioridade(Componente vet[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int idxMax = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++;
            if (vet[j].prioridade > vet[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            Componente tmp = vet[i];
            vet[i] = vet[idxMax];
            vet[idxMax] = tmp;
        }
    }
}

/* -------------------------
   Busca binária por nome (após ordenação por nome)
   - Conta comparações no parâmetro comparacoes
   - Retorna índice do componente encontrado ou -1 se não encontrado
   ------------------------- */
int buscaBinariaPorNome(const Componente vet[], int n, const char *nomeBusca, long *comparacoes) {
    *comparacoes = 0;
    int esquerda = 0;
    int direita = n - 1;
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        (*comparacoes)++;
        int cmp = strcmp(vet[meio].nome, nomeBusca);
        if (cmp == 0) return meio;
        if (cmp < 0) esquerda = meio + 1;
        else direita = meio - 1;
    }
    return -1;
}

/* -------------------------
   Função para medir tempo de execução de um algoritmo de ordenação
   Recebe um ponteiro para função do tipo: void func(Componente[], int, long*)
   Retorna duração em segundos (double) e também fornece comparações via outComparacoes.
   ------------------------- */
double medirTempoOrdenacao(void (*algoritmo)(Componente[], int, long*),
                           Componente vet[], int n, long *outComparacoes) {
    clock_t inicio = clock();
    algoritmo(vet, n, outComparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* -------------------------
   Função para clonar vetor (utilizada para preservar original ao comparar algoritmos)
   ------------------------- */
void clonarVetor(const Componente src[], Componente dest[], int n) {
    for (int i = 0; i < n; ++i) dest[i] = src[i];
}

/* -------------------------
   Menu principal e fluxo
   ------------------------- */
int main() {
    Componente componentes[MAX_COMPONENTES];
    int total = 0;
    int opcao;

    /* Estado: indica se o vetor está atualmente ordenado por nome */
    int ordenadoPorNome = 0;

    printf("=== Torre de Fuga: Priorizador de Componentes ===\n");

    do {
        printf("\nMenu Principal:\n");
        printf("1 - Cadastrar componente (atual: %d/%d)\n", total, MAX_COMPONENTES);
        printf("2 - Listar componentes\n");
        printf("3 - Escolher algoritmo de ordenação e ordenar\n");
        printf("4 - Buscar componente-chave por nome (busca binária - exige ordenação por nome)\n");
        printf("5 - Resetar ordenação (marcar como não ordenado)\n");
        printf("6 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 1) {
            if (total >= MAX_COMPONENTES) {
                printf("Limite de componentes atingido (%d).\n", MAX_COMPONENTES);
                continue;
            }
            Componente c;
            printf("Nome do componente: ");
            leStringSeguro(c.nome, TAM_NOME);
            printf("Tipo (ex: controle, suporte, propulsao): ");
            leStringSeguro(c.tipo, TAM_TIPO);
            printf("Prioridade (1 a 10): ");
            if (scanf("%d", &c.prioridade) != 1) {
                printf("Entrada inválida de prioridade.\n");
                limparBuffer();
                continue;
            }
            limparBuffer();
            if (c.prioridade < 1) c.prioridade = 1;
            if (c.prioridade > 10) c.prioridade = 10;
            componentes[total++] = c;
            ordenadoPorNome = 0;
            printf("Componente cadastrado com sucesso.\n");
        }
        else if (opcao == 2) {
            mostrarComponentes(componentes, total);
        }
        else if (opcao == 3) {
            if (total == 0) {
                printf("Nenhum componente para ordenar.\n");
                continue;
            }

            /* Copiamos vetor caso queiramos comparar tempos sem alterar o original */
            Componente copia[MAX_COMPONENTES];
            clonarVetor(componentes, copia, total);

            printf("\nEscolha o algoritmo de ordenação:\n");
            printf("1 - Bubble Sort por NOME (string)\n");
            printf("2 - Insertion Sort por TIPO (string)\n");
            printf("3 - Selection Sort por PRIORIDADE (int) - decrescente\n");
            printf("4 - Comparar os 3 (executa cada um em cópia)\n");
            printf("Escolha: ");
            int escolhaAlg;
            if (scanf("%d", &escolhaAlg) != 1) { limparBuffer(); printf("Entrada inválida.\n"); continue; }
            limparBuffer();

            if (escolhaAlg == 4) {
                /* Executa e mostra comparações/tempos para cada algoritmo em cópias */
                Componente aux[MAX_COMPONENTES];
                long comps;
                double tempo;

                printf("\n--- Comparação dos algoritmos em cópias do vetor atual ---\n");

                /* Bubble por nome */
                clonarVetor(componentes, aux, total);
                tempo = medirTempoOrdenacao(bubbleSortNome, aux, total, &comps);
                printf("\nBubble Sort por NOME: comparações=%ld | tempo=%.6f s\n", comps, tempo);
                mostrarComponentes(aux, total);

                /* Insertion por tipo */
                clonarVetor(componentes, aux, total);
                tempo = medirTempoOrdenacao(insertionSortTipo, aux, total, &comps);
                printf("\nInsertion Sort por TIPO: comparações=%ld | tempo=%.6f s\n", comps, tempo);
                mostrarComponentes(aux, total);

                /* Selection por prioridade */
                clonarVetor(componentes, aux, total);
                tempo = medirTempoOrdenacao(selectionSortPrioridade, aux, total, &comps);
                printf("\nSelection Sort por PRIORIDADE: comparações=%ld | tempo=%.6f s\n", comps, tempo);
                mostrarComponentes(aux, total);

                printf("\nObservação: as cópias preservam o vetor original para você escolher qual aplicar.\n");
            }
            else {
                long comparacoes = 0;
                double duracao = 0.0;

                switch (escolhaAlg) {
                    case 1:
                        duracao = medirTempoOrdenacao(bubbleSortNome, componentes, total, &comparacoes);
                        ordenadoPorNome = 1;
                        printf("\nResultado: Bubble Sort por NOME finalizado.\n");
                        printf("Comparações: %ld | Tempo: %.6f s\n", comparacoes, duracao);
                        mostrarComponentes(componentes, total);
                        break;

                    case 2:
                        duracao = medirTempoOrdenacao(insertionSortTipo, componentes, total, &comparacoes);
                        ordenadoPorNome = 0;
                        printf("\nResultado: Insertion Sort por TIPO finalizado.\n");
                        printf("Comparações: %ld | Tempo: %.6f s\n", comparacoes, duracao);
                        mostrarComponentes(componentes, total);
                        break;

                    case 3:
                        duracao = medirTempoOrdenacao(selectionSortPrioridade, componentes, total, &comparacoes);
                        ordenadoPorNome = 0;
                        printf("\nResultado: Selection Sort por PRIORIDADE finalizado.\n");
                        printf("Comparações: %ld | Tempo: %.6f s\n", comparacoes, duracao);
                        mostrarComponentes(componentes, total);
                        break;

                    default:
                        printf("Opção inválida de algoritmo.\n");
                }
            }
        }
        else if (opcao == 4) {
            if (total == 0) {
                printf("Nenhum componente cadastrado.\n");
                continue;
            }
            if (!ordenadoPorNome) {
                printf("Vetor não está ordenado por NOME. Deseja ordenar por NOME agora? (s/n): ");
                char r;
                if (scanf(" %c", &r) != 1) { limparBuffer(); continue; }
                limparBuffer();
                if (r == 's' || r == 'S') {
                    long comps;
                    double t = medirTempoOrdenacao(bubbleSortNome, componentes, total, &comps);
                    ordenadoPorNome = 1;
                    printf("Vetor ordenado por nome. Comparações=%ld | tempo=%.6f s\n", comps, t);
                    mostrarComponentes(componentes, total);
                } else {
                    printf("Busca-binária requer ordenação por nome. Operação cancelada.\n");
                    continue;
                }
            }

            char chave[TAM_NOME];
            printf("Digite o NOME do componente-chave a buscar: ");
            leStringSeguro(chave, TAM_NOME);
            long compsBusca;
            clock_t inicio = clock();
            int idx = buscaBinariaPorNome(componentes, total, chave, &compsBusca);
            clock_t fim = clock();
            double duracao = (double)(fim - inicio) / CLOCKS_PER_SEC;

            if (idx != -1) {
                printf("\n🔎 Componente-chave encontrado no índice %d:\n", idx);
                mostrarComponente(&componentes[idx], idx);
                printf("Comparações (busca binária): %ld | tempo: %.6f s\n", compsBusca, duracao);
                printf("✅ Torre pode iniciar montagem com esse componente.\n");
            } else {
                printf("\n🔍 Componente NÃO encontrado.\n");
                printf("Comparações (busca binária): %ld | tempo: %.6f s\n", compsBusca, duracao);
                printf("❌ Falha em localizar componente-chave; montagem não pode iniciar.\n");
            }
        }
        else if (opcao == 5) {
            ordenadoPorNome = 0;
            printf("Ordenação resetada — vetor marcado como não ordenado por nome.\n");
        }
        else if (opcao == 6) {
            printf("Saindo... lembrando de encerrar o programa.\n");
        }
        else {
            printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 6);

    return 0;
}
