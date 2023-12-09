#include <mpi.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

vector<vector<int>> LerGrafo(const string& arquivoGrafo, int& totalVertices) {
    ifstream entrada(arquivoGrafo);
    int totalArestas;
    entrada >> totalVertices >> totalArestas;

    vector<vector<int>> matrizAdjacencia(totalVertices, vector<int>(totalVertices, 0));

    for (int aresta = 0; aresta < totalArestas; ++aresta) {
        int vertice1, vertice2;
        entrada >> vertice1 >> vertice2;
        matrizAdjacencia[vertice1 - 1][vertice2 - 1] = 1;
        matrizAdjacencia[vertice2 - 1][vertice1 - 1] = 1;  // O grafo é não direcionado
    }

    entrada.close();

    return matrizAdjacencia;
}

vector<int> buscarCliqueMaxima(const vector<vector<int>>& matrizAdjacencia, int totalVertices, int inicio, int fim) {
    vector<int> cliqueMax;
    vector<int> possiveisCandidatos;

    for(int i = inicio; i < fim; ++i) {
        possiveisCandidatos.push_back(i);
    }

    while(!possiveisCandidatos.empty()) {
        int verticeAtual = possiveisCandidatos.back();
        possiveisCandidatos.pop_back();

        bool adicionar = true;

        for(int adjacente : cliqueMax) {
            if(matrizAdjacencia[adjacente][verticeAtual] == 0) {
                adicionar = false;
                break;
            }
        }

        if(adicionar) {
            cliqueMax.push_back(verticeAtual);
            vector<int> novosCandidatos;

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

    return cliqueMax;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int idProcesso, numProcessos;
    MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessos);

    int totalVertices;
    vector<vector<int>> matrizAdjacencia = LerGrafo("grafo.txt", totalVertices);

    int verticesPorProc = totalVertices / numProcessos;
    int inicio = idProcesso * verticesPorProc;
    int fim = (idProcesso == numProcessos - 1) ? totalVertices : inicio + verticesPorProc;

    // Exibe a matriz de adjacência
    for (int i = 0; i < totalVertices; ++i) {
        for (int j = 0; j < totalVertices; ++j) {
            cout << matrizAdjacencia[i][j] << " ";
        }
        cout << endl;
    }
    cout << "Processo " << idProcesso << " processa vértices de " << inicio + 1 << " a " << fim << endl;

    chrono::high_resolution_clock::time_point tempoInicio;
    if(idProcesso == 0) {
        tempoInicio = chrono::high_resolution_clock::now();
    }

    vector<int> cliqueMaxLocal = buscarCliqueMaxima(matrizAdjacencia, totalVertices, inicio, fim);

    int tamanhoCliqueMaxLocal = cliqueMaxLocal.size();
    vector<int> tamanhosCliqueMax(numProcessos);

    MPI_Gather(&tamanhoCliqueMaxLocal, 1, MPI_INT, tamanhosCliqueMax.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(idProcesso == 0) {
        int indiceMaximo = 0;
        for(int i = 1; i < numProcessos; i++) {
            if(tamanhosCliqueMax[i] > tamanhosCliqueMax[indiceMaximo]) {
                indiceMaximo = i;
            }
        }

        if(indiceMaximo != 0) {
            cliqueMaxLocal.resize(tamanhosCliqueMax[indiceMaximo]);
            MPI_Recv(cliqueMaxLocal.data(), tamanhosCliqueMax[indiceMaximo], MPI_INT, indiceMaximo, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else if(tamanhoCliqueMaxLocal == *max_element(tamanhosCliqueMax.begin(), tamanhosCliqueMax.end())) {
        MPI_Send(cliqueMaxLocal.data(), tamanhoCliqueMaxLocal, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

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

        cout << "Tamanhos locais: ";
        for (int i = 0; i < numProcessos; i++) {
            cout << tamanhosCliqueMax[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
