#include "s0Title.h"
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include "ui_s0Title.h"

s0Title::s0Title(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::s0Title)
{
    setObjectName("s0Title");
    ui->setupUi(this);
    connect(ui->startBtn, &QPushButton::clicked, this , &s0Title::nextPage);
    connect(ui->aboutBtn, &QPushButton::clicked, this , &s0Title::aboutPage);

    QSequentialAnimationGroup *animationGroup = new QSequentialAnimationGroup(ui->title);
    animationGroup->setLoopCount(-1); // Set the loop count to -1 for infinite looping

    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->title, "pos");
    animation1->setDuration(1250);
    animation1->setStartValue(ui->title->pos());
    animation1->setEndValue(QPoint(ui->title->pos().x(), ui->title->pos().y() + 20));
    animation1->setEasingCurve(QEasingCurve::InOutSine);

    QPropertyAnimation *animation2 = new QPropertyAnimation(ui->title, "pos");
    animation2->setDuration(1250);
    animation2->setStartValue(QPoint(ui->title->pos().x(), ui->title->pos().y() + 20));
    animation2->setEndValue(ui->title->pos());
    animation2->setEasingCurve(QEasingCurve::InOutSine);

    animationGroup->addAnimation(animation1);
    animationGroup->addAnimation(animation2);
    animationGroup->start(); // Start the animation

    QPixmap startImage(":/sprites/icons/start.png");
    ui->startBtn->setIcon(startImage);

    QPixmap aboutImage(":/sprites/icons/about.png");
    ui->aboutBtn->setIcon(aboutImage);


//    //Ruini:add title image
//    QPixmap imageTitle(":/sprites/icons/Title.png");
//    QLabel* title = ui->title;
//    title->setPixmap(imageTitle);
//    title->setScaledContents(true); // Scale the image to fit the label
//    title->setGeometry(title->pos().x(), title->pos().y(), title->width(), title->height());

//    //Ruini:add backgound image
//    QPixmap imageBg(":/sprites/icons/Kitchen.PNG");
//    QLabel* background = ui->background;
//    background->setPixmap(imageBg);
//    background->setScaledContents(true); // Scale the image to fit the label
//    background->setGeometry(QRect(0, 0, background->width(), background->height()));
//    background->lower();
}

s0Title::~s0Title()
{
    delete ui;
}

void s0Title::nextPage()
{
    emit goToPage1();
}

void s0Title::aboutPage()
{
    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("About");

    QVBoxLayout *layout = new QVBoxLayout(&aboutDialog);

    QLabel *infoLabel = new QLabel(&aboutDialog);
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    infoLabel->setTextFormat(Qt::RichText); // Enable rich text formatting

    QString authorsText = "<pre><b>Authors:</b> Andy Duong\n"
                          "         Andy Tran\n"
                          "         Jeffrey Le\n"
                          "         TingTing Zhou\n"
                          "         Renee Tong</pre>";

    infoLabel->setText(
        "<h1>Kitchen Killer&trade;</h1>"
        "<p>We started making Kitchen Killer because we wanted to share our love for Asian heritage with everyone, especially those who might not have any cooking experience."
        " We believe that food is a great way to show people the amazing cultures in Asia."
        " With our app, we hope to bring people together by helping them enjoy a tasty meal, learn about different cooking styles, and discover the rich culture behind each dish. We want everyone who uses our app, from beginners to experienced cooks, "
        " to have fun exploring new flavors and understanding the traditions that make Asian food so special.</p>"

        "\n<h2>Disclaimer</h2>"
         "<p>The cooking steps provided on this website are intended to be a general guide only. "
        "While we have taken care to ensure that the information provided is accurate, we make no guarantees as to the accuracy, completeness, or suitability of the information for any particular purpose. "
        "We recommend that you use your own judgment and follow all relevant safety precautions when preparing food. "
        "Additionally, please note that the steps listed here may not be precisely accurate for every recipe, but are generally representative of the cooking process. "
        "As always, we encourage you to experiment and adjust to your personal taste and preference.</p>"

        "<p><b>Version: 1.0</b></p>"
        + authorsText
        );

    layout->addWidget(infoLabel);

    // Set the layout for the dialog
    aboutDialog.setLayout(layout);
    aboutDialog.setFixedSize(450, 350);
    aboutDialog.exec();
}
