#ifndef QUARTO_H
#define QUARTO_H

typedef struct {
    int numero;
    int capacidade;
    float diaria;
    int ocupado; // 0 = livre, 1 = ocupado
} Quarto;

extern Quarto quartos[100];
extern int totalQuartos;

void carregarQuartos();
void salvarQuartos();

Quarto* buscarQuarto(int numero);
int quartoDisponivel(int numero);

void cadastrarQuarto();
void listarQuartos();
void editarQuarto(int numero);
void removerQuarto(int numero);

// Usado pela estadia
int encontrarQuartoDisponivel(int qtdHospedes);
void marcarQuartoOcupado(int numero);
void marcarQuartoDesocupado(int numero);

double obterPrecoQuarto(int numero);

#endif
