import networkx as nx
import random

qnt_vertices = 5  #Vértices no grafo
probabilidade_conexao = 0.7  #Chances de haver uma aresta entre dois vértices

grafo = nx.fast_gnp_random_graph(qnt_vertices, probabilidade_conexao)
nome_do_arquivo = "grafo_output.txt"
with open(nome_do_arquivo, 'w') as arquivo:
    arquivo.write(f"{qnt_vertices} {grafo.number_of_edges()}\n")

    for aresta in grafo.edges():
        arquivo.write(f"{aresta[0]+1} {aresta[1]+1}\n")  
print(f"Grafo densamente conectado gerado e salvo em '{nome_do_arquivo}'.")