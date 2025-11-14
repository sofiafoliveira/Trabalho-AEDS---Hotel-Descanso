#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stddef.h>

int salvar_dados(const char *nome_arquivo, const void *dados, size_t tamanho, int quantidade);
int carregar_dados(const char *nome_arquivo, void *dados, size_t tamanho, int quantidade);

#endif
