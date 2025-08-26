#ifndef S1PANTRY_H
#define S1PANTRY_H

#include <QWidget>
#include <QGroupBox>
#include"foodLibrary.h"
#include <Model.h>

namespace Ui {
class s1Pantry;
}

/**
 * @brief The s1Pantry class Represents the pantry page
 */
class s1Pantry : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief s1Pantry Constructor
     * @param parent Parent widget
     */
    explicit s1Pantry(QWidget *parent = nullptr);
    ~s1Pantry();

    /**
     * @brief addIngredient Adds Ingredient to selected
     * @param i
     */
    void addIngredient(Ingredient *i);

    /**
     * @brief removeIngredient Removes ingredients from selected
     * @param i
     */
    void removeIngredient(Ingredient *i);

public slots:

    /**
     * @brief nextPage Goes to recipe page
     */
    void nextPage();

signals:

    /**
     * @brief goToPage2 Goes to recipe page
     */
    void goToPage2();

    /**
     * @brief sendSelectedIngredients Send selected ingredients
     */
    void sendSelectedIngredients(QList<Ingredient*>);

private:
    Ui::s1Pantry *ui=nullptr;

    QList<Ingredient*> selectedIngredients;

};




#endif // S1PANTRY_H
