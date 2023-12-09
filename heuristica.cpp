#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;

// Lê o grafo de um arquivo e retorna sua representação como matriz de adjacência
vector<vector<int>> LerGrafo(const string& nomeArquivo, int& numVertices) {
    ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    vector<vector<int>> grafo(numVertices, vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;
    }
    arquivo.close();
    return grafo;
}

void BuscaExaustiva(const vector<vector<int>>& grafo, int numVertices, vector<int>& cliqueAtual, vector<vector<int>>& cliquesMaximais) {
    bool ehMaximal = true;
    for (int i : cliqueAtual) {
        for (int j : cliqueAtual) {
            if (i != j && grafo[i][j] == 0) {
                ehMaximal = false;
                break;
            }
        }
        if (!ehMaximal) break;
    }

    if (ehMaximal) {
        // Adiciona a clique atual às cliques maximais
        cliquesMaximais.push_back(cliqueAtual);
    }

    // Estende a clique atual
    for (int i = 0; i < numVertices; ++i) {
        bool podeAdicionar = true;
        for (int v : cliqueAtual) {
            if (grafo[i][v] == 0) {
                podeAdicionar = false;
                break;
            }
        }

        if (podeAdicionar) {
            cliqueAtual.push_back(i);
            BuscaExaustiva(grafo, numVertices, cliqueAtual, cliquesMaximais);
            cliqueAtual.pop_back();  // Desfaz a adição para explorar outras possibilidades
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    string nome_arquivo_entrada = argv[1];

    int numVertices;
    vector<vector<int>> grafo = LerGrafo(nome_arquivo_entrada, numVertices);

    vector<int> cliqueAtual;
    vector<vector<int>> cliquesMaximais;

    auto start_time = chrono::high_resolution_clock::now();
    BuscaExaustiva(grafo, numVertices, cliqueAtual, cliquesMaximais);
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end_time - start_time;

    vector<int> clique_maxima = *max_element(cliquesMaximais.begin(), cliquesMaximais.end(), [](const vector<int>& a, const vector<int>& b) {
        return a.size() < b.size();
    });

    // Resultado
    cout << "Cliques máximas encontradas: " << endl;
    for (const auto& clique : cliquesMaximais) {
        cout << "[";
        for (int i = 0; i < clique.size(); ++i) {
            cout << clique[i] + 1;
            if (i < clique.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }

    cout << "Clique máxima encontrada: ";
    cout << "[";
    for (int i = 0; i < clique_maxima.size(); ++i) {
        cout << clique_maxima[i] + 1;
        if (i < clique_maxima.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "Tempo de execução: " << elapsed_time.count() << "s" << endl;
    return 0;
}
