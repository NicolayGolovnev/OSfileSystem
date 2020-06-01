#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>

#define CAPACITY 10000

using namespace std;

class File{
    private:
    string name;
    int size;
    int attribute;//только для чтения, только для записи, для обоих вида, особый файл - каталог
    int adressFirstBlock;

    public:
    File(string Name, int Size, int Attribute){
        this->name = Name;
        this->size = Size;
        this->attribute = Attribute;
        this->adressFirstBlock = -1;
    }
    string getName(){
        return this->name;
    }
};

string path = "C:\\"; //строка, в которой записан текущий адрес расположения в памяти
int physMemory[CAPACITY];
vector<File> tableFiles;

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
            string name;
            iss >> name;

            ofstream modify;
            for (int i = 0; i < tableFiles.size(); i++){
                name = tableFiles[i].getName();
                modify.open(name);
                if (modify)
                    break;
            }
            
            break;

        case "delete":

            break;
        
        case "info":
            cout << "something output info of files";
            break;

        case "cls":
            system("cls");
            break;

        case "dir":

            break;

        case "help":
            cout << "\nThat's command you can use in this programm:\n";
            cout << "\tread 'name file' - read a file from the current directory\n";
            cout << "\tview 'name file' - \n";
            cout << "\tmodify 'name file' - open text editor for editting file\n";
            cout << "\tdelete 'name catalog or file' - \n";
            cout << "\tinfo - show how much files created\n";
            cout << "\tcls - clear a screen\n";
            cout << "\thelp - show this page\n";
            cout << "\tdir - view all files in current catalog\n";
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