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

/* conflitoDatas agora IGNORA estadias já finalizadas (finalizada == 1) */
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto) {
    for (int i = 0; i < totalEstadias; i++) {

        if (estadias[i].idQuarto != numeroQuarto)
            continue;

        /* pula estadias finalizadas (não bloqueiam o quarto) */
        if (estadias[i].finalizada)
            continue;

        int eD = estadias[i].diaEntrada;
        int eM = estadias[i].mesEntrada;
        int eA = estadias[i].anoEntrada;

        int sD = estadias[i].diaSaida;
        int sM = estadias[i].mesSaida;
        int sA = estadias[i].anoSaida;

        /* Verifica se há interseção entre [entrada,saida) */
        int inicioNovo = calcularDiarias(eD, eM, eA, d1, m1, a1);
        int fimNovo = calcularDiarias(d1, m1, a1, sD, sM, sA);

        /* Se os períodos se sobrepõem, retorna conflito */
        /* condição simples: se data nova começa antes da saída existente e data nova termina depois da entrada existente */
        int novoInicioEmRelacaoAExistente = calcularDiarias(eD, eM, eA, d1, m1, a1); // >=0 => novo inicio >= entrada existente
        int novoFimEmRelacaoAExistente = calcularDiarias(eD, eM, eA, d2, m2, a2); // >=0 => novo fim >= entrada existente

        /* Mais simples: converter cada data em "dias desde uma base" e comparar intervalos */
        int startExisting = calcularDiarias(1,1,1, eD,eM,eA);
        int endExisting   = calcularDiarias(1,1,1, sD,sM,sA);
        int startNew      = calcularDiarias(1,1,1, d1,m1,a1);
        int endNew        = calcularDiarias(1,1,1, d2,m2,a2);

        if (!(endNew <= startExisting || startNew >= endExisting)) {
            return 1; /* há sobreposição */
        }
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

    e.finalizada = 0; /* marca como ativa */

    estadias[totalEstadias] = e;
    totalEstadias++;
    salvarEstadias();

    marcarQuartoOcupado(numero);

    printf("Estadia cadastrada! Quarto %d reservado.\n", numero);
}

/* finalizarEstadia marca a estadia como finalizada, libera o quarto e salva.
    Mantemos histórico e pontos. */
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

    if (e->finalizada) {
        printf("Estadia ja foi finalizada anteriormente.\n");
        return;
    }

    double preco = obterPrecoQuarto(e->idQuarto);
    double total = preco * e->qtdDiarias;

    printf("\n--- FINALIZANDO ---\n");
    printf("Cliente: %d\n", e->codigoCliente);
    printf("Quarto: %d\n", e->idQuarto);
    printf("Diarias: %d\n", e->qtdDiarias);
    printf("Valor diaria: %.2f\n", preco);
    printf("TOTAL: R$ %.2f\n", total);

    /* marca finalizada e salva */
    e->finalizada = 1;
    salvarEstadias();

    /* libera o quarto */
    marcarQuartoDesocupado(e->idQuarto);

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
        printf("Status: %s\n", e.finalizada ? "Finalizada" : "Ativa");
        printf("------------------------\n");
    }
}

void listarEstadiasPorCliente() {
    int codigo;
    printf("\nCodigo do cliente: ");
    scanf("%d", &codigo);

    // Verifica se o cliente existe
    if (!cliente_existe(codigo)) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    int encontrou = 0;

    printf("\n=== ESTADIAS DO CLIENTE %d ===\n", codigo);

    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigo) {
            encontrou = 1;

            printf("\nEstadia %d:\n", estadias[i].codigoEstadia);
            printf("Quarto: %d\n", estadias[i].idQuarto);
            printf("Data Entrada: %02d/%02d/%04d\n",
                   estadias[i].diaEntrada, estadias[i].mesEntrada, estadias[i].anoEntrada);
            printf("Data Saida: %02d/%02d/%04d\n",
                   estadias[i].diaSaida, estadias[i].mesSaida, estadias[i].anoSaida);
            printf("Diarias: %d\n", estadias[i].qtdDiarias);
            printf("Status: %s\n", estadias[i].finalizada ? "Finalizada" : "Ativa");
        }
    }

    if (!encontrou) {
        printf("\nNenhuma estadia encontrada para esse cliente.\n");
    }
}

int calcularPontosCliente(int codigoCliente) {
    int pontos = 0;

    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigoCliente && estadias[i].finalizada) {
            pontos += estadias[i].qtdDiarias * 10; // 10 pontos por diária
        }
    }

    return pontos;
}
