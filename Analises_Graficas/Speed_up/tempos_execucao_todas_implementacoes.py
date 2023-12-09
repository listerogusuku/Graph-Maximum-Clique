import matplotlib.pyplot as plt

# Dados do primeiro gráfico
n_values_1 = [5, 10, 15, 20, 25]
exaustiva_1 = [0.000113803, 0.00106659, 0.117804, 13.3899, 538.341]
openmp_1 = [0.000210252, 0.000924805, 0.0499834, 7.49242, 244.992]
mpi_1 = [2.5278e-05, 2.646e-05, 3.0694e-05, 4.5942e-05, 3.6753e-05]

# Dados do segundo gráfico
n_values_2 = [30, 40, 50, 70]
exaustiva_2 = [0.0390696, 1.53986, 10.4161, 259.201]
openmp_2 = [0.0300647, 1.19192, 7.62237, 179.651]
mpi_2 = [3.1423e-05, 3.5322e-05, 4.3731e-05, 5.2941e-05]

# Primeiro gráfico
plt.figure(figsize=(12, 6))
plt.subplot(1, 2, 1)
plt.plot(n_values_1, exaustiva_1, marker='o', label='Exaustiva')
plt.plot(n_values_1, openmp_1, marker='s', label='OpenMP')
plt.plot(n_values_1, mpi_1, marker='^', label='MPI')
plt.xlabel('N')
plt.ylabel('Tempo de execução (s)')
plt.title('Tempos de Execução para Diferentes Implementações (Primeiro Conjunto de Dados)')
plt.legend()
plt.grid(True)

# Segundo gráfico
plt.subplot(1, 2, 2)
plt.plot(n_values_2, exaustiva_2, marker='o', label='Exaustiva')
plt.plot(n_values_2, openmp_2, marker='s', label='OpenMP')
plt.plot(n_values_2, mpi_2, marker='^', label='MPI')
plt.xlabel('N')
plt.ylabel('Tempo de execução (s)')
plt.title('Tempos de Execução para Diferentes Implementações (Segundo Conjunto de Dados)')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
