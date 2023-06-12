#include <iostream>
#include <QMap>
#include "circuitConnection.h"
#include "ioFunctions.h"

/*!
*\file
*\brief Главная функция программы
*\mainpage Документация для программы "CircuitMaster"
Программа предназначена для расчета сил токов в ветвях цепи переменного тока. \n
Для функционирования программы необходима операционная система Windows 7 или выше.
Программа разработана на языке С++ с использованием библиотеки Qt.
Стандарт языка ISO C++17 (/std:c++17). \n \n
<b>Указания к формату входных данных</b> \n
Допустимые тэги типов соединений: \c <seq>, \c <par> \n
Атрибут названия соединения: \c name \n
Допустимые атрибуты корневого соединения: \c voltage, \c frequency \n
Тэг элемента: \c <elem> \n
Тэг типа элемента: \c <type> \n
Допустимые типы элементов: \c R, \c L, \c C \n
Тэг сопротивления элемента: \c <res> \n
Тэг индуктивности элемента: \c <ind> \n
Тэг емкости элемента: \c <cap> \n \n
<b>Пример команды запуска программы</b> \n
Программа принимает два аргумента: путь к файлу с входными данными формата xml и путь к файлу для записи выходных данных. \n
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
    // Проверяем кол-во аргументов, завершаем программу, если их недостаточно
    if (argc != 3)
    {
        printf("Неверное количество аргументов.\n");
        return 1;
    }

    // Пути для входного и выходного файла получаем как аргументы командной строки
    QString inputPath(argv[1]);
    QString outputPath(argv[2]);

    // Обработка ошибок
    try {

        // Создаём дерево соединений схемы
        QMap<int, CircuitConnection> circuitMap;
        readInputFromFile(inputPath, circuitMap);

        // Вычисляем сопротивления для всех соединений рекурсивно
        circuitMap[*circuitMap.keyBegin()].calculateResistance();

        // Вычисляем силу тока и напряжение для всех соединений рекурсивно
        circuitMap[*circuitMap.keyBegin()].calculateCurrentAndVoltage();

        // Записываем результат в файл
        writeOutputToFile(outputPath, circuitMap);

    } catch (QString str) {
        // В случае ошибки, вывести её в консоль и завершить выполнение программы
        puts(str.toStdString().c_str());
        return 1;
    }

    return 0;
}
