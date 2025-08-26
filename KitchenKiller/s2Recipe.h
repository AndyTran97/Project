#ifndef S2RECIPE_H
#define S2RECIPE_H

#include <QWidget>
#include "recipenew.h"
#include "foodlibrary.h"
#include <Model.h>

namespace Ui {
class s2Recipe;
}

class s2Recipe : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief s2Recipe This class is the step 2 of the program's UI
     * @param parent
     */
    explicit s2Recipe(QWidget *parent = nullptr);
    ~s2Recipe();

public slots:
    /**
     * @brief nextPage A slot to move to s3
     */
    void nextPage();

    /**
     * @brief recieveSelectedIngredients A slot to receive the list of ingredients were sent from s1
     */
    void recieveSelectedIngredients(QList<Ingredient*>);

signals:
    /**
     * @brief backButtonClicked Signal for Model that the back button was clicked
     */
    void backButtonClicked();

    /**
     * @brief sendSelectedRecipe Signal to send s3 the selected Recipe
     * @param recipe
     */
    void sendSelectedRecipe(RecipeNew* recipe);

    /**
     * @brief goToPage2Half Signal to go to s2 Recipe Transistion
     */
    void goToPage2Half();

private slots:
    /**
     * @brief backButtonClickedSlot Detect when the back button was clicked
     */
    void backButtonClickedSlot();

private:
    Ui::s2Recipe *ui=nullptr;
    FoodLibrary foodLibrary;
    RecipeNew* selectedRecipe = nullptr;

};

#endif // S2RECIPE_H
