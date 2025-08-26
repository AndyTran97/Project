#ifndef MODEL_H
#define MODEL_H

#include "ingredient.h"
#include "recipenew.h"
#include "shape.h"
#include <QObject>
#include <QSoundEffect>

using std::function;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(b2World *world, QObject *parent = nullptr);

    /**
     * @brief Queue functions to run after the world has stepped.
     *
     * Queue functions to run after the world has stepped, creating a lock
     * to ensure the world does not step while objects are being created
     * and destroyed.
     */
    QVector<function<void()>> actionQueue;

    /**
     * @brief Sets the recipe.
     *
     * Sets the recipe and resets the state of the model.
     *
     * @param recipe        the recipe
     */
    void setRecipe(RecipeNew* recipe);

    /**
     * @brief Gets the recipe.
     *
     * Gets the recipe the current model is working with.
     *
     * @return              the recipe
     */
    RecipeNew * getRecipe(){return m_recipe;}

    /**
     * @brief Increments the model to count towards the tasks.
     *
     * Increments the model to count towards the tasks, checking the state of
     * each ingredient and applying it to progression.
     *
     * @param ingredient    the ingredient
     */
    void incrementIngredient(Ingredient* ingredient);

    /**
     * @brief Checks if the tasks assigned are all completed.
     *
     * Checks if the tasks assigned are all completed.
     *
     * @return              true if completed, false otherwise
     */
    bool checkTasks();

    /**
     * @brief Gets the text that needs to be displayed.
     *
     * Gets the text that needs to be displayed by pulling from the recipe
     * and applying string formatting.
     *
     * @return              the text displayed
     */
    QString getDisplayText();

public slots:
    /**
     * @brief Cuts a shape into 4 pieces.
     *
     * If a shape is not already cut, cut the shape into 4 pieces
     * and increment the ingredient that got cut. The passed shape
     * gets deleted.
     *
     * @param shape         the shape
     */
    void cut(Shape* shape);

    /**
     * @brief Apply a boil attribute to the shape.
     *
     * If a shape is not already boiled, increment the ingredient that got
     * boiled and delete the shape.
     *
     * @param shape         the shape
     */
    void boil(Shape* shape);

    /**
     * @brief Apply a fry attribute to the shape.
     *
     * If a shape is not already fried, increment the ingredient that got
     * fried and delete the shape.
     *
     * @param shape         the shape
     */
    void fry(Shape* shape);
//    void mix(Shape*);

private:
    int currentTask;
    map<Ingredient, int> trackerTask;

    RecipeNew *m_recipe=nullptr;
    b2World *m_world=nullptr;
    QSoundEffect *cutSound;
    QSoundEffect *boilSound;
    QSoundEffect *frySound;

signals:
    void updateDisplayText(QString text);
    void enableFinalized();
};

#endif // MODEL_H
