#ifndef FillDiskH
#define FillDiskH

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace std::chrono;

/// Класс для генерации случайных чисел и строк
class RandomGenerator
{
private:
    default_random_engine dre; ///< Генератор случайных чисел
    ofstream logFile; ///< Файл для логирования

    /// Получение текущей даты и времени в виде строки
    string getCurrentTime()
    {
        // Получаем текущее время
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm now_tm;
        localtime_s(&now_tm, &now_time);
        // Буфер для форматированной строки времени
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &now_tm);

        return string(timeStr); // Возвращаем строку с текущим временем
    }

    /// Метод для записи ошибки в лог
    void logError(const string& errorMessage)
    {
        if (logFile.is_open())
        {
            logFile << "[" << getCurrentTime() << "] Error: " << errorMessage << endl;
        }
    }

public:
    /// Конструктор класса
    RandomGenerator()
    {
        // Инициализация генератора случайных чисел
        auto seed = chrono::system_clock::now().time_since_epoch().count();
        dre = default_random_engine(seed);
        logFile.open("error_log.txt", ios::app); // Открываем файл для логирования ошибок
    }

    /// Деструктор класса
    ~RandomGenerator()
    {
        if (logFile.is_open())
        {
            logFile.close(); // Закрываем файл при завершении работы
        }
    }

    /// Генерация случайного числа в диапазоне [min, max]
    int generateRandomNumber(int min, int max)
    {
        if (min > max)
        {
            logError("generateRandomNumber: Минимум больше максимума.");
            return -1; // Ошибка
        }

        uniform_int_distribution<int> range(min, max);
        return range(dre);
    }

    /// Генерация случайной строки длиной length, состоящей из букв и цифр
    string generateRandomString(int length)
    {
        if (length <= 0)
        {
            logError("generateRandomString: Длина строки должна быть больше нуля.");
            return ""; // Ошибка
        }

        const string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        uniform_int_distribution<int> charDist(0, chars.size() - 1);
        string result;

        for (int i = 0; i < length; ++i)
        {
            result += chars[charDist(dre)];
        }

        return result;
    }

    /// Логирование произвольного сообщения
    void logMessage(const string& message)
    {
        if (logFile.is_open())
        {
            logFile << "[" << getCurrentTime() << "] Message: " << message << endl;
        }
    }
};


/** \brief Класс для работы с каталогами*/
class DirectoryManager {

private:

    string currentPath;///<Переменная для хранения текущего пути в файловой системе.
    int createdCount = 0;///<Переменная для подсчёта количества созданных директорий.
    RandomGenerator generator; ///<Экземпляр генератора случайных чисел и строк.

public:
    /** \brief Конструктор DirectoryManager*/
    DirectoryManager() = default;
    /** \brief Деструктор класса DirectoryManager*/
    ~DirectoryManager() {

    }

    /** \brief Метод для логирования ошибок
        \param [in] message Сообщение об ошибке

        Логирование ошибок при работе с директориями в текстовый файл */
    void logError(const string& message) {
        // Открываем файл лога в режиме добавления
        ofstream logFile("error_log.txt", ios::app);
        if (!logFile) {
            cerr << "Unable to open log file." << endl;
            return;
        }

        // Буфер для хранения результата
        char timeString[26]; // Размер буфера выбран исходя из документации для ctime_s

        // Получаем текущее время и дату для добавления в лог
        auto now = system_clock::now();
        time_t now_time = system_clock::to_time_t(now);
        ctime_s(timeString, sizeof(timeString), &now_time);
        logFile << timeString << message << endl;
        logFile.close();
    }
    /** \brief Метод для создания одной директории по заданному пути
        \param [in] dirPath Путь для создания директории
        \return Создана ли директория */
    bool createDirectory(const path& dirPath) {
        const size_t MAX_PATH_LENGTH = 260; // Максимальная длина пути в Windows
        try {
            if (!exists(dirPath)) {
                create_directories(dirPath);
                return true;
            }
            else {
                cout << "Directory already exists: " << dirPath << endl;
                return false; // Директория уже существует, не является ошибкой, но возвращаем false
            }
        }
        catch (const filesystem_error& e) {
             logError(e.what());
             return false; // Возвращаем false при ошибке создания директории
        }
    }

    /** \brief  Метод для создания вложенной файловой структуры
    \param [in] rootPath - Путь для создания корневых каталогов,
    \param [in] depth - Количество вложенных каталогов,
    \param [in] branches - Количество каталогов по основному пути
    \return Количество созданных каталогов */
    int createNestedDirectories(const path& rootPath, int depth, int branches) {
        if (depth <= 0) return 0;

        int createdCount = 0;

        for (int i = 0; i < branches; ++i) {
            int randomNum = generator.generateRandomNumber(90, 100);
            string dirName = generator.generateRandomString(randomNum);
            //string dirName = "Путь указанный вручную" + to_string(i);
            path subDir = rootPath / dirName;
            if (createDirectory(subDir)) {
                createdCount++; // Увеличиваем счетчик созданных каталогов
                // Складываем количество созданных поддиректорий на каждом уровне рекурсии
                createdCount += createNestedDirectories(subDir, depth - 1, branches);
            }
        }
        return createdCount;

    }

    /** \brief Метод для рекурсивного удаления директории
    \param [in] path Путь для удаления директории
    \return Удалён ли каталог*/
    bool removeDirectory(const string& path) {
        try {
            // Возвращает количество удаленных файлов и директорий
            auto removed_count = filesystem::remove_all(path);
            if (removed_count > 0) {
                cout << "Successfully removed " << removed_count << " files/directories at: " << path << endl;
                return true;
            }
            else { cout << "Does not exist " <<  "directory: " << path << endl; }
        }
        catch (const filesystem::filesystem_error& e) {
            cerr << "Error remove directory: " << e.what() << endl;
            logError(e.what());
            return false;
        }
        return false;
    }

    /** \brief Метод для изменения текущей рабочей директории процесса на указанный путь
    \param [in] path Путь изменения текущей директории
    \return Изменена ли текущая директория*/
    bool changeDirectory(const string& path) {
    try {
        if (filesystem::exists(path)) {
            currentPath = path;
            filesystem::current_path(path);
            cout << "Changed current directory to: " << path << endl;
            return true;
        }
        else { cout << "Directory " << path << " does not exist" << endl; }
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Error change directory: " << e.what() << endl;
        logError(e.what());
        return false;
    }
    return false;
}

};


/// Класс для создания файлов, удаления файлов, заполнения файлов
class FileManager
{
private:
    RandomGenerator randomGen; ///< Экземпляр класса для генерации случайных данных

    /// Логирование ошибки
    void logError(const string& errorMessage)
    {
        randomGen.logMessage(errorMessage); // Используем RandomGenerator для логирования
    }

public:
    /// Метод для создания файла
    bool createFile(const string& fileName)
    {
        ofstream file(fileName);
        if (!file)
        {
            logError("Не удалось создать файл: " + fileName);
            return false;
        }
        file.close();
        randomGen.logMessage("Файл создан: " + fileName);
        return true;
    }

    /// Метод для удаления файла
    bool deleteFile(const string& fileName)
    {
        if (filesystem::exists(fileName))
        {
            if (filesystem::remove(fileName))
            {
                randomGen.logMessage("Файл удален: " + fileName);
                return true;
            }
            else
            {
                logError("Не удалось удалить файл: " + fileName);
                return false;
            }
        }
        else
        {
            logError("Файл не существует: " + fileName);
            return false;
        }
    }

    /// Метод для заполнения файла случайными данными
    bool fillFileWithRandomData(const string& fileName, int numberOfLines)
    {
        ofstream file(fileName, ios::app); // Открываем файл в режиме добавления данных
        if (!file)
        {
            logError("Не удалось открыть файл для записи: " + fileName);
            return false;
        }

        for (int i = 0; i < numberOfLines; ++i)
        {
            int randomNumber = randomGen.generateRandomNumber(1, 10000);   // Генерация случайного числа
            string randomString = randomGen.generateRandomString(100);    // Генерация случайной строки длиной 100 символов

            file << randomNumber << " " << randomString << endl;
        }

        file.close();
        randomGen.logMessage("Файл заполнен случайными данными: " + fileName);
        return true;
    }

    /// Логирование произвольного сообщения
    void logMessage(const string& message)
    {
        randomGen.logMessage(message);
    }
};



/**
    @class DiskSpaceChecker
    @brief Класс для проверки свободного места на физическом диске
 */
class DiskSpaceChecker
{
public:
    /**
        @brief Конструктор по умолчанию.
     */
    DiskSpaceChecker() = default;

    /**
        @brief Логирует ошибки в файл.
        @param errorMessage Сообщение об ошибке для логирования.
     */
    void logError(const string& errorMessage)
    {
        ofstream logFile("error_log.log", ios::app);
        if (logFile.is_open())
        {
            logFile << "Ошибка: " << errorMessage << endl;
            logFile.close();
        }
        else
        {
            cerr << "Не удалось открыть файл для записи логов" << endl;
        }
    }

    /**
        @brief Проверяет, достаточно ли свободного места на диске.
        @param path Путь к директории, на которой нужно проверить свободное место.
        @param requiredSpace Необходимое количество свободного места в байтах.
        @return true, если на диске достаточно места, иначе false.
     */
    bool checkDiskSpace(const string& path, uintmax_t requiredSpace)
    {
        try
        {
            filesystem::space_info diskInfo = filesystem::space(path);

            wcout << L"Свободного места: " ; cout << diskInfo.available; wcout << L" байт" << endl;
            wcout << L"Необходимо места: "; cout << requiredSpace; wcout << L" байт" << endl;

            if (diskInfo.available >= requiredSpace)
            {
                return true;
            }
            else
            {
                logError("Недостаточно свободного места на диске");
                return false;
            }
        }
        catch (const filesystem::filesystem_error& e)
        {
            logError(string("Ошибка доступа к файловой системе: ") + e.what());
            return false;
        }
    }
};


#endif
