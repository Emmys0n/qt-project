#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget* parent = nullptr);
    ~QtWidgetsApplication1();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void runTest();

private:
    Ui::QtWidgetsApplication1 ui;
};
