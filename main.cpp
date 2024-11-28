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
        return a.end > b.end; // return + comparação + 2 acessos O(4)
    }

    static bool compareBalanced(const Classroom &a, const Classroom &b) {
        return a.end > b.end || (a.end == b.end && a.used > b.used); // return + 5 comparações + 6 acessos O(12)
    }
};

struct Result {
    size_t numClassrooms;
    vector<int> classroomsHours;
};

Result greedy(vector<Class> &classes, int max_classrooms) {
    sort(classes.begin(), classes.end(), Class::compare); // O(n log n)

    vector<Classroom> classrooms(max_classrooms); // O(n)
    vector<vector<Class>> tasksPerClassroom(max_classrooms); // O(n)
    int front = 0, rear = 0; // Ponteiros da fila circular O(2)
    int size = 0; // Número atual de salas na fila O(1)

    for(const Class &classX : classes) {
        // printf("Classe: init = %d, end = %d\n", classX.init, classX.end);

        bool allocated = false; // Flag para verificar se a aula foi alocada // O(n)
        for (int i = 0; i < size; i++) { // O((2n+2)*n)
            int idx = (front + i) % max_classrooms; // Índice circular O(3n²))
            if (classrooms[idx].end <= classX.init) { // O(4n²)
                // Atualiza a sala existente
                classrooms[idx].end = classX.end; // O(4n²)
                classrooms[idx].used += classX.end - classX.init; // O(7n2)
                tasksPerClassroom[idx].push_back(classX); // Adiciona a aula à sala // O(3n²)
                // printf("Alocada na sala %d (existente): init = %d, end = %d\n", idx + 1, classX.init, classX.end);
                allocated = true; // O(n²)
                break; // O(n²)
            }
        }

        if (!allocated) { // Se não foi possível alocar em nenhuma sala existente // O(n)
            classrooms[rear] = {classX.end, classX.end - classX.init}; // O(6n)
            tasksPerClassroom[rear].push_back(classX); // Adiciona a aula à nova sala O(3n)
            // printf("Alocada na sala %d (nova): init = %d, end = %d\n", rear + 1, classX.init, classX.end);
            rear = (rear + 1) % max_classrooms; // Incrementa circularmente O(3n)
            size++; //O(n)
        }
    }

    // Calcula o número total de horas por sala e imprime as tarefas alocadas
    std::vector<int> classroomHours;
    for (int i = 0; i < size; i++) { // O(2n + 2)
        int idx = (front + i) % max_classrooms; // (3n)
        classroomHours.push_back(classrooms[idx].used); //O(3n)

        // printf("Sala %d:\n", idx + 1);
        // for (const Class &task : tasksPerClassroom[idx]) { // O()
        //     // printf("  Tarefa: init = %d, end = %d\n", task.init, task.end);
        // }
    }

    return {static_cast<size_t>(size), classroomHours}; // O(1)
}

Result balancedGreedy(vector<Class> &classes, int max_classrooms) {
    sort(classes.begin(), classes.end(), Class::compare); // Ordena as classes nlogn

    vector<Classroom> classrooms(max_classrooms); // Vetor dinâmico n
    vector<vector<Class>> tasksPerClassroom(max_classrooms); // Aulas por sala n
    int front = 0, rear = 0; // Ponteiros da fila circular
    int size = 0; // Número atual de salas na fila 3

    for (const Class &classX : classes) {
        // printf("Classe: init = %d, end = %d\n", classX.init, classX.end);

        bool allocated = false; // Flag para verificar se a aula foi alocada n
        int bestIdx = -1; // n
        int minLoad = INT_MAX; // n

        for (int i = 0; i < size; i++) { // 2n²+2n
            int idx = (front + i) % max_classrooms; // Índice circular 3n2
            if (classrooms[idx].end <= classX.init && classrooms[idx].used < minLoad) { // 8n2
                bestIdx = idx;       // Seleciona a sala com menor carga n2
                minLoad = classrooms[idx].used; // 3n2
            }
        }

        if (bestIdx != -1) { // Sala encontrada para alocar n
            classrooms[bestIdx].end = classX.end; // 4n
            classrooms[bestIdx].used += classX.end - classX.init; // 7n
            tasksPerClassroom[bestIdx].push_back(classX); // Adiciona a aula à sala 3n
            // printf("Alocada na sala %d (existente): init = %d, end = %d\n", bestIdx + 1, classX.init, classX.end);
            allocated = true; // n
        }

        if (!allocated) { // Se não foi possível alocar em nenhuma sala existente n 
            classrooms[rear] = {classX.end, classX.end - classX.init}; // 6n
            tasksPerClassroom[rear].push_back(classX); // Adiciona a aula à nova sala 3n
            // printf("Alocada na sala %d (nova): init = %d, end = %d\n", rear + 1, classX.init, classX.end);
            rear = (rear + 1) % max_classrooms; // Incrementa circularmente 3n
            size++; //n
        }
    }

    // Calcula o número total de horas por sala e imprime as tarefas alocadas
    std::vector<int> classroomHours;
    for (int i = 0; i < size; i++) { // 2n+2
        int idx = (front + i) % max_classrooms; // 3n
        classroomHours.push_back(classrooms[idx].used); //3n

        // printf("Sala %d:\n", idx + 1);
        // for (const Class &task : tasksPerClassroom[idx]) {
        //     // printf("  Tarefa: init = %d, end = %d\n", task.init, task.end);
        // }
    }

    return {static_cast<size_t>(size), classroomHours}; // Conversão para evitar warnings 1
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
            "num_classes,unbalanced_time,unbalanced_var,balanced_time,balanced_var\n");

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

        // Determinar MAX_CLASSROOMS dinamicamente
        int max_classrooms = (num_classes < 100) ? 10 : (num_classes / 10);

        for (int exec = 0; exec < 6; exec++) {
            clock_t begin = clock();
            Result unbalanced = greedy(classes, max_classrooms);
            float unbalanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float unbalanced_var = variance(unbalanced.classroomsHours);

            begin = clock();
            Result balanced = balancedGreedy(classes, max_classrooms);
            float balanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float balanced_var = variance(balanced.classroomsHours);

            printf(" ..%d", exec + 1);
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
    return 0;
}
