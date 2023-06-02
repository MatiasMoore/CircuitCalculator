#include <QCoreApplication>
#include <iostream>
#include <complex>
#include <QList>
#include <QMap>
#include <QtXml/QDomDocument>
#include <stdio.h>
#include <windows.h>
#include "circuitElement.h"
#include "circuitConnection.h"
#include "ioFunctions.h"

int main(int argc, char *argv[])
{
    //Проверяем кол-во аргументов, завершаем программу, если их недостаточно
    if (argc != 3)
    {
        printf("Неверное количество аргументов\n");
        return 0;
    }

    //Пути для входного и выходного файла получаем как аргументы командной строки
    QString inputPath(argv[1]);
    QString outputPath(argv[2]);

    //Обработка ошибок
    try {

        //Создаём дерево соединений схемы
        QMap<int, CircuitConnection> circuitMap;
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
