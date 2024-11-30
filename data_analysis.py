import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Carregar os dados dos arquivos CSV
heap_data = pd.read_csv('results.csv')
circular_data = pd.read_csv('resultsCircular.csv')

heap_data['unbalanced_std'] = np.sqrt(heap_data['unbalanced_var'])
heap_data['balanced_std'] = np.sqrt(heap_data['balanced_var'])
circular_data['unbalanced_std'] = np.sqrt(circular_data['unbalanced_var'])
circular_data['balanced_std'] = np.sqrt(circular_data['balanced_var'])

# Comparação entre Heap e Fila Circular
def plot_comparison(data1, label1, data2, label2, column, title, ylabel):
    plt.figure(figsize=(10, 6))
    plt.plot(data1['num_classes'], data1[column], label=label1, marker='o')
    plt.plot(data2['num_classes'], data2[column], label=label2, marker='o')
    plt.xlabel('Número de Classes')
    plt.ylabel(ylabel)
    plt.legend()
    plt.grid(True)
    plt.savefig(f'{title}.png')
    plt.show()


# Gráfico 1: Comparação do Tempo Médio (Unbalanced)
plot_comparison(
    heap_data, 'Heap (Clássica)',
    circular_data, 'Fila Circular (Clássica)',
    'unbalanced_time',
    'Comparação do Tempo Médio - Estratégia Clássica',
    'Tempo Médio (s)'
)

# Gráfico 2: Comparação do Tempo Médio (Balanced)
plot_comparison(
    heap_data, 'Heap (Alternativa)',
    circular_data, 'Fila Circular (Alternativa)',
    'balanced_time',
    'Comparação do Tempo Médio - Estratégia Alternativa',
    'Tempo Médio (s)'
)

# Gráfico 3: Comparação do Balanceamento (Desvio Padrão - Unbalanced)
plot_comparison(
    heap_data, 'Heap (Clássica)',
    circular_data, 'Fila Circular (Clássica)',
    'unbalanced_std',
    'Comparação do Balanceamento - Estratégia Clássica (Desvio Padrão)',
    'Desvio Padrão'
)

# Gráfico 4: Comparação do Balanceamento (Desvio Padrão - Balanced)
plot_comparison(
    heap_data, 'Heap (Alternativa)',
    circular_data, 'Fila Circular (Alternativa)',
    'balanced_std',
    'Comparação do Balanceamento - Estratégia Alternativa (Desvio Padrão)',
    'Desvio Padrão'
)
