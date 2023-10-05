#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("Text Editor");
    this->resize(500,500);

    make_central_widget(); // make text editor + label with filename

    bar = this->menuBar(); // make menubar
    file = bar->addMenu("&File");
    edit = bar->addMenu("&Edit");
    help = bar->addMenu("&Help");

    st_bar = new QStatusBar(this); // make statusbar
    setStatusBar(st_bar);

    make_file(); //make all actions
    make_edit();
    make_help();
    make_tools();

    make_dock();   // make docks
    make_second_dock();

    this->adjustSize();

    update_time();  // timer for status bar
    timer_ = new QTimer(this);
    connect(timer_,SIGNAL(timeout()),this,SLOT(update_time()));
    timer_->start(1000);

    //menu file
    connect(f_new,SIGNAL(triggered(bool)),this, SLOT(new_()));
    connect(f_open,SIGNAL(triggered(bool)),this, SLOT(open_()));
    connect(f_save_as,SIGNAL(triggered(bool)),this, SLOT(save_as()));
    connect(f_save,SIGNAL(triggered(bool)),this, SLOT(save_()));
    connect(f_close,SIGNAL(triggered(bool)),this, SLOT(close_()));

    //menu edit
    connect(clear_,SIGNAL(triggered(bool)),this, SLOT(clear_text()));
    connect(to_upcase,SIGNAL(triggered(bool)),this, SLOT(to_upper()));
    connect(to_lowcase,SIGNAL(triggered(bool)),this, SLOT(to_lower()));
    connect(font_up,SIGNAL(triggered(bool)),this, SLOT(font_upcase()));
    connect(font_low,SIGNAL(triggered(bool)),this, SLOT(font_down()));
    connect(replace_,SIGNAL(triggered(bool)),this, SLOT(find_replace()));

    //menu help
    connect(about_app, SIGNAL(triggered(bool)),this, SLOT(info()));
    connect(ref,SIGNAL(triggered(bool)),this, SLOT(reference()));

    //first dock buttons
    connect(C,SIGNAL(clicked(bool)),this, SLOT(clear_text()));
    connect(UP,SIGNAL(clicked(bool)),this, SLOT(to_upper()));
    connect(LOW,SIGNAL(clicked(bool)),this, SLOT(to_lower()));
    connect(Font_Up,SIGNAL(clicked(bool)),this, SLOT(font_upcase()));
    connect(Font_Down,SIGNAL(clicked(bool)),this, SLOT(font_down()));
    connect(Rep,SIGNAL(clicked(bool)),this, SLOT(find_replace()));

    //second dock buttons
    connect(N,SIGNAL(clicked(bool)),this, SLOT(new_()));
    connect(O,SIGNAL(clicked(bool)),this, SLOT(open_()));
    connect(S_AS,SIGNAL(clicked(bool)),this, SLOT(save_as()));
    connect(S,SIGNAL(clicked(bool)),this, SLOT(save_()));
    connect(Close,SIGNAL(clicked(bool)),this, SLOT(close_()));
}


void MainWindow::open_()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QString("C:\\Users\\Zhema\\Documents\\Qt\\Text_Editor"),

                                                                                          tr("Text files (*.txt)"));
    std::string file = filename.toStdString();
    std::size_t pos = file.rfind('/');
    std::string f = file.substr(pos + 1,file.size() - pos);
    text_label->setText(QString(f.c_str()));

    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }
        QTextStream in(&file);
        QString txt = in.readAll();
        text_editor ->setText(txt);
        file.close();
    }

}
void MainWindow::close_()
{
    if(QMessageBox::question(this, "Exit Dialog", "Do you really want to leave") == QMessageBox::StandardButton::Yes)
    {
        QCoreApplication::quit();
//        this->close();
    }
}

void MainWindow::save_as()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),
                                                    tr("Text files (*.txt)"));

    if(!filename.isEmpty())
    {
        f_save->setEnabled(true);
        S->setEnabled(true);

        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }
        std::string file_op = filename.toStdString();
        std::size_t pos = file_op.rfind('/');
        std::string f = file_op.substr(pos + 1,file_op.size() - pos);
        text_label->setText(QString(f.c_str()));

        QTextStream out(&file);
        out << text_editor->toPlainText();
        current_file = filename;
        out.flush();
        file.close();
    }

}

void MainWindow::save_()
{
    QString str = text_label->text();
    if(str.endsWith('*'))
    {
        const auto it = str.begin() + str.size() - 1;
        str.erase(it);
        text_label->setText(str);
    }

    if(!current_file.isEmpty())
    {
        QFile file(current_file);
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }

        QTextStream out(&file);
        out << text_editor->toPlainText();
        out.flush();
        file.close();
    }
}

void MainWindow::new_()
{

    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),
                                                    tr("Text files (*.txt)"));

    if(!filename.isEmpty())
    {

        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }
        std::string file_op = filename.toStdString();
        std::size_t pos = file_op.rfind('/');
        std::string f = file_op.substr(pos + 1,file_op.size() - pos);
        text_label->setText(QString(f.c_str()));

        QTextStream out(&file);
        out << text_editor->toPlainText();
        current_file = filename;
        out.flush();
        file.close();
    }
}

void MainWindow::clear_text()
{
    text_editor->clear();
}

void MainWindow::to_upper()
{
    if(!text_editor->textCursor().hasSelection())
    {
        QString str = text_editor->toPlainText();
        text_editor->setText(str.toUpper());
        return;
    }
    QString str = text_editor->textCursor().selectedText();
    QString up_str = str.toUpper();
    QTextCursor cursor2 = text_editor->textCursor();
    cursor2.removeSelectedText();
    cursor2.insertText(up_str);
}

void MainWindow::to_lower()
{
    if(!text_editor->textCursor().hasSelection())
    {
        QString str = text_editor->toPlainText();
        text_editor->setText(str.toLower());
        return;
    }
    QString str = text_editor->textCursor().selectedText();
    QString low_str = str.toLower();
    QTextCursor cursor2 = text_editor->textCursor();
    cursor2.removeSelectedText();
    cursor2.insertText(low_str);
}

void MainWindow::font_upcase()
{
    QTextCursor cursor = text_editor->textCursor();
    QTextCharFormat format = cursor.charFormat();
    int currentSize = format.fontPointSize() + 8;
    int newSize = currentSize + 2;

    format.setFontPointSize(newSize);
    cursor.mergeCharFormat(format);
    text_editor->setTextCursor(cursor);
}

void MainWindow::font_down()
{
    QTextCursor cursor = text_editor->textCursor();
    QTextCharFormat format = cursor.charFormat();
    int currentSize = format.fontPointSize();
    int newSize = currentSize - 2;

    format.setFontPointSize(newSize);
    cursor.mergeCharFormat(format);
    text_editor->setTextCursor(cursor);
}

void MainWindow::find_replace()
{
        QDialog findReplaceDialog(this);
        findReplaceDialog.setWindowTitle("find and replace");

        QLineEdit* f_line_edit = new QLineEdit(&findReplaceDialog);
        QLineEdit* r_line_edit = new QLineEdit(&findReplaceDialog);
        QPushButton* find_replace_Button = new QPushButton("Find and replace",&findReplaceDialog);


        QVBoxLayout layout(&findReplaceDialog);
        layout.addWidget(new QLabel("Find text:", &findReplaceDialog));
        layout.addWidget(f_line_edit);
        layout.addWidget(new QLabel("Replace: ", &findReplaceDialog));
        layout.addWidget(r_line_edit);
        layout.addWidget(find_replace_Button);

        connect(find_replace_Button, &QPushButton::clicked,[&]()
            {
                std::string find_ = f_line_edit->text().toStdString();
                std::string replace_ = r_line_edit->text().toStdString();

                std::string text = text_editor->toPlainText().toStdString();
                size_t pos = text.find(find_);
                if(pos != std::string::npos)
                {
                    text.replace(pos, find_.size(),replace_);
                    text_editor->setText(QString(text.c_str()));
                }
                else
                    QMessageBox::warning(this,tr("Warning"),tr("Not found"));
            });

        findReplaceDialog.exec();
}

void MainWindow::info()
{
    QString info{"This app is Text Editor\n "
        "Here we can change font and cctypes\n "
        "Also we can open and saves files."
    };
    QMessageBox::information(this,"Extra Info", info);
}

void MainWindow::reference()
{
    QString info{"We have 3  menu 'File', 'Edit', 'Help'\n "
        "we have some actions to work with files and text\n "
        "Also we can open and saves files."
    };
    QMessageBox::information(this,"How to use", info);
}

void MainWindow::update_time()
{
    st_bar->removeWidget(setTime);
    QDateTime date = QDateTime::currentDateTime();
    setTime = new QLabel;
    setTime->setText(date.toString("dd.MM.yyyy hh:mm:ss"));
    st_bar->addWidget(setTime);
}

void MainWindow::check_updates_file()
{
    QString str = text_label->text();
    if(str.endsWith('*'))
        return;

    if(str != current_p)
    {
        current_p = str;
        return;
    }
    else
    {
        str+="*";
        text_label->setText(str);
    }
}

void MainWindow::make_file()
{
    f_new = new QAction("&New");
    f_new->setShortcut(QKeySequence::New);
    f_new->setIcon(QIcon(":/my_img/img/new.png"));

    f_open = new QAction("&Open");
    f_open->setShortcut(QKeySequence::Open);
    f_open->setIcon(QIcon(":/my_img/img/open.png"));

    f_save_as = new QAction("&Save As...");
    f_save_as->setShortcut(QKeySequence::SaveAs);

    f_save = new QAction("&Save");
    f_save->setShortcut(QKeySequence::Save);
    f_save->setIcon(QIcon(":/my_img/img/save.png"));
    f_save->setEnabled(false);

    f_close = new QAction("&Close");
    f_close->setShortcut(QKeySequence::Close);
    f_close->setIcon(QIcon(":/my_img/img/exit.png"));

    file->addAction(f_new);
    file->addSeparator();
    file->addAction(f_open);
    file->addSeparator();
    file->addAction(f_save_as);
    file->addSeparator();
    file->addAction(f_save);
    file->addSeparator();
    file->addAction(f_close);

}

void MainWindow::make_edit()
{
    clear_ = new QAction("&Clear");
    clear_->setIcon(QIcon(":/my_img/img/clear.png"));

    to_upcase = new QAction("&Up_case");
    to_upcase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Up));
    to_upcase->setIcon(QIcon(":/my_img/img/uppercase.png"));

    to_lowcase = new QAction("&Low_case");
    to_lowcase->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Down));
    to_lowcase->setIcon(QIcon(":/my_img/img/lowercase.png"));

    font_up = new QAction("&Font_up");
    font_up->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    font_up->setIcon(QIcon(":/my_img/img/enlarge.png"));

    font_low = new QAction("&Font_down");
    font_low->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    font_low->setIcon(QIcon(":/my_img/img/shrink.png"));

    replace_ =  new QAction("&Replace");
    replace_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));

    edit->addAction(clear_);
    edit->addSeparator();
    edit->addAction(to_upcase);
    edit->addSeparator();
    edit->addAction(to_lowcase);
    edit->addSeparator();
    edit->addAction(font_up);
    edit->addSeparator();
    edit->addAction(font_low);
    edit->addSeparator();
    edit->addAction(replace_);

}

void MainWindow::make_help()
{
    about_app = new QAction("&About");
    about_app->setIcon(QIcon(":/my_img/img/about.png"));

    ref = new QAction("&Ref");

    help->addAction(about_app);
    help->addSeparator();
    help->addAction(ref);
    help->addSeparator();
}

void MainWindow::make_tools()
{
    tools = new QToolBar(this);
    this->addToolBar(tools);
    tools->addAction(f_new);
    tools->addAction(f_open);
    tools->addAction(f_save);
    tools->addAction(f_close);
    tools->addAction(to_upcase);
    tools->addAction(to_lowcase);
    tools->addAction(font_up);
    tools->addAction(font_low);
}

void MainWindow::make_dock()
{
    dock = new QDockWidget("Dock",this);
    // dock->setFixedSize(QSize(200,500));
    dock->adjustSize();
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    this->addDockWidget(Qt::TopDockWidgetArea ,dock);

    C = new QToolButton();
    C->setText("Clear");
    C->setFixedSize(QSize(100,50));

    UP = new QToolButton();
    UP->setText("UP_CASE");
    UP->setFixedSize(QSize(100,50));

    LOW = new QToolButton();
    LOW->setText("LOW_CASE");
    LOW->setFixedSize(QSize(100,50));

    Font_Up = new QToolButton();
    Font_Up->setText("FONT_UP");
    Font_Up->setFixedSize(QSize(100,50));

    Font_Down = new QToolButton();
    Font_Down->setText("FONT_DOWN");
    Font_Down->setFixedSize(QSize(100,50));

    Rep = new QToolButton();
    Rep->setText("Replace");
    Rep->setFixedSize(QSize(100,50));

    hl = new QHBoxLayout;
    hl->addWidget(C);
    hl->addWidget(UP);
    hl->addWidget(LOW);
    hl->addWidget(Font_Up);
    hl->addWidget(Font_Down);
    hl->addWidget(Rep);

    dockW = new QWidget(dock);
    dockW->setLayout(hl);
    dock->setWidget(dockW);

}

void MainWindow::make_second_dock()
{
    sec_dock = new QDockWidget(this);
    //sec_dock->setFixedSize(500,100);
    sec_dock->adjustSize();
    sec_dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    sec_dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    this->addDockWidget(Qt::LeftDockWidgetArea ,sec_dock);

    N = new QToolButton();
    N->setText("New");
    N->setFixedSize(QSize(100,50));

    O = new QToolButton();
    O->setText("Open");
    O->setFixedSize(QSize(100,50));

    S = new QToolButton();
    S->setText("Save");
    S->setFixedSize(QSize(100,50));
    S->setEnabled(false);

    S_AS = new QToolButton();
    S_AS->setText("Save_As");
    S_AS->setFixedSize(QSize(100,50));

    Close = new QToolButton();
    Close->setText("Close");
    Close->setFixedSize(QSize(100,50));

    l = new QGridLayout;
    l->addWidget(N,0,0);
    l->addWidget(O,1,0);
    l->addWidget(S,2,0);
    l->addWidget(S_AS,3,0);
    l->addWidget(Close,4,0);
    second_dockW = new QWidget(sec_dock);
    second_dockW->setLayout(l);
    sec_dock->setWidget(second_dockW);

}

void MainWindow::make_central_widget()
{
    centralW = new QWidget(this);
    QVBoxLayout* l = new QVBoxLayout;

    text_editor = new QTextEdit;
    text_editor->adjustSize(); //QSize(400,500));
    current_p = "filename.txt";
    text_label = new QLabel( current_p,centralW);
    l->addWidget(text_label,0);
    l->addWidget(text_editor,1);
    centralW->setLayout(l);
    this->setCentralWidget(centralW);

    connect(text_editor, SIGNAL(textChanged()),this,SLOT(check_updates_file()));

}

MainWindow::~MainWindow()
{
        /*std::ifstream fin; // here variants how to open/close files in c++ without qt tools
        fin.open(filename.toStdString());
        if(!fin.is_open())
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }
        text_editor.clear();
        while (fin)
        {
            std::string str;
            std::getline(fin,str);
            if(!fin.fail())
                set_text(QString(str.c_str()));
        }
        fin.close();

        std::ofstream fout;
        fout.open(filename.toStdString());
        if(!fout.is_open())
        {
            qDebug() << "wrong file | file isn't open";
            QMessageBox::critical(this,tr("Error"),tr("wrong file | file isn't open"));
            return;
        }
        QString str = text_editor->toPlainText();
        fout << str.toStdString();
        fout.close();*/
}

