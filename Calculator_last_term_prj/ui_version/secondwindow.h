#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include"QLineEdit"


namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();
    void Set_line(QLineEdit* line_edit)
    {
        line = line_edit;
    }

private:
    Ui::SecondWindow *second_ui;
    QLineEdit* line;



private slots:
    void trygonometry_functions();

};

#endif // SECONDWINDOW_H
