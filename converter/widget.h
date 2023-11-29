#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QRect>
#include <QLineEdit>
#include <QRadioButton>
#include<QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>
#include <QVBoxLayout>>
#include <QGroupBox>
#include <QStringList>
#include <QPalette>
#include <QMessageBox>



class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QPushButton* button1, *button2;
    QDoubleSpinBox *spin_box;
    QCheckBox *check_box;
    QLabel *label, *label2;
    QComboBox *combo_box1, *combo_box2;
    QLineEdit *line_edit;
    QRadioButton *rb1, *rb2, *rb3;
    QGroupBox *radioGroup, *boxGroup;
    QVBoxLayout* radioLayout, *checkLayout;
    QCheckBox *box1, *box2, *box3;

    QRect set_geometry() const  //to avoid multiple coding
    {
        return QRect(100,30,100,30);
    }


private slots:
    void show_result(); // give result to Output label
    void set_currency(); // change currency in spin_box

    void clear_all();
    void change_background();
    void off_background(int);
    void show_info(int); // extra window with needed info
    void change_button_css(int);

private:
    void apply_css(QPushButton* b);
    void make_buttons();
    void make_boxs();
    void make_radio_buttons();
    void set_buttons_size();
    void move_buttons();

};

#endif // WIDGET_H
