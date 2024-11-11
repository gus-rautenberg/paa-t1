#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct Class {
    unsigned init;
    unsigned end;
};

int compare(const void *a, const void *b) {
    Class *classA = (Class *)a;
    Class *classB = (Class *)b;

    return classA->init - classB->init;
}

int findClassroom(vector<int> &classrooms, int init) {
    for (int i = 0; i < classrooms.size(); i++) {
        if (classrooms[i] <= init) {
            return i;
        }
    }
    return -1;
}

unsigned greedy(vector<Class> &classes) {
    qsort(&classes[0], classes.size(), sizeof(Class), compare);
    vector<int> classrooms;

    for (const Class &classX : classes) {
        int classroom = findClassroom(classrooms, classX.init);

        if (classroom == -1) {
            classrooms.push_back(classX.end);
        } else {
            classrooms[classroom] = classX.end;
        }
    }
    return classrooms.size();
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

unsigned balancedGreedy(vector<Class> &classes) {
    qsort(&classes[0], classes.size(), sizeof(Class), compare);
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
    return classrooms.size();
}
int main(int argc, char *argv[]) {
    unsigned num_classes;
    if (argc < 2 || sscanf(argv[1], "%u", &num_classes) != 1) {
        fprintf(stderr, "Informe o número de tarefas\n");
        return 1;
    }
    char input_filename[32];
    sprintf(input_filename, "entradas/Aula%u.txt", num_classes);
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Arquivo '%s' não encontrado\n", input_filename);
        return 1;
    }

    vector<Class> classes;
    unsigned tmp;
    for (int i = 0; i < num_classes; i++) {
        fscanf(input_file, "%u", &tmp);
        classes.push_back({tmp, 0});
    }
    for (int i = 0; i < num_classes; i++) {
        fscanf(input_file, "%u", &tmp);
        classes[i].end = tmp;
    }
    fclose(input_file);

    unsigned num_classrooms = greedy(classes);
    printf("Classrooms: %u\n", num_classrooms);

    unsigned num_classrooms_balanced = balancedGreedy(classes);
    printf("Classrooms (balanced): %u\n", num_classrooms_balanced);
}
