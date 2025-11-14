#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

#define ARQUIVO_CLIENTES "clientes.bin"

int cliente_existe(int codigo) {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) return 0;

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (c.codigo == codigo) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
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

    printf("Cliente cadastrado com sucesso!\n");
    return 1;
}

void listar_clientes() {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }

    Cliente c;
    printf("\n--- LISTA DE CLIENTES ---\n");
    while (fread(&c, sizeof(Cliente), 1, f)) {
        printf("Codigo: %d\nNome: %s\nEndereco: %s\nTelefone: %s\n-------------------\n",
               c.codigo, c.nome, c.endereco, c.telefone);
    }
    fclose(f);
}

/* ------------------------- E D I T A R   C L I E N T E ------------------------------ */

void editar_cliente(int codigo) {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(f);
        return;
    }

    Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (c.codigo == codigo) {
            encontrado = 1;

            printf("Novo nome: ");
            fflush(stdin);
            fgets(c.nome, TAM_NOME, stdin);
            c.nome[strcspn(c.nome, "\n")] = 0;

            printf("Novo endereco: ");
            fgets(c.endereco, TAM_ENDERECO, stdin);
            c.endereco[strcspn(c.endereco, "\n")] = 0;

            printf("Novo telefone: ");
            fgets(c.telefone, TAM_TELEFONE, stdin);
            c.telefone[strcspn(c.telefone, "\n")] = 0;
        }

        fwrite(&c, sizeof(Cliente), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove(ARQUIVO_CLIENTES);
    rename("temp.bin", ARQUIVO_CLIENTES);

    if (encontrado)
        printf("Cliente editado com sucesso!\n");
    else
        printf("Cliente nao encontrado!\n");
}

/* ------------------------- R E M O V E R   C L I E N T E --------------------------- */

void remover_cliente(int codigo) {
    FILE *f = fopen(ARQUIVO_CLIENTES, "rb");
    if (!f) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (!temp) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(f);
        return;
    }

    Cliente c;
    int removido = 0;

    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (c.codigo != codigo) {
            fwrite(&c, sizeof(Cliente), 1, temp);
        } else {
            removido = 1;
        }
    }

    fclose(f);
    fclose(temp);

    remove(ARQUIVO_CLIENTES);
    rename("temp.bin", ARQUIVO_CLIENTES);

    if (removido)
        printf("Cliente removido com sucesso!\n");
    else
        printf("Cliente nao encontrado!\n");
}
