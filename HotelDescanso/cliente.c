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
        printf("Cliente com código %d já existe!\n", novo.codigo);
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
        printf("Código: %d\nNome: %s\nEndereço: %s\nTelefone: %s\n-------------------\n",
               c.codigo, c.nome, c.endereco, c.telefone);
    }
    fclose(f);
}

