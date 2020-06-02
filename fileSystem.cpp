#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <windows.h>

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
        //если папка, то она не имеет веса
        if (this->attribute == 3)
            return {1, 0};
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
    time_t timeinfo; // время создания файла

    File(string Name, int Size, int Attribute, time_t time){
        this->name = Name;
        this->size = Size;
        this->attribute = Attribute;
        this->adressFirstBlock = -1;
        this->timeinfo = time;
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
                cout << "only read):\t\t";
                break;
            case 2:
                cout << "write and read):\t";
                break;
            case 3:
                cout << "catalog):\t\t";
                break;
            case -1:
                cout << "secret):\t\t";
                break;
        }
        cout << this->name << " \tSize: " << this->size; 
        cout << " \tAddress the first block: " << this->adressFirstBlock;
        tm* timeinfo = localtime(&(this->timeinfo));
        cout << " \tTime create: " << timeinfo->tm_mday << "/" << timeinfo->tm_mon + 1 << "/";
        cout << timeinfo->tm_year + 1900 << "\t";
        printf("%02d:%02d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        
    }

    void outFileForDir(){
        tm* timeinfo = localtime(&(this->timeinfo));
        cout << timeinfo->tm_mday << "/" << timeinfo->tm_mon + 1 << "/";
        cout << timeinfo->tm_year + 1900 << "\t";
        printf("%02d:%02d:%02d\t", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
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
    //графическое приветствие
    cout << "Compilition date: " << __DATE__ << " " << __TIME__ << "\n\n";
    cout << "Laboratory Work 9. File System OS\n";
    cout << "Created by Nikolay Golovnev, student of software ingineering ASTU\n\n";

    //записываем обязательный файл в каталог, чтобы можно было возвращаться(такой файл находится на каждом уровне в 0 позиции)
    time_t seconds = time(NULL);

    File* a = new File("C:\\", 0, -1, seconds);
    mainFolder.push_back(a);
    //если хотим спустится ниже, то спустимся сюда же
    mainFolder[0]->folderDown = mainFolder;
    //обнуление памяти
    for (int i = 0; i < CAPACITY; i++)
        physMemory[i] = 0;

    //открываем файл, считываем наши данные в таблицу файлов
    ifstream in("files.txt");
    if (in){
        int n; in >> n;
        for (int i = 0; i < n; i++){
            string name;
            int size, attribute;
            in >> name >> size >> attribute;
            time_t seconds = time(NULL);
            File* a = new File(name, size, attribute, seconds);
            if (a->setInMemory()){
                //добавить в список всех файлов
                tableFiles.push_back(a);
                //если папка, обнуляем размер
                if (attribute == 3)
                    a->setSize(0);
                //добавить в корень
                mainFolder.push_back(a);
                //если это каталог, то нужно сделать в нем секретный файл, через который будем возвращаться
                if (attribute == 3){
                    time_t seconds = time(NULL);
                    File* q = new File(name + "\\", 0, -1, seconds);
                    q->folderDown = mainFolder;
                    a->folderNext.push_back(q);
                }
            }
        }
    }
    in.close();

    //говорим, что мы находимся изначально в корне
    foldersOnLevel.clear();
    currentFolder = mainFolder;
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
            else if (nameFile == "files.txt")
                do{
                    cout << "This name you can not use, its SYSTEM_CATALOG! Enter another name!";
                    cin >> nameFile;
                }while(nameFile == "files.txt");
            cout << "Enter a size of file: ";
            int size; cin >> size;
            cout << "Enter an attribute of file (1 - only read, 2 - write and read, 3 - catalog file): ";
            int attribute; cin >> attribute;

            //забираем текущее время
            time_t seconds = time(NULL);
            
            File* a = new File(nameFile, size, attribute, seconds);
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
                    time_t seconds = time(NULL);
                    File* q = new File(nameFile + "\\", 0, -1, seconds);
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
            int index = -1;
            for (int i = 0; i < tableFiles.size(); i++){
                string check;
                check = tableFiles[i]->getName();
                
                if (check == buf){
                    modify = true;
                    index = i;
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

                int newSize = tableFiles[index]->getSize(), inputLag;
                cout << "Input a new size of file: if you dont want to change, enter a -1: ";
                cin >> inputLag;
                if (inputLag > 0)
                    newSize = inputLag;

                tableFiles[index]->setSize(newSize);
                for (int i = 0; i < currentFolder.size(); i++)
                    if (currentFolder[i]->getName() == tableFiles[index]->getName())
                        currentFolder[i]->setSize(newSize);

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
                //если корень - ничего не делаем
                if (level == 0)
                    continue;
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

            //выписываем занятую память (взято из 8 лабораторной)
            //для работы с цветом в консоли
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            
            SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 15));
            cout << "\n\nCAPACITY OF PHYS MEMORY - " << CAPACITY << " blocks";
            // отрисовка оперативной памяти
            cout << "\n ";
            for (int i = 0; i < 100; i++)
                cout << "-";
            cout << " \n|";
            char flag = 0;
            for (int i = 0; i < 100; i++){
                flag = 0;
                for (int j = 0; j < CAPACITY / 100; j++)
                    if (physMemory[CAPACITY / 100 * i + j] == 1){
                        SetConsoleTextAttribute(hConsole, (WORD) ((14 << 4) | 8));
                        cout << " "; 
                        flag = 1;
                        break;
                    }
                if (!flag){
                    SetConsoleTextAttribute(hConsole, (WORD) ((8 << 4) | 14));
                    cout << " "; 
                }
            }
            SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 15));
            cout << "|\n";
            cout << " ";
            for (int i = 0; i < 100; i++)
                cout << "-";
            cout << " \n";
        }
        else if (choose == "cls"){
            system("cls");
        }
        else if (choose == "dir"){
            //время - место - <аттрибут> - имя файла
            tm* timeinfo = localtime(&(mainFolder[0]->timeinfo));
            cout << endl << timeinfo->tm_mday << "/" << timeinfo->tm_mon + 1 << "/" << timeinfo->tm_year + 1900 << "\t";
            printf("%02d:%02d:%02d\t", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
            int capacitySize = 0;
            for (int i = 0; i < tableFiles.size(); i++)
                capacitySize += tableFiles[i]->getSize();
            cout << capacitySize << " blocks\t< >\t." << endl;

            timeinfo = localtime(&(currentFolder[0]->timeinfo));
            cout << timeinfo->tm_mday << "/" << timeinfo->tm_mon + 1 << "/" << timeinfo->tm_year + 1900 << "\t";
            printf("%02d:%02d:%02d\t", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
            capacitySize = 0;
            for (int i = 1; i < currentFolder.size(); i++)
                capacitySize += currentFolder[i]->getSize();
            cout << capacitySize << " blocks\t< >\t.." << endl;
            //вывод всех файлов текущего каталога, кроме секретного
            for (int i = 1; i < currentFolder.size(); i++)
                currentFolder[i]->outFileForDir();

        }
        else if (choose == "help"){
            cout << "\nThat's command you can use in this programm:\n";
            cout << "\tcreate 'name file' \t\t- create a file with parameters\n";
            cout << "\tread 'name file' \t\t- read a file from the current directory\n";
            cout << "\tmodify 'name file' \t\t- open text editor for editting file\n";
            cout << "\tdelete 'name catalog or file' \t- delete file or catalog\n";
            cout << "\tcd 'catalog' \t\t\t- go up or down in the catalog, which you choose\n";
            cout << "\tinfo \t\t\t\t- show how much files created\n";
            cout << "\tcls \t\t\t\t- clear a screen\n";
            cout << "\tdir \t\t\t\t- view all files in current catalog\n";
            cout << "\thelp \t\t\t\t- show this page\n";
            cout << "\texit \t\t\t\t- exit from programm\n";
        }
        else if (choose == "exit"){
            //cout << "Exit from the programm, press any symbol...\n";
            //getch();
            //сохраняем все файлы, которые у нас остались при работе
            //при этом после запуска, у нас будут имется данные об этих файлах, поэтому их не надо будет заного создавать и терять данные
            //перезаписыванием исходных данных
            ofstream fout("files.txt");
            fout << tableFiles.size() << endl;
            for (int i = 0; i < tableFiles.size(); i++){
                //string name = 
                fout << tableFiles[i]->getName() << " " << tableFiles[i]->getSize() << " " << tableFiles[i]->getAttribute() << endl;
            }
            fout.close();
            return 0;
        }
        else{//аналог дефаулта
            cout << "Unknown command. For the view full list of command enter 'help'\n";
        }
        cout << endl;
    }

}