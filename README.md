# 🚢 Verificação de Contêineres — Porto Seguro Systems

Este projeto simula o funcionamento de um **sistema de vistoria portuária**, desenvolvido como parte da disciplina de **Programação e Análise de Algoritmos**.

O objetivo é **comparar registros oficiais de contêineres com os dados obtidos em uma vistoria**, identificando **discrepâncias de CNPJ e peso líquido** por meio de uma estrutura de **tabela hash** e **algoritmos de ordenação**.

---

## 🎯 Problema

A empresa fictícia **Porto Seguro Systems** precisa validar informações de contêineres armazenados em seu banco de dados contra os dados coletados durante uma vistoria física.

O sistema deve:
* Detectar **diferenças de CNPJ** entre o registro e o verificado.
* Detectar **diferenças de peso líquido** superiores a **10%**.
* Classificar e exibir as discrepâncias em um formato organizado e eficiente.

---

## 📜 Regras do Problema

* A entrada contém:
  * O número total de **contêineres registrados**.
  * As linhas seguintes com os dados de cada contêiner registrado:
    ```
    <código> <cnpj> <peso_liquido>
    ```
  * Uma linha com o número de **contêineres verificados**.
  * As linhas seguintes com os dados verificados:
    ```
    <código> <cnpj> <peso_liquido_verificado>
    ```

* As verificações podem conter:
  * CNPJs incorretos;
  * Diferenças de peso (pequenas ou grandes);
  * Dados corretos (sem discrepância).

* O programa deve registrar apenas:
  * Contêineres com **CNPJs divergentes**;
  * Contêineres com **diferenças de peso acima de 10%**.

---

## 🗂️ Estrutura do Projeto

```
├── porto.c         # Código-fonte principal em C
├── entrada.txt         # Exemplo de arquivo de entrada
└── (gerado após execução) saida.txt   # Saída esperada com discrepâncias encontradas
```

---

## ⚙️ Como Executar

### 1️⃣ Compilar o programa

```bash
gcc porto.c -o porto -lm
```

### 2️⃣ Executar o simulador

```bash
./porto
```

O programa utiliza por padrão os arquivos:

* `entrada.txt` → dados de entrada
* `saida.txt` → resultado com pacotes ordenados

---

## 📥 Exemplo de Entrada

Arquivo: `entrada.txt`

```text
4
TRYJ3563622 63.905.021/4942-31 24326
KOWZ4870165 52.530.906/1018-93 21578
OIIZ2030046 51.663.047/6097-36 15578
UGPJ9389044 19.727.460/2907-10 24325
3
TRYJ3563622 63.905.021/4942-31 24326
OIIZ2030046 51.123.047/6840-36 15578
KOWZ4870165 52.530.906/1018-93 19204
```

**Significado:**

Cada linha final representa os dados verificados durante a vistoria.
* O primeiro número (`4`) representa o total de contêineres registrados.
* As próximas linhas contêm o código, CNPJ e peso líquido.
* O número seguinte (`3`) indica quantos contêineres foram vistoriados.
* Cada linha final representa os dados verificados durante a vistoria.

---

## 📤 Exemplo de Saída (saida.txt)

```text
OIIZ2030046:51.663.047/6097-36<->51.663.047/6097-36
KOWZ4870165:2374(11%)
```

**Significado:**

* OIIZ2030046 possui CNPJ divergente.
* KOWZ4870165 apresenta uma diferença de peso de 11%, sendo considerada significativa.
* TRYJ3563622 e UGPJ9389044 estão dentro dos limites aceitáveis e não aparecem na saída.

---

## 🧮 Estruturas e Algoritmos Utilizados

* **Tabela Hash** para busca eficiente de contêineres por código.
* **qsort** para ordenação de discrepâncias de CNPJ pela ordem de registro.
* **Merge Sort** personalizado para ordenação de discrepâncias de peso por percentual (decrescente).
Cálculo percentual de diferença de peso com arredondamento via **round()**.

---

## 🧠 Conceitos Envolvidos

* Estruturas de dados: **Tabela Hash**, **Listas Ligadas**
* Algoritmos de ordenação: **Merge Sort**, **Quick Sort**
* Manipulação de **arquivos** (`fopen`, `fscanf`, `fprintf`)
* Análise de diferenças percentuais
* Uso de alocação dinâmica de memória (`malloc`, `calloc`, `free`)
* Implementação de funções de `**hash** e **busca**

---

## 📊 Complexidade

* Inserção e busca na hash: **O(1)** em média
* Ordenação de discrepâncias:
  * **CNPJ**: O(n log n)
  * **Peso**: O(n log n)

---

## 👨‍💻 Autor

**Henrick Cardoso**

Projeto desenvolvido na disciplina de **Programação e Análise de Algoritmos** — 2025.
