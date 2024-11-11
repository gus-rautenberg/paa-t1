#include <cstdio>
#include <vector>

using namespace std;

struct Class {
    unsigned init;
    unsigned end;
};

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

    Class *classes = new Class[num_classes];

    unsigned tmp;
    for (int i = 0; i < num_classes; i++) {
        fscanf(input_file, "%u", &tmp);
        classes[i].init = tmp;
    }
    for (int i = 0; i < num_classes; i++) {
        fscanf(input_file, "%u", &tmp);
        classes[i].end = tmp;
    }

    delete[] classes;
    fclose(input_file);
}
