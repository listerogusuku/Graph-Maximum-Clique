import networkx as nx
import random

#Parâmetros:
qnt_vertices = 5  #Vértices no grafo
probabilidade_conexao = 0.7  #Chances de haver uma aresta entre dois vértices (ajuste conforme necessário)

grafo = nx.fast_gnp_random_graph(qnt_vertices, probabilidade_conexao) #Grafo aleatório densamente conectado

nome_arquivo = "grafo.txt"
with open(nome_arquivo, 'w') as arquivo:
    arquivo.write(f"{qnt_vertices} {grafo.number_of_edges()}\n")

    for aresta in grafo.edges():
        arquivo.write(f"{aresta[0]+1} {aresta[1]+1}\n")  
print(f"Grafo densamente conectado gerado e salvo em '{nome_arquivo}'.")