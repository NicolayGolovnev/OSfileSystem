#include <iostream>
#include <conio.h>
#include <string>

#define CAPACITY 10000

using namespace std;

string path = ""; //строка, в которой записан текущий адрес расположения в памяти
int physMemory[CAPACITY];

int main(){

    //обнуление памяти
    for (int i = 0; i < CAPACITY; i++)
        physMemory[i] = 0;

    while(1){
        string str; // строка для ввода команды
        cout << endl;
        //вывод path
        cout << " > ";
        cin >> str;
        //разбиваем строку
        istringstream iss(entered); string choose;
        //copy(istreambuf_iterator<string>(iss), istreambuf_iterator<string>(), back_inserter<vector<string> >(tokens));
        iss >> choose;
        switch (choose)
        {
        case "read":
            /* code */
            break;

        case "view":
            //code
            break;

        case "modify":
            break;

        case "delete":

            break;
        
        case "info":
            cout << "something output info of files";
            break;

        case "cls":
            system("cls");
            break;

        case "help":
            cout << "\nThat's command you can use in this programm:\n";
            cout << "\tread 'name file' - read a file from the current directory\n";
            cout << "\tview 'name file' - \n";
            cout << "\tmodify 'name file' - \n";
            cout << "\tdelete 'name catalog or file' - \n";
            cout << "\tinfo - show how much files created\n";
            cout << "\tcls - clear a screen\n";
            cout << "\thelp - show this page\n";
            cout << "\texit - ext from programm\n";
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