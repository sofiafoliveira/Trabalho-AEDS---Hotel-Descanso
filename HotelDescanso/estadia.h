#ifndef ESTADIA_H
#define ESTADIA_H

#include "cliente.h"
#include "quarto.h"

typedef struct {
    int codigoEstadia;
    int codigoCliente;
    int idQuarto; // corresponde ao numero do quarto
    int qtdHospedes;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
} Estadia;

extern Estadia estadias[200];
extern int totalEstadias;

void salvarEstadias();
void carregarEstadias();

int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2);
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto);

void cadastrarEstadia();
void finalizarEstadia();
void listarEstadias();

void listarEstadiasPorCliente();

double obterPrecoQuarto(int numero);

#endif
