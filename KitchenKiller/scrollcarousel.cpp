#include "scrollcarousel.h"
#include"ingredient.h"

ScrollCarousel::ScrollCarousel(QWidget *parent) : QScrollArea(parent) {
    setStyleSheet(
                "QScrollArea > QWidget > QWidget {background-color: transparent;}"
                //"ScrollCarousel {background-color: grey;}"
                  );

    setWidgetResizable(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // define QScroller
//    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    QScroller *scroller = QScroller::scroller(this);

    QScrollerProperties scrollerProperties = scroller->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DecelerationFactor, 0.75);
    scrollerProperties.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0.1);
    scrollerProperties.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);

    scroller->setScrollerProperties(scrollerProperties);

    // define scroll animations
    horizontalAnimation = new QPropertyAnimation(horizontalScrollBar(), "value", this);
    horizontalAnimation->setEasingCurve(QEasingCurve::OutQuad);
    horizontalAnimation->setDuration(500);

    verticalAnimation = new QPropertyAnimation(verticalScrollBar(), "value", this);
    verticalAnimation->setEasingCurve(QEasingCurve::OutQuad);
    verticalAnimation->setDuration(500);

    // define scroll layout
    QTimer::singleShot(0, this, [this]() {
        bool isHorizontal = property("isHorizontal").value<bool>();
        int spacing = property("spacing").isValid() ? property("spacing").value<int>() : 25;

        verticalScrollBar()->setEnabled(isHorizontal);
        horizontalScrollBar()->setEnabled(isHorizontal);

        QWidget *widgetContents = new QWidget(this);
        scrollLayout =  isHorizontal ? static_cast<QBoxLayout*>(new QHBoxLayout(widgetContents)) :
                                         static_cast<QBoxLayout*>(new QVBoxLayout(widgetContents));

        scrollLayout->setSpacing(spacing);
        scrollLayout->setContentsMargins(isHorizontal * 25, !isHorizontal * 25, isHorizontal * 25, !isHorizontal * 25);
        if (isHorizontal) {
            scrollLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        } else {
            scrollLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        }

        widgetContents->setLayout(scrollLayout);
        setWidget(widgetContents);
    });
}

ScrollCarousel::ScrollCarousel(bool isHorizontal, QWidget *parent) : ScrollCarousel(parent) {
    setProperty("isHorizontal", isHorizontal);
}

void ScrollCarousel::addWidget(QWidget *widget) {
    widgets.append(widget);

    scrollLayout->addWidget(widget);
}

void ScrollCarousel::clearWidgets() {
    for (QWidget *widget : widgets) {
        delete widget;
    }
    widgets.clear();
}


void ScrollCarousel::addFilter(bool (*filter)(QWidget*)) {
    // append the filter to filter list
    filterList.append(filter);

    // loop through all elements
    // hide them if they apply to the filter
    for (QWidget *widget : widgets) {
        if (!filter(widget)) {
            widget->hide();
        }
    }
}

void ScrollCarousel::removeFilter(bool (*filter)(QWidget*)) {
    // remove filter from list
    filterList.removeAll(filter);

    // show all widgets
    reset();

    // reapply existing filters to all widgets
    for (QWidget *widget : widgets) {
        for (bool (*filter)(QWidget*) : filterList) {
            if (!filter(widget)) {
                widget->hide();
            }
        }
    }
}

void ScrollCarousel::reset() {
    // this for loop may need full reset to retain order? idk, figure out later @jeffohh
    for (QWidget *widget : widgets) {
        widget->show();
        scrollLayout->addWidget(widget);
    }
}



void ScrollCarousel::wheelEvent(QWheelEvent *event) {
    // Get current scroll target
    int horizontalOld = horizontalAnimation->endValue().value<int>();
    int verticalOld = verticalAnimation->endValue().value<int>();

    // Adjust scroll target's location
    int horizontalNew = horizontalOld - event->angleDelta().y();
    int verticalNew = verticalOld - event->angleDelta().y();

    // clamp scroll target
    horizontalNew = std::max(0, std::min(horizontalNew, horizontalScrollBar()->maximum()));
    verticalNew = std::max(0, std::min(verticalNew, verticalScrollBar()->maximum()));

    // animate :sunglasses_emoji:
    if (horizontalOld != horizontalNew) {
        horizontalAnimation->stop();
        horizontalAnimation->setStartValue(horizontalScrollBar()->value());
        horizontalAnimation->setEndValue(horizontalNew);
        horizontalAnimation->start();
    }

    if (verticalOld != verticalNew) {
        verticalAnimation->stop();
        verticalAnimation->setStartValue(verticalScrollBar()->value());
        verticalAnimation->setEndValue(verticalNew);
        verticalAnimation->start();
    }
}

//This function is used to rearrange widgets.
void ScrollCarousel::rearrangeWidgets(){
    for (QWidget *widget : widgets) {
        scrollLayout->removeWidget(widget);
    }
    for (QWidget *widget : widgets) {
        scrollLayout->addWidget(widget);
    }
}

//This function act as a logic for rearrangewidget.
void ScrollCarousel::sortWidgetsBy(std::function<bool(QWidget*, QWidget*)> compare) {
    std::sort(widgets.begin(), widgets.end(), compare);
    rearrangeWidgets();
}



