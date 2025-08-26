#ifndef DRAGANDDROP_H
#define DRAGANDDROP_H

#include<QMouseEvent>
#include<QPoint>
#include<QLabel>
#include<QPushButton>
#include<QGraphicsView>
#include<QByteArray>
#include<QDrag>
#include<QMimeData>
#include<QSize>

/**
 * @brief The dragAndDropEvents class This class is a class for custom QLabel and QGraphicsView for Drag and Drop effect
 */
class DragAndDrop
{
public:
    DragAndDrop();
};

/**
 * @brief The DragAndDropLabel class Custom QLabel class
 */
class DragAndDropLabel : public QLabel{
    Q_OBJECT

public:

    /**
    * @brief DragAndDropLabel Constructor
    * @param widget QWidget
    */
   explicit DragAndDropLabel(QWidget *widget = nullptr);

signals:
//    //ruini test
//    void mouseRelease(QPoint pos);

private:

    /**
     * @brief createDrag Creates a drag event.
     * @param pos Position of the mouse
     * @param widget The widget owns this drag
     */
    void createDrag(const QPoint &pos, QWidget *widget );

protected:

    /**
     * @brief mousePressEvent Mouse event for dragging.
     * @param event QMouseEvent
     */
    void mousePressEvent(QMouseEvent *event) override;

    using QLabel::QLabel;


};

#endif // DRAGANDDROP_H
