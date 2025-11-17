#ifndef ESTADIA_H
#define ESTADIA_H

#include "cliente.h"
#include "quarto.h"

/* estrutura de estadia */
typedef struct {
    int codigoEstadia;
    int codigoCliente;
    int idQuarto; /* número do quarto */
    int qtdHospedes;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
} Estadia;

/* variáveis globais (definidas em estadia.c) */
extern Estadia estadias[200];
extern int totalEstadias;

/* persistência */
void salvarEstadias();
void carregarEstadias();

/* utilitários / validações */
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2);
int validarData(int d, int m, int a);
long dateToDays(int d, int m, int a); /* para comparar datas */
int periodoOverlap(int s1d, int s1m, int s1a, int e1d, int e1m, int e1a,
                   int s2d, int s2m, int s2a, int e2d, int e2m, int e2a);
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto);
int quartoAtende(int numeroQuarto, int qtdHospedes); /* capacidade + status */

/* operações do módulo */
void cadastrarEstadia();
void finalizarEstadia();
void listarEstadias();
void listarEstadiasPorCliente();

/* consulta de preço (usa dados de quarto) */
double obterPrecoQuarto(int numero);

/* função extra solicitada em issue: calcular pontos de fidelidade */
int calcularPontosCliente(int codigoCliente);

#endif
