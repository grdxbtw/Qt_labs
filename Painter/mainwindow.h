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
#include <QSpinBox>
#include <QPainter>
#include <QToolBar>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QColorDialog>
#include <QPainterPath>

//c++
#include <random>
#include <regex>
#include <mutex>
#include <functional>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    //for cental widget
    QWidget *sec_centralw;
    QLabel *pixmapLabel,*preview;
    QVBoxLayout *vbl;
    QPixmap pixmap, pixmap_preview;

    //bars
    QStatusBar *stBar;
    QMenuBar* mBar;
    QToolBar* tBar;

    // menu file
    QMenu *file_;
    QAction *f_open, *f_close, *f_save_as, *f_save, *clear, *curve, *circle, *polygon, *airbush, *contur;
    QString current_file;

    //dock
    QDockWidget *dock;
    QFrame *dockContents, *dockCircle, *dockPolygon, *dockAIRBUSH, *dockContur;
    QPushButton *colorButton;
    QComboBox *penWidthComboBox;
    QVBoxLayout* mainLayout;
    QColor currentPenColor;
    int currentPenWidth = 1;

    int x = -1, y = -1;
    int prevX = -1, prevY = -1;
    int diameter = 30;
    QBrush br{Qt::blue};
    enum brushes: int
    {
        CURVE,
        CIRCLE,
        POLYGON,
        AIRBRUSH,
        CONTUR
    };

    int brushIndex = 0;
    QPainter* painter;

    QPen pen;

    QSlider *slider, *outlineThicknessSlider, *radiusSlider, *airbrushRadiusSlider;

    QComboBox *lineTypeBox;
    QSpinBox *pixelBox;

    //random
    std::default_random_engine eng;
    std::normal_distribution<double> distr;
    std::function<double()> rdm;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void open_();
    void close_();
    void save_as();
    void save_();
    void select_colors();
    void clear_pixmap();
    void change_width(int);
    void set_curve();
    void set_circle();
    void set_polygon();
    void set_airbush();
    void set_contur();
    void updatePreview();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void make_menu_file();
    void update_label();
    void make_bars();
    void make_centralw();
    void make_dock_widget();
    void draw();

};
#endif // MAINWINDOW_H
