#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10   // Capacidade máxima da mochila

// ---------------------------------------------------------------
// Struct Item: representa cada objeto do inventário
// Campos:
// - nome: nome do item (ex: Pistola, Bandagem, Chave Inglesa)
// - tipo: categoria do item (arma, munição, cura, ferramenta...)
// - quantidade: quantidade total do item
// ---------------------------------------------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// ---------------------------------------------------------------
// Função: inserirItem
// Objetivo: cadastrar um novo item na mochila, caso haja espaço.
// ---------------------------------------------------------------
void inserirItem(Item mochila[], int *total) {
    if (*total >= MAX_ITENS) {
        printf("\n❌ Mochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    printf("\n--- Cadastro de Item ---\n");
    printf("Nome do item: ");
    scanf(" %29[^\n]", mochila[*total].nome);

    printf("Tipo (arma, municao, cura, ferramenta, etc.): ");
    scanf(" %19[^\n]", mochila[*total].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[*total].quantidade);

    (*total)++;  // Incrementa a quantidade de itens na mochila
    printf("\n✅ Item cadastrado com sucesso!\n");
}

// ---------------------------------------------------------------
// Função: removerItem
// Objetivo: remover um item procurando pelo nome.
// ---------------------------------------------------------------
void removerItem(Item mochila[], int *total) {
    char nomeBusca[30];
    int i, j, encontrado = 0;

    printf("\n--- Remover Item ---\n");
    printf("Informe o nome do item a remover: ");
    scanf(" %29[^\n]", nomeBusca);

    // Busca pelo item
    for (i = 0; i < *total; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = 1;

            // Desloca os itens seguintes para a esquerda
            for (j = i; j < *total - 1; j++) {
                mochila[j] = mochila[j + 1];
            }

            (*total)--; // Reduz o total de itens
            printf("\n🗑️ Item removido com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("\n❌ Item não encontrado na mochila.\n");
    }
}

// ---------------------------------------------------------------
// Função: listarItens
// Objetivo: exibir todos os itens cadastrados com seus dados.
// ---------------------------------------------------------------
void listarItens(Item mochila[], int total) {
    printf("\n--- Itens na Mochila ---\n");

    if (total == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("\nItem %d:\n", i + 1);
        printf("Nome: %s\n", mochila[i].nome);
        printf("Tipo: %s\n", mochila[i].tipo);
        printf("Quantidade: %d\n", mochila[i].quantidade);
    }
}

// ---------------------------------------------------------------
// Função: buscarItem
// Objetivo: realizar busca sequencial pelo nome do item.
// ---------------------------------------------------------------
void buscarItem(Item mochila[], int total) {
    char nomeBusca[30];
    int i, encontrado = 0;

    printf("\n--- Buscar Item ---\n");
    printf("Nome do item: ");
    scanf(" %29[^\n]", nomeBusca);

    for (i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {

            printf("\n🔎 Item encontrado!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n❌ O item não está na mochila.\n");
    }
}

// ---------------------------------------------------------------
// Função principal (main)
// Gerencia o menu do usuário e controla o fluxo do sistema.
// ---------------------------------------------------------------
int main() {
    Item mochila[MAX_ITENS];   // Vetor que armazena até 10 itens
    int total = 0;             // Contador de itens cadastrados
    int opcao;

    do {
        printf("\n========== MENU DO INVENTÁRIO ==========\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("5 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &total);
                listarItens(mochila, total);
                break;
            case 2:
                removerItem(mochila, &total);
                listarItens(mochila, total);
                break;
            case 3:
                listarItens(mochila, total);
                break;
            case 4:
                buscarItem(mochila, total);
                break;
            case 5:
                printf("\nEncerrando o sistema...\n");
                break;
            default:
                printf("\n❌ Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}
