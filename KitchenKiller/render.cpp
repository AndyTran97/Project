#include "render.h"
#include "debugdraw.h"
#include "draganddrop.h"
#include "kitchencontactlistener.h"
#include "qpainter.h"
#include "shape.h"

#include <QMouseEvent>
#include<QApplication>
#include<QLabel>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <future>

using std::future;

class PixmapUserData : public QPixmap {
public:
    PixmapUserData(const QPixmap& pixmap) : QPixmap(pixmap) {}
};

Render::Render(QWidget *parent)
    : QWidget{parent},
      world(b2Vec2(0.0f, 10.0f)),
      model(&world)
{
    setStyleSheet("background-color: grey;");
    setFixedSize(WINDOW_WIDTH+1, WINDOW_HEIGHT+1);
    setAcceptDrops(true);


    {
        KitchenContactListener *contactListen = new KitchenContactListener;
        world.SetContactListener(contactListen);
    }

    {
        DebugDraw *debugDraw = new DebugDraw(this);
        debugDraw->SetFlags(b2Draw::e_shapeBit);
        world.SetDebugDraw(debugDraw);
    }

    {
        b2Vec2 size = b2Vec2(WINDOW_WIDTH/2, GROUND_HEIGHT);
        b2Vec2 position = b2Vec2(size.x, WINDOW_HEIGHT-size.y);
        ground = new Shape(&world, position, size);
        ground->setStatic(true);
    }


    // [== CREATE UTENSILS AND CONNECT THEM ==]
    int const UTENSIL_COUNT = 3;

    {
        b2Vec2 size = b2Vec2(60, 50);
        b2Vec2 position = b2Vec2(WINDOW_WIDTH/(UTENSIL_COUNT+1) * 1, WINDOW_HEIGHT-(GROUND_HEIGHT*2)-size.y);
        position -= b2Vec2(35, 0); // hard coded to space them evenly

        boilingPot = new Shape(&world, position, size);
        boilingPot->setStatic(true);

        connect(boilingPot, &Shape::onContact, &model, &Model::boil);
    }

    {
        b2Vec2 size = b2Vec2(70, 5);
        b2Vec2 position = b2Vec2(WINDOW_WIDTH/(UTENSIL_COUNT+1) * 2, WINDOW_HEIGHT-(GROUND_HEIGHT*2)-size.y);

        cuttingBoard = new Shape(&world, position, size);
        cuttingBoard->setStatic(true);

        connect(cuttingBoard, &Shape::onContact, &model, &Model::cut);
    }

    {
        b2Vec2 size = b2Vec2(60, 15);
        b2Vec2 position = b2Vec2(WINDOW_WIDTH/(UTENSIL_COUNT+1) * 3, WINDOW_HEIGHT-(GROUND_HEIGHT*2)-size.y);
        position += b2Vec2(35, 0); // hard coded to space them evenly

        fryingPan = new Shape(&world, position, size);
        fryingPan->setStatic(true);

        connect(fryingPan, &Shape::onContact, &model, &Model::fry);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Render::renderWorld);
    timer->start(1000 / 60);

}



// [== BOX2D DRAGGING SECTION ==]
void Render::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPointF clickPos = event->pos();
        b2Vec2 targetPos = b2Vec2(clickPos.x(), clickPos.y());

        if (!m_mouseJoints.empty()) {
            mouseReleaseEvent(event);
        }

        // Iterate through all bodies in the world
        for (b2Body* body = world.GetBodyList(); body; body = body->GetNext())
        {
            if (body == ground->getBody()) continue;

            // Calculate the distance between the mouse click and the body position
            b2Vec2 bodyPos = body->GetPosition();
            b2Vec2 clickPosB2(bodyPos.x - clickPos.x(), bodyPos.y - clickPos.y());
            float distance = clickPosB2.Length();

            // Check if the distance is less than the desired radius
            if (distance < 50)
            {
                b2MouseJointDef md;
                md.bodyA = ground->getBody();
                md.bodyB = body;
                md.target = targetPos;
                md.maxForce = 1000.0f * body->GetMass();
                b2MouseJoint* joint = static_cast<b2MouseJoint*>(world.CreateJoint(&md));
                body->SetAwake(true);

                m_mouseJoints.append(joint);
            }
        }
    }
}

void Render::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF clickPos = event->pos();
        b2Vec2 targetPos = b2Vec2(clickPos.x(), clickPos.y());

        for (b2MouseJoint *joint : m_mouseJoints) {
            if (joint) {
                joint->SetTarget(targetPos);
            }
        }
    }
}

void Render::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        for (b2MouseJoint *joint : m_mouseJoints) {
            bool destroyed = (bool) joint->GetUserData();
            if (!destroyed) {
                world.DestroyJoint(joint);
            }
        }
        m_mouseJoints.clear();
    }
}


// [== PAINT SECTION ==]

void Render::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::gray);

    QPixmap background(":/sprites/icons/Kitchen.PNG");
    painter.drawPixmap(rect(), background);

    //painter.drawPixmap(30, 30, currentDrop.getPixmap());

    for (auto& pair : Shape::shapes) {
        Shape* shape = pair.second;
        b2Vec2 position = pair.first->GetPosition();
        //b2Vec2 size =

        if (pair.first->GetType() == b2_dynamicBody) {
            Ingredient* ingredient = static_cast<Ingredient*>(shape->getData());

            //qDebug() <<ingredient->getName();

            // Load image for body type
            QPixmap pixmap = ingredient->getPixmap();
//            QPixmap* userData = static_cast<QPixmap*>(pair.first->GetUserData());
            QPixmap scaledPan = pixmap.scaled(QSize(50, 50), Qt::KeepAspectRatio);
            int xPan = (int)(position.x - scaledPan.width() / 2.0);
            int yPan = (int)(position.y - scaledPan.height() / 2.0);

            // Draw image
            painter.drawPixmap(xPan, yPan, scaledPan);
        }
    }



    //draw the frying pan
    b2Vec2 panPos = fryingPan->getBody()->GetPosition();
    QPixmap panPix(":/sprites/icons/FryPan.png");
    QPixmap scaledPan = panPix.scaled(QSize(200, 200), Qt::KeepAspectRatio);
    int xPan = (int)(panPos.x - scaledPan.width() / 2.0) + 32;//32 is the offset
    int yPan = (int)(panPos.y - scaledPan.height() / 2.0)-5;
    painter.drawPixmap(xPan, yPan, scaledPan);

    //draw the cut board
    b2Vec2 boardPos = cuttingBoard->getBody()->GetPosition();
    QPixmap boardPix(":/sprites/icons/cutboard.png");
    QPixmap scaledBoard = boardPix.scaled(QSize(200, 200), Qt::KeepAspectRatio);
    int xBoard = (int)(boardPos.x - scaledBoard.width() / 2.0)+32;//32 is the offset
    int yBoard = (int)(boardPos.y - scaledBoard.height() / 2.0)-60;
    painter.drawPixmap(xBoard, yBoard, scaledBoard);

    //draw the pot
    b2Vec2 potPos = boilingPot->getBody()->GetPosition();
    QPixmap potPix(":/sprites/icons/pot.png");
    QPixmap scaledPot = potPix.scaled(QSize(200, 200), Qt::KeepAspectRatio);
    int x = (int)(potPos.x - scaledPot.width() / 2.0);//32 is the offset
    int y = (int)(potPos.y - scaledPot.height() / 2.0)-10;
    painter.drawPixmap(x, y, scaledPot);

//    world.DrawDebugData();
//    painter.drawPixmap((int)((panPos.x*13)), (int)(panPos.y*16), panPix);


    painter.end();
}




// [== DRAG AND DROP SECTION ==]
void Render::dropEvent(QDropEvent*event)
{
//    QPointF localPos = this->mapFromGlobal(QCursor::pos());
//    QPointF windowPos = this->mapFromGlobal(QCursor::pos());
//    QPointF screenPos = QCursor::pos();
//    Qt::MouseButton button = Qt::LeftButton;
//    Qt::MouseButtons buttons = Qt::LeftButton;
//    Qt::KeyboardModifiers modifiers = Qt::NoModifier;
//    QMouseEvent *mousePressEvent = new QMouseEvent(QEvent::MouseButtonPress, localPos, windowPos, screenPos, button, buttons, modifiers);
//    QApplication::postEvent(this, mousePressEvent);

    DragAndDropLabel *label = qobject_cast<DragAndDropLabel*>(event->source());
    currentDrop = qvariant_cast<Ingredient>(label->property("Ingredient"));

    QPoint point = event->position().toPoint();

    b2Vec2 position(point.x(), point.y());
    b2Vec2 size(10.0f, 10.0f);

    auto method = [this, position, size]() {
        Shape* shape = new Shape(&world, position, size);
        shape->setStatic(false);
        Ingredient* ingredient = new Ingredient(currentDrop.getName(), {});
        shape->setData(ingredient);

        //store the pix map in the body
        //std::shared_ptr<PixmapUserData> pixmapUserData = std::make_shared<PixmapUserData>(ingredient->getPixmap());
//        QPixmap* userData = new QPixmap(ingredient->getPixmap());
//        shape->getBody()->SetUserData(userData);
        //shapes.push_back(shape);

//        std::shared_ptr<QPixmap> userData(new QPixmap(ingredient->getPixmap()));
//        shape->getBody()->SetUserData(userData.get());
    };

    model.actionQueue.append(method);
}

void Render::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    event->acceptProposedAction();
}

void Render::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

//void Render::dragMoveEvent(QDragMoveEvent *)
//{
//    //event->accept();
//    //event->acceptProposedAction();
//}


// [== WORLD STEP SECTION ==]
bool lock = false;
void Render::renderWorld() {
    if (!lock)
        world.Step(2.0/60.0, 6, 2);

    if (!world.IsLocked() && !lock) {
        lock = true;
        std::vector<future<void>> futures;
        for (auto& method : model.actionQueue) {
            futures.push_back(std::async(std::launch::async, method));
        }
        for (auto& f : futures) {
            f.wait();
        }
        model.actionQueue.clear();
        update();
        lock = false;
    }

}
