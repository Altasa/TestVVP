#include <iostream>
#include <Windows.h>//Для SetConsole
using namespace std;
int main(){
    //Установка ввода и вывода консоли в стандартную Российскую кодировку
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    return 0;
}