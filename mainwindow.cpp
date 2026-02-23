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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int size_of_window = 800;
    resize(size_of_window, size_of_window);
    setWindowTitle("Подзадачча №0");
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    QGraphicsScene *workspace = new QGraphicsScene(this);
    workspace->setSceneRect(0, 0, size_of_window, size_of_window);

    // Белый фон
    workspace->setBackgroundBrush(Qt::white);

    // Рисуем оси координат (черные, жирные)
    QPen axisPen(Qt::black, 2);
    workspace->addLine(0, size_of_window/2, size_of_window, size_of_window/2, axisPen); // ось X
    workspace->addLine(size_of_window/2, 0, size_of_window/2, size_of_window, axisPen); // ось Y

    // Рисуем стрелочки на осях
    QPen arrowPen(Qt::black, 2);
    // Стрелка для оси X (справа)
    workspace->addLine(size_of_window-10, size_of_window/2-5, size_of_window, size_of_window/2, arrowPen);
    workspace->addLine(size_of_window-10, size_of_window/2+5, size_of_window, size_of_window/2, arrowPen);
    // Стрелка для оси Y (сверху) - еще ниже
    workspace->addLine(size_of_window/2-5, 25, size_of_window/2, 15, arrowPen);
    workspace->addLine(size_of_window/2+5, 25, size_of_window/2, 15, arrowPen);

    // Подписи осей
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

    auto addPointWithLabel = [workspace](int x, int y, QString label, bool isVertical = false) {
        QGraphicsEllipseItem *point = new QGraphicsEllipseItem(x-3, y-3, 6, 6);
        point->setBrush(Qt::black);
        point->setPen(Qt::NoPen);
        workspace->addItem(point);

        QGraphicsTextItem *text = new QGraphicsTextItem(label);

        if (isVertical) {
            text->setPos(x + 5, y - 12);  // Для вертикальной оси (подписи справа)
        } else {
            text->setPos(x - 12, y + 5);  // Для горизонтальной оси (подписи сверху)
        }

        QFont font;
        font.setPointSize(8);
        text->setFont(font);
        text->setDefaultTextColor(Qt::black);
        workspace->addItem(text);
    };

    // Горизонтальная ось (X) - подписи сверху
    for(int i = -(size_of_window/2-50); i <= size_of_window/2-50; i += 50) {
        if (i != 0) {  // Пропускаем 0, чтобы не накладывался на центр
            QString qstr = QString::number(i/25);
            addPointWithLabel(i + size_of_window/2, size_of_window/2, qstr, false);
        }
    }

    // Вертикальная ось (Y) - подписи справа
    for(int i = -(size_of_window/2-50); i <= size_of_window/2-50; i += 50) {
        if (i != 0) {  // Пропускаем 0, чтобы не накладывался на центр
            QString qstr = QString::number(-i/25);  // Минус потому что Y в Qt растет вниз
            addPointWithLabel(size_of_window/2, i + size_of_window/2, qstr, true);
        }
    }

    // Добавляем цифру 0 в центре отдельно
    QGraphicsTextItem *zeroLabel = new QGraphicsTextItem("0");
    zeroLabel->setPos(size_of_window/2 + 5, size_of_window/2 + 5);
    QFont zeroFont;
    zeroFont.setPointSize(9);
    zeroFont.setBold(true);
    zeroLabel->setFont(zeroFont);
    workspace->addItem(zeroLabel);

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
