#include <stdio.h>
#include "arquivos.h"

int salvar_dados(const char *nome_arquivo, const void *dados, size_t tamanho, int quantidade) {
    FILE *f = fopen(nome_arquivo, "ab");
    if (!f) return 0;
    fwrite(dados, tamanho, quantidade, f);
    fclose(f);
    return 1;
}

int carregar_dados(const char *nome_arquivo, void *dados, size_t tamanho, int quantidade) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) return 0;
    fread(dados, tamanho, quantidade, f);
    fclose(f);
    return 1;
}
