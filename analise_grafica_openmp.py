# Análise gráfica dos resultados obtidos após rodar as implementações:

import matplotlib.pyplot as plt

# Dados para o primeiro gráfico
n_values_1 = [5, 10, 15, 20, 25]
exaustiva_times_1 = [0.000113803, 0.00106659, 0.117804, 13.3899, 538.341]
openmp_times_1 = [0.000210252, 0.000924805, 0.0499834, 7.49242, 244.992]

# Dados para o segundo gráfico (em que houve variação da probabilidade de conexão)
n_values_2 = [30, 40, 50, 70]
exaustiva_times_2 = [0.0390696, 1.53986, 10.4161, 259.201]
openmp_times_2 = [0.0300647, 1.19192, 7.62237, 179.651]

# Criando o primeiro gráfico
plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(n_values_1, exaustiva_times_1, label='Exaustiva', marker='o')
plt.plot(n_values_1, openmp_times_1, label='OpenMP', marker='o')
plt.title('Tempos de Execução para N = 5, 10, 15, 20, 25')
plt.xlabel('Valor de N')
plt.ylabel('Tempo de Execução (s)')
plt.legend()
plt.grid(True)

# Criando o segundo gráfico
plt.subplot(1, 2, 2)
plt.plot(n_values_2, exaustiva_times_2, label='Exaustiva', marker='o')
plt.plot(n_values_2, openmp_times_2, label='OpenMP', marker='o')
plt.title('Tempos de Execução para N = 30, 40, 50, 70')
plt.xlabel('Valor de N')
plt.ylabel('Tempo de Execução (s)')
plt.legend()
plt.grid(True)

# Mostrando os gráficos
plt.tight_layout()
plt.show()
plt.savefig("figure.png") # save as png
