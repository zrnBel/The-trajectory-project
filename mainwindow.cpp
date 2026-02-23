#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QMessageBox>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int size_of_window = 800;
    resize(size_of_window, size_of_window);
    setWindowTitle("Подзадача №0");
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    //Создаем рабочую область
    QGraphicsScene *workspace = new QGraphicsScene(this);
    workspace->setSceneRect(0, 0, size_of_window, size_of_window);
    workspace->setBackgroundBrush(Qt::white);

    // Рисуем оси координат
    QPen axisPen(Qt::black, 2);
    workspace->addLine(0, size_of_window/2, size_of_window, size_of_window/2, axisPen); // ось X
    workspace->addLine(size_of_window/2, 0, size_of_window/2, size_of_window, axisPen); // ось Y

    //Рисуем стрелочки на осях координат
    QPen arrowPen(Qt::black, 2);
    workspace->addLine(size_of_window-10, size_of_window/2-5, size_of_window, size_of_window/2, arrowPen);
    workspace->addLine(size_of_window-10, size_of_window/2+5, size_of_window, size_of_window/2, arrowPen);
    workspace->addLine(size_of_window/2-5, 25, size_of_window/2, 15, arrowPen);
    workspace->addLine(size_of_window/2+5, 25, size_of_window/2, 15, arrowPen);

    // Подписываем оси
    QFont axisLabelFont;
    axisLabelFont.setPointSize(12);
    axisLabelFont.setBold(true);
    QGraphicsTextItem *xLabel = new QGraphicsTextItem("X");
    xLabel->setPos(size_of_window-20, size_of_window/2-25);
    xLabel->setFont(axisLabelFont);
    workspace->addItem(xLabel);
    QGraphicsTextItem *yLabel = new QGraphicsTextItem("Y");
    yLabel->setPos(size_of_window/2+10, 25);
    yLabel->setFont(axisLabelFont);
    workspace->addItem(yLabel);

    //Функция, рисующая точки
    auto addPointWithLabel = [workspace](int x, int y, QString label, bool isVertical = false) {
        QGraphicsEllipseItem *point = new QGraphicsEllipseItem(x-3, y-3, 6, 6);
        point->setBrush(Qt::black);
        point->setPen(Qt::NoPen);
        workspace->addItem(point);

        QGraphicsTextItem *text = new QGraphicsTextItem(label);

        if (isVertical) {
            text->setPos(x + 5, y - 12);
        } else {
            text->setPos(x - 12, y + 5);
        }

        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        text->setFont(font);
        text->setDefaultTextColor(Qt::black);
        workspace->addItem(text);
    };

    // Рисуем точки на осях
    int scale = 25;
    for(int i = -(size_of_window/2-50); i <= size_of_window/2-50; i += 2*scale) {
        if (i != 0) {
            QString qstr = QString::number(i/scale);
            addPointWithLabel(i + size_of_window/2, size_of_window/2, qstr, false);
        }
    }
    for(int i = -(size_of_window/2-50); i <= size_of_window/2-50; i += 2*scale) {
        if (i != 0) {
            QString qstr = QString::number(-i/scale);
            addPointWithLabel(size_of_window/2, i + size_of_window/2, qstr, true);
        }
    }
    //Добавляем 0
    QGraphicsTextItem *zeroLabel = new QGraphicsTextItem("0");
    zeroLabel->setPos(size_of_window/2 + 5, size_of_window/2 + 5);
    QFont zeroFont;
    zeroFont.setPointSize(10);
    zeroFont.setBold(true);
    zeroLabel->setFont(zeroFont);
    workspace->addItem(zeroLabel);

    // Создаем точки
    struct PointData {
        float x;
        float y;
        QString name;
    };

    QVector<PointData> points;
    points.append({9, 0, "A"});
    points.append({-7, 5, "B"});
    points.append({-4, 1, "C"});

    if (points.size() > 0) {
        QVector<QPointF> screenPoints;
        for (const PointData &point : points) {
            //Рисование точек
            int screenX = size_of_window/2 + point.x * scale;
            int screenY = size_of_window/2 - point.y * scale;
            screenPoints.append(QPointF(screenX, screenY));
            addPointWithLabel(screenX, screenY, point.name);
        }

        QPen linePen(Qt::black, 2);

        //Рисуем прямые
        for (int i = 0; i < screenPoints.size() - 1; i++) {
            QPointF p1 = screenPoints[i];
            QPointF p2 = screenPoints[i + 1];
            workspace->addLine(p1.x(), p1.y(), p2.x(), p2.y(), linePen);
        }
        if (screenPoints.size() > 2) {
            QPointF first = screenPoints.first();
            QPointF last = screenPoints.last();
            workspace->addLine(last.x(), last.y(), first.x(), first.y(), linePen);
        }
    }

    //Улучшение внешнего вида
    QGraphicsView *view = new QGraphicsView(workspace, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(view);
    view->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}
