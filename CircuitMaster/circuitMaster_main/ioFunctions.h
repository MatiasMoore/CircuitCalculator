#ifndef IOFUNCTIONS_H
#define IOFUNCTIONS_H
#include <complex>
#include <QString>
#include <QStringList>
#include <QFile>
#include "circuitConnection.h"

/*!
*\file
*\brief Заголовки функций для работы с файлами
*/

/*!
* \brief Получить строчное отображение комплексного числа
* \param[in] num - комплексное число
* \return - строчное отображение комплексного числа
*/
QString complexToStr(std::complex<double> num);

/*!
* \brief Создать дерево соединений на основе xml файла
* \param[in] inputPath - путь к файлу
* \param[in,out] circuitMap - контейнер для записи дерева соединений
*/
void readInputFromFile(QString inputPath, QMap<int, CircuitConnection>& circuitMap);

/*!
* \brief Записать силы тока для соединений с известным именем в файл
* \param[in] outputPath - путь к файлу
* \param[in] circuitMap - контейнер с деревом соединений
*/
void writeOutputToFile(QString outputPath, QMap<int, CircuitConnection>& circuitMap);

#endif // IOFUNCTIONS_H
