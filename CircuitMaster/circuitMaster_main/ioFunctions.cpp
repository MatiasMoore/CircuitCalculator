#include "ioFunctions.h"

/*!
*\file
*\brief Реализация функций для работы с файлами
*/

QString complexToStr(std::complex<double> num)
{
    // Основные переменные
    QString str;
    double real = num.real();
    double imag = num.imag();

    // Если нет мнимой части
    if (imag == 0)
        str = QString::number(real);
    // Если есть мнимая часть
    else
    {
        // Знак между дейтвительной и мнимой частью
        QString sign = imag > 0 ? "+" : "-";

        str = QString("%1 %2 %3i").arg(QString::number(real), sign, QString::number(abs(imag)));
    }
    return str;
}

void readInputFromFile(QString const & inputPath, QMap<int, CircuitConnection>& circuitMap)
{
    // Создаем QFile на основе пути
    QFile xmlFile(inputPath);

    // Попытатья открыть файл
    // Ошибка, если не удалось открыть
    if (!xmlFile.exists() || !xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        throw QString("Неверно указан файл для входных данных. Возможно указанного расположения не существует или нет прав на запись.");
    }

    // Создаем QDomDocument
    QDomDocument domDocument;

    // Переменные для получения ошибки от QDomDoc
    QString errorMes;
    int errorLine;

    // Пытаемся создать QDomDoc
    // Ошибка, если не удалось создать QDomDoc
    if (!domDocument.setContent(&xmlFile, &errorMes, &errorLine))
    {
        throw QString("Получена ошибка QDomDoc при открытии xml файла: \"%1\" на строке %2.").arg(errorMes, QString::number(errorLine));
    }

    // Закрываем файл, по завершении работы
    xmlFile.close();

    // Получаем корневой элемент QDomDoc
    QDomElement rootElement = domDocument.documentElement();

    // Обработка ошибок корневого элемента
    QString rootTag = rootElement.tagName();
    if (rootTag != "seq" && rootTag != "par")
        throw QString("Корневым элементом должно быть последовательное \"<seq>\" или параллельное \"<par>\" соединение.");

    QString voltageStr = rootElement.attribute("voltage", "");
    if (voltageStr.length() == 0)
        throw QString("У корневого элемента должно быть указано напряжение.");

    QString frequencyStr = rootElement.attribute("frequency", "");
    // Значение -1 означает, что частота неизвестна
    double frequency = -1;
    if (frequencyStr.length() > 0)
    {
        // Пытаемся конвертировать в double
        bool convertedOk;
        frequency = frequencyStr.toDouble(&convertedOk);

        // Ошибка, если не удалось конверертировать
        if (!convertedOk)
            throw QString("Неверный формат значения частоты у корневого элемента.");

        // Ошибка, если значение меньше нуля
        if (frequency <= 0)
            throw QString("Недопустимое значение частоты у корневого элемента. Значение частоты должно быть больше 0.");
    }

    // Элементы всех соединений цепи, кроме корневого
    QDomNodeList seqConnections = rootElement.elementsByTagName("seq");
    QDomNodeList parConnections = rootElement.elementsByTagName("par");
    auto conns = { seqConnections, parConnections};

    // Использованные имена с номером строки
    QMap<QString, int> usedNames;

    // Обработка ошибок связанных с указанием напряжения или частоты у других соединений
    for (auto connGroup = conns.begin(); connGroup != conns.end(); connGroup++)
    {
        // Для каждого соединения
        for (int i = 0; i < connGroup->count(); i++)
        {
            QDomElement connectionElement = connGroup->at(i).toElement();

            // Ошибка, если указано напряжение
            if (connectionElement.attribute("voltage", "").length() != 0)
                throw QString("Неверное указание напряжения цепи на строке %1. "
                              "Напряжение указывается только для корневого элемента схемы.").arg(QString::number(connectionElement.lineNumber()));

            // Ошибка, если указана частота
            if (connectionElement.attribute("frequency", "").length() != 0)
                throw QString("Неверное указание частоты переменного тока на строке %1. "
                              "Частота указывается только для корневого элемента схемы.").arg(QString::number(connectionElement.lineNumber()));

            // Проверка уникальности имен соединений
            QString connectionName = connectionElement.attribute("name", "");
            if (connectionName != "")
            {
                if (usedNames.keys().contains(connectionName))
                    throw QString("Повтор имени соединения на строке %1 и строке %2. Имя соединения должно быть "
                                  "уникальным.").arg(QString::number(usedNames[connectionName]), QString::number(connectionElement.lineNumber()));
                usedNames[connectionName] = connectionElement.lineNumber();
            }
        }
    }

    // Создаем дерево соединений в QMap
    CircuitConnection::connectionFromDocElement(circuitMap, rootElement, frequency);
}

void writeOutputToFile(QString const & outputPath, QMap<int, CircuitConnection>& circuitMap)
{
    // Создаем QFile на основе пути
    QFile outFile(outputPath);

    // Попытатья открыть файл
    // Ошибка, если не удалось открыть
    if (!outFile.open(QFile::WriteOnly | QFile::Text)) {
        throw QString("Неверно указан файл для выходных данных. Возможно указанного расположения не существует или нет прав на запись.");
    }

    // Список строк для вывода
    QStringList outputLines;

    // Для каждого соединения
    for (auto connectionIter = circuitMap.begin(); connectionIter != circuitMap.end(); connectionIter++)
    {
        // Если имя указано пользователем
        if ((*connectionIter).hasCustomName)
        {
            // Формируем строку вывода
            QString outLine = QString("%1 = %2\n").arg((*connectionIter).name, complexToStr((*connectionIter).current));

            // Добавляем в список
            outputLines.append(outLine);
        }
    }

    // Сортируем в алфавитном порядке
    outputLines.sort();

    // Записываем в файл
    for(auto lineIter = outputLines.cbegin(); lineIter != outputLines.cend(); lineIter++)
        outFile.write(lineIter->toStdString().c_str());

    // Закрываем файл
    outFile.close();
}
