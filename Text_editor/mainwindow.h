#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QAction>
#include <QMessageBox>
#include<QCoreApplication>
#include<QDockWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QGridLayout>
#include <QToolButton>
#include<QTextCursor>
#include <QStatusBar>
#include <QLabel>
#include<QLayout>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>


//c++ libs
#include<string>
#include <regex>
#include<filesystem>
//#include<iostream>
//#include <fstream>
//#include<stdexcept>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QToolBar* tools;
    QMenuBar* bar;
    QStatusBar* st_bar;
    QLabel *setTime;
    QTimer *timer_;

    //all for menu and actions
    QMenu*file, *edit, *help;
    QAction* f_open, *f_close, *f_save, *f_save_as, *f_new;
    QAction* clear_, *to_upcase, *to_lowcase, *font_up, *font_low, *replace_ ;
    QAction* about_app, *ref;
    QString current_file;

    //all for dock widgets
    QDockWidget* dock, *sec_dock;
    QWidget *dockW, *second_dockW;
    QToolButton *C,*UP,*LOW, *Font_Up, *Font_Down, *Rep;
    QToolButton *S,*S_AS,*Close, *N, *O;
    QGridLayout *l;
    QHBoxLayout* hl;

    //all for central widget
    QLabel *text_label;
    QTextEdit* text_editor;
    QWidget* centralW;
    QString current_p;

public slots:
    void open_();
    void close_();
    void save_as();
    void save_();
    void new_();

    void clear_text();
    void to_upper();
    void to_lower();
    void font_upcase();
    void font_down();
    void find_replace();

    void info();
    void reference();

    void update_time();
    void check_updates_file();

private:
    ////////////////////////////// all needed comments in ctor
    void make_file();
    void make_edit();
    void make_help();
    void make_tools();
    void make_dock();
    void make_second_dock();
    void make_central_widget();

};
#endif // MAINWINDOW_H
