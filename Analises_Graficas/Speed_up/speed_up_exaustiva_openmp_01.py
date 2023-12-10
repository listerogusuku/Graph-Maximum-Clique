import matplotlib.pyplot as plt
import numpy as np

# Dados para o primeiro gráfico (obtidos rodando as implementações com o primeiro conjunto de dados)
n_values_1 = [5, 10, 15, 20, 25]
exaustiva_times_1 = [0.000113803, 0.00106659, 0.117804, 13.3899, 538.341]
openmp_times_1 = [0.000210252, 0.000924805, 0.0499834, 7.49242, 244.992]

# Calculando o speed up para o primeiro conjunto de dados
speed_up_1 = [exaustiva_times_1[i] / openmp_times_1[i] for i in range(len(exaustiva_times_1))]

fig, ax1 = plt.subplots(figsize=(10, 6))

# Linhas de tempo de execução
ax1.plot(n_values_1, exaustiva_times_1, label='Exaustiva', marker='o', color='blue')
ax1.plot(n_values_1, openmp_times_1, label='OpenMP', marker='o', color='green')
ax1.set_xlabel('Valor de N')
ax1.set_ylabel('Tempo de Execução (s)', color='black')
ax1.tick_params(axis='y', labelcolor='black')
ax1.legend(loc='upper left')
ax1.grid(True)

# Eixo secundário para speed up
ax2 = ax1.twinx()
ax2.plot(n_values_1, speed_up_1, label='Speed Up (Exaustiva/OpenMP)', linestyle='--', color='red', marker='o')
ax2.set_ylabel('Speed Up', color='red')
ax2.tick_params(axis='y', labelcolor='red')
ax2.legend(loc='upper right')

plt.title('Tempos de Execução e Speed Up para N = 5, 10, 15, 20, 25')

plt.show()
