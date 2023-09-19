#include "secondwindow.h"
#include "ui_secondwindow.h"
#include "mainwindow.h"


SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    second_ui(new Ui::SecondWindow)
{
    second_ui->setupUi(this);
    this->setWindowTitle("Trygonometry Function");
    connect(second_ui->sinus,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->cosinus,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->tangen,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->cotan,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->modul,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->sqrt,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->open_bracket,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
    connect(second_ui->close_bracket,SIGNAL(clicked()),this,SLOT(trygonometry_functions()));
}

SecondWindow::~SecondWindow()
{
    delete second_ui;
}

void SecondWindow::trygonometry_functions()
{
    if(MainWindow::GetChecker())
    {
        line->clear();
        MainWindow::SetChecker(false);
    }

    QPushButton *button = (QPushButton *)sender();
    QString string;

    if(line->text() == "0")
        line->clear();

    if(button->text() == "(" or button->text() == ")" )
    {
        string = line->text() + button->text();
        line->setText(string);
    }
    else
    {
        string = line->text() + button->text() + "(";
        line->setText(string);
    }

}
