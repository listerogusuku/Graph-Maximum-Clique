import matplotlib.pyplot as plt

# Dados para o segundo conjunto
n_values_2 = [30, 40, 50, 70]
exaustiva_times_2 = [0.0390696, 1.53986, 10.4161, 259.201]
openmp_times_2 = [0.0300647, 1.19192, 7.62237, 179.651]

# Calculando o speed up para o segundo conjunto de dados
speed_up_2 = [exaustiva_times_2[i] / openmp_times_2[i] for i in range(len(exaustiva_times_2))]

# Criando o gráfico para o segundo conjunto de dados
fig, ax1 = plt.subplots(figsize=(10, 6))

# Linhas de tempo de execução
ax1.plot(n_values_2, exaustiva_times_2, label='Exaustiva', marker='o', color='blue')
ax1.plot(n_values_2, openmp_times_2, label='OpenMP', marker='o', color='green')
ax1.set_xlabel('Valor de N')
ax1.set_ylabel('Tempo de Execução (s)', color='black')
ax1.tick_params(axis='y', labelcolor='black')
ax1.legend(loc='upper left')
ax1.grid(True)

# Eixo secundário para speed up
ax2 = ax1.twinx()
ax2.plot(n_values_2, speed_up_2, label='Speed Up (Exaustiva/OpenMP)', linestyle='--', color='red', marker='o')
ax2.set_ylabel('Speed Up', color='red')
ax2.tick_params(axis='y', labelcolor='red')
ax2.legend(loc='upper right')

plt.title('Tempos de Execução e Speed Up para N = 30, 40, 50, 70')

plt.show()
