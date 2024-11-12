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
};

int compare(const void *a, const void *b) { // O(8)
    Class *classA = (Class *)a; // atribuição e casing O(2)
    Class *classB = (Class *)b; // atribuição e casting O(2)

    return classA->init - classB->init; // return, subtração e acesso ao "init" O(4)
}

struct Result {
    size_t num_classrooms;
    vector<int> classroomsHours;
};

int findClassroom(vector<int> &classrooms, int init) { // O(5n + 3)
    for (int i = 0; i < classrooms.size(); i++) { // for normal? O(2n + 2)
        if (classrooms[i] <= init) { // index e comparação O(2) * for(n) = O(2n)
            return i; // return O(1) * for(n) = O(n)
        }
    }
    return -1; // return O(1)
}

Result greedy(vector<Class> &classes) {
    qsort(&classes[0], classes.size(), sizeof(Class), compare);
    vector<int> classrooms;
    vector<int> classroomsHours;

    for (const Class &classX : classes) {
        int classroom = findClassroom(classrooms, classX.init);

        if (classroom == -1) {
            classrooms.push_back(classX.end);
            classroomsHours.push_back(classX.end - classX.init);
        } else {
            classrooms[classroom] = classX.end;
            classroomsHours[classroom] += classX.end - classX.init;
        }
    }
    return {classrooms.size(), classroomsHours};
}

int findBalancedClassroom(vector<int> &classrooms, int init,
                          vector<int> &classroomsHours) { // O(10n + 5)
    int minHours = INT_MAX; // atribuição O(1)
    int minIndex = -1; // atribuição O(1)
    for (int i = 0; i < classrooms.size(); i++) { // for comum ? O(2n+2) 
        if (classrooms[i] <= init && classroomsHours[i] < minHours) { // 2 index, 2 comparação, 1 logico O(5) * for(n) = O(5n) 
            minHours = classroomsHours[i]; // atribuição, index = O(2) * for(n) = O(2n)
            minIndex = i; // atribuição O(1) * for(n) = O(n)
        }
    }
    return minIndex; // atribuição O(1)
}

Result balancedGreedy(vector<Class> &classes) {
    qsort(&classes[0], classes.size(), sizeof(Class), compare);
    vector<int> classrooms;
    vector<int> classroomsHours;

    for (const Class &classX : classes) {
        int classroom =
            findBalancedClassroom(classrooms, classX.init, classroomsHours); // atribuição O(1) + função O(10n + 5) = O(10n + 6)

        if (classroom == -1) {
            classrooms.push_back(classX.end);
            classroomsHours.push_back(classX.end - classX.init);
        } else {
            classrooms[classroom] = classX.end;
            classroomsHours[classroom] += classX.end - classX.init;
        }
    }
    return {classrooms.size(), classroomsHours};
}

int main(int argc, char *argv[]) {
    array<int, 28> entradas = {10,     25,     50,     100,    150,    200,
                               300,    500,    750,    1000,   1500,   2000,
                               2500,   5000,   7500,   10000,  15000,  20000,
                               30000,  50000,  75000,  100000, 150000, 250000,
                               350000, 500000, 750000, 1000000};

    FILE *results = fopen("results.csv", "w");
    if (results == NULL) {
        fprintf(stderr, "Não pôde criar 'results.csv'\n");
        return 1;
    }

    fprintf(results,
            "num_classes,unbalanced_time,unbalanced_var,balanced_time,balanced_"
            "var\n");
    for (const int &num_classes : entradas) {
        printf("%7d classes", num_classes);

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
            printf(" ..%d", exec + 1);
            fflush(stdout);

            clock_t begin = clock();
            Result unbalanced = greedy(classes);
            float unbalanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float unbalanced_var = variance(unbalanced.classroomsHours);

            begin = clock();
            Result balanced = balancedGreedy(classes);
            float balanced_time = float(clock() - begin) / CLOCKS_PER_SEC;
            float balanced_var = variance(balanced.classroomsHours);

            if (exec == 0) {
                continue;
            }
            fprintf(results, "%u,%f,%f,%f,%f\n", num_classes, unbalanced_time,
                    unbalanced_var, balanced_time, balanced_var);
        }
        printf("\n");
    }
    fclose(results);
}
