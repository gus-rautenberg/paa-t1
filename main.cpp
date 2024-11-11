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

