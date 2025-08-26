#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "s0Title.h"
#include "s1Pantry.h"
#include "s2Recipe.h"
#include "s3Cooking.h"
#include "s4Complete.h"
#include "s2recipetransition.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QMediaPlayer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief MainWindow Constructor
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    /**
     * @brief restart Restart the window
     */
    void restart();

private:
    Ui::MainWindow *ui=nullptr;
    s0Title *page0=nullptr;
    s1Pantry *page1=nullptr;
    s2Recipe *page2=nullptr;
    s2RecipeTransition *page2Half = nullptr;
    s3Cooking *page3=nullptr;
    s4Complete *page4=nullptr;
//    QMediaPlayer *backgroundMusic;
    QSoundEffect * backgroundMusic;

    /**
     * @brief changeBackgroundMusic Changes the background music
     * @param musicFile Music file path
     */
    void changeBackgroundMusic(const QString &musicFile);


};
#endif // MAINWINDOW_H
