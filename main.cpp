#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <math.h>
#include <vector>
using namespace std;

struct Class
{
    int init;
    int end;
};



int main(int argc, char *argv[]) {

    // if(argc < 2){
    //     cout << "Informe o número de aulas" << endl;
    //     return 1;
    // }

    // const char* folder = argv[1];
    // DIR* dir = opendir(folder);

    // if(!dir){
    //     cout << "Não foi possível abrir o diretório" << endl;
    //     return 1;
    // }



    // string folder = "./Entradas para o Problema do Escalonamento de Aulas/";

    // for(const auto&entry: fs::directory_iterator(folder)){
    //     cout << entry.path() << endl;
    // }
    return 0;
}

int compare(const void *a, const void *b){
    Class* classA = (Class*) a;
    Class* classB = (Class*) b;

    return classA->init - classB->init;
}

void greedy(Class classes[], int n){

    qsort(classes, n, sizeof(Class), compare);
    vector<int> classrooms;

    for(int i = 0; i < n; i++){
        Class classX = classes[i];        

        int classroom = findClassroom(classrooms, classX.init);

        if(classroom == -1){
            classrooms.push_back(classX.end);
        }else{
            classrooms[classroom] = classX.end;
        }
    }
    
    cout << "Classrooms: " << classrooms.size() << endl;
}  

int findClassroom(vector<int> classrooms, int init){
    for(int i = 0; i < classrooms.size(); i++){
        if(classrooms[i] <= init){
            return i;
        }
    }

    return -1;
}

int findBalancedClassroom(vector<int> classrooms, int init, vector<int> classroomsHours){
    vector<int> aux;
    for(int i = 0; i < classrooms.size(); i++){
        if(classrooms[i] <= init) {
            aux.push_back(i);
        }
    }
    if(aux.size() == 0) return -1;

    int min = INFINITY;
    int minIndex = 0;
    for(int i = 0; i < aux.size(); i++){
        if(classroomsHours[aux[i]] < min){
            min = classroomsHours[aux[i]];
            minIndex = aux[i];
        }
    }
    
    return minIndex;
}

void balancedGreedy(Class classes[], int n){

    qsort(classes, n, sizeof(Class), compare);
    vector<int> classrooms;
    vector<int> classroomsHours;

    for(int i = 0; i < n; i++){
        Class classX = classes[i];        

        int classroom = findBalancedClassroom(classrooms, classX.init, classroomsHours);

        if(classroom == -1){
            classrooms.push_back(classX.end);
            classroomsHours.push_back(classX.end - classX.init);
        }else{
            classrooms[classroom] = classX.end;
            classroomsHours[classroom] += classX.end - classX.init;
        }
    }
    
    cout << "Classrooms: " << classrooms.size() << endl;
}  