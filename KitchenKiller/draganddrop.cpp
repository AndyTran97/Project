#include "draganddrop.h"
#include <QApplication>
#include <QMouseEvent>

DragAndDrop::DragAndDrop(){

}

DragAndDropLabel::DragAndDropLabel(QWidget *widget)
{
    Q_UNUSED(widget);
    setAcceptDrops(true);
}


void DragAndDropLabel::createDrag(const QPoint &pos, QWidget *widget)
{
    if(widget ==nullptr) return;
    QByteArray byteArray(reinterpret_cast<char*>(&widget), sizeof(QWidget*));
    QMimeData *mimedata = new QMimeData();
    mimedata->setData("Label", byteArray);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimedata);

    QPoint globalpos = mapToGlobal(pos);
    QPoint p = widget->mapFromGlobal(globalpos);

    drag->setHotSpot(p);
    drag->setPixmap(widget->grab());
    drag->exec(Qt::MoveAction);


}

void DragAndDropLabel::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "label press";
    createDrag(event->pos(), this);
}

