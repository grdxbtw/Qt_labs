#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // functional object with random generator
    rdm = std::bind(distr, eng);

    //app
    make_bars();
    make_menu_file();
    make_centralw();
    make_dock_widget();
}

void MainWindow::make_menu_file()
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
    mBar->addMenu(file_);

    clear = new QAction("&Clear");
    curve = new QAction("&Curve");
    circle = new QAction("&Circle");
    polygon = new QAction("&Polygon");
    airbush = new QAction("&Airbush");
    contur = new QAction("&Contur");

    connect(curve,SIGNAL(triggered(bool)),this,SLOT(set_curve()));
    connect(circle,SIGNAL(triggered(bool)),this,SLOT(set_circle()));
    connect(polygon,SIGNAL(triggered(bool)),this,SLOT(set_polygon()));
    connect(airbush,SIGNAL(triggered(bool)),this,SLOT(set_airbush()));
    connect(contur,SIGNAL(triggered(bool)),this,SLOT(set_contur()));

    connect(f_open,SIGNAL(triggered(bool)),this, SLOT(open_()));
    connect(f_save_as,SIGNAL(triggered(bool)),this, SLOT(save_as()));
    connect(f_save,SIGNAL(triggered(bool)),this, SLOT(save_()));
    connect(f_close,SIGNAL(triggered(bool)),this, SLOT(close_()));
    connect(clear,SIGNAL(triggered(bool)),this,SLOT(clear_pixmap()));


    tBar->addAction(f_open);
    tBar->addAction(f_save);
    tBar->addAction(f_close);
    tBar->addSeparator();
    tBar->addAction(clear);
    tBar->addAction(curve);
    tBar->addAction(circle);
    tBar->addAction(polygon);
    tBar->addAction(airbush);
    tBar->addAction(contur);
}

void MainWindow::make_centralw()
{
    sec_centralw = new QWidget(this);
    sec_centralw->setStyleSheet("border: 1px solid grey");

    pixmapLabel = new QLabel;
    pixmapLabel->setFixedSize(500,500);
    pixmapLabel->setAlignment(Qt::AlignCenter);

    pixmap = QPixmap(pixmapLabel->size());
    pixmap.fill(Qt::white);
    pixmapLabel->setPixmap(pixmap.scaled(pixmapLabel->size()));

    QGridLayout *layout = new QGridLayout(sec_centralw);
    layout->addWidget(pixmapLabel,0,3, Qt::AlignCenter);

    this->setCentralWidget(sec_centralw);
    currentPenColor = Qt::black;
    painter = new QPainter(&pixmap);
    pen = QPen(currentPenColor);
    pen.setWidth(currentPenWidth);
    painter->setPen(pen);
}

void MainWindow::make_dock_widget()
{
    dock = new QDockWidget("Tools", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    this->addDockWidget(Qt::RightDockWidgetArea,dock);

    preview = new QLabel;
    preview->setFixedSize(100,100);
    preview->setStyleSheet("border: 1px solid grey");

    pixmap_preview = QPixmap(pixmapLabel->size());
    pixmap_preview.fill(Qt::white);
    preview->setPixmap(pixmap.scaled(preview->size()));

    colorButton = new QPushButton;
    colorButton->setIconSize(QSize(40,40));
    colorButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colorButton->setFlat(true);
    colorButton->setStyleSheet("border: 1px solid grey");
    colorButton->setToolTip("Choose color");
    connect(colorButton, SIGNAL(clicked(bool)),this, SLOT(select_colors()));

    slider = new QSlider(Qt::Horizontal);
    slider->setRange(1,20);
    slider->setSliderPosition(0);
    slider->setTickPosition(QSlider::TicksBothSides);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(change_width(int)));

    dockContents =  new QFrame;
    dockContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout;
    dockContents->setLayout(mainLayout);
    dockContents->layout()->addWidget(preview);
    dockContents->layout()->addWidget(slider);
    dockContents->layout()->addWidget(colorButton);
    dock->setWidget(dockContents);
    updatePreview();
}

void MainWindow::make_bars()
{
    mBar = new QMenuBar(this);
    this->setMenuBar(mBar);
    tBar = new QToolBar(this);
    this->addToolBar(tBar);
    stBar = new QStatusBar(this);
    this->setStatusBar(stBar);
}

void MainWindow::update_label()
{
    pixmapLabel->setPixmap(pixmap.scaled(pixmapLabel->size()));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPointF pos = pixmapLabel->mapFromGlobal(event->globalPosition());

        x = pos.x() * pixmap.width() / pixmapLabel->width();
        y = pos.y() * pixmap.height() / pixmapLabel->height();

        if (brushIndex != CURVE)
        {
            draw();
        }

        prevX = x;
        prevY = y;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF widgetPos = pixmapLabel->mapFromGlobal(event->globalPosition());

        x = widgetPos.x() * pixmap.width() / pixmapLabel->width();
        y = widgetPos.y() * pixmap.height() /pixmapLabel->height();

        if (brushIndex == CURVE || brushIndex == POLYGON)
        {
            draw();
        }

        prevX = x;
        prevY = y;
    }
}

void MainWindow::draw()
{
    switch (brushIndex)
    {
        case CURVE:
            painter->drawLine(prevX, prevY, x, y);
            break;
        case CIRCLE:
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setBrush(br);
            painter->drawEllipse(QPoint(x, y), diameter, diameter);
            break;
        case POLYGON:
        {
            QPolygon hexagon;
            for (int i = 0; i < 6; ++i)
            {
                double angle = 60 * i;
                double x1 = x + radiusSlider->value() * cos(angle * M_PI / 180.0);
                double y1 = y + radiusSlider->value() * sin(angle * M_PI / 180.0);
                hexagon << QPoint(x1, y1);
            }
            painter->setBrush(br);
            painter->drawPolygon(hexagon);
            break;
        }
        case AIRBRUSH:
        {
            int numPoints = pixelBox->value();
            for (int i = 0; i < numPoints; ++i)
            {
                double angle = rdm() * 2 * M_PI;
                double radius = airbrushRadiusSlider->value() * sqrt(rdm());
                double dx = radius * cos(angle);
                double dy = radius * sin(angle);

                painter->drawPoint(x + dx, y + dy);
            }
            break;
        }
        case CONTUR:
            QPainterPath catPath;

            // draw head
            catPath.addEllipse(QPoint(x, y), 50, 50);

            // draw ears
            QPolygonF leftEar;
            leftEar << QPointF(x - 20, y - 30) << QPointF(x - 10, y - 50) << QPointF(x, y - 30);
            catPath.addPolygon(leftEar);

            QPolygonF rightEar;
            rightEar << QPointF(x, y - 30) << QPointF(x + 10, y - 50) << QPointF(x + 20, y - 30);
            catPath.addPolygon(rightEar);

            // draw eyes
            catPath.addEllipse(QPointF(x - 15, y - 10), 5, 5);
            catPath.addEllipse(QPointF(x + 15, y - 10), 5, 5);

            // draw nose
            QPolygonF nose;
            nose << QPointF(x - 2, y - 5) << QPointF(x, y - 2) << QPointF(x + 2, y - 5);
            catPath.addPolygon(nose);

            // draw body
            catPath.addEllipse(QPointF(x, y + 20), 20, 30);

            // draw mustache
            catPath.moveTo(x - 10, y + 10);
            catPath.lineTo(x - 30, y + 15);

            catPath.moveTo(x + 10, y + 10);
            catPath.lineTo(x + 30, y + 15);

            painter->setPen(currentPenColor);

            painter->setBrush(br);
            painter->drawPath(catPath);
    }
        pixmapLabel->setPixmap(pixmap.scaled(pixmapLabel->size()));
}

void MainWindow::select_colors()
{
    if(brushIndex == CIRCLE || brushIndex == POLYGON || brushIndex == CONTUR)
        br = QColorDialog::getColor(Qt::white ,this, "Select Color");

    else
    {
        currentPenColor = QColorDialog::getColor(Qt::white ,this, "Select Color");
        pen.setColor(currentPenColor);
        painter->setPen(pen);
    }
    updatePreview();
}

void MainWindow::clear_pixmap()
{
    pixmap.fill(Qt::white);
    pixmapLabel->setPixmap(pixmap.scaled(pixmapLabel->size()));
}

void MainWindow::change_width(int width)
{
    currentPenWidth = width;
    pen.setWidth(currentPenWidth);
    painter->setPen(pen);
    updatePreview();
}

void MainWindow::set_curve()
{
    brushIndex = CURVE;

    QWidget* w = new QWidget;
    w->setLayout(new QVBoxLayout);
    w->layout()->addWidget(preview);
    w->layout()->addWidget(slider);
    w->layout()->addWidget(colorButton);
    dock->setWidget(w);
    updatePreview();
}

void MainWindow::set_circle()
{
    brushIndex = CIRCLE;
    QComboBox* combo_box = new QComboBox;
    combo_box->addItem("Dash Line");
    combo_box->addItem("Dot Line");
    combo_box->addItem("Solid Line");
    combo_box->addItem("Dash Dot Line");

    connect(combo_box,&QComboBox::currentIndexChanged,[this](int ind){

        switch (ind)
        {
        case 0:
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            break;
        case 1:
            pen.setStyle(Qt::DotLine);
            painter->setPen(pen);
            break;
        case 2:
            pen.setStyle(Qt::SolidLine);
            painter->setPen(pen);
            break;
        case 3:
            pen.setStyle(Qt::DashDotDotLine);
            painter->setPen(pen);
            break;
        default:
            break;
        }
        updatePreview();

    });

    QPushButton* colorB = new QPushButton;
    colorB->setIconSize(QSize(40,40));
    colorB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colorB->setFlat(true);
    colorB->setStyleSheet("border: 1px solid grey");
    colorB->setToolTip("Pen Color");
    connect(colorB, &QPushButton::clicked,[this](){
        currentPenColor = QColorDialog::getColor(Qt::white ,this, "Select Color");
        pen.setColor(currentPenColor);
        painter->setPen(pen);
        updatePreview();
    });

    QSlider *sl = new QSlider(Qt::Horizontal);
    sl->setRange(1,100);
    sl->setSliderPosition(0);
    sl->setTickPosition(QSlider::TicksBothSides);
    connect(sl,&QSlider::valueChanged,[this](int v){
        diameter = v;
        updatePreview();
    });

    QWidget* w = new QWidget;
    w->setLayout(new QVBoxLayout);
    w->layout()->addWidget(preview);
    w->layout()->addWidget(slider);
    w->layout()->addWidget(colorButton);
    w->layout()->addWidget(combo_box);
    w->layout()->addWidget(sl);
    w->layout()->addWidget(colorB);
    dock->setWidget(w);
    updatePreview();
}

void MainWindow::set_polygon()
{
    brushIndex = POLYGON;
    QComboBox* combo_box = new QComboBox;
    combo_box->addItem("Dash Line");
    combo_box->addItem("Dot Line");
    combo_box->addItem("Solid Line");
    combo_box->addItem("Dash Dot Line");

    connect(combo_box,&QComboBox::currentIndexChanged,[this](int ind){

        switch (ind)
        {
        case 0:
            pen.setStyle(Qt::DashLine);
            painter->setPen(pen);
            break;
        case 1:
            pen.setStyle(Qt::DotLine);
            painter->setPen(pen);
            break;
        case 2:
            pen.setStyle(Qt::SolidLine);
            painter->setPen(pen);
            break;
        case 3:
            pen.setStyle(Qt::DashDotDotLine);
            painter->setPen(pen);
            break;
        default:
            break;
        }
        updatePreview();

    });

    QPushButton* colorB = new QPushButton;
    colorB->setIconSize(QSize(40,40));
    colorB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colorB->setFlat(true);
    colorB->setStyleSheet("border: 1px solid grey");
    colorB->setToolTip("Pen Color");
    connect(colorB, &QPushButton::clicked,[this](){
        currentPenColor = QColorDialog::getColor(Qt::white ,this, "Select Color");
        pen.setBrush(currentPenColor);
        painter->setPen(pen);
        updatePreview();
    });

    radiusSlider = new QSlider(Qt::Horizontal);
    radiusSlider->setRange(1,100);
    radiusSlider->setSliderPosition(0);
    radiusSlider->setTickPosition(QSlider::TicksBothSides);

    QWidget* w = new QWidget;
    w->setLayout(new QVBoxLayout);
    w->layout()->addWidget(preview);
    w->layout()->addWidget(slider);
    w->layout()->addWidget(colorButton);
    w->layout()->addWidget(combo_box);
    w->layout()->addWidget(radiusSlider);
    w->layout()->addWidget(colorB);
    dock->setWidget(w);
    updatePreview();
}

void MainWindow::set_airbush()
{
    brushIndex = AIRBRUSH;

    airbrushRadiusSlider = new QSlider(Qt::Horizontal);
    airbrushRadiusSlider->setRange(1,100);
    airbrushRadiusSlider->setSliderPosition(0);
    airbrushRadiusSlider->setTickPosition(QSlider::TicksBothSides);

    pixelBox = new QSpinBox;
    pixelBox->setRange(1, 1000);
    pixelBox->setSingleStep(1);

    QWidget* w = new QWidget;
    w->setLayout(new QVBoxLayout);
    w->layout()->addWidget(preview);
    w->layout()->addWidget(slider);
    w->layout()->addWidget(airbrushRadiusSlider);
    w->layout()->addWidget(pixelBox);
    w->layout()->addWidget(colorButton);
    dock->setWidget(w);
    updatePreview();
}

void MainWindow::set_contur()
{
    brushIndex = CONTUR;
    QPushButton* colorB = new QPushButton;
    colorB->setIconSize(QSize(40,40));
    colorB->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    colorB->setFlat(true);
    colorB->setStyleSheet("border: 1px solid grey");
    colorB->setToolTip("Pen Color");
    connect(colorB, &QPushButton::clicked,[this](){
        currentPenColor = QColorDialog::getColor(Qt::white ,this, "Select Color");
        pen.setBrush(currentPenColor);
        painter->setPen(pen);
        updatePreview();
    });

    QWidget* w = new QWidget;
    w->setLayout(new QVBoxLayout);
    w->layout()->addWidget(preview);
    w->layout()->addWidget(colorB);
    w->layout()->addWidget(colorButton);

    dock->setWidget(w);
    updatePreview();
}

void MainWindow::updatePreview()
{
    QPixmap pixmap_preview(preview->width(), preview->height());
    pixmap_preview.fill(Qt::white);
    QPainter painter_prev(&pixmap_preview);

    switch (brushIndex)
    {
    case CURVE:
    {
        QPen p(currentPenColor);
        p.setWidth(currentPenWidth);
        painter_prev.setPen(p);
        painter_prev.drawLine(0, pixmap_preview.height() / 2, pixmap_preview.width(), pixmap_preview.height() / 2);
        preview->setPixmap(pixmap_preview.scaled(preview->size()));
        break;
    }
    case CIRCLE:
    {
        QPen p(currentPenColor);
        p.setWidth(currentPenWidth);
        painter_prev.setPen(p);
        painter_prev.setBrush(br);
        painter_prev.drawEllipse(pixmap_preview.rect().center(), diameter, diameter);
        preview->setPixmap(pixmap_preview.scaled(preview->size()));
        break;
    }
    case POLYGON:
    {
        QPen p(currentPenColor);
        p.setWidth(currentPenWidth);
        painter_prev.setPen(p);
        painter_prev.setBrush(br);
        QPolygon hexagon;
        for (int i = 0; i < 6; ++i)
        {
            double angle = 60 * i;
            double x = diameter * cos(angle * M_PI / 180.0);
            double y = diameter * sin(angle * M_PI / 180.0);
            hexagon << QPoint(pixmap_preview.rect().center().x() + x, pixmap_preview.rect().center().y() + y);
        }

        painter_prev.drawPolygon(hexagon);
        preview->setPixmap(pixmap_preview.scaled(preview->size()));
        break;
    }
    case AIRBRUSH:
    {
        QPen p(currentPenColor);
        int numPoints = 40;
        for (int i = 0; i < numPoints; ++i)
        {
            double angle = rdm() * 2 * M_PI;
            double radius = airbrushRadiusSlider->value() * sqrt(rdm());
            double dx = radius * cos(angle);
            double dy = radius * sin(angle);

            painter_prev.drawPoint(pixmap_preview.rect().center().x() + dx, pixmap_preview.rect().center().y() + dy);
        }
        preview->setPixmap(pixmap_preview.scaled(preview->size()));
        break;
    }
    case CONTUR:
    {
        int x = pixmap_preview.rect().center().x();
        int y = pixmap_preview.rect().center().y();

        QPainterPath catPath;

        // draw head
        catPath.addEllipse(pixmap_preview.rect().center(), 50, 50);

        // draw ears
        QPolygonF leftEar;
        leftEar << QPointF(x - 20, y - 30) << QPointF(x - 10, y - 50) << QPointF(x, y - 30);
        catPath.addPolygon(leftEar);

        QPolygonF rightEar;
        rightEar << QPointF(x, y - 30) << QPointF(x + 10, y - 50) << QPointF(x + 20, y - 30);
        catPath.addPolygon(rightEar);

        // draw eyes
        catPath.addEllipse(QPointF(x - 15, y - 10), 5, 5);
        catPath.addEllipse(QPointF(x + 15, y - 10), 5, 5);

        // draw nose
        QPolygonF nose;
        nose << QPointF(x - 2, y - 5) << QPointF(x, y - 2) << QPointF(x + 2, y - 5);
        catPath.addPolygon(nose);

        // draw body
        catPath.addEllipse(QPointF(x, y + 20), 20, 30);

        // draw mustache
        catPath.moveTo(x - 10, y + 10);
        catPath.lineTo(x - 30, y + 15);

        catPath.moveTo(x + 10, y + 10);
        catPath.lineTo(x + 30, y + 15);

        painter_prev.setPen(currentPenColor);

        painter_prev.setBrush(br);
        painter_prev.drawPath(catPath);
        preview->setPixmap(pixmap_preview.scaled(preview->size()));
    }
    default:
        break;
    }
    preview->setPixmap(pixmap_preview);
}

void MainWindow::open_()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),QString(), tr("Images (*.png *.jpg *.bmp)"));

    pixmap.load(filename);
    if (pixmap.isNull())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot load %1").arg(QDir::toNativeSeparators(filename)));
        return;
    }

    // TODO: update painter and centarl widget before painting
    f_save->setEnabled(true);
    current_file = filename;
    update_label();
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

    if (pixmap.isNull())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot save %1").arg(QDir::toNativeSeparators(filename)));
        return;
    }
    else
    {
        f_save->setEnabled(true);
        current_file = filename;
        pixmap.save(current_file);
    }
}

void MainWindow::save_()
{
    if (pixmap.isNull() or current_file.isEmpty())
    {
        QMessageBox::warning(this, QGuiApplication::applicationDisplayName(),
                             tr("Cannot save %1").arg(QDir::toNativeSeparators(current_file)));
        return;
    }
    else
        pixmap.save(current_file);
}


MainWindow::~MainWindow()
{
    delete ui;
}
