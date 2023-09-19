#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <string>
#include "Kalkulator.h"
#include <QFileDialog>

bool MainWindow::checker = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setWindowTitle("Calculator");
    window = new SecondWindow(this);
    window->setFixedSize(240,179);
    window->Set_line(ui->lineEdit);
    connect(ui->pushButton0,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton1,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton2,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton3,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton4,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton5,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton6,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton7,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton8,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton9,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->backspace,SIGNAL(clicked()),this,SLOT(Backspace()));
    connect(ui->division,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->multip,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->minus,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->plus,SIGNAL(clicked()),this,SLOT(math_operations()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::check_checkers_state()
{
    if(checker)
    {
        ui->lineEdit->clear();
        checker = false;
    }
}

void MainWindow::digits_numbers()
{
    check_checkers_state();
    QPushButton *button = (QPushButton *) sender();

    if(ui->lineEdit->text() == "0")
        ui->lineEdit->clear();

    QString string;
    string = ui->lineEdit->text() + button->text();
    ui->lineEdit->setText(string);
}

void MainWindow::on_dot_clicked()
{
     check_checkers_state();
     ui->lineEdit->setText(ui->lineEdit->text() + ".");
}


void MainWindow::Backspace()
{
     QString string = ui->lineEdit->text();
     string.chop(1);

    if(string.isEmpty())
        string = "0";

    ui->lineEdit->setText(string);
}

void MainWindow::math_operations()
{
    if(checker)
    {
        checker = false;
    }

    QPushButton *button = (QPushButton *)sender();
    if (button->text() == "÷")
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "/");
    }
    else if (button->text() == "×")
    {
        ui->lineEdit->setText(ui->lineEdit->text() + "*");
    }
    else
    {
        QString string = ui->lineEdit->text() + button->text();
        ui->lineEdit->setText(string);
    }

}

void MainWindow::on_ac_clicked()
{
    ui->lineEdit->setText("0");
}


void MainWindow::on_equal_clicked()
{
    Kalkulator<float> K;
    Reader file;
    checker = true;

    std::string expression = (ui->lineEdit->text()).toStdString();

    try
    {
        K.Parsing(expression);
        QString result = QString::fromStdString(expression);

        ui->lineEdit->setText(result);
    }
    catch (...)
    {
        ui->lineEdit->setText("divide by 0 ");
    }

}


void MainWindow::on_abc_clicked()
{
    QPushButton *button = (QPushButton *)sender();

    if(button->isChecked())
    {
        button->setText("123");
        window->show();
    }
    else
    {
        button->setText("abc");
        window->hide();
    }

}


void MainWindow::on_actionOpen_File_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a file");

    std::string file_path = filename.toStdString();
    Reader file;
    try
    {
        file.ReadfromFile(file_path);
    }
    catch (...)
    {

    }

}

