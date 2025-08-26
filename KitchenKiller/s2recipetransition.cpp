#include "s2recipetransition.h"
#include "qtimer.h"
#include "ui_s2recipetransition.h"
#include <QFont>
#include <QColor>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

s2RecipeTransition::s2RecipeTransition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::s2RecipeTransition)
{
    ui->setupUi(this);

    movie = new QMovie(":/sprites/icons/funny.gif");
    gifLabel = new QLabel(this);
    gifLabel->setMovie(movie);
    movie->start();

    startTextAnimations();

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    loadingTimer = new QTimer(this);
    loadingTimer->setSingleShot(true);
    connect(loadingTimer, &QTimer::timeout, this, &s2RecipeTransition::nextPage);

    progressBarTimer = new QTimer(this);
    connect(progressBarTimer, &QTimer::timeout, this, &s2RecipeTransition::updateProgressBar);

    ui->layout->addWidget(gifLabel, 0, Qt::AlignCenter);
}


s2RecipeTransition::~s2RecipeTransition()
{
    delete ui;
}

void s2RecipeTransition::showLoadingScreen()
{
    progressBarValue = 0;
    ui->progressBar->setValue(progressBarValue);
    loadingTimer->start(4000);
    progressBarTimer->start(10); // Start the timer for updating the progress bar
}


void s2RecipeTransition::updateProgressBar()
{

    int elapsedTime = loadingTimer->remainingTime();
    if (elapsedTime > 0) {
        double progress = 100.0 * (1.0 - (static_cast<double>(elapsedTime) / 4000.0));
        progressBarValue = static_cast<int>(progress);
        ui->progressBar->setValue(progressBarValue);
    } else {
        progressBarValue = 100;
        ui->progressBar->setValue(progressBarValue);
        progressBarTimer->stop();
}
}
void s2RecipeTransition::startTextAnimations()
{
    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(ui->prepLab);
    animationGroup->setLoopCount(-1); // Set the loop count to -1 for infinite looping

    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->prepLab, "pos");
    animation1->setDuration(1000);
    animation1->setStartValue(ui->prepLab->pos());
    animation1->setEndValue(QPoint(ui->prepLab->pos().x(), ui->prepLab->pos().y() + 20));
    animation1->setEasingCurve(QEasingCurve::InOutSine);

    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->prepLab, "pos");
    animation2->setDuration(1000);
    animation2->setStartValue(QPoint(ui->prepLab->pos().x(), ui->prepLab->pos().y() + 20));
    animation2->setEndValue(ui->prepLab->pos());
    animation2->setEasingCurve(QEasingCurve::InOutSine);

    animationGroup->addAnimation(animation1);
    animationGroup->addAnimation(animation2);

    connect(animationGroup, &QSequentialAnimationGroup::currentAnimationChanged, [this](QAbstractAnimation *current) {
        if (current == nullptr) {
            return;
        }

        // Change the text when the second animation starts
        if (ui->prepLab->text() == "Preparing Ingredients") {
            ui->prepLab->setText("Preparing Kitchen");
        } else {
            ui->prepLab->setText("Preparing Ingredients");
        }
    });

    animationGroup->start(); // Start the animation
}


void s2RecipeTransition::nextPage(){
    emit goToPage3();
}
