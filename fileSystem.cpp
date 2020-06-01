#include <iostream>
#include <conio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define CAPACITY 10000

using namespace std;

string path = "C:\\"; //строка, в которой записан текущий адрес расположения в памяти
int physMemory[CAPACITY];

class File{
    private:
    string name; //имя файла
    int size; //размер файла
    int attribute;//только для чтения (1), для чтения и записи (2), особый файл - каталог (3), секретный обязательный файл (-1)
    int adressFirstBlock; // место расположения первого блока в памяти

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
    vector<File*> folderNext; // Папка при определенном атрибуте, где хранятся другие файлы
    vector<File*> folderDown; // папка, которую надо запомнить, чтобы вернутся назад

    File(string Name, int Size, int Attribute){
        this->name = Name;
        this->size = Size;
        this->attribute = Attribute;
        this->adressFirstBlock = -1;
    }
    string getName(){
        return this->name;
    }

    int setInMemory(){
        pair<int, int> flag = this->goInMemory();
        //если можем запихать файл в память, делаем это
        if (flag.first){
            this->adressFirstBlock = flag.second;
            for (int i = flag.second; i < this->size + flag.second; i++)
                physMemory[i] = 1;
            return 1;
        }
        else{
            cout << "Could not create a file, because we do not have a space for this!\n";
            return 0;
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
            case -1:
                cout << "secret)";
                break;
        }
        cout << ": " << this->name << "\tSize: " << this->size; 
        cout << "\tAddress the first block: " << this->adressFirstBlock << endl;
        
    }

    void outFileForDir(){
        cout << this->size << " blocks\t<";
        switch(this->attribute){
            case 1:
                cout << "R>";
                break;
            case 2:
                cout << "W>";
                break;
            case 3:
                cout << "C>";
                break;
            case -1:
                cout << "S>";
                break;
        }
        cout << "\t" << this->name << endl;
    }

    int getSize(){
        return this->size;
    }
    void setSize(int s){
        this->size = s;
    }
    int getAttribute(){
        return this->attribute;
    }

};

vector<File*> tableFiles;
vector<File*> currentFolder, mainFolder; // текущий каталог, и каталог главного тома - голова
vector<File*> forSave = mainFolder; //нужно для связи каталогов между собой
vector<string> foldersOnLevel; // путь, где расписаны по уровням каталоги, находимся в данный момент тут по программе
int level = 0; // уровень, на котором находимся
int indexForSaveList; // индекс для сохранения каталога, когда спускаемся вниз

void startProgram(){
    //записываем обязательный файл в каталог, чтобы можно было возвращаться(такой файл находится на каждом уровне в 0 позиции)
    File* a = new File("C:\\", 0, -1);
    mainFolder.push_back(a);
    //говорим, что мы находимся изначально в корне
    foldersOnLevel.clear();
    currentFolder = mainFolder;
    //обнуление памяти
    for (int i = 0; i < CAPACITY; i++)
        physMemory[i] = 0;
}

int main(){
    
    startProgram();

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
            if (nameFile.empty()){
                cout << "Enter a name of file: ";
                cin >> nameFile;
            }
            cout << "Enter a size of file: ";
            int size; cin >> size;
            cout << "Enter an attribute of file (1 - only read, 2 - write and read, 3 - catalog file): ";
            int attribute; cin >> attribute;
            
            File* a = new File(nameFile, size, attribute);
            if(a->setInMemory()){ //есть место в памяти, записываем в процессы    
                //добавить в список всех файлов
                tableFiles.push_back(a);
                //если папка, обнуляем размер
                if (attribute == 3)
                    a->setSize(0);
                //добавить в папку, котором мы находимся прямо сейчас
                currentFolder.push_back(a);
                if (attribute != 3){
                    //создаем настоящий файл для дальнейшей работы с ним (кроме каталогов)
                    ofstream af(nameFile);
                    af.close();
                }
                //если создали каталог, то нужно сделать в нем секретный файл, через который будем возвращаться
                else{
                    File* q = new File(nameFile + "\\", 0, -1);
                    q->folderDown = currentFolder;
                    a->folderNext.push_back(q);
                }
            }
            
        }
        else if (choose == "read"){
            //открываем файл, выводим просто все строчки в консоль, готово
            string nameFile;
            iss >> nameFile;
            ifstream file(nameFile);
            if (!file)
                cout << "Could not open file!\n";
            else{
                string buf;
                cout << endl;
                while(getline(file, buf, '\n'))
                    cout << buf << endl;
            }
        }
        else if (choose == "modify"){
            string buf;
            bool modify;
            modify = false;
            iss >> buf; 
            
            for (int i = 0; i < tableFiles.size(); i++){
                string check;
                check = tableFiles[i]->getName();
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
            string nameFile;
            iss >> nameFile;
            for (int i = 1; i < currentFolder.size(); i++){
                if (currentFolder[i]->getName() == nameFile){

                }
            }
        }
        else if (choose == "cd"){
            string cd;
            iss >> cd;
            if (cd == ".."){
                //если корень, то куда то ниже спускаться???
                if (level == 0)
                    continue;
                //спускаемся вниз
                //считаем, сколько было символов у каталога, чтобы изменить путь, который мы получим в итоге
                int sizeStr = currentFolder[0]->getName().size();                
                path.erase(path.size() - sizeStr, sizeStr);
                
                level--;
                foldersOnLevel.pop_back();
                currentFolder[0]->folderDown[indexForSaveList]->folderNext = currentFolder;
                currentFolder = currentFolder[0]->folderDown;
                forSave = currentFolder;
            }
            else if (cd == "."){
                //сохраняем наш текущий список перед изменением
                currentFolder[0]->folderDown[indexForSaveList]->folderNext = currentFolder;
                //спускаемся в самый корень
                level = 0;
                for (int i = 0; i < foldersOnLevel.size(); i++)
                    foldersOnLevel.pop_back();
                forSave = mainFolder;
                currentFolder = mainFolder;
                path = "C:\\";
            }
            else{ // поднимаем наверх
                //сохраняем текущий каталог в корневой, если находимся в корневом
                if (level == 0){
                    mainFolder = currentFolder;
                }
                    
                for (int j = 1; j < currentFolder.size(); j++){
                    //переход в ту дерикторию, куда нам нужно создать файл
                    if (currentFolder[j]->getName() == cd && currentFolder[j]->getAttribute() == 3){
                        forSave = currentFolder;
                        indexForSaveList = j;
                        currentFolder = currentFolder[j]->folderNext;
                        currentFolder[0]->folderDown = forSave;
                        level++;
                        foldersOnLevel.push_back(cd);
                        path += cd + "\\";
                        break;
                    }
                }
            }
            
        }
        else if (choose == "info"){
            //выписываем таблицу файлов
            for (int i = 0; i < tableFiles.size(); i++)
                tableFiles[i]->outFileInfo();

            //выписываем занятую память
        }
        else if (choose == "cls"){
            system("cls");
        }
        else if (choose == "dir"){
            //место - <аттрибут> - имя файла
            int capacitySize = 0;
            for (int i = 1; i < tableFiles.size(); i++)
                capacitySize += tableFiles[i]->getSize();
            cout << endl << capacitySize << " blocks\t< >\t.";
            capacitySize = 0;
            for (int i = 0; i < currentFolder.size(); i++)
                capacitySize += currentFolder[i]->getSize();
            cout << endl << capacitySize << " blocks\t< >\t.." << endl;
            //вывод всех файлов текущего каталога, кроме секретного
            for (int i = 1; i < currentFolder.size(); i++)
                currentFolder[i]->outFileForDir();

        }
        else if (choose == "help"){
            cout << "\nThat's command you can use in this programm:\n";
            cout << "\tcreate 'name file' \t\t- create a file with parameters\n";
            cout << "\tread 'name file' \t\t- read a file from the current directory\n";
            cout << "\tmodify 'name file' \t\t- open text editor for editting file\n";
            cout << "\tdelete 'name catalog or file' \t- \n";
            cout << "\tcd 'catalog' \t\t\t- go up in the catalog, which you choose\n";
            cout << "\tinfo \t\t\t\t- show how much files created\n";
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