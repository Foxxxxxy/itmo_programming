#include <stdio.h>
#include <string.h>

void file_Info(FILE *file, long res[]) {
    // переменные для хранения результатов
    long lines = 0, words = 0;
    char *s;    char str[128];   // буфер строки

    // чтение файла построчно, подсчёт кол-ва слов
    while ((s = fgets(str, sizeof (str), file)) != NULL) {
        int i = 0;
        for (; i < strlen(s); i++) {
            if (i == 0 && s[i] == '\n') // пустая строка
                break;
            char c = s[i];  //текущий символ

            // обработка если символ последний в строке
            if (c == '\n' && s[i - 1] != ' ') {
                words++;
                break;
            } else if (c == '\n' && s[i - 1] == ' ') {
                break;
            }

            if (c == ' ' && i != 0) //если есть пробел не в начале строки - новое слово
                words++;

            while ((c == ' ' || c == '\t') && i < strlen(s)) { // удаление пробелов между словами
                i++;
                c = s[i];
                if (c != ' ') {
                    i--;
                    break;
                }
            }
        }
        if (s[i] == '\n')   // переход на новую строку
            lines++;
    }

    long bytes = ftell(file); //кол-во байт
    fclose(file);

    res[0] = lines + 1; res[1] = words; res[2] = bytes;
}

int main(int argc, char * argv[]) {

    // список доступных команд
    const char * com[3][2] = {
            {"-l", "--lines"},
            {"-w", "--words"},
            {"-c", "--bytes"}
    };

    char * commands[argc - 2];  //список команд из аргументов cmd
    char * file_name; //имя файла из cmd

    // чтение аргументов из argv
    for (int i = 1, c = 0; i < argc; i++) {
        if (i == argc - 1) {
            file_name = argv[i];
            break;
        }
        if (c < argc - 2) {
            commands[c] = argv[i];
        }
        c++;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("No such file.");
        return 0;
    }

    long file_info_array[3] = {0, 0, 0};    //массив с инф-ей о файле
    file_Info(file, file_info_array);   //сбор инф-ии о файле

    // вывод результата используя список команд
    for (int i = 0; i < argc - 2; i++) {
        if (strcmp(commands[i], com[0][0]) == 0 || strcmp(commands[i], com[0][1]) == 0) {
            printf("lines: %ld\n", file_info_array[0]);
            continue;
        }
        if (strcmp(commands[i], com[1][0]) == 0 || strcmp(commands[i], com[1][1]) == 0) {
            printf("words: %ld\n", file_info_array[1]);
            continue;
        }
        if (strcmp(commands[i], com[2][0]) == 0 || strcmp(commands[i], com[2][1]) == 0) {
            printf("bytes: %ld\n", file_info_array[2]);
            continue;
        }

        printf("Unknown command!\n");
    }

    return 0;
}
