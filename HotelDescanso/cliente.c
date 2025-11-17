#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

#define ARQUIVO_CLIENTES "clientes.bin"

Cliente clientes[200];
int totalClientes = 0;

void carregar_clientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) {
        totalClientes = 0;
        return;
    }

    totalClientes = 0;
    while (fread(&clientes[totalClientes], sizeof(Cliente), 1, f)) {
        totalClientes++;
    }

    fclose(f);
}

int cliente_existe(int codigo) {
    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo == codigo)
            return 1;
    }
    return 0;
}

int cadastrar_cliente(Cliente novo) {
    if (cliente_existe(novo.codigo)) {
        printf("Cliente com codigo %d ja existe!\n", novo.codigo);
        return 0;
    }

    FILE *f = fopen(ARQUIVO_CLIENTES, "ab");
    if (!f) {
        printf("Erro ao abrir arquivo de clientes.\n");
        return 0;
    }

    fwrite(&novo, sizeof(Cliente), 1, f);
    fclose(f);

    clientes[totalClientes++] = novo;

    printf("Cliente cadastrado com sucesso!\n");
    return 1;
}

void listar_clientes() {
    if (totalClientes == 0) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }

    printf("\n--- LISTA DE CLIENTES ---\n");
    for (int i = 0; i < totalClientes; i++) {
        Cliente c = clientes[i];
        printf("Codigo: %d\nNome: %s\nEndereco: %s\nTelefone: %s\n-------------------\n",
               c.codigo, c.nome, c.endereco, c.telefone);
    }
}

void editar_cliente(int codigo) {
    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo == codigo) {

            printf("Novo nome: ");
            fflush(stdin);
            fgets(clientes[i].nome, TAM_NOME, stdin);
            clientes[i].nome[strcspn(clientes[i].nome, "\n")] = 0;

            printf("Novo endereco: ");
            fgets(clientes[i].endereco, TAM_ENDERECO, stdin);
            clientes[i].endereco[strcspn(clientes[i].endereco, "\n")] = 0;

            printf("Novo telefone: ");
            fgets(clientes[i].telefone, TAM_TELEFONE, stdin);
            clientes[i].telefone[strcspn(clientes[i].telefone, "\n")] = 0;

            // salvar no arquivo
            FILE *f = fopen(ARQUIVO_CLIENTES, "wb");
            if (!f) return;
            fwrite(clientes, sizeof(Cliente), totalClientes, f);
            fclose(f);

            printf("Cliente editado com sucesso!\n");
            return;
        }
    }

    printf("Cliente nao encontrado!\n");
}

void remover_cliente(int codigo) {
    int index = -1;

    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo == codigo) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    for (int i = index; i < totalClientes - 1; i++)
        clientes[i] = clientes[i + 1];

    totalClientes--;

    FILE *f = fopen(ARQUIVO_CLIENTES, "wb");
    if (!f) return;
    fwrite(clientes, sizeof(Cliente), totalClientes, f);
    fclose(f);

    printf("Cliente removido com sucesso!\n");
}

Cliente buscar_cliente_por_codigo(int codigo) {
    Cliente vazio;
    vazio.codigo = -1;

    for (int i = 0; i < totalClientes; i++) {
        if (clientes[i].codigo == codigo) {
            Cliente c = clientes[i];

            printf("\n=== CLIENTE ENCONTRADO ===\n");
            printf("Codigo: %d\nNome: %s\nEndereco: %s\nTelefone: %s\n",
                   c.codigo, c.nome, c.endereco, c.telefone);

            return c;
        }
    }

    printf("Cliente com codigo %d nao encontrado.\n", codigo);
    return vazio;
}

void buscar_cliente_por_nome(const char *nome) {
    int encontrou = 0;

    printf("\n=== RESULTADOS PARA '%s' ===\n", nome);

    for (int i = 0; i < totalClientes; i++) {
        if (strstr(clientes[i].nome, nome) != NULL) {
            encontrou = 1;
            Cliente c = clientes[i];

            printf("\nCodigo: %d\nNome: %s\nEndereco: %s\nTelefone: %s\n",
                   c.codigo, c.nome, c.endereco, c.telefone);
        }
    }

    if (!encontrou) {
        printf("Nenhum cliente encontrado com esse nome.\n");
    }
}
