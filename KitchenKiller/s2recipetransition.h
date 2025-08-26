#ifndef S2RECIPETRANSITION_H
#define S2RECIPETRANSITION_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QMovie>

namespace Ui {
class s2RecipeTransition;
}

class s2RecipeTransition : public QWidget
{
    Q_OBJECT

public:
    explicit s2RecipeTransition(QWidget *parent = nullptr);
    ~s2RecipeTransition();

signals:
    void goToPage3();

public slots:
    void nextPage();
    void showLoadingScreen();

private:
    Ui::s2RecipeTransition *ui;
    QTimer *loadingTimer;
    QTimer *progressBarTimer;
    QLabel *gifLabel;
    QLabel *displayText;
    QProgressBar *progressBar;
    QMovie *movie;
    int progressBarValue;
    void updateProgressBar();
    void startTextAnimations();
};

#endif // S2RECIPETRANSITION_H
