#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>

using namespace std;

struct Class
{
    int init;
    int end;
};


int main(int argc, char *argv[]) {

    if(argc < 2){
        cout << "Informe o número de aulas" << endl;
        return 1;
    }

    const char* folder = argv[1];
    DIR* dir = opendir(folder);
    
    if(!dir){
        cout << "Não foi possível abrir o diretório" << endl;
        return 1;
    }



    string folder = "./Entradas para o Problema do Escalonamento de Aulas/";

    for(const auto&entry: fs::directory_iterator(folder)){
        cout << entry.path() << endl;
    }

}

