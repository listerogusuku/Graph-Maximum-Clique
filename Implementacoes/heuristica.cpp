// Projeto de Supercomputação | Engenharia de Computação - Insper
// Professores: André Filipe e Michel Fornaciali

// Inclui bibliotecas padrão necessárias para o funcionamento do programa.
#include <iostream>  // Para operações de entrada e saída.
#include <vector>    // Para usar o tipo de dados 'vector', uma coleção dinâmica.
#include <algorithm> // Para algoritmos como max_element.
#include <fstream>   // Para manipulação de arquivos.
#include <chrono>    // Para medir o tempo de execução do programa.

using namespace std;

// Função que lê um grafo de um arquivo e retorna sua matriz de adjacência.
vector<vector<int>> LerGrafo(const string& fileName, int& qntVertices) {
    ifstream arquivo(fileName); // Abre o arquivo para leitura.
    int qntArestas;
    arquivo >> qntVertices >> qntArestas; // Lê a quantidade de vértices e arestas do arquivo.
    vector<vector<int>> grafo(qntVertices, vector<int>(qntVertices, 0)); // Cria uma matriz de adjacência.

    // Lê as arestas do arquivo e atualiza a matriz de adjacência.
    for (int i = 0; i < qntArestas; ++i) {
        int u, v;
        arquivo >> u >> v; // Lê um par de vértices que formam uma aresta.
        grafo[u-1][v-1] = 1; // Atualiza a matriz para representar a aresta.
        grafo[v-1][u-1] = 1; // Garante a bidirecionalidade da aresta.
    }
    arquivo.close(); // Fecha o arquivo.
    return grafo; // Retorna a matriz de adjacência.
}

// Função que busca exaustivamente todas as cliques maximais no grafo.
void buscaExaustiva(const vector<vector<int>>& grafo, int qntVertices, vector<int>& cliqueAtual, vector<vector<int>>& cliquesMaximas) {
    bool maxima = true; // Flag para verificar se a clique atual é máxima.
    
    // Verifica se todos os vértices na cliqueAtual estão conectados entre si.
    for (int i : cliqueAtual) {
        for (int j : cliqueAtual) {
            if (i != j && grafo[i][j] == 0) {
                maxima = false; // Se algum par de vértices não está conectado, não é uma clique máxima.
                break;
            }
        }
        if (!maxima) break;
    }

    // Se a clique atual é uma clique máxima, adiciona-a ao vetor de cliques maximais.
    if (maxima) {
        cliquesMaximas.push_back(cliqueAtual);
    }

    // Tenta adicionar cada vértice à cliqueAtual e faz uma chamada recursiva.
    for (int i = 0; i < qntVertices; ++i) {
        bool podeAdicionar = true; // Flag para verificar se um vértice pode ser adicionado à clique.
        for (int v : cliqueAtual) {
            if (grafo[i][v] == 0) { // Se o vértice não está conectado a todos na cliqueAtual, não pode ser adicionado.
                podeAdicionar = false;
                break;
            }
        }

        // Se o vértice pode ser adicionado, faz uma chamada recursiva.
        if (podeAdicionar) {
            cliqueAtual.push_back(i);
            buscaExaustiva(grafo, qntVertices, cliqueAtual, cliquesMaximas);
            cliqueAtual.pop_back(); // Remove o vértice adicionado para explorar outras possibilidades.
        }
    }
}

int main(int argc, char* argv[]) {
    // Verifica se o nome do arquivo foi passado como argumento na linha de comando.
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <input_file>" << endl;
        return 1; // Encerra o programa se o nome do arquivo não foi fornecido.
    }

    string input_file = argv[1]; // Nome do arquivo passado como argumento.
    int qntVertices;
    vector<vector<int>> grafo = LerGrafo(input_file, qntVertices); // Lê o grafo do arquivo.
    vector<int> cliqueAtual; // Armazena a clique que está sendo explorada no momento.
    vector<vector<int>> cliquesMaximas; // Armazena todas as cliques maximais encontradas.

    // Mede o tempo de execução da busca exaustiva.
    auto start_time = chrono::high_resolution_clock::now();
    buscaExaustiva(grafo, qntVertices, cliqueAtual, cliquesMaximas);
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed_time = end_time - start_time; // Calcula o tempo decorrido.

    // Encontra a maior clique máxima.
    vector<int> clique_maxima = *max_element(cliquesMaximas.begin(), cliquesMaximas.end(), [](const vector<int>& a, const vector<int>& b) {
        return a.size() < b.size(); // Função para comparar o tamanho das cliques.
    });

    // Imprime todas as cliques maximais encontradas.
    cout << "Cliques máximas encontradas: " << endl;
    for (const auto& clique : cliquesMaximas) {
        cout << "[";
        for (int i = 0; i < clique.size(); ++i) {
            cout << clique[i]+1; // Imprime cada vértice da clique.
            if (i < clique.size() - 1) {
                cout << ", ";
            }
        }
        cout << "]" << endl;
    }

    // Imprime a maior clique máxima encontrada.
    cout << "Clique máxima encontrada: ";
    cout << "[";
    for (int i = 0; i < clique_maxima.size(); ++i) {
        cout << clique_maxima[i] + 1; // Imprime cada vértice da clique.
        if (i < clique_maxima.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "Tempo de execução: " << elapsed_time.count() << "s" << endl; // Imprime o tempo de execução.
    return 0;
}
