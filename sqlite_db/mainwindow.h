#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QLabel>
#include<QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QToolBar>
#include <QAction>
#include <QDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QStatusBar>
#include <QSqlError>
#include<QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    QLabel *name, *number_of_pages, *country, *author;
    QComboBox *combo_box1, *combo_box2, *combo_box3, *combo_box4;

    QToolBar *tbar;
    QStatusBar *sbar;
    QAction *add, *del, *fnd;

private:
    void make_actions();
    void make_menu();
    void read_bd();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
        void add_item();
        void delete_item();
        void find_item();
};
#endif // MAINWINDOW_H
