#include <iostream>
#include <conio.h>
#include <string>

#define CAPACITY 10000

using namespace std;

string path = ""; //строка, в которой записан текущий адрес расположения в памяти
int physMemory[CAPACITY];

int main(){

    while(1){
        string str; // строка для ввода команды
        //vector <string> attributes; // массив слов, которые разбились из строки выше
        cin >> str;
        //разбиваем строку
        istringstream iss(entered); string choose;
        //copy(istreambuf_iterator<string>(iss), istreambuf_iterator<string>(), back_inserter<vector<string> >(tokens));
        iss >> choose;
        switch (choose)
        {
        case "":
            /* code */
            break;
        
        case "exit":
            cout << "Exit from the programm, press any symbol...\n";
            getch();
            return 0;

        default:
            break;
        }
    }
}