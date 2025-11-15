#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionario.h"

#define ARQ_FUNCIONARIOS "funcionarios.bin"

Funcionario funcionarios[200];
int totalFuncionarios = 0;

void salvarFuncionarios() {
    FILE *f = fopen(ARQ_FUNCIONARIOS, "wb");
    if (!f) {
        printf("Erro ao salvar funcionarios.\n");
        return;
    }
    fwrite(&totalFuncionarios, sizeof(int), 1, f);
    fwrite(funcionarios, sizeof(Funcionario), totalFuncionarios, f);
    fclose(f);
}

void carregarFuncionarios() {
    FILE *f = fopen(ARQ_FUNCIONARIOS, "rb");
    if (!f) {
        totalFuncionarios = 0;
        return;
    }
    fread(&totalFuncionarios, sizeof(int), 1, f);
    fread(funcionarios, sizeof(Funcionario), totalFuncionarios, f);
    fclose(f);
}

int funcionario_existe(int id) {
    for (int i = 0; i < totalFuncionarios; i++) {
        if (funcionarios[i].id == id) return 1;
    }
    return 0;
}

int cadastrar_funcionario(Funcionario f) {
    if (funcionario_existe(f.id)) {
        printf("Funcionario com id %d ja existe!\n", f.id);
        return 0;
    }
    funcionarios[totalFuncionarios++] = f;
    salvarFuncionarios();
    printf("Funcionario cadastrado com sucesso!\n");
    return 1;
}

void listar_funcionarios() {
    if (totalFuncionarios == 0) {
        printf("\nNenhum funcionario cadastrado!\n");
        return;
    }
    printf("\n--- Lista de Funcionarios ---\n");
    for (int i = 0; i < totalFuncionarios; i++) {
        printf("ID: %d\nNome: %s\nCargo: %s\nSalario: %.2f\nTelefone: %s\n-------------------\n",
               funcionarios[i].id,
               funcionarios[i].nome,
               funcionarios[i].cargo,
               funcionarios[i].salario,
               funcionarios[i].telefone);
    }
}

/* Editar: reescreve o registro escolhido em memória e salva arquivo */
void editar_funcionario(int id) {
    for (int i = 0; i < totalFuncionarios; i++) {
        if (funcionarios[i].id == id) {
            printf("Novo nome: ");
            fflush(stdin);
            fgets(funcionarios[i].nome, TAM_NOME_FUNC, stdin);
            funcionarios[i].nome[strcspn(funcionarios[i].nome, "\n")] = 0;

            printf("Novo cargo: ");
            fgets(funcionarios[i].cargo, TAM_CARGO, stdin);
            funcionarios[i].cargo[strcspn(funcionarios[i].cargo, "\n")] = 0;

            printf("Novo salario: ");
            scanf("%f", &funcionarios[i].salario);
            getchar();

            printf("Novo telefone: ");
            fgets(funcionarios[i].telefone, TAM_TEL_FUNC, stdin);
            funcionarios[i].telefone[strcspn(funcionarios[i].telefone, "\n")] = 0;

            salvarFuncionarios();
            printf("Funcionario editado com sucesso!\n");
            return;
        }
    }
    printf("Funcionario nao encontrado!\n");
}

/* Remover: shift para esquerda e salvar */
void remover_funcionario(int id) {
    int index = -1;
    for (int i = 0; i < totalFuncionarios; i++) {
        if (funcionarios[i].id == id) { index = i; break; }
    }
    if (index == -1) {
        printf("Funcionario nao encontrado!\n");
        return;
    }
    for (int i = index; i < totalFuncionarios - 1; i++) {
        funcionarios[i] = funcionarios[i+1];
    }
    totalFuncionarios--;
    salvarFuncionarios();
    printf("Funcionario removido com sucesso!\n");
}
