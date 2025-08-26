#ifndef INGREDIENTBUTTON_H
#define INGREDIENTBUTTON_H

#include "ingredient.h"
#include <QPushButton>
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QToolTip>
#include <QEvent>

class IngredientButton : public QPushButton
{
    Q_OBJECT
public:

    /**
     * @brief IngredientButton Constructor
     * @param parent parent widget
     */
    IngredientButton(Ingredient, int, QWidget *parent = nullptr);

    /**
     * @brief getIngredient Getter
     * @return The ingredient
     */
    Ingredient getIngredient() const;

    /**
     * @brief getSelected Getter
     * @return true, if the ingredient is selcted; otherwise, false
     */
    bool getSelected() const;

    /**
     * @brief setSelected Setter
     */
    void setSelected(bool);

    /**
     * @brief setSelectable Setter
     */
    void setSelectable(bool);

    /**
     * @brief event Hover events for ingredient buttons
     * @param event The event
     * @return true, if it is hoverred; otherwise, false
     */
    bool event(QEvent *event);

private:
    Ingredient ingredient;
    bool selected;
    bool selectable;

signals:
    /**
     * @brief onSelectedListUpdate Updates the selected ingredient list
     */
    void onSelectedListUpdate(Ingredient);

    /**
     * @brief onSelected The ingredient is selected
     */
    void onSelected(bool);
};

#endif // INGREDIENTBUTTON_H
