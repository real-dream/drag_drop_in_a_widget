#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    QLabel *label = new QLabel(this);
    QPixmap pixmap("../untitled/images/6.png");
    label->setPixmap(pixmap);
    label->resize(pixmap.size());
    label->move(100,100);
//    label->setAttribute(Qt::WA_DeleteOnClose);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if(!child->inherits("QLabel")) return;
    QPixmap pixmap = *(child->pixmap());
    QByteArray itemdata;
    QDataStream stream(&itemdata,QIODevice::WriteOnly);
    stream<<pixmap;
    stream<<QPoint(event->pos() - child->pos());
    QMimeData *mimedata = new QMimeData;
    mimedata->setData("myimage/png",itemdata);
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());
    QPixmap temppixmap = pixmap;
    QPainter painter;
    painter.begin(&temppixmap);
    painter.fillRect(pixmap.rect(),QColor(127,127,127,127));
    painter.end();
    child->setPixmap(temppixmap);

    if(drag->exec(Qt::CopyAction|Qt::MoveAction,Qt::CopyAction) == Qt::MoveAction)
    {
        child->close();
        qDebug()<<1;
    }

    else
    {
        child->show();
        child->setPixmap(pixmap);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        qDebug()<<2;
//        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        qDebug()<<3;
//        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasFormat("myimage/png"))
    {
        QByteArray itemdata = event->mimeData()->data("myimage/png");
        QDataStream stream(&itemdata,QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint offset;
        stream>>pixmap>>offset;
        QLabel *label = new QLabel(this);
        label->setPixmap(pixmap);
        label->resize(pixmap.size());
        label->move(event->pos() - offset);
        label->show();
        qDebug()<<4;
        event->setDropAction(Qt::MoveAction);
        event->accept();
        qDebug()<<5;
    }else {
        event->ignore();
    }
}
