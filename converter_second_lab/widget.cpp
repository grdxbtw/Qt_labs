#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("Converter");
    this->setFixedSize(QSize(430,400));

    button1 = new QPushButton("Clear", this);
    apply_css(button1);

    button2 = new QPushButton("Convert", this);
    QPalette p = button2->palette();
    p.setColor(QPalette::Button,Qt::blue);
    button2->setPalette(p);
    button2->setAutoFillBackground(true);
    button2->setStyleSheet("QPushButton {"
                           "float: right;"
                           "font-size: 1.2em;"
                           "}");

    spin_box = new QDoubleSpinBox(this);
    spin_box->setValue(37.4);
    spin_box->setRange(0.01, 10000.0);
    spin_box->setDecimals(2);
    check_box = new QCheckBox(this);
    label= new QLabel("Invert Colors",this);

    combo_box1 = new QComboBox(this);
    combo_box2 = new QComboBox(this);

    combo_box1->addItem("USD");
    combo_box1->addItem("EUR");
    combo_box1->addItem("UAH");
    combo_box2->addItem("UAH");
    combo_box2->addItem("EUR");
    combo_box2->addItem("USD");
    line_edit = new QLineEdit(this);
    label2 = new QLabel("Output",this);

    rb1 = new QRadioButton("Background color 1",this);
    rb2 = new QRadioButton("Background color 2",this);
    rb3 = new QRadioButton("Background color 3",this);

    radioGroup = new QGroupBox(this);
    radioLayout = new QVBoxLayout;

    box1 = new QCheckBox("Background color off",this);
    box2 = new QCheckBox("Extra info-table", this);

    boxGroup = new QGroupBox(this);
    checkLayout = new QVBoxLayout;

    line_edit->setToolTip("Input");
    line_edit->setStyleSheet("QLineEdit:hover {"
    "background-color: rgb(239, 243, 255);"
    "}");

    button1->setGeometry(set_geometry());
    button2->setGeometry(set_geometry());
    check_box->setGeometry(set_geometry());
    line_edit->setGeometry(set_geometry());
    spin_box->setFixedSize(50,30);
    combo_box1->setFixedSize(50,30);
    combo_box2->setFixedSize(50,30);


    button1->move(30,170);
    button2->move(165, 170);
    spin_box->move(210,70);
    check_box->move(30,10);
    label->move(50,15);

    line_edit->move(30,70);
    combo_box1->move(130,70);
    label2->move(300,75);
    combo_box2->move(350,70);

    radioGroup->move(30, 270);
    radioLayout->addWidget(rb1);
    radioLayout->addWidget(rb2);
    radioLayout->addWidget(rb3);
    radioGroup->setLayout(radioLayout);

    boxGroup->move(270,280);
    checkLayout->addWidget(box1);
    checkLayout->addWidget(box2);
    boxGroup->setLayout(checkLayout);

    connect(line_edit,SIGNAL(returnPressed()),this,SLOT(show_result()) );
    connect(combo_box1,SIGNAL(currentIndexChanged(int)),this,SLOT(set_currency()));
    connect(combo_box2,SIGNAL(currentIndexChanged(int)),this,SLOT(set_currency()));
    connect(button1,SIGNAL(clicked()),this, SLOT(clear_all()));
    connect(button2,SIGNAL(clicked()),this, SLOT(show_result()));
    connect(rb1 ,SIGNAL(clicked()),this,SLOT(change_background()));
    connect(rb2 ,SIGNAL(clicked()),this,SLOT(change_background()));
    connect(rb3 ,SIGNAL(clicked()),this,SLOT(change_background()));
    connect(box1,SIGNAL(stateChanged(int)),this,SLOT(off_background(int)));
    connect(box2,SIGNAL(stateChanged(int)),this,SLOT(show_info(int)));
    connect(check_box,SIGNAL(stateChanged(int)),this,SLOT(change_button_css(int)));


}

Widget::~Widget()
{

}

void Widget::show_result()
{
    label2->clear();
    double result = line_edit->text().toDouble() * spin_box->value();
    label2->setText(QString::number(result));

}

void Widget::set_currency()
{
    QString from = combo_box1->currentText();
    QString to = combo_box2->currentText();

    if(from == "USD" )
    {
        if(to == "USD")
        {
            spin_box->setValue(1.00);
        }
        else if(to == "EUR")
        {
            spin_box->setValue(0.94);
        }
        else if(to == "UAH")
        {
            spin_box->setValue(36.87);
        }
    }
    else if(from == "EUR")
    {
        if(to == "USD")
        {
            spin_box->setValue(1.07);
        }
        else if(to == "EUR")
        {
            spin_box->setValue(1.00);
        }
        else if(to == "UAH")
        {
            spin_box->setValue(39.32);
        }
    }
    else if(from == "UAH")
    {
        if(to == "USD")
        {
            spin_box->setValue(0.27);
        }
        else if(to == "EUR")
        {
            spin_box->setValue(0.25);
        }
        else if(to == "UAH")
        {
            spin_box->setValue(1.00);
        }
    }

}

void Widget::apply_css(QPushButton *b)
{
    QString css("QPushButton {"
                "float: right;"
                "margin-right: 3%;"
                "margin-top: 5px;"
                "border-radius: 5px;"
                "padding: 5px 9px;"
                "font-size: 1.2em;"
                "background-color: rgba(236, 101, 80, 1);"
                "border-bottom: 4px solid rgba(217, 91, 72, 1);"
                "color: white;"
                "font-family: 'Roboto Slab', serif;"

                "}"
                "QPushButton:pressed {"
                "float: right;"
                "margin-right: 3%;"
                "margin-top: 5px;"
                "border-radius: 5px;"
                "padding: 5px 9px;"
                "font-size: 1.2em;"
                "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0#FF7832, stop: 1 #FF9739);"
                "}");

    b->setStyleSheet(css);
}

void Widget::clear_all()
{
    line_edit->clear();
    label2->setText("Output");
    combo_box1->setCurrentIndex(0);
    combo_box2->setCurrentIndex(0);
    spin_box->setValue(36.87);

}

void Widget::change_background()
{
    if(box1->isChecked())
        return;

    if(rb1->isChecked())
    {
        this->setStyleSheet("background-color: #ADD8E6;");
    }
    else if (rb2->isChecked())
    {
        this->setStyleSheet("background-color: #E6E6FA;");
    }
    else if (rb3->isChecked())
    {
        this->setStyleSheet("background-color: #F5F5DC;");
    }
    else
        qDebug() << "other button cant be checked";
}

void Widget::off_background(int state)
{
    if(state == Qt::Checked)
    {
        this->setStyleSheet("");
    }
    else if (state == Qt::Unchecked )
    {
        change_background();
    }
}

void Widget::show_info(int state)
{
    if(state == Qt::Checked)
    {
        QString info{"This app is Currency Converter\n "
             "Currency available: USD, EUR, UAH\n "
            "Backgrounds: Blue, Beige, Lavander."
        };
        QMessageBox::information(this,"Extra Info", info);
        box2->setCheckState(Qt::Unchecked);
    }
}

void Widget::change_button_css(int state)
{
    if(state == Qt::Checked)
    {
        apply_css(button2);
        button1->setStyleSheet("QPushButton {"
                               "float: right;"
                               "font-size: 1.2em;"
                               "}");
    }
    else if (state == Qt::Unchecked )
    {
        apply_css(button1);
        QPalette p = button2->palette();
        p.setColor(QPalette::Button,Qt::blue);
        button2->setPalette(p);
        button2->setAutoFillBackground(true);
        button2->setStyleSheet("QPushButton {"
                               "float: right;"
                               "font-size: 1.2em;"
                               "}");
    }
}

