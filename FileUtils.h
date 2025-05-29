#pragma once

#include <QStringList>
#include <QMap>

class FileUtils
{
public:
    static QStringList readKeywordsFromFile(const QString& filePath);
    static QMap<QString, QList<QPair<int, int>>> countWordOccurrences(const QString& filePath, const QStringList& keywords);
    static bool saveOccurrencesToFile(const QString& filePath, const QMap<QString, QList<QPair<int, int>>>& wordOccurrences);
};
