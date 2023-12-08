#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <fstream>

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

std::vector<std::vector<int>> cliquesMaximaisGlobal;
std::vector<int> cliqueMaximaGlobal;

bool PodeAdicionarAoClique(const std::vector<std::vector<int>>& grafo, const std::vector<int>& cliqueAtual, int vizinho) {
    for (auto j : cliqueAtual) {
        if (grafo[vizinho][j] == 0) {
            return false;
        }
    }
    return true;
}

void AdicionarVizinhosPossiveis(const std::vector<std::vector<int>>& grafo, std::vector<int>& possiveisVertices, const std::vector<int>& cliqueAtual, const std::vector<bool>& visitados) {
    for (int i = 0; i < grafo.size(); ++i) {
        if (!visitados[i] && PodeAdicionarAoClique(grafo, cliqueAtual, i)) {
            possiveisVertices.emplace_back(i);
        }
    }
}

void AtualizarCliquesGlobais(const std::vector<int>& cliqueAtual) {
    #pragma omp critical
    {
        cliquesMaximaisGlobal.push_back(cliqueAtual);
        if (cliqueAtual.size() > cliqueMaximaGlobal.size()) {
            cliqueMaximaGlobal = cliqueAtual;
        }
    }
}

void EncontrarCliqueMaximalRecursivo(const std::vector<std::vector<int>>& grafo, std::vector<int>& cliqueAtual, std::vector<bool>& visitados, int vertice) {
    cliqueAtual.push_back(vertice);
    visitados[vertice] = true;

    std::vector<int> possiveisVertices;
    AdicionarVizinhosPossiveis(grafo, possiveisVertices, cliqueAtual, visitados);

    for (auto vizinho : possiveisVertices) {
        EncontrarCliqueMaximalRecursivo(grafo, cliqueAtual, visitados, vizinho);
    }

    if (possiveisVertices.empty() && cliqueAtual.size() > 1) {
        AtualizarCliquesGlobais(cliqueAtual);
    }

    cliqueAtual.pop_back();
    visitados[vertice] = false;
}

void EncontrarCliquesMaximaisOpenMP(const std::vector<std::vector<int>>& grafo, int numVertices) {
    #pragma omp parallel
    {
        std::vector<bool> visitados(numVertices, false);
        std::vector<int> cliqueAtual;

        #pragma omp for nowait
        for (int i = 0; i < numVertices; ++i) {
            EncontrarCliqueMaximalRecursivo(grafo, cliqueAtual, visitados, i);
        }
    }

    std::sort(cliquesMaximaisGlobal.begin(), cliquesMaximaisGlobal.end(), [](const auto& a, const auto& b) {
        return a.size() > b.size();
    });

    cliquesMaximaisGlobal.erase(std::unique(cliquesMaximaisGlobal.begin(), cliquesMaximaisGlobal.end()), cliquesMaximaisGlobal.end());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome_do_arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string nome_arquivo_entrada = argv[1];
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo(nome_arquivo_entrada, numVertices);

    EncontrarCliquesMaximaisOpenMP(grafo, numVertices);

    for (const auto& clique : cliquesMaximaisGlobal) {
        std::cout << "[";
        for (int i = 0; i < clique.size(); ++i) {
            std::cout << clique[i] + 1;
            if (i < clique.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    std::cout << "Clique máxima encontrada: [";
    for (int i = 0; i < cliqueMaximaGlobal.size(); ++i) {
        std::cout << cliqueMaximaGlobal[i] + 1;
        if (i < cliqueMaximaGlobal.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    return 0;
}