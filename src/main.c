#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
    #define SLEEP(x) Sleep(1000 * (x))
#else
    #include <unistd.h>
    #define CLEAR "clear"
    #define SLEEP(x) sleep(x)
#endif

#define TAM_TURMA 4
#define TAM_MATERIA 50
#define MAX_CORES 4

/* Estruturas para o grafo */
typedef struct Vizinhanca{
    int id; // ID referente ao vértice
    struct vertice *atual; // Endereço do vértice
    struct Vizinhanca *proximo; // Endereço do próximo
    struct Vizinhanca *anterior; // Endereço do anterior
} Vizinhanca;

typedef struct vertice{
    int id; 
    int grau;
    char turma[TAM_TURMA]; //Restrição
    char materia[TAM_MATERIA]; //Restrição
    int cor;
    struct Vizinhanca *vizinhos; // Lista encadeada da vizinhança
} Vertice;

typedef struct{
    int id; //Id referente a Vertice
    int grau;
    Vertice *endereco; // Endereço do vértice no grafo
} Graus;
/* END estruturas */

/* Protótipos */

void creditos();
int preencherDados(Vertice **grafo, int *tam, int *posicao_livre, Graus **verticesGrau, const char* caminho_importacao); // Dados simulados
int carregarCsv(const char* filename, char*** p_vetor_turmas, char*** p_vetor_materias); // IA para me ajudar carregar os dados
void visualizarVerticecs(Vertice *grafo, int tam); // Visualiza os vértices
void quickSort(Graus *vet, int ini, int fim); //Ordena o vetor de graus
int definirGrau(Vertice **grafo, int tam, Graus *verticesGrau); //Define os graus dos vértices
void aumentarGrausGrau(int id, int tam, Graus *grau); //Aumenta o grau do vértice no vetor de graus
void liberarGrafo(Vertice *grafo, Graus *verticesGrau, int tam_grafo); //Libera todos os dados alocados dinamicamente
void visualizarGraus(Graus *verticesGrau, int tam); // Visualiza os graus dos vértices
int colorirGrafo(Vertice *grafo, int tam, Graus *verticesGrau); //Defini a cor com base no algorítimo Welsh-Powell
void exportarGraphviz(Vertice *grafo, int tam, const char* caminho_exportacao); //Exporta o grafo para o formato Graphviz
void exportarGradeHorariosCSV(Vertice *grafo, int tam, int max_horario, const char* caminho_exportacao); //Exporta a grade de horários para CSV

/* End Protótipos */

int main(){
    /*Para substituir o main*/
    Vertice *grafo; // O grafo será alocado dinamicamente
    Graus *verticesGrau;
    int tam_grafo = 0;
    int posicao_livre = 0; // Primeira posição livre
    int validacao_dados = 0;
    int opcao;
    int max_cor;
    // Caminhos dos arquivos
    const char* caminho_importacao= "../data/dados.csv";
    const char* caminho_exportacao= "../output/grade_horarios.csv";
    const char* caminho_graphviz= "../output/grafo.dot";
    // Fim caminhos dos arquivos
    creditos();
    printf("\nVizualize os dados exportados na pasta 'output' apos a execucao\n\n");
    printf("Iniciando em 5 segundos...\n");
    SLEEP(5);
    system(CLEAR);
    printf("Iniciando execucao...\n\n");
    printf("Alocando grafo...\n\n");
    /* Alocar uma posição ao grafo*/
    grafo = (Vertice *)malloc(tam_grafo*sizeof(Vertice)); //Alocar 1 posição inicial para o grafo
    if(grafo == NULL){
        printf("Erro de alocação do grafo, encerrando aplicação...\n");
        return 0;
    }
    /* Alocar uma posição ao vetor VerticeGrau*/
    verticesGrau = (Graus *)malloc(tam_grafo*sizeof(Graus)); //Alocar 1 posição inicial para o vetor
    if(grafo == NULL){
        printf("Erro de alocação do vetor de Graus, encerrando aplicação...");
        return 0;
    }
    printf("Iniciado!\n");
    system(CLEAR);
    /* Menu de Opções*/
    do{
        printf("Sistema de Alocacao de Horarios - github.com/odavimendes\n\n");
        printf("Menu de Opcoes:\n");
        printf("1 - Executar automaticamente\n");
        printf("2 - Carregar Dados\n");
        printf("3 - Definir Horarios\n");
        printf("4 - Visualizar Turmas\n");
        printf("5 - Visualizar Graus\n");
        printf("6 - Exportar para Graphviz\n");
        printf("7 - Exportar Grade de Horarios CSV\n");
        printf("8 - Creditos\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%i",&opcao);
        system(CLEAR);
        switch(opcao){
            case 1://Execução automática
                //Carregar dados
                validacao_dados = preencherDados(&grafo,&tam_grafo,&posicao_livre,&verticesGrau, caminho_importacao);
                if(validacao_dados){
                    printf("Dados carregados na memoria!\n\n");
                }
                else
                    printf("Nao ha dados!\n\n");
                //Definir Conflitos
                if(definirGrau(&grafo,tam_grafo,verticesGrau)){
                    printf("Conflitos Definidos!\n\n");
                } else{
                    printf("Falha na definição dos conflitos!\n\n");
                }
                if(verticesGrau[0].grau > MAX_CORES){
                    printf("ATENCAO: o numero maximo de horarios definido pelo programa nao eh suficiente para os dados definidos devido a quantidade de conflitos\n");
                    printf("Sujestoes: Aumentar o numero de professores ou diminuir a quantidade de turmas\n\n");
                    SLEEP(3);
                }
                //Colorir Grafo
                max_cor = colorirGrafo(grafo,tam_grafo,verticesGrau);
                if(max_cor>MAX_CORES){
                    printf("\nATENCAO: o numero de horarios minimo definido pelo programa foi %i, alem do limite definido de horarios (%i)\n",max_cor,MAX_CORES);
                    printf("Confira os vertices para analisar mudancas necessarias\n");
                    printf("Sujestoes: Aumentar o numero de professores ou diminuir a quantidade de turmas\n\n");
                    SLEEP(5);
                }
                //Visualizar Vértices
                printf("Grafo colorido!\n\n");
                visualizarVerticecs(grafo,tam_grafo);
                exportarGraphviz(grafo,tam_grafo,caminho_graphviz);
                printf("Arquivo grafo.dot exportado para visualizacao no Graphviz.\n\n");
                //Exportar Grade de Horários
                exportarGradeHorariosCSV(grafo,tam_grafo,max_cor,caminho_exportacao);
                break;               

            case 2: //Carregar dados
                validacao_dados = preencherDados(&grafo,&tam_grafo,&posicao_livre,&verticesGrau,caminho_importacao);
                if(validacao_dados){
                    printf("Dados carregados na memoria!\n\n");
                }
                else
                    printf("Nao ha dados!\n\n");
                if(definirGrau(&grafo,tam_grafo,verticesGrau)){
                    printf("Conflitos Definidos!\n\n");
                } else{
                    printf("Falha na definição dos conflitos!\n\n");
                }
                if(verticesGrau[0].grau > MAX_CORES){
                    printf("ATENCAO: o numero maximo de horarios definido pelo programa nao eh suficiente para os dados definidos devido a quantidade de conflitos\n");
                    printf("Sujestoes: Aumentar o numero de professores ou diminuir a quantidade de turmas\n\n");
                }
                break;
            case 3: //Colorir Grafo
                if(!validacao_dados){
                    printf("Nenhum dado carregado!\n\n");
                    break;
                }
                max_cor = colorirGrafo(grafo,tam_grafo,verticesGrau);
                printf("Grafo colorido!\n\n");
                if(max_cor>MAX_CORES){
                    printf("\nATENCAO: o numero de horarios minimo definido pelo programa foi %i, alem do limite definido de horarios (%i)\n",max_cor,MAX_CORES);
                    printf("Confira os vertices para analisar mudancas necessarias\n");
                    printf("Sujestoes: Aumentar o numero de professores ou diminuir a quantidade de turmas\n\n");
                }
                break;
            case 4: //Visualizar Vértices
                if(!validacao_dados){
                    printf("Nenhum dado carregado!\n\n");
                    break;
                }
                visualizarVerticecs(grafo,tam_grafo);
                break;
            case 5: //Visualizar Graus
                if(!validacao_dados){
                    printf("Nenhum dado carregado!\n\n");
                    break;
                }
                visualizarGraus(verticesGrau,tam_grafo);
                break;
            case 6://Exportar para Graphviz
                if(!validacao_dados){
                    printf("Nenhum dado carregado!\n\n");
                    break;
                }
                exportarGraphviz(grafo,tam_grafo,caminho_graphviz);
                break;
            case 7://Exportar Grade de Horários CSV
                if(!validacao_dados){
                    printf("Nenhum dado carregado!\n\n");
                    break;
                }
                exportarGradeHorariosCSV(grafo,tam_grafo,max_cor,caminho_exportacao);
                break;
            case 8: //Créditos
                creditos();
                break;
            case 0: //Sair
                printf("Finalizando execucao!\n\n");
                liberarGrafo(grafo,verticesGrau,tam_grafo);
                break;
            default:
                printf("Opcao invalida!\n\n");
                break;
        }
    }while(opcao != 0);
    
    return 0;
}

void creditos(){
   printf("github.com/\n");
   printf(
"                  ░██                       ░██                                             ░██                       \n"
"                  ░██                                                                       ░██                       \n"
" ░███████   ░████████  ░██████   ░██    ░██ ░██░█████████████   ░███████  ░████████   ░████████  ░███████   ░███████  \n"
"░██    ░██ ░██    ░██       ░██  ░██    ░██ ░██░██   ░██   ░██ ░██    ░██ ░██    ░██ ░██    ░██ ░██    ░██ ░██        \n"
"░██    ░██ ░██    ░██  ░███████   ░██  ░██  ░██░██   ░██   ░██ ░█████████ ░██    ░██ ░██    ░██ ░█████████  ░███████  \n"
"░██    ░██ ░██   ░███ ░██   ░██    ░██░██   ░██░██   ░██   ░██ ░██        ░██    ░██ ░██   ░███ ░██               ░██ \n"
" ░███████   ░█████░██  ░█████░██    ░███    ░██░██   ░██   ░██  ░███████  ░██    ░██  ░█████░██  ░███████   ░███████  \n"
"                                                                                                                      \n"
"                                                                                                                      \n"
"                                                                                                                      \n"
);
}


void visualizarVerticecs(Vertice *grafo, int tam){
    Vizinhanca *atual;
    printf("Lista de Vertices:\n\n");
    for(int i = 0; i<tam; i++){
        printf("##########################\n");
        printf("%s - %s\n",grafo[i].turma,grafo[i].materia);
        printf("Cor: %i | Grau: %i\n",grafo[i].cor,grafo[i].grau);
        printf("Conflitos com:\n");
        //printf("--------------------------\nID: %i\nMateria: %s\nTurma: %s\nGrau: %i\nConflitos:\n",grafo[i].id,grafo[i].materia,grafo[i].turma,grafo[i].grau);
        atual = grafo[i].vizinhos;
        while(atual != NULL){
            printf("ID: %i | Turma: %s | Materia: %s | Horario: %i\n",atual->id,atual->atual->turma,atual->atual->materia,atual->atual->cor);
            //printf("ID: %i | Turma: %s | Materia: %s\n",atual->id,atual->atual->turma,atual->atual->materia);
            atual = atual->proximo;
        }
        printf("\n");
    }
}


/*  Carrega os dados a partir da função carregarCsv, que devolve vetores com os dados de turmas, materias e o tamanho
    Realoca o tamanho do grafo e do vetor de graus, após isso ele carrega os dados coletados do CSV em cada vértice  */
int preencherDados(Vertice **grafo, int *tam, int *posicao_livre, Graus **verticesGrau, const char* caminho_importacao){
    char **turmas = NULL;
    char **materias = NULL;
    int v = 0; // Número de vértices
    printf("\nCarregando arquivos...\n");
    v = carregarCsv(caminho_importacao,&turmas,&materias); //Importando dados do CSV
    printf("%i aulas carregadas na memoria.\n\n",v);
    if(v == 0)
        return 0;
    /* Visualizar os dados importados */
    printf("Dados carregados: \n");
    for(int i = 0; i<v; i++){
        printf("%i: Turma: %s | Materia: %s;\n",i+1,turmas[i],materias[i]);
    };
    /**/
    *tam += v;
    /* Realocando tamanho do grafo */
    printf("\nRealocando Grafo...\n");
    Vertice *temp = (Vertice *) realloc(*grafo,sizeof(Vertice)*(*tam));
    if(temp == NULL)
    {
        printf("Erro de alocação Grafo\n\n");
        return 0;
    }
    *grafo = temp;
    /* END realocação */
    printf("Realocando Vetor...\n");
    /* Realocando tamanho do Vetor */
    Graus *tempVet = (Graus *) realloc(*verticesGrau,sizeof(Graus)*(*tam));
    if(tempVet == NULL)
    {
        printf("Erro de alocação Vetor\n\n");
        return 0;
    }
    *verticesGrau = tempVet;
    /* END realocação */
    printf("Carregando aulas nos vertices...\n");
    /* Carregando dados nos vértices */
    for(int i = 0; i<v; i++){
        (*grafo)[*posicao_livre+i].id = *posicao_livre+i;
        strcpy((*grafo)[*posicao_livre+i].materia,materias[i]);
        strcpy((*grafo)[*posicao_livre+i].turma,turmas[i]);
        /* Estes dados abaixo ainda serão processados por outras funções */
        (*grafo)[*posicao_livre+i].cor = 0;
        (*grafo)[*posicao_livre+i].grau = 0;
        (*grafo)[*posicao_livre+i].vizinhos = NULL;
        (*verticesGrau)[*posicao_livre+i].grau = (*grafo)[*posicao_livre+i].grau;
        (*verticesGrau)[*posicao_livre+i].id = (*grafo)[*posicao_livre+i].id;
        (*verticesGrau)[*posicao_livre+i].endereco = &(*grafo)[*posicao_livre+i];
        //printf("Posicao: %i; ID: %i\n",*posicao_livre+i,(*verticesGrau)[*posicao_livre+i].id);
    }
    *posicao_livre += v; //Atualizando ultima posição livre
    printf("Aulas carregadas nos vertices.\n");
    /* Liberando memória das matrizes */
    printf("Liberando memoria das matrizes...\n");
    for (int i = 0; i < v; i++) {
        free(turmas[i]);
        free(materias[i]);
    }
    // Depois, libera os vetores principais
    free(turmas);
    free(materias);
    printf("Memoria liberada.\n\n");
    return 1;
    /* end Liberando memória das matrizes*/
}


/* Ordena do maior para o menor usando o algorítimo QuickSort */
void quickSort(Graus *vet, int ini, int fim){
    int i,j;
    Graus meio, aux;
    i = ini;
    j = fim;
    meio = vet[(i+j)/2];
    while(i<=j){
        while(vet[i].grau>meio.grau){
            i++;
        }
        while(vet[j].grau<meio.grau){
            j--;
        }
        if(i<=j){
            //printf("Troca: %i %i\n",(*vet)[i],(*vet)[j]);
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
        if(ini<j){
            quickSort(vet,ini,j);
        }
        if(i<fim){
            quickSort(vet,i,fim);
        }
    }
}

/*  Passa por todos os vértices e compara os dados de turmas e materias para definir o grau.
    Ao validar que há conflito entre dois vertices, a função chama aumentarGrausGrau para aumentar
    o grau do ID i */
/*  !!!! Essa função não está perfeita, pois não está levando em consideração ser chamada uma segunda vez
    dentro de uma única execução do programa, Se der tempo vou resolver, mas vai ficar para o final. */
int definirGrau(Vertice **grafo, int tam, Graus *verticesGrau){
    char temp_turma[TAM_TURMA];
    char temp_materia[TAM_MATERIA];
    /* Passo por todos os vértices e comparo com todos os outros. Se uma restrição bater virar aresta */
    for(int i = 0; i<tam; i++){
        // Cria a lista ligada de vizinhos e atribui valores nulos
        Vizinhanca *vizinhosI;
        if((*grafo)[i].vizinhos == NULL){
            vizinhosI = (Vizinhanca *)malloc(sizeof(Vizinhanca));
            if(vizinhosI == NULL){
                printf("Erro malloc\n\n");
                return 0;
            }
            vizinhosI->anterior = NULL;
            vizinhosI->proximo = NULL;
            vizinhosI->id = -1;
            vizinhosI->atual = NULL;

            (*grafo)[i].vizinhos = vizinhosI; // Atribui o endereço do primeiro
        }
        else{
            vizinhosI = (*grafo)[i].vizinhos;
        }
        strcpy(temp_materia,(*grafo)[i].materia);
        strcpy(temp_turma,(*grafo)[i].turma);

        for(int j = 0; j<tam; j++){
            if(j==i){
                continue; //Como não é digrafo, então se for o mesmo vertice pula
            }
            /* Se há conflito */
            if(strcmp(temp_materia,(*grafo)[j].materia) == 0 || strcmp(temp_turma,(*grafo)[j].turma) == 0){
                (*grafo)[i].grau++;
                vizinhosI = (*grafo)[i].vizinhos;

                if(vizinhosI->id == -1){
                    /* Se é a primeira posição, já define o grau */
                    vizinhosI->id = (*grafo)[j].id;
                    vizinhosI->atual = &(*grafo)[j];
                    aumentarGrausGrau((*grafo)[i].id,tam,verticesGrau);
                } else{
                    while(vizinhosI->proximo != NULL){
                            // vai avançar na lista enquanto tiver próximos
                            vizinhosI = vizinhosI->proximo; 
                    }
                    Vizinhanca *novo = (Vizinhanca *)malloc(sizeof(Vizinhanca));
                    vizinhosI->proximo = novo;
                    novo->id = (*grafo)[j].id;
                    novo->atual = &(*grafo)[j];
                    novo->anterior = vizinhosI;
                    novo->proximo = NULL;
                    aumentarGrausGrau((*grafo)[i].id,tam,verticesGrau);
                }
                  
            }
        }
    }
    return 1;
}



/* Essa função vai identificar o ID no vetor de graus e aumentar +1 no grau
   Foi criada pois pode haver a possibilidade de não está ordenado igual ao grafo
   Caso for a segunda vez inserindo dados dentro da mesma execução (pois já passou pelo QuickSort) */
void aumentarGrausGrau(int id, int tam, Graus *grau){
    for(int i = 0; i<tam; i++){
        if(grau[i].id != id)
            continue;
        grau[i].grau++;
        return;
    }
}

/* Essa função apenas libera os dados alocados dinamicamente pelo programa*/
void liberarGrafo(Vertice *grafo, Graus *verticesGrau, int tam_grafo){
    if (grafo == NULL && verticesGrau == NULL) {
        return; // Nada para liberar
    }

    //Libera as listas de Vizinhanca e, em seguida, o array de Vertices
    if (grafo != NULL) {
        for (int i = 0; i < tam_grafo; i++) {

            if (grafo[i].vizinhos != NULL) {
                Vizinhanca *atual = grafo[i].vizinhos;
                Vizinhanca *proximo_no;
                // Percorre e libera todos os nós da lista de vizinhança
                while (atual != NULL) {
                    proximo_no = atual->proximo;
                    // Libera o nó da vizinhança
                    free(atual); 
                    atual = proximo_no;
                }
                // Zera o ponteiro no vértice
                grafo[i].vizinhos = NULL; 
            }
        }

        //Libera o array principal de Vértices
        free(grafo);
    }
    //Libera o array de Graus
    if (verticesGrau != NULL) {
        free(verticesGrau);
    }
}

/*  Visualizar o vetor de graus para conferência */
void visualizarGraus(Graus *verticesGrau, int tam) {
    printf("Lista de Graus:\n");
    for (int i = 0; i < tam; i++) {
        printf("ID: %d | Grau: %d\n", verticesGrau[i].id, verticesGrau[i].grau);
    }
    printf("\n");
}

/*  Aqui funciona a pricipal parte do programa: Definir os horários.
    utilizei o algorítimo Welsh-Powell que foi publicado no jornal The Computer Journal em 1967
    O algorítimo colore o vértice com maior grau (que já foi ordenano pelo quicksort) e colore com a primeira cor disponível 
    (em ordem crescente). Após isso ele precorre o vetor de graus em ordem decrescente e colore todos os vértices possíveis com essa
    mesma cor. Após percorrer toda a lista de vértices, ele passa para a próxima cor disponível e repete o processo até todos
    os vértices estiverem coloridos. A função vai retornar o número de cores utilizadas.*/
int colorirGrafo(Vertice *grafo, int tam, Graus *verticesGrau){
    //Algorítimo Welsh-Powell
    int cor_atual = 1; //Começa com a cor 1
    int vertices_coloridos = 0;
    int *colorido = (int *)calloc(tam, sizeof(int)); //Vetor que mostra quais já foram coloridos; Calloc para iniciar com 0
    if(colorido == NULL){
        printf("Erro de alocação memoria vetor\n\n");
        return 0;
    }
    while(vertices_coloridos < tam){
        for(int i = 0; i<tam; i++){
            int id_atual = verticesGrau[i].id;
            if(colorido[id_atual] == 1){
                continue; // Já está colorido
            }
            // Verifica se pode ser colorido com a cor atual
            Vizinhanca *vizinho = grafo[id_atual].vizinhos;
            int pode_colorir = 1;
            while(vizinho != NULL){
                int id_vizinho = vizinho->id;
                if(grafo[id_vizinho].cor == cor_atual){
                    pode_colorir = 0; // Não pode ser colorido com esta cor
                    break;
                }
                vizinho = vizinho->proximo;
            }
            if(pode_colorir){
                grafo[id_atual].cor = cor_atual;
                colorido[id_atual] = 1; // Marca como colorido
                vertices_coloridos++;
            }
        }
        cor_atual++; // Muda para a próxima cor
    }
    free(colorido);
    return cor_atual - 1; // Retorna o número de cores usadas
}

/* ----------------------------------------------------------------------------------------- */
/* FUNÇÕES CRIADAS COM AUXÍLIO DE IA (funções que não fazem parte da lógica principal) */
            /* IMPORTAÇÃO,EXPORTAÇÃO E MANIPULAÇÃO DE ARQUIVOS */
/* ----------------------------------------------------------------------------------------- */

/* Exporta o grafo para o formato Graphviz (.dot) */
void exportarGraphviz(Vertice *grafo, int tam, const char* caminho_exportacao) {
    const char* cores[] = {
        "gray",    // 0 (Cor padrão/não colorido)
        "red",     // 1
        "green",   // 2
        "blue",    // 3
        "yellow",  // 4
        "purple",  // 5
        "orange",  // 6
        "cyan",    // 7
        "pink",    // 8
        "lightgreen", // 9
        "lightblue" // 10
        // Adicione mais cores se precisar
    };
    // Calcula o número de cores no array
    int num_cores = sizeof(cores) / sizeof(cores[0]);

    FILE *arquivo = fopen(caminho_exportacao, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo Graphviz\n");
        return;
    }

    fprintf(arquivo, "strict graph {\n");
    
    // Define uma configuração global para os nós (vértices)
    // 'style=filled' é essencial para que a cor de preenchimento funcione
    fprintf(arquivo, "    node [style=filled];\n\n");

    // --- PARTE 1: DEFINIR TODOS OS NÓS (VÉRTICES) E SUAS CORES ---
    for (int i = 0; i < tam; i++) {
        Vertice v = grafo[i];
        int cor_idx = v.cor;

        // Garante que o índice da cor seja válido
        if (cor_idx < 0 || cor_idx >= num_cores) {
            cor_idx = 0; // Usa a cor padrão (cinza)
        }

        // Sintaxe para definir um NÓ:
        // "ID_do_Nó" [atributo1=valor, atributo2=valor];
        // 
        // Usamos 'fillcolor' para a cor de preenchimento (a "bolinha")
        // Usamos 'label' para o texto que aparece dentro dela
        fprintf(arquivo, "    \"%s-%s\" [label=\"Turma: %s\\nMateria: %s\\n(Cor: %d)\", fillcolor=%s];\n",
                v.turma, v.materia,  // Este é o ID único do nó
                v.turma, v.materia, v.cor, // Este é o texto do label
                cores[cor_idx]);     // Esta é a cor de preenchimento
    }

    fprintf(arquivo, "\n    // --- ARESTAS (CONFLITOS) ---\n");

    // --- PARTE 2: DEFINIR TODAS AS ARESTAS (CONEXÕES) ---
    for (int i = 0; i < tam; i++) {
        Vizinhanca *vizinho = grafo[i].vizinhos;
        while (vizinho != NULL) {
            
            // Para evitar duplicatas, só desenha a aresta se o ID do vizinho for maior
            if (vizinho->id > grafo[i].id) {
                
                // Sintaxe para definir uma ARESTA (sem cor):
                // "ID_Nó_A" -- "ID_Nó_B";
                fprintf(arquivo, "    \"%s-%s\" -- \"%s-%s\";\n",
                        grafo[i].turma, grafo[i].materia,
                        vizinho->atual->turma, vizinho->atual->materia);
            }
            vizinho = vizinho->proximo;
        }
    }

    fprintf(arquivo, "}\n");
    fclose(arquivo);

    printf("Arquivo 'grafo.dot' gerado com sucesso!\n");
}

/* Importar dados de um CSV */
int carregarCsv(const char* filename, char*** p_vetor_turmas, char*** p_vetor_materias) {

    // --- Etapa 1: Abrir o arquivo e contar as linhas de dados ---
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    char linha[256];
    
    // Pular a primeira linha (cabeçalho)
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        printf("Arquivo vazio ou erro ao ler cabecalho.\n");
        fclose(arquivo);
        return 0;
    }

    int n_linhas = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Ignora linhas em branco
        if (linha[0] != '\n' && linha[0] != '\0') {
            n_linhas++;
        }
    }

    if (n_linhas == 0) {
        printf("Nenhum dado encontrado apos o cabecalho.\n");
        fclose(arquivo);
        return 0;
    }

    // --- Etapa 2: Alocar memória para os vetores principais (char**) ---
    // Note que não estamos usando 'p_vetor_turmas' ainda,
    // estamos criando ponteiros locais primeiro.
    char** turmas_ptr = (char**) malloc(n_linhas * sizeof(char*));
    char** materias_ptr = (char**) malloc(n_linhas * sizeof(char*));

    if (turmas_ptr == NULL || materias_ptr == NULL) {
        perror("Erro ao alocar memoria para os vetores principais");
        free(turmas_ptr); // free(NULL) é seguro
        free(materias_ptr);
        fclose(arquivo);
        exit(1); // Erro crítico de memória
    }

    // --- Etapa 3: Reler o arquivo e preencher os dados ---
    rewind(arquivo); // Volta ao início do arquivo
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho novamente

    int i = 0;
    const char* delimitador = ";";
    
    while (i < n_linhas && fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Ignora linhas em branco
        if (linha[0] == '\n' || linha[0] == '\0') {
            continue;
        }

        // Remove o '\n' ou '\r\n' do final da linha
        linha[strcspn(linha, "\r\n")] = '\0';

        // 1. Extrair a Turma
        char *token = strtok(linha, delimitador);
        if (token == NULL) continue; // Linha mal formatada

        // Aloca memória para a string da turma e copia
        turmas_ptr[i] = (char*) malloc(strlen(token) + 1);
        if (turmas_ptr[i] == NULL) { exit(1); /* Erro de memória */ }
        strcpy(turmas_ptr[i], token);

        // 2. Extrair a Matéria
        token = strtok(NULL, delimitador);
        if (token == NULL) { // Linha mal formatada (só tinha uma coluna)
             free(turmas_ptr[i]); // Libera o que acabou de alocar
             continue; 
        }

        // Aloca memória para a string da matéria e copia
        materias_ptr[i] = (char*) malloc(strlen(token) + 1);
        if (materias_ptr[i] == NULL) { exit(1); /* Erro de memória */ }
        strcpy(materias_ptr[i], token);

        i++;
    }

    fclose(arquivo);

    // --- Etapa 4: "Devolver" os vetores alocados para o chamador ---
    // Atribui os ponteiros locais (turmas_ptr) aos ponteiros do chamador
    // usando a indireção (*p_vetor_turmas)
    *p_vetor_turmas = turmas_ptr;
    *p_vetor_materias = materias_ptr;

    return i; // Retorna o número real de itens lidos
}

/*  Exportar a tabela de horários */
void exportarGradeHorariosCSV(Vertice *grafo, int tam, int max_horario, const char* caminho_exportacao){
    // Descobrir todas as turmas únicas
    char turmas_unicas[tam][TAM_TURMA];
    int num_turmas = 0;
    for (int i = 0; i < tam; i++) {
        int existe = 0;
        for (int j = 0; j < num_turmas; j++) {
            if (strcmp(grafo[i].turma, turmas_unicas[j]) == 0) {
                existe = 1;
                break;
            }
        }
        if (!existe) {
            strcpy(turmas_unicas[num_turmas], grafo[i].turma);
            num_turmas++;
        }
    }

    FILE *arquivo = fopen(caminho_exportacao, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s\n", caminho_exportacao);
        return;
    }

    // Escreve cabeçalho (turmas)
    fprintf(arquivo, "Horario");
    for (int t = 0; t < num_turmas; t++) {
        fprintf(arquivo, ";%s", turmas_unicas[t]);
    }
    fprintf(arquivo, "\n");

    // Para cada horário, escreve as matérias de cada turma
    for (int h = 1; h <= max_horario; h++) {
        fprintf(arquivo, "Horario %d", h);
        for (int t = 0; t < num_turmas; t++) {
            int encontrado = 0;
            for (int v = 0; v < tam; v++) {
                if (grafo[v].cor == h && strcmp(grafo[v].turma, turmas_unicas[t]) == 0) {
                    fprintf(arquivo, ";%s", grafo[v].materia);
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                fprintf(arquivo, ";");
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Arquivo gerado com sucesso!\n");
}
