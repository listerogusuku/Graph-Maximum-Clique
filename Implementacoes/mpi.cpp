// Projeto de Supercomputação | Engenharia de Computação - Insper
// Professores: André Filipe e Michel Fornaciali

#include <mpi.h>           // Inclui a biblioteca MPI para programação paralela em clusters.
#include <fstream>         // Inclui a biblioteca para manipulação de arquivos.
#include <iostream>        // Inclui a biblioteca padrão de entrada e saída.
#include <vector>          // Inclui a biblioteca para usar vetores dinâmicos.
#include <chrono>          // Inclui a biblioteca para medir o tempo.
#include <algorithm>       // Inclui a biblioteca para operações de algoritmo como sort.

using namespace std;

// Função para ler um grafo de um arquivo e retornar sua matriz de adjacência.
vector<vector<int>> LerGrafo(const string& fileName, int& qntVertices) {
    ifstream entrada(fileName);  // Abre o arquivo para leitura.
    int totalArestas;
    entrada >> qntVertices >> totalArestas;  // Lê a quantidade de vértices e arestas.

    vector<vector<int>> matrizAdjacencia(qntVertices, vector<int>(qntVertices, 0));  // Cria matriz de adjacência.

    // Lê as arestas e preenche a matriz de adjacência.
    for (int aresta = 0; aresta < totalArestas; ++aresta) {
        int vertice1, vertice2;
        entrada >> vertice1 >> vertice2;  // Lê um par de vértices conectados.
        matrizAdjacencia[vertice1-1][vertice2-1] = 1;
        matrizAdjacencia[vertice2-1][vertice1-1] = 1;
    }
    entrada.close();  // Fecha o arquivo.
    return matrizAdjacencia;  // Retorna a matriz de adjacência.
}

// Função para buscar a clique máxima em uma parte do grafo.
vector<int> buscarCliqueMaxima(const vector<vector<int>>& matrizAdjacencia, int qntVertices, int inicio, int fim) {
    vector<int> cliqueMax;  // Armazena a maior clique encontrada.
    vector<int> possiveisCandidatos;  // Armazena os candidatos para serem adicionados à clique.

    // Inicializa a lista de possíveis candidatos.
    for(int i = inicio; i < fim; ++i) {
        possiveisCandidatos.push_back(i);
    }

    // Busca pela clique máxima.
    while(!possiveisCandidatos.empty()) {
        int verticeAtual = possiveisCandidatos.back();
        possiveisCandidatos.pop_back();

        bool adicionar = true;  // Flag para verificar se o vértice atual pode ser adicionado à clique.

        // Verifica se o vértice atual é adjacente a todos na clique.
        for(int adjacente : cliqueMax) {
            if(matrizAdjacencia[adjacente][verticeAtual] == 0) {
                adicionar = false;
                break;
            }
        }

        // Adiciona o vértice à clique e atualiza os candidatos.
        if(adicionar) {
            cliqueMax.push_back(verticeAtual);
            vector<int> novosCandidatos;

            // Atualiza a lista de candidatos para incluir apenas aqueles que são adjacentes a todos na clique.
            for(int candidato : possiveisCandidatos) {
                bool adjacenteATodos = true;

                for(int membroClique : cliqueMax) {
                    if(matrizAdjacencia[candidato][membroClique] == 0) {
                        adjacenteATodos = false;
                        break;
                    }
                }

                if(adjacenteATodos) {
                    novosCandidatos.push_back(candidato);
                }
            }

            possiveisCandidatos = novosCandidatos;
        }
    }

    return cliqueMax;  // Retorna a maior clique encontrada.
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);  // Inicializa o ambiente MPI.

    int idProcesso, qntProcessos;
    MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);  // Obtém o ID do processo atual.
    MPI_Comm_size(MPI_COMM_WORLD, &qntProcessos);  // Obtém o número total de processos.

    int qntVertices;
    vector<vector<int>> matrizAdjacencia = LerGrafo("grafo_output.txt", qntVertices);  // Lê o grafo do arquivo.

    // Divide os vértices entre os processos.
    int verticesPorProcesso = qntVertices/qntProcessos;
    int inicio = idProcesso * verticesPorProcesso;
    int fim = (idProcesso == qntProcessos - 1) ? qntVertices : inicio + verticesPorProcesso;

    // Cada processo busca pela clique máxima na sua parte do grafo.
    chrono::high_resolution_clock::time_point tempoInicio;
    if(idProcesso == 0) {
        tempoInicio = chrono::high_resolution_clock::now();  // Inicia a contagem do tempo no processo principal.
    }

    vector<int> cliqueMaxLocal = buscarCliqueMaxima(matrizAdjacencia, qntVertices, inicio, fim);

    // Coleta os tamanhos das cliques maximas de todos os processos.
    int tamanhoCliqueMaxLocal = cliqueMaxLocal.size();
    vector<int> tamanhosCliqueMax(qntProcessos);
    MPI_Gather(&tamanhoCliqueMaxLocal, 1, MPI_INT, tamanhosCliqueMax.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // O processo principal determina a maior clique.
    if(idProcesso == 0) {
        int indiceMaximo = distance(tamanhosCliqueMax.begin(), max_element(tamanhosCliqueMax.begin(), tamanhosCliqueMax.end()));

        if(indiceMaximo != 0) {
            cliqueMaxLocal.resize(tamanhosCliqueMax[indiceMaximo]);
            MPI_Recv(cliqueMaxLocal.data(), tamanhosCliqueMax[indiceMaximo], MPI_INT, indiceMaximo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else if(tamanhoCliqueMaxLocal == tamanhosCliqueMax[indiceMaximo]) {
        MPI_Send(cliqueMaxLocal.data(), tamanhoCliqueMaxLocal, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // O processo principal imprime o resultado e o tempo de execução.
    if(idProcesso == 0) {
        auto tempoFim = chrono::high_resolution_clock::now();
        chrono::duration<double> tempoDecorrido = tempoFim - tempoInicio;
        
        sort(cliqueMaxLocal.begin(), cliqueMaxLocal.end());
        cout << "Clique máxima (MPI): ";
        for(int vertice : cliqueMaxLocal) {
            cout << vertice << " ";
        }
        cout << endl;
        
        cout << "Tempo de execução: " << tempoDecorrido.count() << "s" << endl;
    }

    MPI_Finalize();  // Finaliza o ambiente MPI.
    return 0;
}
