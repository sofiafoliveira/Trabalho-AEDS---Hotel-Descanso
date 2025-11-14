#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quarto.h"

#define ARQ_QUARTOS "quartos.bin"

Quarto quartos[100];
int totalQuartos = 0;

void salvarQuartos() {
    FILE *f = fopen(ARQ_QUARTOS, "wb");
    if (f == NULL) {
        printf("Erro ao salvar.\n");
        return;
    }
    fwrite(&totalQuartos, sizeof(int), 1, f);
    fwrite(quartos, sizeof(Quarto), totalQuartos, f);
    fclose(f);
}

void carregarQuartos() {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (f == NULL) return;

    fread(&totalQuartos, sizeof(int), 1, f);
    fread(quartos, sizeof(Quarto), totalQuartos, f);
    fclose(f);
}

void cadastrarQuarto() {
    Quarto q;

    printf("\n--- Cadastro de Quarto ---\n");
    q.id = totalQuartos + 1;

    printf("Tipo do quarto: ");
    fflush(stdin);
    fgets(q.tipo, 50, stdin);
    q.tipo[strcspn(q.tipo, "\n")] = 0;

    printf("Preco: ");
    scanf("%f", &q.preco);

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
        printf("ID: %d | Tipo: %s | Preco: %.2f | Ocupado: %s\n",
               quartos[i].id,
               quartos[i].tipo,
               quartos[i].preco,
               quartos[i].ocupado ? "Sim" : "Nao");
    }
}

void editarQuarto(int id) {
    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].id == id) {
            printf("Novo tipo: ");
            fflush(stdin);
            fgets(quartos[i].tipo, 50, stdin);
            quartos[i].tipo[strcspn(quartos[i].tipo, "\n")] = 0;

            printf("Novo preco: ");
            scanf("%f", &quartos[i].preco);

            salvarQuartos();
            printf("Quarto editado com sucesso!\n");
            return;
        }
    }
    printf("Quarto nao encontrado!\n");
}

void removerQuarto(int id) {
    int index = -1;

    for (int i = 0; i < totalQuartos; i++) {
        if (quartos[i].id == id) {
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
