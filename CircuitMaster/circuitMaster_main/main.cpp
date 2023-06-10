#include <iostream>
#include <QMap>
#include "circuitConnection.h"
#include "ioFunctions.h"

/*!
*\file
*\brief Главная функция программы
*\mainpage Документация для программы "CircuitMaster"
Программа предназначена для расчета сил токов в ветвях цепи переменного тока.
Для функционирования программы необходима операционная система Windows 7 или выше.
Программа разработана на языке С++ с использованием библиотеки Qt.
Стандарт языка ISO C++17 (/std:c++17).
Программа принимает два аргумента: путь к файлу с входными данными формата xml и путь к файлу для записи выходных данных.

Пример команды запуска программы:
*\code
circuitMaster_main.exe C:\input.xml C:\output.txt
*\endcode
*\author Biryukov Nikita
*\date June 2023
*\version 1.0
*/

/*!
*\brief Главная функция программы
*\param[in] argv[1] - путь к файлу с входными данными
*\param[in] argv[2] - путь к файлу с выходными данными
*\return 0 - запуск программы прошел успешно
*/
int main(int argc, char *argv[])
{
    //Проверяем кол-во аргументов, завершаем программу, если их недостаточно
    if (argc != 3)
    {
        printf("Неверное количество аргументов.\n");
        return 0;
    }

    //Пути для входного и выходного файла получаем как аргументы командной строки
    QString inputPath(argv[1]);
    QString outputPath(argv[2]);

    //Обработка ошибок
    try {

        //Создаём дерево соединений схемы
        QMap<QString, CircuitConnection> circuitMap;
        readInputFromFile(inputPath, circuitMap);

        //Вычсиляем сопротивления для всех соединений рекурсивно
        circuitMap[*circuitMap.keyBegin()].calculateResistance();

        //Вычсиляем сопротивления силу тока и напряжение для всех соединений рекурсивно
        circuitMap[*circuitMap.keyBegin()].calculateCurrentAndVoltage();

        //Записываем результат в файл
        writeOutputToFile(outputPath, circuitMap);

    } catch (QString str) {
        //В случае ошибки вывести её в консоль и завершить выполнение программы
        puts(str.toStdString().c_str());
        return 0;
    }

    return 0;
}
