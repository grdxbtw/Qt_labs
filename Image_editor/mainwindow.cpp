#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    make_login();
    // load email and pass if they exist
    load_settings();

    this->setCentralWidget(centralw);
    connect(login,SIGNAL(clicked(bool)),this,SLOT(main_app()));

}

void MainWindow::make_login()
{
    this->resize(150,120);
    centralw = new QWidget(this);

    log_title = new QLabel("Log In");
    log_title->setAlignment(Qt::AlignCenter);

    line_em = new QLineEdit;
    line_em->setFixedSize(200,20);
    line_em->setToolTip("Enter your email");
    line_em->setPlaceholderText("email");

    line_pass = new QLineEdit;
    line_pass->setFixedSize(200,20);
    line_pass->setToolTip("Enter your password");
    line_pass->setEchoMode(QLineEdit::Password);
    line_pass->setEnabled(false);
    line_pass->setPlaceholderText("password");

    show_pass = new QCheckBox;
    show_pass->setToolTip("show password");
    show_pass->setCheckable(false);

    remember = new QCheckBox;
    remember->setToolTip("remember my data");
    remember->setCheckable(false);

    info_ = new QLabel;
    info_->setAlignment(Qt::AlignCenter);

    connect(show_pass, &QCheckBox::stateChanged,[this](int state)
            {
                if(state == Qt::Checked)
                {
                    line_pass->setEchoMode(QLineEdit::Normal);
                }
                else if(state == Qt::Unchecked)
                {
                    line_pass->setEchoMode(QLineEdit::Password);
                }
            });

    connect(line_em, SIGNAL(textChanged(QString)),this,SLOT(check_email(QString)));
    connect(line_pass, SIGNAL(textChanged(QString)),this,SLOT(check_pass(QString)));
    login = new QPushButton("Authoristion");

    grl = new QGridLayout;
    grl->addWidget(log_title,0,0);
    grl->addWidget(line_em,1,0);
    grl->addWidget(line_pass,2,0);
    grl->addWidget(show_pass,2,1);
    grl->addWidget(login,3,0);
    grl->addWidget(remember,3,1);
    grl->addWidget(info_,4,0);
    centralw->setLayout(grl);

}

void MainWindow::load_settings()
{
    QSettings setting("My_app, My_app");
    QString my_em = setting.value("user/email").toString();
    QString passw = setting.value("user/password").toString();
    if(!my_em.isEmpty() && !passw.isEmpty())
    {
        line_em->setText(my_em);
        line_pass->setText(passw);
        remember->setCheckState(Qt::Checked);
    }
}

void MainWindow::check_email(QString text)
{
    info_->setText("");
    info_->setStyleSheet("");
    std::regex numb("\\w+@\\w+\\.\\w+");
    if(std::regex_match(text.toStdString(),numb))
    {
        line_em->setStyleSheet("");
        line_pass->setEnabled(true);
        show_pass->setCheckable(true);
    }
    else
    {
        line_em->setStyleSheet("border: 1px solid red; }");
    }
}
void MainWindow::check_pass(QString)
{
    info_->setText("");
    info_->setStyleSheet("");
    line_em->setStyleSheet("");
    line_pass->setStyleSheet("");
    remember->setCheckable(true);
}

void MainWindow::main_app()
{
    QString mail = line_em->text();
    QString pass = line_pass->text();
    if(mail == my_email && pass == password)
    {
        if(remember->isChecked())
        {
            QSettings setting("My_app, My_app");
            setting.setValue("user/email",mail);
            setting.setValue("user/password",pass);
        }
        else
        {
            QSettings setting("My_app, My_app");
            setting.setValue("user/email","");
            setting.setValue("user/password","");
        }

        centralw->close();
        this->showMaximized();
        make_bars();
        add_menu_file();
        make_dock_w();
        make_centralw();
    }
    else
    {
        line_em->setStyleSheet("border: 1px solid red; }");
        line_pass->setStyleSheet("border: 1px solid red; }");
        info_->setText("  wrong email or wrong password");
        info_->setStyleSheet("color: red;");
    }
}

void MainWindow::make_bars()
{
    mbar = this->menuBar();
    sbar = this->statusBar();
    tbar = new QToolBar(this);
    this->addToolBar(tbar);
}

void MainWindow::add_menu_file()
{
    file_ = new QMenu("&File");
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

    file_->addAction(f_open);
    file_->addSeparator();
    file_->addAction(f_save_as);
    file_->addSeparator();
    file_->addAction(f_save);
    file_->addSeparator();
    file_->addAction(f_close);
    mbar->addMenu(file_);
    tbar->addAction(f_open);
    tbar->addAction(f_save);
    tbar->addAction(f_close);

    // are used after log in
    connect(f_open,SIGNAL(triggered(bool)),this, SLOT(open_()));
    connect(f_save_as,SIGNAL(triggered(bool)),this, SLOT(save_as()));
    connect(f_save,SIGNAL(triggered(bool)),this, SLOT(save_()));
    connect(f_close,SIGNAL(triggered(bool)),this, SLOT(close_()));
}

void MainWindow::add_tools_for_img()
{
    apply = new QAction("&Apply");
    apply->setToolTip("apply all changes");
    apply->setIcon(QIcon(":/my_img/img/apply.png"));

    deny = new QAction("&Cancel");
    deny->setToolTip("cancel all changes");
    deny->setIcon(QIcon(":/my_img/img/cancel.png"));

    to_grey = new QAction("&Grey");
    to_grey->setToolTip("turn to grey ");

    sepia =  new QAction("&Sepia");
    sepia->setToolTip("apply sepia ");

    pixel = new QAction("&Pixel");
    pixel->setToolTip("pixelise");

    twirl_ = new QAction("&Twirl");
    twirl_->setToolTip("make twirl");

    invertion = new QAction("&Invertion");
    invertion->setToolTip(" make invertion ");

    brightness = new QAction("&Bright");
    brightness->setToolTip(" make more bright ");

    rotate_img = new QAction("&Rotate");
    rotate_img->setToolTip(" make rotation ");

    use_monochrome = new QAction("&Monochrome");
    use_monochrome->setToolTip(" make Monochrome ");

    tbar->addSeparator();
    tbar->addAction(apply);
    tbar->addAction(deny);
    tbar->addAction(sepia);
    tbar->addAction(to_grey);
    tbar->addAction(invertion);
    tbar->addAction(pixel);
    tbar->addAction(twirl_);
    tbar->addAction(brightness);
    tbar->addAction(rotate_img);
    tbar->addAction(use_monochrome);


    connect(apply,SIGNAL(triggered(bool)),this,SLOT(apply_changes()));
    connect(deny,SIGNAL(triggered(bool)),this,SLOT(cancel_changes()));
    connect(to_grey,SIGNAL(triggered(bool)),this,SLOT(make_grey()));
    connect(sepia,SIGNAL(triggered(bool)),this,SLOT(make_sepya()));
    connect(brightness,SIGNAL(triggered(bool)),this,SLOT(make_brightness()));
    connect(invertion,SIGNAL(triggered(bool)),this,SLOT(make_invertion()));;
    connect(pixel,SIGNAL(triggered(bool)),this,SLOT(make_pixelizise()));
    connect(twirl_,SIGNAL(triggered(bool)),this, SLOT(make_twirl()));
    connect(use_monochrome,SIGNAL(triggered(bool)),this,SLOT(make_use_monochrome()));
    connect(rotate_img,SIGNAL(triggered(bool)),this,SLOT(make_rotation()));
}

void MainWindow::make_centralw()
{
    sec_centralw = new QWidget(this);
    sec_centralw->setStyleSheet("border: 1px solid grey");

    imglabel = new QLabel;
    imglabel->setFixedSize(500,500);
    imglabel->setAlignment(Qt::AlignCenter);

    QGridLayout *layout = new QGridLayout(sec_centralw);
    layout->addWidget(imglabel,0,3, Qt::AlignCenter);
    pixmap = QPixmap(imglabel->size());

    this->setCentralWidget(sec_centralw);
}

void MainWindow::make_dock_w()
{
    dock = new QDockWidget("Tools", this);
    dock->resize(300,this->height());
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    this->addDockWidget(Qt::RightDockWidgetArea ,dock);
}

void MainWindow::drow_img()
{
    imglabel->setPixmap(pixmap.scaled(imglabel->size()));
}

void MainWindow::reset_widget()
{
    dock->setWidget(nullptr);
    editedimg = image;
}

void MainWindow::apply_changes()
{
    image = editedimg;
    pixmap = QPixmap::fromImage(editedimg);
    drow_img();
}

void MainWindow::cancel_changes()
{
    editedimg = image;
    pixmap = QPixmap::fromImage(editedimg);
    drow_img();
}

void MainWindow::make_pixelizise()
{
    QWidget *w = new QWidget;
    pixel_box = new QSpinBox;
    pixel_box->resize(200,20);
    pixel_box->setMaximum(100);
    pixel_box->setMinimum(1);
    QVBoxLayout *vb = new QVBoxLayout(w);
    vb->addWidget(pixel_box);
    w->setLayout(vb);
    dock->setWidget(w);

    connect(pixel_box, &QSpinBox::valueChanged,[this](int pos){

        int width = editedimg.width();
        int height = editedimg.height();

        for (int y = 0; y < height; y += pos)
        {
            for (int x = 0; x < width; x += pos)
            {
                int totalRed = 0;
                int totalGreen = 0;
                int totalBlue = 0;

                int windowWidth = qMin(pos, width - x);
                int windowHeight = qMin(pos, height - y);

                for (int j = 0; j < windowHeight; j++) {
                    for (int i = 0; i < windowWidth; i++) {
                        QRgb pixel = image.pixel(x + i, y + j);
                        totalRed += qRed(pixel);
                        totalGreen += qGreen(pixel);
                        totalBlue += qBlue(pixel);
                    }
                }

                int numPixels = windowWidth * windowHeight;

                int averageRed = totalRed / numPixels;
                int averageGreen = totalGreen / numPixels;
                int averageBlue = totalBlue / numPixels;

                for (int j = 0; j < windowHeight; j++) {
                    for (int i = 0; i < windowWidth; i++) {
                        editedimg.setPixel(x + i, y + j, qRgb(averageRed, averageGreen, averageBlue));
                    }
                }
            }
        }
        pixmap = QPixmap::fromImage(editedimg);
        drow_img();

    });
}

void MainWindow::make_twirl()
{
    QWidget *w = new QWidget;
    sltwirl = new QSlider(Qt::Horizontal);
    sltwirl->resize(200,20);
    sltwirl->setMaximum(100);
    sltwirl->setMinimum(-100);
    sltwirl->setSliderPosition(0);
    sltwirl->setTickPosition(QSlider::TicksBothSides);
    QVBoxLayout *vb = new QVBoxLayout(w);
    vb->addWidget(sltwirl);
    w->setLayout(vb);
    dock->setWidget(w);

    connect(sltwirl,&QSlider::sliderMoved,[this](int pos){
        editedimg = image.copy(image.rect());
        float maxDistance = QVector2D(editedimg.width(),
                                      editedimg.height()).length()/2;
        int maxAngle = pos;
        QVector2D imageCenter(editedimg.width()/2, editedimg.height()/2);
        for(int i=0; i < editedimg.height(); i++)
        {
            for(int j = 0; j < editedimg.width(); j++)
            {
                QVector2D currentPos(j, i);
                QVector2D v = currentPos - imageCenter;
                float r = v.length();
                float twirlAngle = r/maxDistance * maxAngle;
                float angle = atan2(v.y(), v.x());
                QVector2D out(r*cos(angle+twirlAngle)+imageCenter.x(),
                              r*sin(angle+twirlAngle)+imageCenter.y());
                int x = out.x();
                int y = out.y();
                if (x < 0)
                    x = 0;
                if (x > editedimg.width()-1)
                    x = editedimg.width()-1;
                if (y < 0)
                    y = 0;
                if (y > editedimg.height()-1)
                    y = editedimg.height()-1;
                editedimg.setPixel(j, i, image.pixel(x, y));
            }
        }
        pixmap = QPixmap::fromImage(editedimg);
        drow_img();
    });
}


void MainWindow::make_use_monochrome()
{
    QWidget *w = new QWidget;
    edit_combo_box = new QComboBox();
    edit_combo_box->addItem("Colors");
    edit_combo_box->addItem("Red");
    edit_combo_box->addItem("Green");
    edit_combo_box->addItem("Blue");

    QVBoxLayout *vb = new QVBoxLayout(w);
    vb->addWidget(edit_combo_box);
    w->setLayout(vb);
    dock->setWidget(w);

    connect(edit_combo_box,&QComboBox::currentIndexChanged,[this](int ind){

        if(ind == 0)
            return;
        editedimg = image.copy(image.rect());

        for (int y = 0; y < editedimg.height(); y++)
        {
            for (int x = 0; x < editedimg.width(); x++)
            {
                QRgb pixel = editedimg.pixel(x, y);

                int grayValue = 0;

                if (ind == 1)
                {
                    grayValue = qRed(pixel);
                } else if (ind == 2)
                {
                    grayValue = qGreen(pixel);
                } else if (ind == 3)
                {
                    grayValue = qBlue(pixel);
                }

                editedimg.setPixel(x, y, qRgb(grayValue, grayValue, grayValue));
            }
        }
        pixmap = QPixmap::fromImage(editedimg);
        drow_img();

    });
}

void MainWindow::make_rotation()
{
    QWidget *w = new QWidget;
    double_spin_box = new QDoubleSpinBox;
    double_spin_box->resize(200,20);
    double_spin_box->setMaximum(250.0);
    double_spin_box->setMinimum(0.0);
    QVBoxLayout *vb = new QVBoxLayout(w);
    vb->addWidget(double_spin_box);
    w->setLayout(vb);
    dock->setWidget(w);

    connect(double_spin_box,&QDoubleSpinBox::valueChanged,[this](double c){
        editedimg = image;
        for (int y = 0; y < editedimg.height(); y++)
        {
            for (int x = 0; x < editedimg.width(); x++)
            {
                QColor pixelColor = editedimg.pixelColor(x, y);

                int red = qBound(0, int(qLn(pixelColor.red() + 1) * c), 255);
                int green = qBound(0, int(qLn(pixelColor.green() + 1) * c), 255);
                int blue = qBound(0, int(qLn(pixelColor.blue() + 1) * c), 255);

                editedimg.setPixelColor(x, y, QColor(red, green, blue));
            }
        }
        pixmap = QPixmap::fromImage(editedimg);
        drow_img();


    });
}

void MainWindow::make_brightness()
{

    QWidget *w = new QWidget;
    bright = new QSlider(Qt::Horizontal);
    bright->resize(200,20);
    bright->setMaximum(100);
    bright->setMinimum(-100);
    bright->setSliderPosition(0);
    bright->setTickPosition(QSlider::TicksBothSides);
    QVBoxLayout *vb = new QVBoxLayout(w);
    vb->addWidget(bright);
    w->setLayout(vb);
    dock->setWidget(w);
    connect(bright,&QSlider::sliderMoved,[this](int delta){
        editedimg = image;
        for (int y = 0; y < editedimg.height(); ++y)
        {
            for (int x = 0; x < editedimg.width(); ++x)
            {
                QRgb pixel = editedimg.pixel(x, y);

                int red = qRed(pixel) + delta;
                int green = qGreen(pixel) + delta;
                int blue = qBlue(pixel) + delta;

                red = qBound(0, red, 255);
                green = qBound(0, green, 255);
                blue = qBound(0, blue, 255);

                editedimg.setPixel(x, y, qRgb(red, green, blue));
            }
        }
        pixmap = QPixmap::fromImage(editedimg);
        drow_img();
    });
}

void MainWindow::make_grey()
{
    reset_widget();
    editedimg = image.convertToFormat(QImage::Format_Grayscale8);
    pixmap = QPixmap::fromImage(editedimg);
    drow_img();
}

void MainWindow::make_sepya()
{
    reset_widget();
    for (int y = 0; y < editedimg.height(); ++y)
    {
        for (int x = 0; x < editedimg.width(); ++x)
        {
            QRgb pixel = editedimg.pixel(x, y);

            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);


            int sepiaRed = qMin(255, (int)(0.272 * red + 0.534 * green + 0.131 * blue));
            int sepiaGreen = qMin(255, (int)(0.349 * red + 0.686 * green + 0.168 * blue));
            int sepiaBlue = qMin(255, (int)(0.393 * red + 0.769 * green + 0.189 * blue));

            editedimg.setPixel(x, y, qRgb(sepiaRed, sepiaGreen, sepiaBlue));
        }
    }
    pixmap = QPixmap::fromImage(editedimg);
    drow_img();
}

void MainWindow::make_invertion()
{
    reset_widget();
    for (int y = 0; y < editedimg.height(); ++y)
    {
        for (int x = 0; x < editedimg.width(); ++x)
        {
            QRgb pixel = editedimg.pixel(x, y);

            int red = 255 - qRed(pixel);
            int green = 255 - qGreen(pixel);
            int blue = 255 - qBlue(pixel);

            editedimg.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    pixmap = QPixmap::fromImage(editedimg);
    drow_img();
}

void MainWindow::open_()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QString(), tr("Images (*.png *.jpg *.bmp)"));

    image.load(filename);
    if (image.isNull())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot load %1").arg(QDir::toNativeSeparators(filename)));
        return;
    }
    // called only one time
    std::call_once(f_to_make_tools,[this]()->void{ add_tools_for_img();});

    editedimg = image;
    f_save->setEnabled(true);
    pixmap = QPixmap::fromImage(image);
    current_file = filename;
    drow_img();
}

void MainWindow::close_()
{
    if(QMessageBox::question(this, "Exit Dialog", "Do you really want to leave") == QMessageBox::StandardButton::Yes)
    {
        QCoreApplication::quit();
    }
}

void MainWindow::save_as()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),QString(),
                                                    tr("Images (*.png *.jpg *.bmp)"));

    if (image.isNull())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot save %1").arg(QDir::toNativeSeparators(filename)));
        return;
    }
    else
    {
        f_save->setEnabled(true);
        current_file = filename;
        image.save(filename);
    }
}

void MainWindow::save_()
{
    if (image.isNull() or current_file.isEmpty())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot save %1").arg(QDir::toNativeSeparators(current_file)));
        return;
    }
    else
        image.save(current_file);
}



MainWindow::~MainWindow()
{
    delete ui;
}
