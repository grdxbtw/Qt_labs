#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //// set db
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\Zhema\\Documents\\Qt\\sqlite_test\\books.sqlite");
    sbar = this->statusBar();
    this->setStatusBar(sbar);

    tbar = new QToolBar(this);
    this->addToolBar(tbar);
    this->resize(600,150);

    ////////////
    make_actions();
    make_menu();

    ///// connecting to db
    if(db.open())
    {
        QString message = "is open!";
        read_bd();                          //reading previos data
        sbar->showMessage(message);
        sbar->setStyleSheet("color: green");
    }
    else
    {
        QString message = "not open!";
        sbar->showMessage(message);
        sbar->setStyleSheet("color: red");
    }
}

void MainWindow::make_actions()
{
    add = new QAction("Add");
    add->setToolTip("Add new row to db");
    del = new QAction("Delete");
    del->setToolTip("Delete new row from db");
    fnd = new QAction("Find");
    fnd->setToolTip("Find  row ");

    tbar->addAction(add);
    tbar->addSeparator();
    tbar->addAction(del);
    tbar->addSeparator();
    tbar->addAction(fnd);

    connect(add,SIGNAL(triggered(bool)),this,SLOT(add_item()));
    connect(del,SIGNAL(triggered(bool)),this,SLOT(delete_item()));
    connect(fnd,SIGNAL(triggered(bool)),this,SLOT(find_item()));

}

void MainWindow::make_menu()
{
    QWidget *w = new QWidget(this);
    name = new QLabel("Book name");
    number_of_pages = new QLabel("Number of pages");
    country = new QLabel("Country");
    author = new QLabel("Author");

    combo_box1 = new QComboBox;
    combo_box2 = new QComboBox;
    combo_box3 = new QComboBox;
    combo_box4 = new QComboBox;

    QGridLayout *grl = new QGridLayout;
    grl->addWidget(name,0,0);
    grl->addWidget(combo_box1,1,0);
    grl->addWidget(number_of_pages,0,1);
    grl->addWidget(combo_box2,1,1);
    grl->addWidget(country,0,2);
    grl->addWidget(combo_box3,1,2);
    grl->addWidget(author,0,3);
    grl->addWidget(combo_box4,1,3);
    w->setLayout(grl);
    this->setCentralWidget(w);
}

void MainWindow::read_bd()
{
    QSqlQuery query;
    if(query.exec("SELECT * FROM books "))
    {
       while(query.next())
        {
            combo_box1->addItem(query.value(1).toString());
            combo_box2->addItem(query.value(2).toString());
            combo_box3->addItem( query.value(3).toString());
            combo_box4->addItem( query.value(4).toString());

        }
    }
    else
        qDebug() << "error in reading db" << query.lastError().text();
}

void MainWindow::add_item()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add");

    QLineEdit* f_line_edit = new QLineEdit(&dialog);
    QLineEdit* r_line_edit = new QLineEdit(&dialog);
    QLineEdit* t_line_edit = new QLineEdit(&dialog);
    QLineEdit* l_line_edit = new QLineEdit(&dialog);
    QPushButton* add = new QPushButton("add to bd",&dialog);


    QVBoxLayout layout(&dialog);
    layout.addWidget(new QLabel("title: ", &dialog));
    layout.addWidget(f_line_edit);
    layout.addWidget(new QLabel("pages: ", &dialog));
    layout.addWidget(r_line_edit);
    layout.addWidget(new QLabel("country: ", &dialog));
    layout.addWidget(t_line_edit);
    layout.addWidget(new QLabel("author: ", &dialog));
    layout.addWidget(l_line_edit);
    layout.addWidget(add);

    connect(add, &QPushButton::clicked,[&]()
            {
                QString title = f_line_edit->text();
                QString country = t_line_edit->text();
                QString pages = r_line_edit->text();
                QString author = l_line_edit->text();


                if(!title.isEmpty() && !country.isEmpty() && !pages.isEmpty() && !author.isEmpty())
                {
                    QSqlQuery insertQ;
                    insertQ.prepare("INSERT INTO books (name, pages, country, author) VALUES (:name, :pages, :country, :author)");
                    insertQ.bindValue(":name", title);
                    insertQ.bindValue(":pages", pages.toInt());
                    insertQ.bindValue(":country", country);
                    insertQ.bindValue(":author", author);
                    if(insertQ.exec())
                        sbar->showMessage("added row");
                    else
                    {
                        QString err = "error exec fail: " +  insertQ.lastError().text();
                        sbar->showMessage(err);
                        sbar->setStyleSheet("color: red");
                    } ;

                    combo_box1->addItem(title);
                    combo_box2->addItem(pages);
                    combo_box3->addItem(country);
                    combo_box4->addItem(author);
                }
                else
                    QMessageBox::warning(this,tr("Warning"),tr("is Empthy"));
            });

    dialog.exec();
}

void MainWindow::delete_item()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Delete");

    QLineEdit* f_line_edit = new QLineEdit(&dialog);
    QPushButton* add = new QPushButton("delete from bd",&dialog);


    QVBoxLayout layout(&dialog);
    layout.addWidget(new QLabel("id:", &dialog));
    layout.addWidget(f_line_edit);
    layout.addWidget(add);

    connect(add, &QPushButton::clicked,[&]()
            {
                QString id = f_line_edit->text();
                if(!id.isEmpty())
                {
                    QSqlQuery deleteQ;
                    deleteQ.prepare("DELETE FROM books WHERE id = :id");
                    deleteQ.bindValue(":id", id.toInt());
                    if(deleteQ.exec())
                        sbar->showMessage("deleted row");
                    else
                    {
                        QString err = "error exec fail: " +  deleteQ.lastError().text();
                        sbar->showMessage(err);
                        sbar->setStyleSheet("color: red");
                    }

                    combo_box1->removeItem(id.toInt() - 1);
                    combo_box2->removeItem(id.toInt() - 1);
                    combo_box3->removeItem(id.toInt() - 1);
                    combo_box4->removeItem(id.toInt() - 1);
                }
                else
                    QMessageBox::warning(this,tr("Warning"),tr("is Empthy"));
            });

    dialog.exec();

}

void MainWindow::find_item()
{
    QDialog dialog(this);
    dialog.setWindowTitle("find");

    QLineEdit* f_line_edit = new QLineEdit(&dialog);
    QPushButton* add = new QPushButton("Find in bd",&dialog);
    QTextEdit* tedit = new QTextEdit(&dialog);

    QVBoxLayout layout(&dialog);
    layout.addWidget(new QLabel("author:", &dialog));
    layout.addWidget(f_line_edit);
    layout.addWidget(tedit);
    layout.addWidget(add);

    connect(add, &QPushButton::clicked,[&]()
            {
                QString author = f_line_edit->text();
                if(!author.isEmpty() )
                {
                    QSqlQuery findQ;
                    findQ.prepare("SELECT * FROM books WHERE author = :author");
                    findQ.bindValue(":author", author);
                    if(findQ.exec())
                    {
                        sbar->showMessage("found");
                        while(findQ.next())
                        {
                            QString title = findQ.value(1).toString();
                            int pages = findQ.value(2).toInt();
                            QString country = findQ.value(3).toString();
                            QString author = findQ.value(4).toString();

                            tedit->append("Title: " + title );
                            tedit->append("Pages: " + QString::number(pages) );
                            tedit->append("Country: " + country);
                            tedit->append("Author: " + author);
                            tedit->append("------------");
                        }
                    }
                    else
                    {
                        QString err = "error exec fail: " +  findQ.lastError().text();
                        sbar->showMessage(err);
                        sbar->setStyleSheet("color: red");
                    }
                }
                else
                    QMessageBox::warning(this,tr("Warning"),tr("is Empthy"));
            });

    dialog.exec();
}
MainWindow::~MainWindow()
{

}
