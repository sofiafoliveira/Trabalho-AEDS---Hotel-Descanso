#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"
#include "quarto.h"
#include "funcionario.h"   
#include "estadia.h"      

void menuClientes() {
    int op;
    Cliente novo;

    while (1) {
        printf("\n=== MENU CLIENTES ===\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("3 - Editar cliente\n");
        printf("4 - Remover cliente\n");
        printf("0 - Voltar\n> ");
        scanf("%d", &op);
        getchar();

        if (op == 0) return;

        else if (op == 1) {
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
        else if (op == 2) listar_clientes();
        else if (op == 3) {
            int codigo;
            printf("Codigo do cliente para editar: ");
            scanf("%d", &codigo);
            getchar();
            editar_cliente(codigo);
        }
        else if (op == 4) {
            int codigo;
            printf("Codigo do cliente para remover: ");
            scanf("%d", &codigo);
            getchar();
            remover_cliente(codigo);
        }
        else printf("Opcao invalida!\n");
    }
}

void menuQuartos() {
    int op;

    while (1) {
        printf("\n=== MENU QUARTOS ===\n");
        printf("1 - Cadastrar quarto\n");
        printf("2 - Listar quartos\n");
        printf("3 - Editar quarto\n");
        printf("4 - Remover quarto\n");
        printf("0 - Voltar\n> ");
        scanf("%d", &op);
        getchar();

        if (op == 0) return;

        else if (op == 1) cadastrarQuarto();
        else if (op == 2) listarQuartos();
        else if (op == 3) {
            int numero;
            printf("Numero do quarto para editar: ");
            scanf("%d", &numero);
            getchar();
            editarQuarto(numero);
        }
        else if (op == 4) {
            int numero;
            printf("Numero do quarto para remover: ");
            scanf("%d", &numero);
            getchar();
            removerQuarto(numero);
        }
        else printf("Opcao invalida!\n");
    }
}

void menuFuncionarios() {
    int op;
    Funcionario f;

    while (1) {
        printf("\n=== MENU FUNCIONARIOS ===\n");
        printf("1 - Cadastrar funcionario\n");
        printf("2 - Listar funcionarios\n");
        printf("3 - Editar funcionario\n");
        printf("4 - Remover funcionario\n");
        printf("0 - Voltar\n> ");
        scanf("%d", &op);
        getchar();

        if (op == 0) return;

        else if (op == 1) {
            printf("ID: ");
            scanf("%d", &f.id);
            getchar();

            printf("Nome: ");
            fgets(f.nome, TAM_NOME_FUNC, stdin);
            f.nome[strcspn(f.nome, "\n")] = 0;

            printf("Cargo: ");
            fgets(f.cargo, TAM_CARGO, stdin);
            f.cargo[strcspn(f.cargo, "\n")] = 0;

            printf("Salario: ");
            scanf("%f", &f.salario);
            getchar();

            printf("Telefone: ");
            fgets(f.telefone, TAM_TEL_FUNC, stdin);
            f.telefone[strcspn(f.telefone, "\n")] = 0;

            cadastrar_funcionario(f);
        }
        else if (op == 2) listar_funcionarios();
        else if (op == 3) {
            int id;
            printf("ID do funcionario para editar: ");
            scanf("%d", &id);
            getchar();
            editar_funcionario(id);
        }
        else if (op == 4) {
            int id;
            printf("ID do funcionario para remover: ");
            scanf("%d", &id);
            getchar();
            remover_funcionario(id);
        }
        else printf("Opcao invalida!\n");
    }
}

void menuEstadias() {
    int op;

    while (1) {
        printf("\n=== MENU ESTADIAS ===\n");
        printf("1 - Cadastrar estadia\n");
        printf("2 - Finalizar estadia\n");
        printf("3 - Listar estadias\n");
        printf("0 - Voltar\n> ");
        scanf("%d", &op);
        getchar();

        if (op == 0) return;

        else if (op == 1) cadastrarEstadia();
        else if (op == 2) finalizarEstadia();
        else if (op == 3) listarEstadias();
        else printf("Opcao invalida!\n");
    }
}

int main(void) {
    carregarQuartos();
    carregarEstadias();
    carregar_funcionarios();  
    carregar_clientes();      

    int opcao;

    while (1) {
        printf("\n=== HOTEL DESCANSO GARANTIDO ===\n");
        printf("1 - Menu Clientes\n");
        printf("2 - Menu Quartos\n");
        printf("3 - Menu Funcionarios\n");
        printf("4 - Menu Estadias\n");
        printf("0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 0) break;
        else if (opcao == 1) menuClientes();
        else if (opcao == 2) menuQuartos();
        else if (opcao == 3) menuFuncionarios();
        else if (opcao == 4) menuEstadias();
        else printf("Opcao invalida!\n");
    }

    printf("Saindo...\n");
    return 0;
}
