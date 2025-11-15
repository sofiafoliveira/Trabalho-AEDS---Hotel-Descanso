#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quarto.h"

#define ARQ_QUARTOS "quartos.bin"

Quarto quartos[100];
int totalQuartos = 0;

void salvarQuartos() {
    FILE *f = fopen(ARQ_QUARTOS, "wb");
    if (!f) {
        printf("Erro ao salvar.\n");
        return;
    }
    fwrite(&totalQuartos, sizeof(int), 1, f);
    fwrite(quartos, sizeof(Quarto), totalQuartos, f);
    fclose(f);
}

void carregarQuartos() {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) return;

    fread(&totalQuartos, sizeof(int), 1, f);
    fread(quartos, sizeof(Quarto), totalQuartos, f);
    fclose(f);
}

Quarto* buscarQuarto(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero)
            return &quartos[i];
    }
    return NULL;
}

int quartoDisponivel(int numero) {
    Quarto *q = buscarQuarto(numero);
    if (!q) return 0;
    return q->ocupado == 0;
}

void cadastrarQuarto() {
    Quarto q;

    printf("\n--- Cadastro de Quarto ---\n");

    printf("Numero do quarto: ");
    scanf("%d", &q.numero);

    if (buscarQuarto(q.numero) != NULL) {
        printf("Erro: Ja existe um quarto com esse numero!\n");
        return;
    }

    printf("Capacidade do quarto: ");
    scanf("%d", &q.capacidade);

    printf("Valor da diaria: ");
    scanf("%f", &q.diaria);

    q.ocupado = 0;

    quartos[totalQuartos] = q;
    totalQuartos++;

    salvarQuartos();

    printf("Quarto cadastrado com sucesso!\n");
}

void listarQuartos() {
    if (totalQuartos == 0) {
        printf("\nNenhum quarto cadastrado!\n");
        return;
    }

    printf("\n--- Lista de Quartos ---\n");
    for (int i = 0; i < totalQuartos; i++) {
        printf("Numero: %d | Capacidade: %d | Diaria: %.2f | Status: %s\n",
               quartos[i].numero,
               quartos[i].capacidade,
               quartos[i].diaria,
               quartos[i].ocupado ? "Ocupado" : "Desocupado");
    }
}

void editarQuarto(int numero) {
    Quarto *q = buscarQuarto(numero);
    if (!q) {
        printf("Quarto nao encontrado!\n");
        return;
    }

    printf("Nova capacidade: ");
    scanf("%d", &q->capacidade);

    printf("Novo valor da diaria: ");
    scanf("%f", &q->diaria);

    salvarQuartos();
    printf("Quarto editado com sucesso!\n");
}

void removerQuarto(int numero) {
    int index = -1;

    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Quarto nao encontrado!\n");
        return;
    }

    for (int i = index; i < totalQuartos - 1; i++) {
        quartos[i] = quartos[i + 1];
    }

    totalQuartos--;
    salvarQuartos();

    printf("Quarto removido com sucesso!\n");
}

int encontrarQuartoDisponivel(int qtdHospedes) {
    for (int i = 0; i < totalQuartos; i++) {

        if (quartos[i].ocupado) continue;
        if (quartos[i].capacidade < qtdHospedes) continue;

        return quartos[i].numero;
    }
    return -1;
}

void marcarQuartoOcupado(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero) {
            quartos[i].ocupado = 1;
            salvarQuartos();
            return;
        }
    }
}

void marcarQuartoDesocupado(int numero) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].numero == numero) {
            quartos[i].ocupado = 0;
            salvarQuartos();
            return;
        }
    }
}
