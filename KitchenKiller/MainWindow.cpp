#include "MainWindow.h"
#include "s1Pantry.h"
#include "s2Recipe.h"
#include "s2recipetransition.h"
#include "ui_MainWindow.h"
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioDevice>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->widget->show();
    setFixedSize(1000, 700);


    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    // setWindowFlag(Qt::FramelessWindowHint);

    statusBar()->hide();

    //create stack element
    QStackedWidget *stackedWidget = new QStackedWidget;
    setCentralWidget(stackedWidget);

    stackedWidget->raise();

    //add all the pages
    page0 = new s0Title(this);
    page1 = new s1Pantry(this);
    page2 = new s2Recipe(this);
    page3 = new s3Cooking(this);
    page4 = new s4Complete(this);
    page2Half = new s2RecipeTransition(this);

    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    stackedWidget->addWidget(page0);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);
    stackedWidget->addWidget(page2Half);
    stackedWidget->addWidget(page3);
    stackedWidget->addWidget(page4);
    stackedWidget->setCurrentWidget(page0);
    backgroundMusic = new QSoundEffect(this);
//    backgroundMusic->setSource(QUrl("qrc:/sprites/icons/s0Music.wav"));
//    backgroundMusic -> play();
    changeBackgroundMusic(":/sprites/icons/s0Music.wav");

    //connect to next page button
    connect(page0, &s0Title::goToPage1, this, [=]() {
        stackedWidget->setCurrentWidget(page1);
    });
    connect(page1, &s1Pantry::goToPage2, this, [=]() {
        stackedWidget->setCurrentWidget(page2);
    });


    connect(page2, &s2Recipe::goToPage2Half, this, [=]() {
        stackedWidget->setCurrentWidget(page2Half);
        page2Half->showLoadingScreen();
    });

    connect(page2Half, &s2RecipeTransition::goToPage3, this, [=]() {
        stackedWidget->setCurrentWidget(page3);
        changeBackgroundMusic("qrc:/sprites/icons/s3Music.wav");
    });

    connect(page3, &s3Cooking::goToPage4, this, [=]() {
        stackedWidget->setCurrentWidget(page4);
        changeBackgroundMusic("qrc:/sprites/icons/s4Music.wav");
    });
    connect(page4, &s4Complete::goToPage1, this, [=]() {
        stackedWidget->setCurrentWidget(page0);

    });
    connect(page2, &s2Recipe::backButtonClicked, this, [=]() {
        stackedWidget->setCurrentWidget(page1);
    });

//    connect(backgroundMusic, &QMediaPlayer::errorOccurred, this, [this](QMediaPlayer::Error error) {
//        qDebug() << "Media error occurred:" << error << backgroundMusic->errorString();
//    });


    connect(page1, &s1Pantry::sendSelectedIngredients, page2, &s2Recipe::recieveSelectedIngredients);
    connect(page2, &s2Recipe::sendSelectedRecipe, page3, &s3Cooking::recieveSelectedRecipe);
    connect(page2, &s2Recipe::sendSelectedRecipe, page4, &s4Complete::receiveSelectedRecipe);
    connect(page4, &s4Complete::restartApp, this, &MainWindow::restart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::restart()
{
    // Create a new MainWindow instance
    MainWindow *newMainWindow = new MainWindow();

    // Show the new MainWindow instance
    newMainWindow->show();

    // Close the current MainWindow instance
    this->close();
}

void MainWindow::changeBackgroundMusic(const QString &musicFile)
{

    backgroundMusic->stop();
    backgroundMusic->setSource(QUrl(musicFile));
    backgroundMusic->play();
}
