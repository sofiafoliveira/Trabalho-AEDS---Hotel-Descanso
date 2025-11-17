#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estadia.h"

#define ARQ_ESTADIAS "estadias.bin"

Estadia estadias[200];
int totalEstadias = 0;

/* --- Persistência --- */
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

/* --- Datas / utilitários --- */

/* retorna 1 se data válida (simples: checa limites comuns) */
int validarData(int d, int m, int a) {
    if (a < 1900 || a > 9999) return 0;
    if (m < 1 || m > 12) return 0;
    if (d < 1) return 0;
    int diasMes[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    /* ano bissexto */
    int bissexto = ( (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0) );
    if (bissexto && m == 2) {
        if (d > 29) return 0;
    } else {
        if (d > diasMes[m]) return 0;
    }
    return 1;
}

/* converte data para número contínuo de dias (aproximação correta para comparação) */
long dateToDays(int d, int m, int a) {
    /* usa contagem simples: dias + meses*31 + anos*365 — suficiente para comparar ordem */
    return (long)a * 365L + (long)m * 31L + (long)d;
}

/* retorna 1 se os períodos [s1, e1) e [s2, e2) se sobrepõem */
int periodoOverlap(int s1d, int s1m, int s1a, int e1d, int e1m, int e1a,
                   int s2d, int s2m, int s2a, int e2d, int e2m, int e2a) {
    long S1 = dateToDays(s1d,s1m,s1a);
    long E1 = dateToDays(e1d,e1m,e1a); /* saída deve ser > entrada */
    long S2 = dateToDays(s2d,s2m,s2a);
    long E2 = dateToDays(e2d,e2m,e2a);
    /* período inválido: consideramos que saída precisa ser > entrada */
    if (E1 <= S1 || E2 <= S2) return 1; /* sinaliza conflito/erro */
    /* overlap se S1 < E2 && S2 < E1 */
    return (S1 < E2 && S2 < E1) ? 1 : 0;
}

/* calcula número de diárias entre duas datas (assume datas válidas; retorno >0) */
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2) {
    long inicio = dateToDays(d1,m1,a1);
    long fim    = dateToDays(d2,m2,a2);
    long diff = fim - inicio;
    if (diff <= 0) return 0;
    return (int)diff;
}

/* verifica se o quarto atende a qtdHospedes e está desocupado */
int quartoAtende(int numeroQuarto, int qtdHospedes) {
    /* usar a array quartos[] e totalQuartos do módulo quarto (declared extern em quarto.h) */
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numeroQuarto) {
            if (quartos[i].ocupado) return 0;
            if (quartos[i].capacidade < qtdHospedes) return 0;
            return 1;
        }
    }
    return 0;
}

/* checa se existe conflito de datas para o quarto dado
   retorna 1 se CONFLITO (não pode reservar), 0 se OK */
int conflitoDatas(int d1, int m1, int a1, int d2, int m2, int a2, int numeroQuarto) {
    /* valida datas primeiro */
    if (!validarData(d1,m1,a1) || !validarData(d2,m2,a2)) return 1; /* trata como conflito */
    /* para cada estadia existente no mesmo quarto verifica overlap */
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].idQuarto != numeroQuarto) continue;
        /* se a estadia já estiver finalizada (não temos campo finalizada separado aqui),
           ela já foi removida no nosso fluxo. Então comparamos só com o que está no array. */
        if ( periodoOverlap( estadias[i].diaEntrada, estadias[i].mesEntrada, estadias[i].anoEntrada,
                             estadias[i].diaSaida,  estadias[i].mesSaida,  estadias[i].anoSaida,
                             d1, m1, a1, d2, m2, a2 ) ) {
            return 1; /* conflito */
        }
    }
    return 0; /* sem conflito */
}

/* --- Operações principais --- */

/* cadastrar estadia: sistema escolhe quarto automaticamente (encontrarQuartoDisponivel) */
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

    printf("Quantidade de hospedes: ");
    if (scanf("%d", &e.qtdHospedes) != 1) { getchar(); printf("Entrada invalida.\n"); return; }

    printf("Data de entrada (dd mm aaaa): ");
    if (scanf("%d %d %d", &e.diaEntrada, &e.mesEntrada, &e.anoEntrada) != 3) { printf("Formato invalido.\n"); return; }

    printf("Data de saida (dd mm aaaa): ");
    if (scanf("%d %d %d", &e.diaSaida, &e.mesSaida, &e.anoSaida) != 3) { printf("Formato invalido.\n"); return; }

    /* valida datas */
    if (!validarData(e.diaEntrada,e.mesEntrada,e.anoEntrada) || !validarData(e.diaSaida,e.mesSaida,e.anoSaida)) {
        printf("Erro: uma das datas eh invalida.\n");
        return;
    }

    /* saída deve ser depois da entrada */
    if (dateToDays(e.diaSaida,e.mesSaida,e.anoSaida) <= dateToDays(e.diaEntrada,e.mesEntrada,e.anoEntrada)) {
        printf("Erro: data de saida deve ser depois da data de entrada.\n");
        return;
    }

    /* encontra quarto que atende à capacidade e esteja desocupado */
    int numero = encontrarQuartoDisponivel(e.qtdHospedes);
    if (numero == -1) {
        printf("Nenhum quarto disponível que atenda a quantidade de hospedes.\n");
        return;
    }

    /* verifica capacidade/status no quarto encontrado (proteção extra) */
    if (!quartoAtende(numero, e.qtdHospedes)) {
        printf("Quarto %d nao atende os requisitos (capacidade/ocupado).\n", numero);
        return;
    }

    /* verifica conflito de datas (sobreposição) */
    if (conflitoDatas(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                      e.diaSaida, e.mesSaida, e.anoSaida, numero)) {
        printf("Quarto %d ja possui estadia nesse periodo ou datas invalidas.\n", numero);
        return;
    }

    /* define quarto e calcula diárias */
    e.idQuarto = numero;
    e.qtdDiarias = calcularDiarias(e.diaEntrada, e.mesEntrada, e.anoEntrada,
                                   e.diaSaida, e.mesSaida, e.anoSaida);

    if (e.qtdDiarias <= 0) {
        printf("Erro no calculo de diarias (datas invalidas).\n");
        return;
    }

    /* persiste em memória e arquivo */
    estadias[totalEstadias] = e;
    totalEstadias++;
    salvarEstadias();

    /* marcar quarto como ocupado (função vem de quarto.c) */
    marcarQuartoOcupado(numero);

    printf("Estadia cadastrada com sucesso! Quarto %d reservado.\n", numero);
}

/* finalizar estadia: calcular valor, liberar quarto e remover estadia da lista */
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
    double preco = obterPrecoQuarto(e->idQuarto);
    double total = preco * e->qtdDiarias;

    printf("\n--- FINALIZANDO ESTADIA ---\n");
    printf("Cliente: %d\n", e->codigoCliente);
    printf("Quarto: %d\n", e->idQuarto);
    printf("Diarias: %d\n", e->qtdDiarias);
    printf("Valor diaria: %.2f\n", preco);
    printf("TOTAL A PAGAR: R$ %.2f\n", total);

    /* libera quarto e remove estadia do vetor */
    marcarQuartoDesocupado(e->idQuarto);

    for (int i = index; i < totalEstadias - 1; i++)
        estadias[i] = estadias[i + 1];

    totalEstadias--;
    salvarEstadias();

    printf("Estadia finalizada e quarto liberado com sucesso!\n");
}

/* obter preco do quarto (usa array quartos[] do módulo quarto) */
double obterPrecoQuarto(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero)
            return quartos[i].diaria;
    }
    return 0;
}

/* listar todas as estadias */
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

/* listar estadias de um cliente (por código) */
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
        }
    }
    if (!encontrou) printf("\nNenhuma estadia encontrada para esse cliente.\n");
}

/* calcular pontos de fidelidade (10 pontos por diaria) */
int calcularPontosCliente(int codigoCliente) {
    int pontos = 0;
    for (int i = 0; i < totalEstadias; i++) {
        if (estadias[i].codigoCliente == codigoCliente) {
            pontos += estadias[i].qtdDiarias * 10;
        }
    }
    return pontos;
}
