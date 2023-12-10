# Projeto de Supercomputação | Engenharia de Computação - Insper
# Professores: André Filipe e Michel Fornaciali

import networkx as nx

file_name = "grafo_output.txt"
with open(file_name, 'r') as file:
    next(file)
    G = nx.parse_adjlist(file)

max_click_list = list(nx.find_cliques(G))
max_click = max(max_click_list, key=len)
print("Cliques máximas encontradas: ")

for click in max_click_list:
    print(click)

print("Clique máxima encontrada: ", max_click)