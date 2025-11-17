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

extern Funcionario funcionarios[200];
extern int totalFuncionarios;

void carregarFuncionarios();
void salvarFuncionarios();

int cadastrar_funcionario(Funcionario f);   // <-- AGORA RETORNA int
void listar_funcionarios();
void editar_funcionario(int id);
void remover_funcionario(int id);

// NOVAS FUNÇÕES
void buscar_funcionario_por_codigo(int codigo);
void buscar_funcionario_por_nome(char *nome);

#endif
