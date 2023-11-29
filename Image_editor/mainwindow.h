#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QDockWidget>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>
#include <QSettings>
#include <QFormLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QSlider>
#include <QtMinMax>
#include <QVector2D>
#include <QSpinBox>

//c++
#include <regex>
#include <mutex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::once_flag f_to_make_tools;

    // bars
    QMenuBar *mbar;
    QStatusBar *sbar;
    QToolBar *tbar;

    // menu file
    QMenu *file_;
    QAction *f_open, *f_close, *f_save_as, *f_save;

    QString current_file;

    //login
    QWidget *centralw, *sec_centralw;
    QLabel *log_title, *info_;
    QLineEdit *line_em, *line_pass;
    QPushButton *login;
    QCheckBox *show_pass, *remember;
    QGridLayout *grl;
    QString password {"grdxbtw"};
    QString my_email{"grdxbtw@gmail.com"};

    //for cental widget
    QLabel *imglabel;
    QVBoxLayout *vbl;

    // dynamic changing tools
    QComboBox *edit_combo_box; // for monochrome
    QSpinBox *spin_box, *pixel_box; //pixel
    QDoubleSpinBox *double_spin_box; //rotate ln
    QSlider *bright, *sltwirl; // make twirl
    QPixmap pixmap;
    QImage image;
    QImage editedimg;

    // actions on toolbar
    QAction *apply, *deny, *to_grey, *sepia, *invertion, *brightness, *rotate_img, *use_monochrome, *pixel, *twirl_;

    //dock
    QDockWidget *dock;

private:
    void make_bars();
    void add_menu_file();
    void add_tools_for_img();
    void make_login();
    void make_centralw();
    void make_dock_w();
    void drow_img();
    void load_settings();
    void reset_widget();

public slots:
    void open_();
    void close_();
    void save_as();
    void save_();
    void check_pass(QString);
    void check_email(QString);
    void main_app();

    ///////////
    void apply_changes();
    void cancel_changes();
    void make_grey();
    void make_sepya();
    void make_invertion();
    void make_pixelizise();
    void make_twirl();
    void make_use_monochrome();
    void make_rotation();
    void make_brightness();
};
#endif // MAINWINDOW_H
