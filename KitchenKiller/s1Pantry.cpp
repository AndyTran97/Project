#include "s1Pantry.h"
#include "ingredientbutton.h"
#include "ui_s1Pantry.h"
#include "ingredient.h"
#include <QGraphicsDropShadowEffect>

s1Pantry::s1Pantry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::s1Pantry)
{
    ui->setupUi(this);
    connect(ui->nextBtn, &QPushButton::clicked, this, &s1Pantry::nextPage);

    // initialize Pantry
    class FoodLibrary f;

    QList<ScrollCarousel*> pantryList;

    for (int i = STAPLE; i <= CONDIMENT; i++) {
        FoodCategory food = static_cast<FoodCategory>(i);

        QGroupBox *box = new QGroupBox();
        QFont font("Tahoma", 10, QFont::Bold);
        box->setFont(font);
        box->setStyleSheet("QGroupBox {border: 0px;};");
        box->setFixedSize(675, 150);
        //box->setStyleSheet("QGroupBox {background-color: white};");
        QHBoxLayout *layout = new QHBoxLayout(box);

        switch (food) {
        case STAPLE:
            box->setTitle("Staple");
            break;
        case MEAT:
            box->setTitle("Meat");
            break;
        case VEGETABLE:
            box->setTitle("Vegetable");
            break;
        case CONDIMENT:
            box->setTitle("Condiment");
            break;
        default:
            break;
        }

        ScrollCarousel *pantry = new ScrollCarousel(true);

        pantry->setStyleSheet("ScrollCarousel > QWidget > QWidget {background-color: transparent};");
        pantry->setFixedSize(650, 100);
        layout->addWidget(pantry);
//        pantry->setStyleSheet("background-image: url(:/sprites/icons/Kitchen.PNG);");
//        ui->scrollAreaWidgetContents_2->setStyleSheet("background-image: url(:/sprites/icons/Kitchen.PNG);");

        pantryList.append(pantry);

        QTimer::singleShot(0, this, [this, box, f, pantry, food] {

            for (Ingredient *ingredient : f.getAllIngredients()) {
                if (ingredient->getCate() == food) {

                    IngredientButton *button = new IngredientButton(*ingredient, 75);
                    connect(button, &IngredientButton::onSelected, this, [this, ingredient](bool selected) {
                        if (selected) {
                            addIngredient(ingredient);
                        } else {
                            removeIngredient(ingredient);
                        }
                    });
                    pantry->addWidget(button);

                }
            }

            ui->pantryCategory->addWidget(box);

        });
    }


    // create filters
    auto is_not_peanut = [](QWidget* widget) {
        IngredientButton *button = dynamic_cast<IngredientButton*>(widget);
        if (button) {
            Ingredient ingredient = button->getIngredient();
            return !ingredient.getTags().contains(NUT);
        }

        return true;
    };

    auto is_vegan = [](QWidget* widget) {
        IngredientButton *button = dynamic_cast<IngredientButton*>(widget);
        if (button) {
            Ingredient ingredient = button->getIngredient();
            return ingredient.getTags().contains(VEGAN);
        }

        return true;
    };


    // connect checkboxes to filters
    connect(ui->allergyBox, &QCheckBox::stateChanged, this, [this, pantryList, is_not_peanut]() {
        bool checked = ui->allergyBox->isChecked();
        if (checked) {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->addFilter(is_not_peanut);
            }
        } else {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->removeFilter(is_not_peanut);
            }
        }
    });

    connect(ui->veganBox, &QCheckBox::stateChanged, this, [this, pantryList, is_vegan]() {
        bool checked = ui->veganBox->isChecked();
        if (checked) {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->addFilter(is_vegan);
            }
        } else {
            for (ScrollCarousel *pantry : pantryList) {
                pantry->removeFilter(is_vegan);
            }
        }
    });


}

s1Pantry::~s1Pantry()
{
    delete ui;
}

void s1Pantry::addIngredient(Ingredient *i) {
    selectedIngredients.append(i);
}

void s1Pantry::removeIngredient(Ingredient *i) {
    selectedIngredients.removeAll(i);
}

void s1Pantry::nextPage()
{
    emit goToPage2();
    emit sendSelectedIngredients(selectedIngredients);

}
