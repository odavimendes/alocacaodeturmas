# Alocação de Turmas

Este projeto realiza a modelagem de horários escolares utilizando o algoritmo de coloração de grafos Welsh-Powell. O objetivo é alocar turmas em horários e salas, evitando conflitos de uso simultâneo.

Este projeto foi desenvolvido como trabalho acadêmico para a disciplina de **Introcução à Teoria dos Grafos** do curso de Ciência da Computação no IFNMG.


## Estrutura de Pastas

- `src/`: Contém o código-fonte principal do programa.
- `data/`: Armazena arquivos de entrada, como listas de turmas, professores e salas.
- `output/`: Guarda os resultados gerados após a execução, como horários alocados.
- `bin/`: Guarda o executável do programa.

## Como Usar

### 1. Preparar o arquivo de dados

Crie um arquivo CSV no caminho `data/turmas.csv` com a seguinte estrutura:
```csv
turma;materia
1AA;Materia 1
1AA;Materia 2
1AA;Materia 3
2AA;Materia 1
2AA;Materia 2
2AA;Materia 3
```

> **Importante:** Respeite os limites de caracteres definidos no código:
> - **Nome da turma** (primeira coluna): máximo de **3 caracteres**
> - **Nome da matéria** (segunda coluna): máximo de **49 caracteres**
> 
> Estes limites são definidos pelas constantes no `main.c`:
> ```c
> #define TAM_TURMA 4
> #define TAM_MATERIA 50
> ```
>> **Observação:** O código ainda não está preparado para lidar com múltiplos professores lecionando uma matéria ou multiplas matérias lecionadas por um professor. Nestes caso recomendo, ou colocar o nome dos professores no lugar da matéria, ou numerar as matérias para diferenciá-las.

### 2. Compilar e executar o programa

**Compilar o código:**
```bash
gcc src/main.c -o bin/alocacaodeturmas
```

**Executar o programa:**
```bash
cd bin
./alocacaodeturmas
```

3. A grade de horários resultante será exportada para `output/grade_horarios.csv`.
Também será gerado um arquivo `output/grafo.dot` com o grafo resultante.
> **Acesse** [GraphvizOnline](https://dreampuf.github.io/GraphvizOnline) e cole o conteúdo de `grafo.dot` no site para visualizar o grafo ou execute em sua máquina utilizando o [Graphviz](https://graphviz.org/).

