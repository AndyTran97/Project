
#include "titledisplay.h"
#include <QPaintEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QPainterPath>

titleDisplay::titleDisplay(QWidget *parent) : QWidget(parent), animationProgress(0) {
    setFixedHeight(100);

    animationTimer.setInterval(27);
    connect(&animationTimer, &QTimer::timeout, this, &titleDisplay::updateAnimation);
    animationTimer.start();
}

void titleDisplay::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QString title = "Bad Habits ©️";
    QFont font("Brush Script MT", 18);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(title);

    int xPos = (width() - textWidth) / 2;
    int yPos = height() / 2 + fm.ascent() / 2;

    QColor color;
    color.setHsv(animationProgress % 360, 255, 255);
    painter.setFont(font);

    QPainterPath textPath;
    textPath.addText(xPos, yPos, font, title);

    QColor shadowColor(0, 0, 0, 55);
    painter.setPen(QPen(shadowColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPainterPath shadowPath = textPath.translated(5, 5);
    painter.drawPath(shadowPath);

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(textPath);

    painter.fillPath(textPath, color);
}

void titleDisplay::updateAnimation() {

    animationProgress += 2;
    update();
}

