import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

def process_data(file_path):
    df = pd.read_csv(file_path)
    avg_data = df.groupby('num_classes').mean().reset_index()

    return avg_data

def add_labels_line(x, y, color):
    for i in range(len(x)):
        plt.text(x[i], y[i], f'{y[i]:.2f}', ha='center', va='bottom', fontsize=10, color=color)


def plot_times(avg_data): # grafico de linha para comparar linha e tamanho

    plt.figure(figsize=(10, 6))
    
    plt.plot(avg_data['num_classes'], avg_data['unbalanced_time'], marker='o', label='Clássica', color='red')
    # add_labels_line(avg_data['num_classes'], avg_data['unbalanced_time'], 'black')
    
    plt.plot(avg_data['num_classes'], avg_data['balanced_time'], marker='o', label='Alternativa', color='green')
    # add_labels_line(avg_data['num_classes'], avg_data['balanced_time'], 'black')
    
    plt.xlabel('N° de elementos')
    plt.ylabel('Tempo de Execução (ms)')
    # plt.title('Tempo de Execução vs. Tamanho')
    plt.legend()
    plt.savefig('tempo.png')
    plt.show()

def plot_times_10000(avg_data):
    import matplotlib.pyplot as plt
    
    # Filtrar até num_classes = 10000
    filtered_data = avg_data[avg_data['num_classes'] <= 2500]
    
    plt.figure(figsize=(10, 6))
    
    # Plotar os tempos da linha Clássica e Alternativa
    plt.plot(filtered_data['num_classes'], filtered_data['unbalanced_time'], marker='o', label='Clássica', color='red')
    plt.plot(filtered_data['num_classes'], filtered_data['balanced_time'], marker='o', label='Alternativa', color='green')
    
    plt.xlabel('N° de elementos')
    plt.ylabel('Tempo de Execução (ms)')
    plt.legend()
    
    # Salvar e exibir o gráfico
    plt.savefig('tempo2500.png')
    plt.show()

def add_labels(bars): 
    
    for bar in bars:
        height = bar.get_height()
        plt.text(
            bar.get_x() + bar.get_width() / 2,
            height,
            f'{height:.2f}',
            ha='center', va='bottom', fontsize=10
        )

def plot_absolute_values(avg_data): # barras para tempo e variância
    plt.figure(figsize=(10, 6))
    bar_width = 0.35
    x = np.arange(len(avg_data['num_classes']))
    
    bars1 = plt.bar(x - bar_width/2, avg_data['unbalanced_time'], bar_width, label='Clássica', color='red')
    bars2 = plt.bar(x + bar_width/2, avg_data['balanced_time'], bar_width, label='Alternativa', color='green')
    
    # add_labels(bars1)
    # add_labels(bars2)

    plt.xlabel('N° de elementos')
    plt.ylabel('Tempo de Execução (ms)')
    # plt.title('Tempo de Execução: Alternativa vs. Clássica')
    plt.xticks(x, avg_data['num_classes'], rotation=-90)
    plt.legend()
    plt.grid(False)
    plt.tight_layout()
    plt.savefig('tempo-elementos.png')
    plt.show()


    filtered_data = avg_data[avg_data['num_classes'] <= 2500]
    x_filtered = np.arange(len(filtered_data['num_classes']))

    plt.figure(figsize=(10, 6))

    bars5 = plt.bar(x_filtered - bar_width/2, filtered_data['unbalanced_var'], bar_width, label='Clássica', color='red')
    bars6 = plt.bar(x_filtered + bar_width/2, filtered_data['balanced_var'], bar_width, label='Alternativa', color='green')
    
    # add_labels(bars3)
    # add_labels(bars4)

    plt.xlabel('N° de elementos')
    plt.ylabel('Variância')
    # plt.title('Variância: Alternativa vs. Clássica')
    plt.xticks(x_filtered, filtered_data['num_classes'], rotation=-90)
    plt.legend()
    plt.grid(False)
    plt.tight_layout()
    plt.savefig('variancia-elementos2500.png')
    plt.show()

    plt.figure(figsize=(10, 6))
    bars3 = plt.bar(x - bar_width/2, avg_data['unbalanced_var'], bar_width, label='Clássica', color='red')
    bars4 = plt.bar(x + bar_width/2, avg_data['balanced_var'], bar_width, label='Alternativa', color='green')
    
    # add_labels(bars3)
    # add_labels(bars4)

    plt.xlabel('N° de elementos')
    plt.ylabel('Variância')
    # plt.title('Variância: Alternativa vs. Clássica')
    plt.xticks(x, avg_data['num_classes'], rotation=-90)
    plt.legend()
    plt.grid(False)
    plt.tight_layout()
    plt.savefig('variancia-elementos.png')
    plt.show()


file_path = './results.csv'
avg_data = process_data(file_path)

plot_times(avg_data)
plot_times_10000(avg_data)
plot_absolute_values(avg_data)

