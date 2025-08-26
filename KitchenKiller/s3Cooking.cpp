//#include "simulations.h"
#include "s3Cooking.h"
#include "render.h"
#include "ui_s3Cooking.h"
#include"draganddrop.h"
#include"model.h"
#include <QPainter>
#include <QDebug>

s3Cooking::s3Cooking(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::s3Cooking)
{
    ui->setupUi(this);

    //Andy Tran
    ui->nextBtn->setEnabled(false);
    connect(render->getModel(), &Model::enableFinalized, this, &s3Cooking::enableFinalized);

    // jeffeeeeeeeee
    QTimer::singleShot(0, this, [this]() {
        ui->scrollArea->addWidget(render);//render moved to .h
    });
    connect(render->getModel(), &Model::updateDisplayText, ui->stepLabel, &QLabel::setText);
     connect(ui->nextBtn, &QPushButton::clicked, this, &s3Cooking::nextPage);

    //Andy Tran:
    setAcceptDrops(true);

    xmaps.insert("Tomato",QPixmap(":/sprites/icons/Tomato.png"));
    xmaps.insert("Carrot",QPixmap(":/sprites/icons/Carrot.png"));
}

s3Cooking::~s3Cooking()
{
    delete ui;
}

void s3Cooking::nextPage()
{
    emit goToPage4();
}

void s3Cooking::enableFinalized(){
    ui->nextBtn->setEnabled(true);
}

void s3Cooking::recieveSelectedRecipe(RecipeNew* recipe) {
    render->getModel()->setRecipe(recipe);


    ui->ingredientList->clearWidgets();
    for (const auto& ingredient : recipe->getIngredeints()) {
        DragAndDropLabel *label = new DragAndDropLabel(this);
        label->setProperty("Ingredient", QVariant::fromValue(ingredient));
        label->setText(ingredient.getName());
        label->setStyleSheet("background-color: transparent");
        label->setFixedSize(50, 50);
        if (!ingredient.getPixmap().isNull()) {
            label->setPixmap(ingredient.getPixmap());
            label->setScaledContents(true);
        }
        label->show();

        ui->ingredientList->addWidget(label);
    }
}
