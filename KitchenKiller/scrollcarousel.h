/**
 * @brief Scroll Carousel
 *
 * @author Jeffrey Le
 *
 */

#ifndef SCROLLCAROUSEL_H
#define SCROLLCAROUSEL_H

#include"ingredient.h"

#include <QObject>

#include <QScroller>
#include <QScrollArea>
#include <QScrollBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <QTimer>
#include <QScrollerProperties>

/**
 * @brief The ScrollCarousel class Represent Scroll area
 */
class ScrollCarousel : public QScrollArea
{
    Q_OBJECT
public:

    /**
     * @brief ScrollCarousel Constructor
     * @param parent Parents widget
     */
    explicit ScrollCarousel(QWidget *parent = nullptr);

    /**
     * @brief ScrollCarousel Constructor
     * @param parent parent widget
     */
    ScrollCarousel(bool, QWidget *parent = nullptr);

    /**
     * @brief addWidget Adds a widget to scroll area
     */
    void addWidget(QWidget*);

    /**
     * @brief clearWidgets Remove widget from scroll area
     */
    void clearWidgets();

    /**
     * @brief addFilter Applies a Filter Adds a filter
     */
    void addFilter(bool (*)(QWidget*));

    /**
     * @brief removeFilter Removed the filter
     */
    void removeFilter(bool (*)(QWidget*));

    /**
     * @brief sortWidgetsBy Sorts the widget by a given delegate
     */
    void sortWidgetsBy(std::function<bool(QWidget*, QWidget*)>);


protected:

    void wheelEvent(QWheelEvent *event) override;

private:
    /**
     * @brief reset Resets the whole area
     */
    void reset();

    /**
     * @brief rearrangeWidgets Reaaranges the widges in scroll area
     */
    void rearrangeWidgets();


    QBoxLayout *scrollLayout;

    QList<QWidget*> widgets;

    QList<bool (*)(QWidget*)> filterList;

    QPropertyAnimation *horizontalAnimation;
    QPropertyAnimation *verticalAnimation;

};

#endif // SCROLLCAROUSEL_H
