#ifndef RENDER_H
#define RENDER_H

#include "model.h"
#include <Box2D/Box2D.h>

#include <QObject>
#include <QWidget>

/**
 * @brief The Render class Represents the mouse movement and dropping effect in cooking page.
 */
class Render : public QWidget
{
    Q_OBJECT
public:

    //window width
    static const int WINDOW_WIDTH = 750;
    //window height
    static const int WINDOW_HEIGHT = 600;
    //ground height for physics
    static const int GROUND_HEIGHT = 30;

    /**
     * @brief Render Costructor
     * @param parent Parent widget
     */
    explicit Render(QWidget *parent = nullptr);

    /**
     * @brief getModel Getter
     * @return The model
     */
    Model* getModel() { return &model; }

protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;

    void paintEvent(QPaintEvent* event) override;
    void dropEvent(QDropEvent*) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    //void dragMoveEvent(QDragMoveEvent*) override;

    using QWidget::QWidget;

private:

    /**
     * @brief renderWorld Renders the world
     */
    void renderWorld();

    b2World world;
    QList<b2MouseJoint*> m_mouseJoints;

    Model model;

    Ingredient currentDrop;

    QVector<Shape*> shapes;

    Shape* ground;
    Shape* fryingPan;
    Shape* cuttingBoard;
    Shape* boilingPot;


signals:
    /**
     * @brief mouseClicked mouse is clicked in the cooking area
     */
    void mouseClicked(QPoint);

    /**
     * @brief itemDrop Ingredient is dropped in the cooking area
     */
    void itemDrop(QPoint);

};

#endif // RENDER_H
