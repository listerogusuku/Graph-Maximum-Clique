import matplotlib.pyplot as plt
import numpy as np

# Dados do primeiro gráfico
n_values_1 = [5, 10, 15, 20, 25]
times_exhaustive_1 = [0.000113803, 0.00106659, 0.117804, 13.3899, 538.341]
times_openmp_1 = [0.000210252, 0.000924805, 0.0499834, 7.49242, 244.992]
times_mpi_1 = [2.5278e-05, 2.646e-05, 3.0694e-05, 4.5942e-05, 3.6753e-05]

# Dados do segundo gráfico
n_values_2 = [30, 40, 50, 70]
times_exhaustive_2 = [0.0390696, 1.53986, 10.4161, 259.201]
times_openmp_2 = [0.0300647, 1.19192, 7.62237, 179.651]
times_mpi_2 = [3.1423e-05, 3.5322e-05, 4.3731e-05, 5.2941e-05]

# Calculando o Speed Up para o primeiro conjunto de dados
speedup_openmp_1 = np.array(times_exhaustive_1) / np.array(times_openmp_1)
speedup_mpi_1 = np.array(times_exhaustive_1) / np.array(times_mpi_1)

# Calculando o Speed Up para o segundo conjunto de dados
speedup_openmp_2 = np.array(times_exhaustive_2) / np.array(times_openmp_2)
speedup_mpi_2 = np.array(times_exhaustive_2) / np.array(times_mpi_2)

# Criando gráficos de Speed Up com linhas pontilhadas para análise
fig, axs = plt.subplots(2, 1, figsize=(10, 15))

# Gráfico de Speed Up para o primeiro conjunto de dados com linha pontilhada
axs[0].plot(n_values_1, speedup_openmp_1, label='Speed Up OpenMP', marker='o')
axs[0].plot(n_values_1, speedup_mpi_1, label='Speed Up MPI', marker='o')
axs[0].plot(n_values_1, speedup_openmp_1 - speedup_mpi_1, label='Diferença de Speed Up (OpenMP - MPI)', linestyle='--', color='gray')
axs[0].set_xlabel('N')
axs[0].set_ylabel('Speed Up')
axs[0].set_title('Speed Up Comparativo com Análise (Primeiro conjunto de dados)')
axs[0].legend()
axs[0].grid(True)

# Gráfico de Speed Up para o segundo conjunto de dados com linha pontilhada
axs[1].plot(n_values_2, speedup_openmp_2, label='Speed Up OpenMP', marker='o')
axs[1].plot(n_values_2, speedup_mpi_2, label='Speed Up MPI', marker='o')
axs[1].plot(n_values_2, speedup_openmp_2 - speedup_mpi_2, label='Diferença de Speed Up (OpenMP - MPI)', linestyle='--', color='gray')
axs[1].set_xlabel('N')
axs[1].set_ylabel('Speed Up')
axs[1].set_title('Speed Up Comparativo com Análise (Segundo conjunto de dados)')
axs[1].legend()
axs[1].grid(True)

# Mostrar gráficos
plt.tight_layout()
plt.show()
