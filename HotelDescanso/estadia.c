#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estadia.h"

#define ARQ_ESTADIAS "estadias.bin"

Estadia estadias[500];
int totalEstadias = 0;

/* ----------------- Persistência ----------------- */
void salvarEstadias() {
    FILE *f = fopen(ARQ_ESTADIAS, "wb");
    if (!f) {
        printf("Erro ao salvar as estadias.\n");
        return;
    }
    fwrite(&totalEstadias, sizeof(int), 1, f);
    fwrite(estadias, sizeof(Estadia), totalEstadias, f);
    fclose(f);
}

void carregarEstadias() {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) {
        totalEstadias = 0;
        return;
    }
    fread(&totalEstadias, sizeof(int), 1, f);
    fread(estadias, sizeof(Estadia), totalEstadias, f);
    fclose(f);
}

/* ----------------- Datas / utilitários ----------------- */

/* valida data real (considera ano bissexto) */
int validarData(int d, int m, int a) {
    if (a < 1900 || a > 9999) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1) return 0;
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int bissexto = ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0));
    if (m == 2) {
        if (bissexto) { if (d > 29) return 0; }
        else { if (d > 28) return 0; }
    } else {
        if (d > diasMes[m]) return 0;
    }
    return 1;
}

/* converte data em dias absolutos (base arbitrária), suficiente para comparar */
long dateToDays(int d, int m, int a) {
    /* usa algoritmo simples: dias + meses*31 + anos*365.
       Precisão perfeita de diferença não é necessária, só ordem e diferenças. */
    return (long)a * 365L + (long)m * 31L + (long)d;
}

/* retorna 1 se períodos [s1,e1) e [s2,e2) se sobrepõem; assume datas válidas.
   Intervalo usa semi-aberto [entrada, saida) — saída não inclui o dia final. */
int periodoOverlap(int s1d, int s1m, int s1a, int e1d, int e1m, int e1a,
                   int s2d, int s2m, int s2a, int e2d, int e2m, int e2a) {
    long S1 = dateToDays(s1d,s1m,s1a);
    long E1 = dateToDays(e1d,e1m,e1a);
    long S2 = dateToDays(s2d,s2m,s2a);
    long E2 = dateToDays(e2d,e2m,e2a);
    if (E1 <= S1 || E2 <= S2) return 1; /* trata intervalos inválidos como conflito */
    /* overlap se S1 < E2 && S2 < E1 */
    return (S1 < E2 && S2 < E1) ? 1 : 0;
}

/* calcula diárias entre entrada e saída (assume datas válidas e saída > entrada) */
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2) {
    long start = dateToDays(d1,m1,a1);
    long end   = dateToDays(d2,m2,a2);
    long diff = end - start;
    if (diff <= 0) return 0;
    return (int)diff;
}

/* ----------------- Regras / validações ----------------- */

/* Verifica se o quarto atende (desocupado e capacidade suficiente) */
int quartoAtende(int numeroQuarto, int qtdHospedes) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numeroQuarto) {
            if (quartos[i].ocupado) return 0; /* ocupado */
            if (quartos[i].capacidade < qtdHospedes) return 0; /* pouca capacidade */
            return 1;
        }
    }
    return 0; /* quarto não encontrado / não atende */
}

/* Verifica se o cliente possui alguma estadia ATIVA (finalizada == 0) */
int clienteTemEstadiaAtiva(int codigoCliente) {
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigoCliente && estadias[i].finalizada == 0)
            return 1;
    }
    return 0;
}

/* verifica conflito de datas para um quarto: retorna 1 se houver conflito (não pode reservar),
   considera apenas estadias ativas (finalizada == 0) */
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto) {
    /* valida datas */
    if (!validarData(d1,m1,a1) || !validarData(d2,m2,a2)) return 1;
    long Snew = dateToDays(d1,m1,a1);
    long Enew = dateToDays(d2,m2,a2);
    if (Enew <= Snew) return 1; /* saída <= entrada => inválido/conflict */

    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].idQuarto != numeroQuarto) continue;
        if (estadias[i].finalizada) continue; /* só considera ativas */
        if ( periodoOverlap( estadias[i].diaEntrada, estadias[i].mesEntrada, estadias[i].anoEntrada,
                             estadias[i].diaSaida,  estadias[i].mesSaida,  estadias[i].anoSaida,
                             d1, m1, a1, d2, m2, a2 ) ) {
            return 1;
        }
    }
    return 0; /* sem conflito */
}

/* ----------------- Operações principais ----------------- */

/* cadastrar estadia: sistema encontra quarto automaticamente (encontrarQuartoDisponivel),
   mas aqui aplicamos validações explicitamente. */
void cadastrarEstadia() {
    Estadia e;
    printf("\n--- CADASTRO DE ESTADIA ---\n");

    e.codigoEstadia = totalEstadias + 1;

    printf("Codigo do cliente: ");
    if (scanf("%d", &e.codigoCliente) != 1) { getchar(); printf("Entrada invalida.\n"); return; }

    if (!cliente_existe(e.codigoCliente)) {
        printf("Cliente inexistente!\n");
        return;
    }

    /* regra: apenas 1 estadia ativa por cliente */
    if (clienteTemEstadiaAtiva(e.codigoCliente)) {
        printf("Erro: cliente ja possui uma estadia ativa.\n");
        return;
    }

    printf("Quantidade de hospedes: ");
    if (scanf("%d", &e.qtdHospedes) != 1) { getchar(); printf("Entrada invalida.\n"); return; }

    printf("Data de entrada (dd mm aaaa): ");
    if (scanf("%d %d %d", &e.diaEntrada, &e.mesEntrada, &e.anoEntrada) != 3) { printf("Formato invalido.\n"); return; }

    printf("Data de saida (dd mm aaaa): ");
    if (scanf("%d %d %d", &e.diaSaida, &e.mesSaida, &e.anoSaida) != 3) { printf("Formato invalido.\n"); return; }

    /* valida datas e ordem */
    if (!validarData(e.diaEntrada,e.mesEntrada,e.anoEntrada) || !validarData(e.diaSaida,e.mesSaida,e.anoSaida)) {
        printf("Erro: uma das datas eh invalida.\n");
        return;
    }
    if (dateToDays(e.diaSaida,e.mesSaida,e.anoSaida) <= dateToDays(e.diaEntrada,e.mesEntrada,e.anoEntrada)) {
        printf("Erro: data de saida deve ser depois da data de entrada.\n");
        return;
    }

    /* encontra quarto disponível (função do módulo quarto.h) */
    int numero = encontrarQuartoDisponivel(e.qtdHospedes);
    if (numero == -1) {
        printf("Nenhum quarto disponivel que atenda a quantidade de hospedes.\n");
        return;
    }

    /* verificação extra: capacidade e status */
    if (!quartoAtende(numero, e.qtdHospedes)) {
        printf("Quarto %d nao atende os requisitos (capacidade/ocupado).\n", numero);
        return;
    }

    /* conflito de datas apenas com estadias ativas */
    if (conflitoDatas(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                      e.diaSaida, e.mesSaida, e.anoSaida, numero)) {
        printf("Quarto %d ja possui estadia ativa nesse periodo.\n", numero);
        return;
    }

    /* define e calcula diarias */
    e.idQuarto = numero;
    e.qtdDiarias = calcularDiarias(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                                   e.diaSaida, e.mesSaida, e.anoSaida);
    e.finalizada = 0; /* ativa */

    /* persiste em memória e arquivo */
    estadias[totalEstadias] = e;
    totalEstadias++;
    salvarEstadias();

    /* marca quarto ocupado */
    marcarQuartoOcupado(numero);

    printf("Estadia cadastrada com sucesso! Codigo: %d | Quarto: %d | Diarias: %d\n",
           e.codigoEstadia, e.idQuarto, e.qtdDiarias);
}

/* finalizar estadia: marca como finalizada, libera quarto, mantém registro para histórico */
void finalizarEstadia() {
    int cod;
    printf("\nCodigo da estadia: ");
    if (scanf("%d", &cod) != 1) { getchar(); printf("Entrada invalida.\n"); return; }

    int index = -1;
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoEstadia == cod) { index = i; break; }
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

    printf("\n--- FINALIZANDO ESTADIA ---\n");
    printf("Cliente: %d\n", e->codigoCliente);
    printf("Quarto: %d\n", e->idQuarto);
    printf("Diarias: %d\n", e->qtdDiarias);
    printf("Valor da diaria: %.2f\n", preco);
    printf("TOTAL A PAGAR: R$ %.2f\n", total);

    /* marca finalizada, salva e libera quarto */
    e->finalizada = 1;
    salvarEstadias();
    marcarQuartoDesocupado(e->idQuarto);

    printf("Estadia finalizada e quarto liberado com sucesso!\n");
}

/* obter preco do quarto (usa array quartos[] do modulo quarto) */
double obterPrecoQuarto(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero)
            return quartos[i].diaria;
    }
    return 0;
}

/* listar estadias (mostra status) */
void listarEstadias() {
    if (totalEstadias == 0) {
        printf("\nNenhuma estadia cadastrada!\n");
        return;
    }
    printf("\n=== LISTA DE ESTADIAS ===\n");
    for (int i = 0; i < totalEstadias; i++) {
        Estadia *e = &estadias[i];
        printf("\nCodigo da Estadia: %d\n", e->codigoEstadia);
        printf("Cliente: %d\n", e->codigoCliente);
        printf("Quarto: %d\n", e->idQuarto);
        printf("Hospedes: %d\n", e->qtdHospedes);
        printf("Entrada: %02d/%02d/%04d\n", e->diaEntrada, e->mesEntrada, e->anoEntrada);
        printf("Saida:   %02d/%02d/%04d\n", e->diaSaida, e->mesSaida, e->anoSaida);
        printf("Diarias: %d\n", e->qtdDiarias);
        printf("Status: %s\n", e->finalizada ? "Finalizada" : "Ativa");
        printf("------------------------\n");
    }
}

/* listar estadias por cliente */
void listarEstadiasPorCliente() {
    int codigo;
    printf("\nCodigo do cliente: ");
    if (scanf("%d", &codigo) != 1) { getchar(); printf("Entrada invalida.\n"); return; }

    if (!cliente_existe(codigo)) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    int encontrou = 0;
    printf("\n=== ESTADIAS DO CLIENTE %d ===\n", codigo);
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigo) {
            encontrou = 1;
            Estadia *e = &estadias[i];
            printf("\nEstadia %d:\n", e->codigoEstadia);
            printf("Quarto: %d\n", e->idQuarto);
            printf("Entrada: %02d/%02d/%04d\n", e->diaEntrada, e->mesEntrada, e->anoEntrada);
            printf("Saida:   %02d/%02d/%04d\n", e->diaSaida, e->mesSaida, e->anoSaida);
            printf("Diarias: %d\n", e->qtdDiarias);
            printf("Status: %s\n", e->finalizada ? "Finalizada" : "Ativa");
        }
    }
    if (!encontrou) printf("\nNenhuma estadia encontrada para esse cliente.\n");
}

/* calcular pontos: 10 pontos por diaria, soma apenas estadias finalizadas */
int calcularPontosCliente(int codigoCliente) {
    int pontos = 0;
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigoCliente && estadias[i].finalizada) {
            pontos += estadias[i].qtdDiarias * 10;
        }
    }
    return pontos;
}
