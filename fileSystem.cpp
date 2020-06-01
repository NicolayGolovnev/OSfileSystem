#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define CAPACITY 1000

using namespace std;

string path = "C:\\"; //строка, в которой записан текущий адрес расположения в памяти
int physMemory[CAPACITY];

class File{
    private:
    string name; //имя файла
    int size; //размер файла
    int attribute;//только для чтения (1), для чтения и записи (2), особый файл - каталог (3)
    int adressFirstBlock; // место расположения первого блока в памяти
    vector<File> folderNext; // Папка при определенном атрибуте, где хранятся другие файлы
    vector<File> folderDown; // папка, которую надо запомнить, чтобы вернутся назад

    //функция взятая из моего кода 8 лабы и модифицирована
    //pair<булева переменная для определения размещения в памяти, адрес первого блока>
    pair<int, int> goInMemory(){
        int begin;
        for (int i = 0; i < CAPACITY; i++){
            int noSpace = 0;
            for (int j = i; j < i + this->size; j++)
            // память
                if (physMemory[j] == 1){
                    noSpace = 1;
                    break;
                }
            if (!noSpace)
                return {1, i};
        }
        return {0, -1};
    }

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

    void setInMemory(){
        pair<int, int> flag = this->goInMemory();
        //если можем запихать файл в память, делаем это
        if (flag.first){
            this->adressFirstBlock = flag.second;
            for (int i = flag.second; i < this->size + flag.second; i++)
                physMemory[i] = 1;
        }
        else{
            cout << "Could not create a file, because we do not have a space for this!\n";
        }
    }

    void outFileInfo(){
        cout << "File (Attribute: "; 
        switch(this->attribute){
            case 1:
                cout << "only read)";
                break;
            case 2:
                cout << "write and read)";
                break;
            case 3:
                cout << "catalog)";
                break;
        }
        cout << ": " << this->name << "\tSize: " << this->size; 
        cout << "\tAddress the first block: " << this->adressFirstBlock << endl;
        
    }

    vector<File> getFolder(){
        return this->folderNext;
    }
    void setLevelDown(vector<File> a){
        this->folderDown = a;
    }
    vector<File> getLevelDown(){
        return this->folderDown;
    }
};

vector<File> tableFiles;
vector<File> currentFolder, mainFolder; // текущий каталог, и каталог главного тома - голова
vector<File> forSave = mainFolder; //нужно для связи каталогов между собой
vector<string> foldersOnLevel; // путь, где расписаны по уровням каталоги, находимся в данный момент тут по программе
int level = 0; // уровень, на котором находимся

int main(){

    /* File a("1.txt", 1, 1);
    tableFiles.push_back(a);
    for (int i = 0; i < tableFiles.size(); i++)
        tableFiles[i].outFileInfo(); */
    
    //говорим, что мы находимся изначально в корне
    foldersOnLevel.clear();
    currentFolder = mainFolder;
    //обнуление памяти
    for (int i = 0; i < CAPACITY; i++)
        physMemory[i] = 0;

    while(1){
        string str; // строка для ввода команды
        //вывод path
        cout << path << " > ";
        fflush(stdin);
        getline(cin, str, '\n');
        
        //разбиваем строку
        stringstream iss(str);
        string choose;
        iss >> choose;

        //аналог свитча через if
        if (choose == "create"){
            string nameFile = "";
            iss >> nameFile;
            cout << "Enter a size of file: ";
            int size; cin >> size;
            cout << "Enter an attribute of file (1 - only read, 2 - write and read, 3 - catalog file): ";
            int attribute; cin >> attribute;
            
            File a(nameFile, size, attribute);
            //добавить в список всех файлов
            tableFiles.push_back(a);
            
            //добавить возврат в предыдущий каталог
            a.setLevelDown(forSave);
            //добавить в папку, котором мы находимся прямо сейчас
            currentFolder.push_back(a);

        }
        else if (choose == "read"){
            //открываем файл, выводим просто все строчки в консоль, готово

        }
        else if (choose == "modify"){
            string buf;
            bool modify;
            modify = false;
            iss >> buf; 
            
            for (int i = 0; i < tableFiles.size(); i++){
                string check;
                check = tableFiles[i].getName();
                cout << "Checker: " << check << endl;
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
        else if (choose == "cd"){
            string cd;
            iss >> cd;
            if (cd == ".."){
                //спускаемся вниз
                level--;
                foldersOnLevel.pop_back();
                currentFolder = currentFolder[0].getLevelDown();
                forSave = currentFolder;//на всякий для защиты
            }
            else if (cd == "."){
                //спускаемся в самый корень
                level = 0;
                for (int i = 0; i < foldersOnLevel.size(); i++)
                    foldersOnLevel.pop_back();
                forSave = mainFolder;
                currentFolder = mainFolder;
            }
            else{ // поднимаем наверх
                foldersOnLevel.push_back(cd);
                forSave = currentFolder;
                for (int j = 0; j < currentFolder.size(); j++){
                    //переход в ту дерикторию, куда нам нужно создать файл
                    if (currentFolder[j].getName() == cd){
                        forSave = currentFolder;
                        currentFolder = currentFolder[j].getFolder();
                        level++;
                        break;
                    }
                }
            }
            
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
        cout << endl;
    }

}