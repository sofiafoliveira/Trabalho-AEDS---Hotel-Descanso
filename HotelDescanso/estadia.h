#ifndef ESTADIA_H
#define ESTADIA_H

#include "cliente.h"
#include "quarto.h"

/* Estrutura de estadia */
typedef struct {
    int codigoEstadia;
    int codigoCliente;
    int idQuarto; /* numero do quarto */
    int qtdHospedes;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
    int finalizada; /* 0 = ativa, 1 = finalizada */
} Estadia;

/* Variáveis globais (definidas em estadia.c) */
extern Estadia estadias[500];
extern int totalEstadias;

/* Persistência */
void salvarEstadias();
void carregarEstadias();

/* Utilitários / validações */
int validarData(int d, int m, int a);
long dateToDays(int d, int m, int a);
int periodoOverlap(int s1d, int s1m, int s1a, int e1d, int e1m, int e1a,
                   int s2d, int s2m, int s2a, int e2d, int e2m, int e2a);
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2);

/* Regras específicas */
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto);
int quartoAtende(int numeroQuarto, int qtdHospedes);
int clienteTemEstadiaAtiva(int codigoCliente);

/* Operações do módulo */
void cadastrarEstadia();
void finalizarEstadia();
void listarEstadias();
void listarEstadiasPorCliente();

/* Cálculo e utilitários */
double obterPrecoQuarto(int numero);
int calcularPontosCliente(int codigoCliente); /* 10 pontos por diária, só de estadias finalizadas */

#endif
