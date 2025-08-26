#include "s4Complete.h"
#include "ui_s4Complete.h"


s4Complete::s4Complete(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::s4Complete)

{
    ui->setupUi(this);
    connect(ui->tryAgainBtn, &QPushButton::clicked, this, &s4Complete::restartApp);
      connect(ui->ExitBtn, &QPushButton::clicked, QApplication::instance(), &QApplication::quit);
}


s4Complete::~s4Complete()
{
    delete ui;
}

void s4Complete::receiveSelectedRecipe(RecipeNew *recipe)
{
    currentRecipe = recipe;
    ui->titleLab->setText(QString("You have successfully made"));
    ui->nameLab->setText(QString("%1").arg(currentRecipe->getName()));
    // Set the pixmap for the recipePic QLabel
    QPixmap pixmap = currentRecipe->getPixmap();
    ui->recipePic->setPixmap(pixmap);
    ui->recipePic->setScaledContents(true); // This line ensures the pixmap is scaled to fit the QLabel
}


void s4Complete::nextPage()
{
    emit goToPage1();
}


