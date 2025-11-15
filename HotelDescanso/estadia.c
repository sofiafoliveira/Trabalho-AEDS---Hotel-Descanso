#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estadia.h"

#define ARQ_ESTADIAS "estadias.bin"

Estadia estadias[200];
int totalEstadias = 0;

void salvarEstadias() {
    FILE *f = fopen(ARQ_ESTADIAS, "wb");
    if (!f) return;

    fwrite(&totalEstadias, sizeof(int), 1, f);
    fwrite(estadias, sizeof(Estadia), totalEstadias, f);

    fclose(f);
}

void carregarEstadias() {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) return;

    fread(&totalEstadias, sizeof(int), 1, f);
    fread(estadias, sizeof(Estadia), totalEstadias, f);

    fclose(f);
}

int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2) {
    int total = (a2 - a1) * 365;
    total += (m2 - m1) * 30;
    total += (d2 - d1);
    return total;
}

int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto) {
    for (int i = 0; i < totalEstadias; i++) {

        if (estadias[i].idQuarto != numeroQuarto)
            continue;

        int eD = estadias[i].diaEntrada;
        int eM = estadias[i].mesEntrada;
        int eA = estadias[i].anoEntrada;

        int sD = estadias[i].diaSaida;
        int sM = estadias[i].mesSaida;
        int sA = estadias[i].anoSaida;

        int ini = calcularDiarias(eD, eM, eA, d1, m1, a1);
        int fim = calcularDiarias(d1, m1, a1, sD, sM, sA);

        if (ini <= 0 && fim <= 0)
            return 1;
    }
    return 0;
}

void cadastrarEstadia() {
    Estadia e;

    printf("\n--- CADASTRO DE ESTADIA ---\n");

    e.codigoEstadia = totalEstadias + 1;

    printf("Codigo do cliente: ");
    scanf("%d", &e.codigoCliente);

    if (!cliente_existe(e.codigoCliente)) {
        printf("Cliente inexistente!\n");
        return;
    }

    printf("Quantidade de hospedes: ");
    scanf("%d", &e.qtdHospedes);

    printf("Data de entrada (dd mm aaaa): ");
    scanf("%d %d %d", &e.diaEntrada, &e.mesEntrada, &e.anoEntrada);

    printf("Data de saida (dd mm aaaa): ");
    scanf("%d %d %d", &e.diaSaida, &e.mesSaida, &e.anoSaida);

    int numero = encontrarQuartoDisponivel(e.qtdHospedes);

    if (numero == -1) {
        printf("Nenhum quarto disponível!\n");
        return;
    }

    if (conflitoDatas(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                      e.diaSaida, e.mesSaida, e.anoSaida, numero)) {
        printf("Quarto ocupado no período!\n");
        return;
    }

    e.idQuarto = numero;

    e.qtdDiarias = calcularDiarias(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                                   e.diaSaida, e.mesSaida, e.anoSaida);

    if (e.qtdDiarias <= 0) {
        printf("Datas invalidas!\n");
        return;
    }

    estadias[totalEstadias] = e;
    totalEstadias++;
    salvarEstadias();

    marcarQuartoOcupado(numero);

    printf("Estadia cadastrada! Quarto %d reservado.\n", numero);
}

void finalizarEstadia() {
    int cod;
    printf("\nCodigo da estadia: ");
    scanf("%d", &cod);

    int index = -1;

    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoEstadia == cod) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Estadia nao encontrada!\n");
        return;
    }

    Estadia *e = &estadias[index];

    double preco = obterPrecoQuarto(e->idQuarto);
    double total = preco * e->qtdDiarias;

    printf("\n--- FINALIZANDO ---\n");
    printf("Cliente: %d\n", e->codigoCliente);
    printf("Quarto: %d\n", e->idQuarto);
    printf("Diarias: %d\n", e->qtdDiarias);
    printf("Valor diaria: %.2f\n", preco);
    printf("TOTAL: R$ %.2f\n", total);

    marcarQuartoDesocupado(e->idQuarto);

    for (int i = index; i < totalEstadias - 1; i++)
        estadias[i] = estadias[i + 1];

    totalEstadias--;
    salvarEstadias();

    printf("Estadia finalizada!\n");
}

double obterPrecoQuarto(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero)
            return quartos[i].diaria;
    }
    return 0;
}

void listarEstadias() {
    if (totalEstadias == 0) {
        printf("\nNenhuma estadia cadastrada!\n");
        return;
    }

    printf("\n=== LISTA DE ESTADIAS ===\n");

    for (int i = 0; i < totalEstadias; i++) {
        Estadia e = estadias[i];

        printf("\nCodigo da Estadia: %d\n", e.codigoEstadia);
        printf("Cliente: %d\n", e.codigoCliente);
        printf("Quarto: %d\n", e.idQuarto);
        printf("Hospedes: %d\n", e.qtdHospedes);
        printf("Entrada: %02d/%02d/%04d\n", e.diaEntrada, e.mesEntrada, e.anoEntrada);
        printf("Saida:   %02d/%02d/%04d\n", e.diaSaida, e.mesSaida, e.anoSaida);
        printf("Diarias: %d\n", e.qtdDiarias);
        printf("------------------------\n");
    }
}
