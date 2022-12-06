#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
using namespace std;
using namespace std::this_thread; //Использование sleep_for
using namespace std::chrono; //Использование seconds
//Massives - Life
/*Ограничение размера поля связано с особенностями
использования консольного вывода*/
const int MAX_HEIGHT=30; //Выделение памяти для строк поля
const int MAX_WIDTH=20; //Выделение памяти для столбцов поля
const int MAX_CYCLES=10000; //Максимальное значение повторений
//Создать новое случайное поле
void Initiate(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width);
//Отобразить поле
void Print(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width);
//Стереть напечатанное поле
void Erase(int height);
//Перезаписать остаточное число циклов
void RestCycles(int n);
//Вычислить следующее состояние поля
void Next(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width);
//Ввод натурального числа от 1 до MAX с проверкой ошибок
int forcint(int MAX);
int main()
{
    /*Подключение виртуального терминала для Windows 10
    Позволяет использовать управляющие символы ANSI CSI*/
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    /*Конец подключения*/
    //Вывод в консоль windows 10 в Unicode-8
    //Исходный код должен быть в Unicode-8
    SetConsoleOutputCP(CP_UTF8);
    //Задать размер поля и количество повторений
    int height,width,n;
    cout<<"Введите высоту поля от 1 до "<<MAX_HEIGHT<<":\t";
    height=forcint(MAX_HEIGHT);
    cout<<"Введите ширину поля от 1 до "<<MAX_WIDTH<<":\t";
    width=forcint(MAX_WIDTH);
    //Задать лимит расчетов
    cout<<"Введите лимит циклов от 1 до "<<MAX_CYCLES<<":\t";
    n=forcint(MAX_CYCLES);
    cout<<"Осталось циклов: "<<n<<endl;
    //Создать и отобразить случайное поле
    srand(time(0)); //привязка случайности ко времени
    bool array[MAX_HEIGHT][MAX_WIDTH];
    Initiate(array, height, width);
    Print(array, height, width);
    //Задержка на 1 секунду для начальной визуализации 
    sleep_for(seconds(1));
    //Рассчитать циклическую демонстрацию
    do{
        //Вычислить следующее состояние поля
        Next(array, height, width);
        //Стереть предыдущее поле
        Erase(height);
        //Перезаписать остаточное число циклов
        RestCycles(n);
        //Отобразить новое поле
        Print(array, height, width);
        //Задержка на 1 секунду для визуализации 
        sleep_for(seconds(1));
        //Обновить счетчик циклов
        n--;
    }while(n>0);
    return 0;
}
//Создать новое поле
void Initiate(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width){
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            array[i][j]=rand()%2;
}
//Отобразить поле
void Print(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(array[i][j])
                cout<<" x ";
            else
                cout<<"   ";
        }
        cout<<endl;
    }
}
//Стереть напечатанное поле
void Erase(int height){
    for(int i=0;i<height;i++)
        //Перевод каретки на строку вверх через ANSI CSI ESC + [F
        cout<<"\x1B[F";
}
//Перезаписать остаточное число циклов
void RestCycles(int n){
        cout<<"\x1B[FОсталось циклов: ";
        //Стереть число n по длине и перезаписать новым значением
        for(int localn=n;localn>0;localn=localn/10)
            cout<<" ";
        for(int localn=n;localn>0;localn=localn/10)
            cout<<"\b";
        cout<<n-1<<endl;
}
//Вычислить следующее состояние поля
void Next(bool array[MAX_HEIGHT][MAX_WIDTH], int height, int width){
    //Создать массив жизнеспособности, т.е. количества соседей у клеток
    int vital[height][width]={0};
    //Заполнить нулями
    fill(&vital[0][0],&vital[height-1][width-1],0);
    /*Для крайних элементов поля вычислить жизнеспособность,
    включая противоположную сторону поля, т.е. зациклить поле*/
    //Для угловых клеток
    //Для клетки [0][0]
    if(array[height-1][width-1])
        vital[0][0]++;
    if(array[height-1][0])
        vital[0][0]++;
    if(array[height-1][1])
        vital[0][0]++;
    if(array[1][width-1])
        vital[0][0]++;
    if(array[1][0])
        vital[0][0]++;
    if(array[1][1])
        vital[0][0]++;
    if(array[0][width-1])
        vital[0][0]++;
    if(array[0][1])
        vital[0][0]++;
    //Для клетки [0][width-1]
    if(array[height-1][width-2])
        vital[0][width-1]++;
    if(array[height-1][width-1])
        vital[0][width-1]++;
    if(array[height-1][0])
        vital[0][width-1]++;
    if(array[1][width-2])
        vital[0][width-1]++;
    if(array[1][width-1])
        vital[0][width-1]++;
    if(array[1][0])
        vital[0][width-1]++;
    if(array[0][width-2])
        vital[0][width-1]++;
    if(array[0][0])
        vital[0][width-1]++;
    //Для клетки [height-1][0]
    if(array[height-2][width-1])
        vital[height-1][0]++;
    if(array[height-2][0])
        vital[height-1][0]++;
    if(array[height-2][1])
        vital[height-1][0]++;
    if(array[0][width-1])
        vital[height-1][0]++;
    if(array[0][0])
        vital[height-1][0]++;
    if(array[0][1])
        vital[height-1][0]++;
    if(array[height-1][width-1])
        vital[height-1][0]++;
    if(array[height-1][1])
        vital[height-1][0]++;
    //Для клетки [height-1][width-1]
    if(array[height-2][width-2])
        vital[height-1][width-1]++;
    if(array[height-2][width-1])
        vital[height-1][width-1]++;
    if(array[height-2][0])
        vital[height-1][width-1]++;
    if(array[0][width-2])
        vital[height-1][width-1]++;
    if(array[0][width-1])
        vital[height-1][width-1]++;
    if(array[0][0])
        vital[height-1][width-1]++;
    if(array[height-1][width-2])
        vital[height-1][width-1]++;
    if(array[height-1][0])
        vital[height-1][width-1]++;
    //Для первой и последней строки, кроме угловых
    for(int j=1;j<width-1;j++){
        for(int i=0;i<3;i++){
            //Проверка трех элементов "сверху" и снизу первой строки
            if(array[height-1][j-1+i])
                vital[0][j]++;
            if(array[1][j-1+i])
                vital[0][j]++;
            //Проверка трех элементов сверху и "снизу" последней строки
            if(array[height-2][j-1+i])
                vital[height-1][j]++;
            if(array[0][j-1+i])
                vital[height-1][j]++;
        }
        //Проверка боковых элементов
        if(array[0][j-1])
            vital[0][j]++;
        if(array[0][j+1])
            vital[0][j]++;
        if(array[height-1][j-1])
            vital[height-1][j]++;
        if(array[height-1][j+1])
            vital[height-1][j]++;
    }
    //Для левого и правого столбца, кроме угловых
    for(int i=1;i<height-1;i++){
        for(int j=0;j<3;j++){
            //Проверка трех элементов "слева" и справа от левого столбца
            if(array[i-1+j][width-1])
                vital[i][0]++;
            if(array[i-1+j][1])
                vital[i][0]++;
            //Проверка трех элементов слева и "справа" от правого столбца
            if(array[i-1+j][width-2])
                vital[i][width-1]++;
            if(array[i-1+j][0])
                vital[i][width-1]++;
        }
        //Проверка элементов снизу и сверху
        if(array[i-1][0])
            vital[i][0]++;
        if(array[i+1][0])
            vital[i][0]++;
        if(array[i-1][width-1])
            vital[i][width-1]++;
        if(array[i+1][width-1])
            vital[i][width-1]++;
    }
    //Для центральных элементов построчно
    for(int i=1;i<height-1;i++){
        for(int j=1;j<width-1;j++){
            for(int n=0;n<3;n++){
                //Проверка трех элементов сверху и снизу строки
                if(array[i-1][j-1+n])
                    vital[i][j]++;
                if(array[i+1][j-1+n])
                    vital[i][j]++;
            }
            //Проверка боковых элементов
            if(array[i][j-1])
                vital[i][j]++;
            if(array[i][j+1])
                vital[i][j]++;
        }
    }
    /*Обновить поле согласно значению жизнеспособности клеток
    x<2 || x>3 -> смерть
    x == 3 -> жизнь
    x == 2 -> x */
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++){
            switch(vital[i][j]){
                case 2: break;
                case 3:
                    array[i][j]=true;   break;
                default: 
                    array[i][j]=false;  break;
            }
        }
}
//Ввод натурального числа от 1 до MAX с проверкой ошибок
int forcint(int MAX){
    int n;
    do{
    cin>>n;
    if((n>0)&&(n<=MAX)){
        //Сброс остаточных символов для корректности ввода
        cin.clear();
        cin.ignore(100,'\n');
        return n;
    }
    cout<<"Неверное значение! Повторите ввод:\t";
    //Сброс неверно введенных символов
    cin.clear();
    cin.ignore(100,'\n');
    }while(true);
}