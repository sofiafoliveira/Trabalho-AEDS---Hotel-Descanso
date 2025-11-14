#ifndef QUARTO_H
#define QUARTO_H

typedef struct {
    int id;
    char tipo[50];
    float preco;
    int ocupado; // 0 = livre / 1 = ocupado
} Quarto;

void cadastrarQuarto();
void listarQuartos();
void carregarQuartos();
void salvarQuartos();
void editarQuarto(int id);
void removerQuarto(int id);

#endif
