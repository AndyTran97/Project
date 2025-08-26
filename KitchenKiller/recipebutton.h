#ifndef RECIPEBUTTON_H
#define RECIPEBUTTON_H

#include <QPushButton>
#include <QWidget>
#include "recipenew.h"
#include <QPainter>
#include <QMessageBox>
#include <QTextBlockFormat>
#include <QTextCursor>

namespace Ui {
class RecipeButton;
}

class RecipeButton : public QPushButton
{
    Q_OBJECT

public:
    /**
     * @brief RecipeButton Constuctor
     * @param recipe Recipe
     * @param pantry All ingredients in the pantry
     * @param parent Parent widget
     */
    explicit RecipeButton(RecipeNew* recipe, QList<Ingredient*> pantry, QWidget *parent = nullptr);
    ~RecipeButton();

    /**
     * @brief getSelected Getter
     * @return True, if selected; otherwise, false
     */
    bool getSelected() const;

    /**
     * @brief setSelected Setter
     */
    void setSelected(bool);

    /**
     * @brief getRecipe Getter
     * @return The recipe
     */
    RecipeNew* getRecipe() const { return m_recipe; }

    // jeffohh
    /**
     * @brief setTitle Setter
     * @param text Title text
     */
    void setTitle(QString text);

    /**
     * @brief setDescription Setter
     * @param text Destripton text
     */
    void setDescription(QString text);

    /**
     * @brief setLearnMore Setter
     * @param text Learn more text
     */
    void setLearnMore(QString text);

    /**
     * @brief setDifficulty Setter
     * @param difficulty difficulty level
     */
    void setDifficulty(int difficulty);

    /**
     * @brief setPixmap Setter
     * @param pixmap The picture of the recipe
     */
    void setPixmap(QPixmap pixmap);

    /**
     * @brief getMatchingCount Getter
     * @return The number matched recipes
     */
    int getMatchingCount() const{return matchingCount;}

    /**
     * @brief setIngredientList Setter
     * @param ingredients The ingredient list of the recipe
     * @param pantry The ingredient from the pantry
     */
    void setIngredientList(QList<Ingredient> ingredients, QList<Ingredient*> pantry);


protected:
    RecipeButton(QWidget *parent = nullptr);

private:
    Ui::RecipeButton *ui=nullptr;
    RecipeNew* m_recipe=nullptr;
    bool selected;
    int matchingCount;
    QString msgBoxStyle;
    QMessageBox *msgBox;
};

#endif // RECIPEBUTTON_H
