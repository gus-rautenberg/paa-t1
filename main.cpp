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

Result greedy(vector<Class> &classes) { // O(NlogN + 5n² + 6n + 4)
    sort(classes.begin(), classes.end(), Class::compare); // sort O(NlogN)
    // for (auto& Class : classes) {
    //     printf("%d %d\n", Class.init, Class.end);
    // }
    vector<Classroom> classrooms;
    // printf("Greed: \n", classes.size());


    for (const Class &classX : classes) { // for normal? O(2n + 2)
        if (classrooms.empty()) {
            classrooms.push_back({classX.end, classX.end - classX.init});
            push_heap(classrooms.begin(), classrooms.end(), Classroom::compare);
            continue;
            // printf("Vazio\n");
        }

        // for(int i = 0; i < classrooms.size(); i++){
        //     printf("%d %d\n", classrooms[i].used, classrooms[i].end);
        // }
        Classroom classroom = classrooms.front();
        if (classroom.end > classX.init) {
            classrooms.push_back({classX.end, classX.end - classX.init});
            push_heap(classrooms.begin(), classrooms.end(), Classroom::compare);
            // printf("classroom.end > classX.init\n");
            // for(int i = 0; i < classrooms.size(); i++){
            //     printf("%d %d\n", classrooms[i].used, classrooms[i].end);
            // }
            continue;
        }

        

        pop_heap(classrooms.begin(), classrooms.end(), Classroom::compare);
        classrooms.pop_back();
        // printf("pop_heap\n");

        // for(int i = 0; i < classrooms.size(); i++){
        //     printf("%d %d\n", classrooms[i].used, classrooms[i].end);
        // }

        classroom.end = classX.end;
        classroom.used += classX.end - classX.init;

        classrooms.push_back(classroom);
        push_heap(classrooms.begin(), classrooms.end(), Classroom::compare);
        // printf("push_heap\n");
        // for(int i = 0; i < classrooms.size(); i++){
        //     printf("%d %d\n", classrooms[i].used, classrooms[i].end);
        // }
    }
    std::vector<int> classroomHours;
    for (Classroom &classroom : classrooms) {
        classroomHours.push_back(classroom.used);
    }
    // printf("Acabou Greed %d, classrooms.size()\n", classrooms.size());
    return {classrooms.size(), classroomHours};
}

Result balancedGreedy(vector<Class> &classes) { // O(NlogN + 10n² + 18n + 4)
    sort(classes.begin(), classes.end(), Class::compare); // sort O(NlogN)
    vector<Classroom> classrooms;

    for (const Class &classX : classes) { // for normal? O(2n + 2)
        if (classrooms.empty()) {
            classrooms.push_back({classX.end, classX.end - classX.init});
            push_heap(classrooms.begin(), classrooms.end(),
                      Classroom::compareBalanced);
            continue;
        }

        Classroom classroom = classrooms.front();
        if (classroom.end > classX.init) {
            classrooms.push_back({classX.end, classX.end - classX.init});
            push_heap(classrooms.begin(), classrooms.end(),
                      Classroom::compareBalanced);
            continue;
        }

        pop_heap(classrooms.begin(), classrooms.end(),
                 Classroom::compareBalanced);
        classrooms.pop_back();

        classroom.end = classX.end;
        classroom.used += classX.end - classX.init;

        classrooms.push_back(classroom);
        push_heap(classrooms.begin(), classrooms.end(),
                  Classroom::compareBalanced);
    }
    std::vector<int> classroomHours;
    for (Classroom &classroom : classrooms) {
        classroomHours.push_back(classroom.used);
    }
    return {classrooms.size(), classroomHours};
}

int main(int argc, char *argv[]) {
    array<int, 27> entradas = {
        10,    25,    50,    100,    150,    200,    300,    500,    750,
        1000,  1500,  2000,  2500,   5000,   7500,   10000,  15000,  20000,
        30000, 50000, 75000, 100000, 150000, 250000, 350000, 500000, 750000};
    // array<int, 1> entradas = {10};

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