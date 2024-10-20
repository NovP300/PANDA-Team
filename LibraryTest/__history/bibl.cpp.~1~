#include <tchar.h>
#include <iostream>
#include "DataAccessLibConnect.h"
#include <string.h>
#include <fstream>
#include <codecvt>
#include <chrono>

using namespace std;
using namespace std::chrono;

int NumberOfDir = 0;
int NumberOfFile = 0;

void DirTest();
int Dir(FileHandle fileObject, FileSystemHandle fileSystem);

void ErrorCheck();
void StorageTest();
void PrintStorageType(StorageType type);
void PrintFileSystemType(FileSystemTypeEnum type);
bool WriteDataFromBufferToFile(const BYTE* dataBuffer, LONGLONG bufferSize, const wstring& fileName);

int _tmain(int argc, _TCHAR* argv[]) {

	setlocale(LC_ALL, "russian");



    if(DataAccessLibInit()) {
		wcout << L"Библиотека открыта успешно" << endl;
	}
	else{
		wcout << L"Произошла ошибка при открытии библиотеки" << endl;
	}



    StorageTest();   //Тест хранилища


    DirTest();      //Тест последовательного обхода директорий

    DataAccessLibCleanup();

    system("pause");

    return 0;
}


//Тест хранлища
void StorageTest()
{
	wstring StoragePath(L"D:\\Загрузки\\ext234_ntfs\\my_ntfs_image.vhd");

	StorageHandle storage = CreateStorageHandle(StoragePath.c_str());
    if(storage != STORAGE_ERROR){
		wcout << L"Дескриптор хранилища успешно создан" << endl;

		StorageType storageType;
		storageType = GetStorageType(storage);
		wcout << L"Тип хранилища: ";
		PrintStorageType(storageType);
		cout << endl;

        // Вывод информации о физическом носиетеле
		if(storageType == StorageType::PhysicalDrive){
			PhysicalDriveInfoStruct info = GetPhysicalDriveInfo(storage);
            if(!info.Error){
				wcout << L"Устройство: " << info.Device << endl;
				wcout << L"Id Продукта: " << info.ProductId << endl;
				wcout << L"Серийный номер: " << info.Serial << endl;
				wcout << L"Размер: " << info.Size << endl;
            }
            else{
				cout << "Произошла ошибка при попытке получить информацию о физическом диске" << endl;
				ErrorCheck();
            }
        }
        cout << endl;


		int NumberOfPartitions = GetNumberOfPartitions(storage);
		wcout << L"Число разделов хранилища: " <<  NumberOfPartitions << endl;
		cout << endl;
        DWORD storageBlockSize = GetStorageBlockSize(storage);
        wcout << L"Определенный библиотекой размер сектора: " << storageBlockSize << endl;

        //Вывод информации о всех раздех хранилища с помощью итератора в цикле

        PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;
        for(unsigned int i = 1; i <= NumberOfPartitions; i++){
			GetPartitionInfoByNumber(storage, i, PartitionInfo);
			wcout << L"Номер раздела: " << i << endl;
			wcout << L"Тип раздела: " ;
            PrintFileSystemType(PartitionInfo->RecognizedType);
            cout << " || " ;
            FileSystemTypeEnum type = RecognizeFileSystem(storage, PartitionInfo->FirstSector * DefaultSectorSize);
            PrintFileSystemType(type);
            wcout << L"(определен с помощью сигнатур файловых систем)" << endl;
			wcout << L"Идентификатор типа раздела: " <<  PartitionInfo->RawType << endl;
			wcout << L"Первый сектор: " <<  PartitionInfo->FirstSector << endl;
			wcout << L"Последний сектор: " << PartitionInfo->LastSector << endl;
			wcout << L"Размер в секторах: " << PartitionInfo->SizeInSectors << endl;
			wcout << L"GUID типа раздела: " <<  PartitionInfo->TypeGuid << endl;
			wcout << L"GUID раздела: " <<  PartitionInfo->PartGuid << endl;
			wcout << L"Флаги: " <<  PartitionInfo->Flags << endl;
			wcout << L"Имя: " <<  PartitionInfo->Name << endl;
            cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        }
        delete PartitionInfo;
        cout << endl;

        //Определение размера сектора и задание нового размера сектора
        if (storageBlockSize != 0){
			wcout << L"Введите новый размер сектора: " << endl;
			DWORD newStorageBlockSize;
            cin >>  newStorageBlockSize;
            if(storageType == StorageType::PhysicalDrive || storageType == StorageType::LogicalDrive || storageType == StorageType::DataStream){
				if (newStorageBlockSize % storageBlockSize == 0) {
					if(SetStorageBlockSize(storage,newStorageBlockSize)){
						wcout << L"Размер сектора хранилища успешно изменен на : " <<  newStorageBlockSize << endl;
					}
                    else{
						wcout << L"Произошла ошибка при изменении размера сектора хранилища" << endl;
						ErrorCheck();
                    }
                }
				else{
					wcout << L"Новый размер сектора должен быть кратен старому!" << endl;
				}
            }
            else{
                if(SetStorageBlockSize(storage,newStorageBlockSize)){
                	wcout << L"Размер сектора успешно изменен, новый размер: " <<  newStorageBlockSize << endl;
                }
                else{
                	wcout << L"Ошибка задания нового размера сектора" << endl;
                    ErrorCheck();
                }

            }


            //Обход всех секторов диска
            wcout << L"Происходит обход секторов хранилища..." << endl;
            StorageIteratorHandle it = GetStorageIterator(storage);

            if(it != STORAGE_ERROR){
                int k = 0;
                auto start = high_resolution_clock::now();
				for(StorageBlockFirst(it); !StorageBlockIsDone(it); StorageBlockNext(it)){
                    k++;
                }
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start).count();
                wcout << L"Количество пройденных секторов: " << k << endl;
                wcout << L"Время работы цикла: " << duration << L" миллисекунд" << endl;
                CloseStorageIterator(it);

            }
            else{
                wcout << L"Ошибка создания итератора хранилища" << endl;
                ErrorCheck();
            }

			//Считывание раздела хранилища в файл по его номеру
			StorageBlockFirst(it);
            wcout << L"Введите номер раздела, который необходимо считать в буфер или нажмите ENTER, если хотите пропустить этот шаг" << endl;


            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string inputLine;
    		getline(cin, inputLine);

            if (inputLine.empty()) {
        	wcout << L"Операция считывания раздела была пропущена." << endl;

    		}
            else{
                DWORD partitionNumber = stoi(inputLine);

    //          DWORD partitionNumber;
    //			cin >>  partitionNumber;

                PartitionInfoStruct* PartitionInfo = new PartitionInfoStruct;

                GetPartitionInfoByNumber(storage, partitionNumber, PartitionInfo);


                LONGLONG startBlockPosition = PartitionInfo->FirstSector;
                LONGLONG endBlockPosition = PartitionInfo->LastSector;

                wcout << L"Стартовая позиция: " << startBlockPosition << endl;
                wcout << L"Конечная позиция: " << endBlockPosition << endl;



                if(SetStoragePosition(storage, startBlockPosition, OriginEnum::Begin)){
                    LONGLONG bytesToRead = (endBlockPosition -  startBlockPosition + 1) * storageBlockSize;
                    wcout << L"Количество байтов для чтения: " <<  bytesToRead << endl;
                    BYTE* dataBuffer = new BYTE[bytesToRead];
                    LONGLONG BytesRead = ReadStorageData(storage, bytesToRead, dataBuffer);
                    if(BytesRead == bytesToRead){
                        wcout << L"Раздел успешно считан в буфер" << endl;

                        if(WriteDataFromBufferToFile(dataBuffer, bytesToRead, L"partition.bin")){
                            wcout << L"Буффер записан в файл" << endl;
                        }
                    }
                    else{
                        wcout << L"Произошла ошибка при считывании раздела в буфер" << endl;
                        ErrorCheck();
                    }

                    delete [] dataBuffer;

                }
                else{
                    wcout << L"Ошибка позиционирования " << endl;
                    ErrorCheck();
                }
                delete PartitionInfo;
            }
        }
        else{
            wcout << L"Ошибка получения размера сектора хранилища" << endl;
            ErrorCheck();
        }

        CloseStorageHandle(storage);
    }

    else{
		wcout << L"Произошла ошибка при создании дескриптора хранилища" << endl;
		ErrorCheck();
    }

}

bool isFirstRun = true;

// Функция для записи ошибок в лог-файл
void ErrorCheck()
{
    if (HasError())
    {
        WCHAR ErrorString[1024];
        GetLastErrorStringW(ErrorString);

        ofstream logFile;
        if (isFirstRun)
        {
            logFile.open("logfile.txt", ios::out | ios::trunc | ios::binary); // Очистка файла при первом запуске
            isFirstRun = false;
        }
        else
        {
            logFile.open("logfile.txt", ios::out | ios::app | ios::binary);
        }

        if (logFile.is_open())
        {
            // Конвертация wchar_t* в std::string с кодировкой UTF-8
            wstring_convert<codecvt_utf8<wchar_t>> converter;
            string utf8ErrorString = converter.to_bytes(ErrorString);

            // Запись ошибки в файл
            logFile << utf8ErrorString << endl;
            logFile.close();
        }
        else
        {
            wcerr << L"Ошибка открытия файла лога" << endl;
        }
    }
}


//Напечатать тип хранлища
void PrintStorageType(StorageType type) {
    switch (type) {
        case StorageType::LogicalDrive: wcout<< L"LogicalDrive" << endl;   break;
        case StorageType::PhysicalDrive: wcout<< L"PhysicalDrive" << endl; break;
        case StorageType::ImageFile: wcout << L"ImageFile" << endl;   break;
        case StorageType::DataStream: wcout << L"DataStream" << endl;    break;
        case StorageType::DirectoryFile: wcout << L"DirectoryFile" << endl;  break;
        case StorageType::RAIDx: wcout << L"RAIDx" << endl;    break;
        case StorageType::VMDK: wcout << L"VMDK" << endl;    break;
        case StorageType::VDI: wcout << L"VDI" << endl;    break;
        case StorageType::VHD: wcout << L"VHD" << endl;    break;
        case StorageType::VHDX: wcout << L"VHDX" << endl;  break;
        case StorageType::QEMU: wcout << L"QEMU" << endl;  break;
        case StorageType::HDD: wcout << L"HDD" << endl;  break;
        case StorageType::ErrorType: wcout << L"ErrorType" << endl;  break;
        case StorageType::UnknownType: wcout << L"UnknownType" << endl;   break;
    }
}

//Напечатать тип файловой системы
void PrintFileSystemType(FileSystemTypeEnum type) {
    switch (type) {
        case FileSystemTypeEnum::NTFS:        wcout << L"NTFS" ; break;
        case FileSystemTypeEnum::FAT12:       wcout << L"FAT12" ; break;
        case FileSystemTypeEnum::FAT16:       wcout << L"FAT16" ; break;
        case FileSystemTypeEnum::FAT32:       wcout << L"FAT32" ; break;
        case FileSystemTypeEnum::exFAT:       wcout << L"exFAT" ; break;
        case FileSystemTypeEnum::Ext2:        wcout << L"Ext2" ; break;
        case FileSystemTypeEnum::Ext3:        wcout << L"Ext3" ; break;
        case FileSystemTypeEnum::Ext4:        wcout << L"Ext4" ; break;
        case FileSystemTypeEnum::DFS:         wcout << L"DFS" ; break;
        case FileSystemTypeEnum::MBR:         wcout << L"MBR" ; break;
        case FileSystemTypeEnum::PMBR:        wcout << L"PMBR (защитный MBR для GPT)" ; break;
        case FileSystemTypeEnum::EMBR:        wcout << L"EMBR (расширенный MBR)"  ; break;
        case FileSystemTypeEnum::GPT:         wcout << L"GPT" ; break;
        case FileSystemTypeEnum::GPT_4K:      wcout << L"GPT_4K" ; break;
        case FileSystemTypeEnum::PTFS:        wcout << L"PTFS (виртуальные файловые системы)" ; break;
        case FileSystemTypeEnum::FS_Custom:   wcout << L"Custom FS" ; break;
        case FileSystemTypeEnum::FS_None:     wcout << L"No FS"; break;
        case FileSystemTypeEnum::Encrypted:   wcout << L"Encrypted" ; break;
        case FileSystemTypeEnum::FS_Error:    wcout << L"Error"  ; break;
        case FileSystemTypeEnum::FS_Debug:    wcout << L"Debug"  ; break;
        case FileSystemTypeEnum::VMDK:        wcout << L"VMDK"  ; break;
        case FileSystemTypeEnum::VDI:         wcout << L"VDI"  ; break;
        case FileSystemTypeEnum::VHD:         wcout << L"VHD" ; break;
        case FileSystemTypeEnum::VHDX:        wcout << L"VHDX"  ; break;
        case FileSystemTypeEnum::QEMU:        wcout << L"QEMU" ; break;
		case FileSystemTypeEnum::HDD:         wcout << L"HDD (Parallels)" ; break;
        case FileSystemTypeEnum::TAR:         wcout << L"TAR" ; break;
        default:                              wcout << L"Unknown"  ; break;
    }
}

//Функция для записи данных в файл
bool WriteDataFromBufferToFile(const BYTE* dataBuffer, LONGLONG bufferSize, const wstring& fileName){

	ofstream outFile(fileName, ios::binary);
	if(outFile.is_open()){
		outFile.write(reinterpret_cast<const char*>(dataBuffer), bufferSize);

		if(!outFile.fail()){
			return true;

		}
		else{
			wcout << L"Произошла ошибка записи буфера в файл" << endl;
			return false;
		}
		outFile.close();
    }

	else{
        wcerr << L"Ошибка открытия файла для записи" << endl;
		return false;
	}
}




 int Dir(FileHandle fileObject, FileSystemHandle fileSystem)
{
    if (FileIsDir(fileObject))
    {
        DirectoryIteratorHandle dirHandle = CreateDirectoryIteratorHandle(fileObject);//Создаем итератор
        if (dirHandle != DIRECTORY_ITERATOR_ERROR)
        {
            for (DirFirst(dirHandle); !DirIsDone(dirHandle); DirNext(dirHandle))//Перебираем каталог
            {
                if (DirFileIsDir(dirHandle))//Если нашли каталог в каталоге
                {
                    NumberOfDir++;
                    int fullPathLength = GetFullPathW(dirHandle, NULL);
                    WCHAR* fullPath = new WCHAR[fullPathLength + 1];
                    GetFullPathW(dirHandle, fullPath);//Запоминаем путь к каталогу через массив
                    FileHandle currentFileObject = CreateFileHandle(fileSystem, fullPath);//Открываем каталог

                    int fileNameLength = GetFileNameW(dirHandle, NULL);
                    WCHAR* fileName = new WCHAR[fileNameLength + 1];
                    GetFileNameW(dirHandle, fileName);//Запоминаем имя каталога через массив
                    wcout << fileName << L" : " << endl;

                    delete[] fileName;
                    delete[] fullPath;
                    Dir(currentFileObject, fileSystem);//Опять запускаем функцию с новым открытым каталогом
                }
                else
                {
                    NumberOfFile++;
                    int fileNameLength = GetFileNameW(dirHandle, NULL);
                    WCHAR* fileName = new WCHAR[fileNameLength + 1];
                    GetFileNameW(dirHandle, fileName);//Запоминаем имя файла через массив
                    wcout << fileName << L" : " << endl;
                }
            }
        }
        else
        {
            wcout << L"Ошибка создания итератора" << endl;
        }
        CloseDirectoryIterator(dirHandle);
    }
    return 0;
}
void DirTest()
{
    wstring imageStoragePath(L"D:\\Загрузки\\ext234_ntfs\\my_ntfs_image.vhd");

    StorageHandle dataStorage = CreateStorageHandle(imageStoragePath.c_str());

    if (dataStorage == STORAGE_ERROR) { wcout << L"Ошибка открытия файла-образа!" << endl; return; }

    unsigned int selectedNumber = 2;
    PartitionInfoStruct selectedPartitionInfo;
    if (!GetPartitionInfoByNumber(dataStorage, selectedNumber, &selectedPartitionInfo)) {
        wcout << L"Ошибка получения информации о разделе!" << endl;	return;
    }

    // Установка размера блока чтения равным 1 сектор физического диска (DefaultSectorSize == 512)
    if (!SetStorageBlockSize(dataStorage, DefaultSectorSize)) { wcout << L"Ошибка задания размера блока!" << endl; }

    ULONGLONG startOffset = selectedPartitionInfo.FirstSector * DefaultSectorSize;
    ULONGLONG partitionSize = selectedPartitionInfo.SizeInSectors * DefaultSectorSize;

    // Распознавание типа файловой системы (альтернатива selectedPartitionInfo.RecognizedType)
    FileSystemTypeEnum fsType = RecognizeFileSystem(dataStorage, startOffset);

    if (fsType == FileSystemTypeEnum::FS_Error || fsType == FileSystemTypeEnum::FS_None) {
        wcout << L"Файловая система не распознана!" << endl;
        CloseStorageHandle(dataStorage);
        return;
    }

    // Открытие файловой системы
    FileSystemHandle fileSystem = CreateFileSystemHandle(fsType, dataStorage,
    startOffset, partitionSize, DefaultSectorSize);

    if (fileSystem == FILESYSTEM_ERROR) {
        wcout << L"Ошибка открытия файловой системы!" << endl;
        CloseStorageHandle(dataStorage); 	return;
    }
    else
    {
        wcout << L"Файловая система открыта!" << endl;
    }

    //Открытие файла (создание дискриптора)
    FileHandle fileObject = CreateFileHandle(fileSystem, L"\\.\\First");

    if (fileObject != FILE_OBJECT_ERROR)
    {
        wcout << L"Файловый объект открыт" << endl;
    }
    else
    {
        wcout << L"Файловый объект НЕ открыт" << endl;
    }

    //Проверка, является ли файл директорией
    if (FileIsDir(fileObject))
    {
        Dir(fileObject, fileSystem);
        wcout << L"Колличество каталогов: " << NumberOfDir << endl;
        wcout << L"Количество файлов: " << NumberOfFile << endl;

    }
    else
    {
        wcout << L"Это не директория" << endl;
    }
    //Получение количества объектов в файловой системе (Работает только с директориями)
    wcout << L"Количество объектов файловой системы: " << GetNumberOfFiles(fileObject) << endl;

    //Для файла
    //Получение количества потоков данных файлового объекта
    //wcout << L"Количество потоков данных файлового объекта: " << GetNumberOfStreams(fileObject) << endl;

    // Определение размера основного (нулевого) потока данных файла
    //wcout << L"Размер данных внутри файла: " << GetFileDataSize(fileObject, 0) << L" байт" << endl;

    //Закрытие файла (дискриптора)
    CloseFileHandle(fileObject);
}
