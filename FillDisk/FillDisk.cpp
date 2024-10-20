#include <iostream>
#include <tchar.h>
#include "FillDisk.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
    DirectoryManager generator;
    string rootPath = "G:\\";
    RandomGenerator generatorRan;
    int depth = generatorRan.generateRandomNumber(5, 10);
    int branches = generatorRan.generateRandomNumber(1, 10);
    int createdCount = generator.createNestedDirectories(rootPath, depth, branches); /*При создании вложенных директорий у ФС стоит ограничение
                                                                            длины имени файла в 260 символов, то есть при попытке создать
                                                                            большую вложенность, где имя каталогов привышает допустимое,
                                                                            воникает ошибка, программой создаются все возможные каталоги*/
    wcout << L"Глубина вложенности: ";
    cout << depth << endl;
    wcout << L"Количество веток: ";
    cout << branches << endl;
    cout << "Total directories created: " << createdCount << endl;
//    string Path = "F:\\1230\\";
//    generator.removeDirectory(Path);
//    string Path1 = "F:\\1231\\";
//    generator.changeDirectory(Path1);
//    path currentPath = current_path();
//    string Path2 = currentPath.string();
//    cout << "Current directory: " << Path2 << std::endl;

          // экземпляр класса FileManager
    FileManager fileManager;

    // Имя файла
    string fileName = "G://example.txt";

    // 1. Создание файла
    if (fileManager.createFile(fileName))
    {
        wcout << L"Файл успешно создан: " ; cout << fileName << endl;
    }
    else
    {
        wcout << L"Ошибка при создании файла: " ; cout << fileName << endl;
    }

    // 2. Заполнение файла случайными данными
    srand(time(NULL));
    int numberOfLines = rand(); // Количество строк, которые нужно записать
    if (fileManager.fillFileWithRandomData(fileName, numberOfLines))
    {
        wcout << L"Файл успешно заполнен случайными данными. Количество строк: "; cout  << numberOfLines << endl;
    }
    else
    {
        wcout << L"Ошибка при заполнении файла случайными данными." << endl;
    }

    DiskSpaceChecker checker;
    uintmax_t requiredSpace = 1024 * 1024 * 1024; //1 Гбайт

    // Пример
    if (checker.checkDiskSpace("G://", requiredSpace))
    {
        wcout << L"Достаточно места для записи на диск" << endl;
    }
    else
    {
        wcout << L"Недостаточно места для записи на диске" << endl;
    }



    system("pause");

    return 0;
}
