#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>  // Adicione esta linha para incluir a biblioteca <fstream>
#include <chrono>

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();

    return grafo;
}
// Função para encontrar todas as cliques maximais usando busca exaustiva
void BuscaExaustiva(const std::vector<std::vector<int>>& grafo, int numVertices, std::vector<int>& cliqueAtual, std::vector<std::vector<int>>& cliquesMaximais) {
    // Verifica se a clique atual é maximal
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
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo_de_entrada>" << std::endl;
        return 1;
    }

    // Nome do arquivo de entrada passado como argumento
    std::string nome_arquivo_entrada = argv[1];

    // Leitura do grafo
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo(nome_arquivo_entrada, numVertices);

    // Inicialização da busca exaustiva
    std::vector<int> cliqueAtual;
    std::vector<std::vector<int>> cliquesMaximais;

    // Medição do tempo inicial
    auto start_time = std::chrono::high_resolution_clock::now();

    // Executa a busca exaustiva
    BuscaExaustiva(grafo, numVertices, cliqueAtual, cliquesMaximais);

    // Medição do tempo final
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    // Encontrar a clique máxima (a maior)
    std::vector<int> clique_maxima = *std::max_element(cliquesMaximais.begin(), cliquesMaximais.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a.size() < b.size();
    });

    // Saída do resultado
    std::cout << "Cliques máximas encontradas:" << std::endl;
    for (const auto& clique : cliquesMaximais) {
        std::cout << "[";
        for (int i = 0; i < clique.size(); ++i) {
            std::cout << clique[i] + 1;  // Ajuste para índices baseados em 1
            if (i < clique.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    std::cout << "Clique máxima encontrada:";
    std::cout << "[";
    for (int i = 0; i < clique_maxima.size(); ++i) {
        std::cout << clique_maxima[i] + 1;  // Ajuste para índices baseados em 1
        if (i < clique_maxima.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;

    // Saída do tempo de execução
    std::cout << "Tempo de execução: " << elapsed_time.count() << "s" << std::endl;

    return 0;
}
