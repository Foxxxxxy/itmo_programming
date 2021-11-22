#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>

const int BASE_STR_LEN = 400;

int *data_time;
int size = 0, capacity = 2;

int cur_bottom, cur_top;

char **error_list;
int error_counter;

int parse_time (int day, int hrs, int mins, int sec) { // перевод времени в секунды
    int time_sec = day * 86400 + hrs * 3600 + mins * 60 + sec;
    return time_sec;
}

void back_parse_time (int time) {
    int days = time / 86400;
    time %= 86400;
    int hrs = time / 3600;  // перевод из секунд в нормальное время
    time %= 3600;
    int mins = time / 60;
    time %= 60;
    int sec = time;
    printf("%dJul %d:%d:%d\n", days, hrs, mins, sec);
}

int * parse_str (char *s) { // парсинг входной строки
    int c = 0;
    int *res = (int *)malloc(sizeof (int) * 5);

    while (s[c] != '[')
        ++c;

    int day = (s[c + 1] - '0') * 10 + (s[c + 2] - '0');
    int hrs = (s[c + 13] - '0') * 10 + (s[c + 14] - '0');
    int mins = (s[c + 16] - '0') * 10 + (s[c + 17] - '0');
    int sec = (s[c + 19] - '0') * 10 + (s[c + 20] - '0');

    int k = 0;
    while (k < 2) {
        if (s[c] == '"')
            ++k;
        ++c;
    }

    int code = s[c + 1] - '0';

    res[0] = day;
    res[1] = hrs;
    res[2] = mins;
    res[3] = sec;
    res[4] = code;

    return res;
}

void push (int val) {   // выыделение памяти под массив времени
    if (size + 1 >= capacity) {
        data_time = (int *) realloc(data_time, sizeof(int) * capacity * 2);
        capacity *= 2;
    }
    data_time[size] = val;
    ++size;
}

void add_str (char *s, int ec) {    // выделение памяти под массив строк
    error_list = (char **) realloc(error_list, sizeof(char *) * (ec + 1));
    error_list[ec] = (char *) malloc(sizeof(char) * BASE_STR_LEN);
    strcpy(error_list[ec], s);
}

void print_error_list () { // вывод списка ошибок
    printf("Errors occured: %d\n", error_counter);
    for (int i = 0; i < error_counter; ++i)
        printf("%s\n", error_list[i]);
}

int request_count (int time) {  // подсчёт запросов во временнгом окне
    int p1 = 0, p2 = 0;
    int max_diff = 0;
    while (p1 < size && p2 < size) {
        while (p2 < size && (data_time[p2] - data_time[p1]) <= time)
            ++p2;
        int t = p2 - p1;
        if (t >= max_diff) {
            max_diff = t;
            cur_bottom = p1;
            cur_top = p2 - 1;
        }
        ++p1;
    }

    return max_diff;
}

int main() {
    FILE *file = fopen("C:\\Users\\Nikita\\Documents\\prog_labs\\NASAlog\\access_log_Jul95.txt", "r");
    data_time = (int *) malloc(sizeof(int) * capacity);
    error_list = NULL;
    error_counter = 0;      //открытие файла и создание динамических массивов

    char *s = (char *)malloc(BASE_STR_LEN); // буфер строки

    while (fgets(s, BASE_STR_LEN, file)) {  // первое считывание файла, подсчёт кол-ва ошибок в запросах
        int * data = parse_str(s);
        int sec = parse_time(data[0], data[1], data[2], data[3]);
        if (data[4] == 5) {
            add_str(s, error_counter);  // формирование списка запросов с ошибками
            ++error_counter;
        }
        push(sec);

        free(data);
    }
    rewind(file); // возврат указателя в начало файла
    //printf("%d\n", size);
    printf("Choose an option :\n1 - print error list\n2 - select time\n0 - exit\n");
    int option;
    while (scanf("%d", &option)) {
        if (option == 1) // вывод списка ошибок
            print_error_list();
        else if (option == 2) { // параметризация временного окна и вывод ответа
            int days, hrs, min, sec;
            printf("Days :");
            scanf("%d", &days);
            printf("Hours :");
            scanf("%d", &hrs);
            printf("Minutes :");
            scanf("%d", &min);
            printf("Seconds :");
            scanf("%d", &sec);

            int time = parse_time(days, hrs, min, sec);
            int res = request_count(time);

            printf("%d\n", res);
            back_parse_time(data_time[cur_bottom]);
            back_parse_time(data_time[cur_top]);
        } else if (option == 0) {
            break;  // выход из программы
        } else {
            printf("No such option.\n");
        }
    }


    free(error_list);
    free(data_time);
    fclose(file);
    return 0;
}
