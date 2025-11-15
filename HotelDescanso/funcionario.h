#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#define TAM_NOME_FUNC 100
#define TAM_CARGO 50
#define TAM_TEL_FUNC 20

typedef struct {
    int id;
    char nome[TAM_NOME_FUNC];
    char cargo[TAM_CARGO];
    float salario;
    char telefone[TAM_TEL_FUNC];
} Funcionario;

/* CRUD e utilitários */
int funcionario_existe(int id);
int cadastrar_funcionario(Funcionario f);
void listar_funcionarios();
void editar_funcionario(int id);
void remover_funcionario(int id);
void carregarFuncionarios();
void salvarFuncionarios();

#endif
