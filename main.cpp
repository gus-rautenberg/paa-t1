#include <array>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

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

int compare(const void *a, const void *b) {
    Class *classA = (Class *)a;
    Class *classB = (Class *)b;

    return classA->init - classB->init;
}

struct Result {
    size_t num_classrooms;
    vector<int> classroomsHours;
};

int findClassroom(vector<int> &classrooms, int init) {
    for (int i = 0; i < classrooms.size(); i++) {
        if (classrooms[i] <= init) {
            return i;
        }
    }
    return -1;
}

Result greedy(vector<Class> &classes) {
    sort(classes.begin(), classes.end(), compare);
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
                          vector<int> &classroomsHours) {
    int minHours = INT_MAX;
    int minIndex = -1;
    for (int i = 0; i < classrooms.size(); i++) {
        if (classrooms[i] <= init && classroomsHours[i] < minHours) {
            minHours = classroomsHours[i];
            minIndex = i;
        }
    }
    return minIndex;
}

Result balancedGreedy(vector<Class> &classes) {
    sort(classes.begin(), classes.end(), compare);
    vector<int> classrooms;
    vector<int> classroomsHours;

    for (const Class &classX : classes) {
        int classroom =
            findBalancedClassroom(classrooms, classX.init, classroomsHours);

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
