/*
 * Cabeçalho para o trabalho de software básico
 * NÂO modifique esse arquivo
 */

// tipo ponteiro para função que recebe um inteiro e retorna um inteiro
typedef int (*funcp) (int x);

// protótipo da função que recebe um arquivo previamente aberto e preenche o vetor de bytes (char) com o código gerado
void gera_codigo (FILE *f, unsigned char code[], funcp *entry);