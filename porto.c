#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura para armazenar contêineres registrados
typedef struct {
    char codigo[20];
    char cnpj[20];
    int peso;
    int indice;
} Container;

typedef struct {
    char codigo[20];
    char cnpjRegistrado[20];
    char cnpjVerificado[20];
    int indice;
} DiscrepanciaCNPJ;

typedef struct {
    char codigo[20];
    int pesoVerificado;
    int valorDiferenca;
    int percentualDiferenca;
    int indice;
} DiscrepanciaPeso;

// Estrutura para a tabela hash
typedef struct HashNode {
    char codigo[20];
    Container* container;
    struct HashNode* next;
} HashNode;

int calcPercentual(int pesoOriginal, int pesoAtual) {
    return (int)round(fabs((double)(pesoAtual - pesoOriginal) / pesoOriginal) * 100.0);
}

unsigned int hash(char* codigo, int hash_size) {
    unsigned int hash_value = 0;
    for (int i = 0; codigo[i] != '\0'; i++) {
        hash_value = hash_value * 31 + codigo[i];
    }
    return hash_value % hash_size;
}

void hash_insert(HashNode** hash_table, Container* container, int hash_size) {
    unsigned int index = hash(container->codigo, hash_size);
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    strcpy(new_node->codigo, container->codigo);
    new_node->container = container;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

Container* hash_search(HashNode** hash_table, char* codigo, int hash_size) {
    unsigned int index = hash(codigo, hash_size);
    HashNode* current = hash_table[index];
    while (current != NULL) {
        if (strcmp(current->codigo, codigo) == 0) {
            return current->container;
        }
        current = current->next;
    }
    return NULL;
}

// Função para ordenar discrepâncias de CNPJ por índice
int compareCNPJ(const void *a, const void *b) {
    return ((DiscrepanciaCNPJ*)a)->indice - ((DiscrepanciaCNPJ*)b)->indice;
}

// Ordenação por percentual e, em caso de empate, por índice de entrada
void merge(DiscrepanciaPeso arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    DiscrepanciaPeso leftArr[n1], rightArr[n2];
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].percentualDiferenca > rightArr[j].percentualDiferenca ||
           (leftArr[i].percentualDiferenca == rightArr[j].percentualDiferenca && leftArr[i].indice < rightArr[j].indice)) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

void mergesort(DiscrepanciaPeso arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergesort(arr, left, mid);
        mergesort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main(int argc, char* argv[]) {
    FILE *entrada = fopen("entrada.txt", "r");
    FILE *saida = fopen("saida.txt", "w");
    if (!entrada || !saida) return 1;

    int nContainers, nVerificados;
    fscanf(entrada, "%d", &nContainers);

    Container *containers = malloc(nContainers * sizeof(Container));
    HashNode** hash_table = calloc(nContainers, sizeof(HashNode*));
    DiscrepanciaCNPJ *discrepanciasCNPJ = malloc(nContainers * sizeof(DiscrepanciaCNPJ));
    DiscrepanciaPeso *discrepanciasPeso = malloc(nContainers * sizeof(DiscrepanciaPeso));

    for (int i = 0; i < nContainers; i++) {
        fscanf(entrada, "%s %s %d", containers[i].codigo, containers[i].cnpj, &containers[i].peso);
        containers[i].indice = i;
        hash_insert(hash_table, &containers[i], nContainers);
    }

    fscanf(entrada, "%d", &nVerificados);
    int cnpj_cont = 0, peso_cont = 0;

    for (int i = 0; i < nVerificados; i++) {
        char codigo[20], cnpjVerificado[20];
        int pesoVerificado;
        fscanf(entrada, "%s %s %d", codigo, cnpjVerificado, &pesoVerificado);
        Container* container = hash_search(hash_table, codigo, nContainers);
        if (container) {
            if (strcmp(container->cnpj, cnpjVerificado) != 0) {
                strcpy(discrepanciasCNPJ[cnpj_cont].codigo, codigo);
                strcpy(discrepanciasCNPJ[cnpj_cont].cnpjRegistrado, container->cnpj);
                strcpy(discrepanciasCNPJ[cnpj_cont].cnpjVerificado, cnpjVerificado);
                discrepanciasCNPJ[cnpj_cont].indice = container->indice;
                cnpj_cont++;
            } else {
                int diffPercentual = calcPercentual(container->peso, pesoVerificado);
                if (diffPercentual > 10) {
                    strcpy(discrepanciasPeso[peso_cont].codigo, codigo);
                    discrepanciasPeso[peso_cont].pesoVerificado = pesoVerificado;
                    discrepanciasPeso[peso_cont].valorDiferenca = abs(container->peso - pesoVerificado);
                    discrepanciasPeso[peso_cont].percentualDiferenca = diffPercentual;
                    discrepanciasPeso[peso_cont].indice = container->indice;
                    peso_cont++;
                }
            }
        }
    }

    qsort(discrepanciasCNPJ, cnpj_cont, sizeof(DiscrepanciaCNPJ), compareCNPJ);
    for (int i = 0; i < cnpj_cont; i++) {
        fprintf(saida, "%s:%s<->%s\n", discrepanciasCNPJ[i].codigo, discrepanciasCNPJ[i].cnpjRegistrado, discrepanciasCNPJ[i].cnpjVerificado);
    }
    mergesort(discrepanciasPeso, 0, peso_cont - 1);
    for (int i = 0; i < peso_cont; i++) {
        fprintf(saida, "%s:%dkg(%d%%)\n", discrepanciasPeso[i].codigo, discrepanciasPeso[i].valorDiferenca, discrepanciasPeso[i].percentualDiferenca);
    }

    fclose(entrada); fclose(saida);
    return 0;
}