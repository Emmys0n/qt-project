#include "FileUtils.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

QStringList FileUtils::readKeywordsFromFile(const QString& filePath) {
    QStringList keywords;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString keyword = stream.readLine().trimmed();
            keywords.append(keyword);
        }
        file.close();
    }
    return keywords;
}

QMap<QString, QList<QPair<int, int>>> FileUtils::countWordOccurrences(const QString& filePath, const QStringList& keywords) {
    QMap<QString, QList<QPair<int, int>>> wordOccurrences;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        int row = 1;
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList words = line.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
            int column = 1;
            for (const QString& word : words) {
                if (!keywords.contains(word)) {
                    if (!wordOccurrences.contains(word)) {
                        wordOccurrences[word] = QList<QPair<int, int>>();
                    }
                    wordOccurrences[word].append(QPair<int, int>(row, column));
                }
                column++;
            }
            row++;
        }
        file.close();
    }
    return wordOccurrences;
}

bool FileUtils::saveOccurrencesToFile(const QString& filePath, const QMap<QString, QList<QPair<int, int>>>& wordOccurrences) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString& word : wordOccurrences.keys()) {
            out << word << " ";
            for (const QPair<int, int>& occurrence : wordOccurrences.value(word)) {
                out << "(" << occurrence.first << ", " << occurrence.second << "), ";
            }
            out << "\n";
        }
        file.close();
        return true;
    }
    return false;
}
