
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "secondwindow.h"
#include<QIcon>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void SetChecker(bool state)
    {
        checker = state;
    }

    static bool GetChecker()
    {
        return checker;
    }

private:
    Ui::MainWindow *ui;
    SecondWindow *window;
    static bool checker;
    void check_checkers_state();


private slots:
    void digits_numbers();
    void on_dot_clicked();
    void Backspace();
    void on_ac_clicked();
    void on_equal_clicked();
    void math_operations();
    void on_abc_clicked();
    void on_actionOpen_File_triggered();
};

#endif // MAINWINDOW_H
