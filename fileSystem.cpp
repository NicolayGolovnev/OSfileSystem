#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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
vector<File> currentCatalog;

int main(){

    //обнуление памяти
    for (int i = 0; i < CAPACITY; i++)
        physMemory[i] = 0;

    while(1){
        string str = ""; // строка для ввода команды
        //вывод path
        cout << path << " > ";
        cin >> str;
        cout << endl;
        //разбиваем строку
        stringstream iss(str);
        string choose;
        iss >> choose;

        //аналог свитча через if
        if (choose == "create"){
            string nameFile = "";
            iss >> nameFile;
        }
        else if (choose == "read"){

        }
        else if (choose == "modify"){
            string buf;
            bool modify;
            modify = false;
            iss >> buf;
            for (int i = 0; i < tableFiles.size(); i++){
                string check;
                check = tableFiles[i].getName();
                if (check == buf){
                    modify = true;
                    break;
                }
            }
            if (!modify){
                cout << "Wrong enter a name file! I cannot open invisible file!\n";
            }
            else{
                //делаем запрос системе на открытия файла в блокноте
                string q = "notepad.exe ";
                q += buf;
                const char* openningANIME = q.c_str();
                system(openningANIME);
            }
        }
        else if (choose == "delete"){

        }
        else if (choose == "info"){

        }
        else if (choose == "cls"){
            system("cls");
        }
        else if (choose == "dir"){

        }
        else if (choose == "help"){
            cout << "\nThat's command you can use in this programm:\n";
            cout << "\tcreate 'name file' \t\t- \n";
            cout << "\tread 'name file' \t\t- read a file from the current directory\n";
            cout << "\tmodify 'name file' \t\t- open text editor for editting file\n";
            cout << "\tdelete 'name catalog or file' \t- \n";
            cout << "\tinfo \t\t\\t- show how much files created\n";
            cout << "\tcls \t\t\t\t- clear a screen\n";
            cout << "\tdir \t\t\t\t- view all files in current catalog\n";
            cout << "\thelp \t\t\t\t- show this page\n";
            cout << "\texit \t\t\t\t- ext from programm\n";
        }
        else if (choose == "exit"){
            cout << "Exit from the programm, press any symbol...\n";
            getch();
            return 0;
        }
        else{//аналог дефаулта
            cout << "Unknown command. For the view full list of command enter 'help'\n";
        }
        
    }

}