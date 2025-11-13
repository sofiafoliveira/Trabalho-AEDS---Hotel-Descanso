#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

int main(void) {
    int opcao;
    Cliente novo;

    while (1) {
        printf("\n=== HOTEL DESCANSO GARANTIDO ===\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 0) break;
        else if (opcao == 1) {
            printf("Codigo: ");
            scanf("%d", &novo.codigo);
            getchar();

            printf("Nome: ");
            fgets(novo.nome, TAM_NOME, stdin);
            novo.nome[strcspn(novo.nome, "\n")] = 0;

            printf("Endereco: ");
            fgets(novo.endereco, TAM_ENDERECO, stdin);
            novo.endereco[strcspn(novo.endereco, "\n")] = 0;

            printf("Telefone: ");
            fgets(novo.telefone, TAM_TELEFONE, stdin);
            novo.telefone[strcspn(novo.telefone, "\n")] = 0;

            cadastrar_cliente(novo);
        }
        else if (opcao == 2) {
            listar_clientes();
        }
        else {
            printf("Opção invalida!\n");
        }
    }

    printf("Saindo...\n");
    return 0;
}
