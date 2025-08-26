#ifndef TITLEDISPLAY_H
#define TITLEDISPLAY_H

#include <QTimer>
#include <QWidget>

class titleDisplay : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief titleDisplay This class is for displaying the title and its animation
     * @param parent
     */
    explicit titleDisplay(QWidget *parent = nullptr);


protected:
    /**
     * @brief paintEvent The event to draw the animation
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief animationTimer
     * a QTimer for FPS
     */
    QTimer animationTimer;

    /**
     * @brief animationProgress
     * a number to keep track of the progress which displayed in progress bar
     */
    int animationProgress;

private slots:
    /**
     * @brief updateAnimation
     * Update the animation in a time interval
     */
    void updateAnimation();

};

#endif // TITLEDISPLAY_H
