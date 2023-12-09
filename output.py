import networkx as nx

nome_arquivo = "grafo.txt"

with open(nome_arquivo, 'r') as arquivo:
    next(arquivo)  #Pula a primeira linha do arquivo
    G = nx.parse_adjlist(arquivo) #Lê o grafo:

cliques_maximais = list(nx.find_cliques(G)) # Encontra cliques máximas:
clique_maxima = max(cliques_maximais, key=len) # Encontrar a maior clique máxima

print("Cliques máximas encontradas: ")
for clique in cliques_maximais:
    print(clique)

print("Clique máxima encontrada: ", clique_maxima)