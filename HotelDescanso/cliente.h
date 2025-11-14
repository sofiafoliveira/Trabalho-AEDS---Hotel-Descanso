#ifndef CLIENTE_H
#define CLIENTE_H

#define TAM_NOME 100
#define TAM_ENDERECO 150
#define TAM_TELEFONE 20

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char endereco[TAM_ENDERECO];
    char telefone[TAM_TELEFONE];
} Cliente;

int cadastrar_cliente(Cliente novo);
void listar_clientes();
int cliente_existe(int codigo);

#endif

