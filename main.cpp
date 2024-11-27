#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

float variance(vector<int> &hours) {
    float mean = 0.0;
    for (const int &v : hours) {
        mean += v;
    }
    mean /= hours.size();

    float var = 0.0;
    for (const int &v : hours) {
        var += (v - mean) * (v - mean);
    }
    var /= hours.size();
    return var;
}

struct Class {
    int init;
    int end;

    static bool compare(const Class &a, const Class &b) {
        return a.init < b.init; // return + comparação + 2 acessos O(4)
    }
};

struct Classroom {
    int end;
    int used;

    static bool compare(const Classroom &a, const Classroom &b) {
        return a.end > b.end;
    }

    static bool compareBalanced(const Classroom &a, const Classroom &b) {
        return a.end > b.end || (a.end == b.end && a.used > b.used);
    }
};

struct Result {
    size_t numClassrooms;
    vector<int> classroomsHours;
};

Result greedy(vector<Class> &classes) {
    sort(classes.begin(), classes.end(), Class::compare); // Ordena as classes

    constexpr int MAX_CLASSROOMS = 10; // Tamanho máximo da fila circular
    Classroom classrooms[MAX_CLASSROOMS]; // Fila circular
    vector<vector<Class>> tasksPerClassroom(MAX_CLASSROOMS); // Aulas por sala
    int front = 0, rear = 0; // Ponteiros da fila circular
    int size = 0; // Número atual de salas na fila

    for (const Class &classX : classes) {
        // printf("Classe: init = %d, end = %d\n", classX.init, classX.end);

        bool allocated = false; // Flag para verificar se a aula foi alocada
        for (int i = 0; i < size; i++) {
            int idx = (front + i) % MAX_CLASSROOMS; // Índice circular
            if (classrooms[idx].end <= classX.init) {
                // Atualiza a sala existente
                classrooms[idx].end = classX.end;
                classrooms[idx].used += classX.end - classX.init;
                tasksPerClassroom[idx].push_back(classX); // Adiciona a aula à sala
                // printf("Alocada na sala %d (existente): init = %d, end = %d\n", idx + 1, classX.init, classX.end);
                allocated = true;
                break;
            }
        }

        if (!allocated) { // Se não foi possível alocar em nenhuma sala existente
            classrooms[rear] = {classX.end, classX.end - classX.init};
            tasksPerClassroom[rear].push_back(classX); // Adiciona a aula à nova sala
            // printf("Alocada na sala %d (nova): init = %d, end = %d\n", rear + 1, classX.init, classX.end);
            rear = (rear + 1) % MAX_CLASSROOMS; // Incrementa circularmente
            size++;
        }
    }

    // Calcula o número total de horas por sala e imprime as tarefas alocadas
    std::vector<int> classroomHours;
    for (int i = 0; i < size; i++) {
        int idx = (front + i) % MAX_CLASSROOMS;
        classroomHours.push_back(classrooms[idx].used);

        // printf("Sala %d:\n", idx + 1);
        for (const Class &task : tasksPerClassroom[idx]) {
            // printf("  Tarefa: init = %d, end = %d\n", task.init, task.end);
        }
    }

    return {static_cast<size_t>(size), classroomHours};
}

Result balancedGreedy(vector<Class> &classes) {
    sort(classes.begin(), classes.end(), Class::compare); // Ordena as classes

    constexpr int MAX_CLASSROOMS = 10; // Tamanho máximo da fila circular
    Classroom classrooms[MAX_CLASSROOMS]; // Fila circular
    vector<vector<Class>> tasksPerClassroom(MAX_CLASSROOMS); // Aulas por sala
    int front = 0, rear = 0; // Ponteiros da fila circular
    int size = 0; // Número atual de salas na fila

    for (const Class &classX : classes) {
        // printf("Classe: init = %d, end = %d\n", classX.init, classX.end);

        bool allocated = false; // Flag para verificar se a aula foi alocada
        int bestIdx = -1;
        int minLoad = INT_MAX;

        for (int i = 0; i < size; i++) {
            int idx = (front + i) % MAX_CLASSROOMS; // Índice circular
            if (classrooms[idx].end <= classX.init && classrooms[idx].used < minLoad) {
                bestIdx = idx;       // Seleciona a sala com menor carga
                minLoad = classrooms[idx].used;
            }
        }

        if (bestIdx != -1) { // Sala encontrada para alocar
            classrooms[bestIdx].end = classX.end;
            classrooms[bestIdx].used += classX.end - classX.init;
            tasksPerClassroom[bestIdx].push_back(classX); // Adiciona a aula à sala
            // printf("Alocada na sala %d (existente): init = %d, end = %d\n", bestIdx + 1, classX.init, classX.end);
            allocated = true;
        }

        if (!allocated) { // Se não foi possível alocar em nenhuma sala existente
            classrooms[rear] = {classX.end, classX.end - classX.init};
            tasksPerClassroom[rear].push_back(classX); // Adiciona a aula à nova sala
            // printf("Alocada na sala %d (nova): init = %d, end = %d\n", rear + 1, classX.init, classX.end);
            rear = (rear + 1) % MAX_CLASSROOMS; // Incrementa circularmente
            size++;
        }
    }

    // Calcula o número total de horas por sala e imprime as tarefas alocadas
    std::vector<int> classroomHours;
    for (int i = 0; i < size; i++) {
        int idx = (front + i) % MAX_CLASSROOMS;
        classroomHours.push_back(classrooms[idx].used);

        // printf("Sala %d:\n", idx + 1);
        for (const Class &task : tasksPerClassroom[idx]) {
            // printf("  Tarefa: init = %d, end = %d\n", task.init, task.end);
        }
    }

    return {static_cast<size_t>(size), classroomHours}; // Conversão para evitar warnings
}


int main(int argc, char *argv[]) {
    array<int, 27> entradas = {
        10,    25,    50,    100,    150,    200,    300,    500,    750,
        1000,  1500,  2000,  2500,   5000,   7500,   10000,  15000,  20000,
        30000, 50000, 75000, 100000, 150000, 250000, 350000, 500000, 750000};
    // array<int, 1> entradas = {10};

    FILE *results = fopen("resultsCircular.csv", "w");
    if (results == NULL) {
        fprintf(stderr, "Não pôde criar 'results.csv'\n");
        return 1;
    }

    fprintf(results,
            "num_classes,unbalanced_time,unbalanced_var,balanced_time,balanced_"
            "var\n");
    for (const int &num_classes : entradas) {
        // printf("%7d classes", num_classes);

        char input_filename[32];
        sprintf(input_filename, "entradas/Aula%u.txt", num_classes);

        FILE *input_file = fopen(input_filename, "r");
        if (input_file == NULL) {
            fprintf(stderr, "Arquivo '%s' não encontrado\n", input_filename);
            return 1;
        }

        vector<Class> classes;
        int tmp;
        for (int i = 0; i < num_classes; i++) {
            fscanf(input_file, "%u", &tmp);
            classes.push_back({tmp, 0});
        }
        for (int i = 0; i < num_classes; i++) {
            fscanf(input_file, "%u", &tmp);
            classes[i].end = tmp;
        }
        fclose(input_file);

        for (int exec = 0; exec < 6; exec++) {

            clock_t begin = clock();
            Result unbalanced = greedy(classes);
            float unbalanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float unbalanced_var = variance(unbalanced.classroomsHours);

            begin = clock();
            Result balanced = balancedGreedy(classes);
            float balanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float balanced_var = variance(balanced.classroomsHours);

            printf(" ..%d", exec + 1);
            /*printf("\nun: %zu , bl: %zu", unbalanced.numClassrooms,*/
            /*       balanced.numClassrooms);*/
            fflush(stdout);

            if (exec != 0) {
                fprintf(results, "%u,%f,%f,%f,%f\n", num_classes,
                        unbalanced_time, unbalanced_var, balanced_time,
                        balanced_var);
            }
        }
        fflush(results);
        printf("\n");
    }
    fclose(results);
}