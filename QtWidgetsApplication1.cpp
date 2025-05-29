#include "QtWidgetsApplication1.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QStringList>
#include <QRegularExpression>
#include "FileUtils.h"

QtWidgetsApplication1::QtWidgetsApplication1(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.pushButton->disconnect();
    ui.pushButton_2->disconnect();

    connect(ui.pushButton, &QPushButton::clicked, this, &QtWidgetsApplication1::on_pushButton_clicked);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &QtWidgetsApplication1::on_pushButton_2_clicked);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::on_pushButton_clicked()
{
    QString textFilePath = QFileDialog::getOpenFileName(this, tr("Select file"), "", tr("Text files (*.txt)"));
    if (textFilePath.isEmpty()) {
        return;
    }

    QFile file(textFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        QString content = stream.readAll();
        file.close();

        ui.textEdit->setPlainText(content);
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open file"));
        return;
    }

    QString keywordsFilePath = QFileDialog::getOpenFileName(this, tr("Select keywords file"), "", tr("Text files (*.txt)"));
    if (keywordsFilePath.isEmpty()) {
        return;
    }

    QStringList keywords = FileUtils::readKeywordsFromFile(keywordsFilePath);

    QString keywordsText = keywords.join("\n");
    ui.textEdit_2->setPlainText(keywordsText);

    QMap<QString, QList<QPair<int, int>>> wordOccurrences = FileUtils::countWordOccurrences(textFilePath, keywords);

    QString resultText;
    for (const QString& word : wordOccurrences.keys()) {
        resultText += word + ":\n";
        for (const QPair<int, int>& occurrence : wordOccurrences.value(word)) {
            resultText += QString("(%1, %2), ").arg(occurrence.first).arg(occurrence.second);
        }
        resultText.chop(2);
        resultText += "\n\n";
    }

    ui.textEdit_3->setPlainText(resultText);

    QString newFilePath = QFileDialog::getSaveFileName(this, tr("Save as"), "", tr("Text files (*.txt)"));
    if (newFilePath.isEmpty()) {
        return;
    }

    if (FileUtils::saveOccurrencesToFile(newFilePath, wordOccurrences)) {
        QMessageBox::information(this, tr("Success"), tr("Table of occurrences has been created and saved successfully"));
    }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Unable to save file"));
    }
}

void QtWidgetsApplication1::on_pushButton_2_clicked()
{
    runTest();
}

void QtWidgetsApplication1::runTest() {
    QString textFilePath = "1.txt";
    QString keywordsFilePath = "keywords.txt";

    QStringList keywords = FileUtils::readKeywordsFromFile(keywordsFilePath);
    QMap<QString, QList<QPair<int, int>>> wordOccurrences = FileUtils::countWordOccurrences(textFilePath, keywords);

    QMap<QString, QList<QPair<int, int>>> expectedOccurrences;
    expectedOccurrences["cd"] = { QPair<int, int>(1, 3), QPair<int, int>(2, 3) };
    expectedOccurrences["kl"] = { QPair<int, int>(3, 3) };
    expectedOccurrences["nm"] = { QPair<int, int>(1, 2), QPair<int, int>(3, 1), QPair<int, int>(3, 2) };
    expectedOccurrences["ry"] = { QPair<int, int>(2, 1) };

    if (wordOccurrences == expectedOccurrences) {
        QMessageBox::information(this, tr("Success"), tr("Test passed successfully"));
    }
    else {
        assert(false);
    }
}
