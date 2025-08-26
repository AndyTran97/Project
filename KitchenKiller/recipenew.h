#ifndef RECIPENEW_H
#define RECIPENEW_H

#include "ingredient.h"

using std::map;
using std::tuple;

/**
 * @brief The RecipeNew class Represent a recipe
 */
class RecipeNew {
public:

    /**
     * @brief RecipeNew Constructor
     * @param name The name of the recipe
     */
    RecipeNew(QString name);

    // [== RECIPE DEFINITIONS ==]

    /**
     * @brief getName Getter
     * @return The name of the recipe
     */
    QString getName() const { return m_name; }

    /**
     * @brief getDescription Getter
     * @return The description
     */
    QString getDescription() const { return m_description; }

    /**
     * @brief getLearnMore Getter
     * @return learn more
     */
    QString getLearnMore() const { return m_learnMore; }

    /**
     * @brief getDifficulty Getter
     * @return difficulty level
     */
    int getDifficulty() const { return m_difficulty; }

    /**
     * @brief getPixmap Getter
     * @return The picture
     */
    QPixmap getPixmap() const { return m_pixmap; }

    //Setter
    /**
     * @brief setDescription Set the description
     * @param text description text
     */
    void setDescription(QString text) { m_description = text; }

    /**
     * @brief setLearnMore Setter
     * @param text Learn more text
     */
    void setLearnMore(QString text) { m_learnMore = text; }

    /**
     * @brief setDifficulty Setter
     * @param difficulty The difficulty level
     */
    void setDifficulty(int difficulty) { m_difficulty = difficulty; }

    /**
     * @brief setPixmap Setter
     * @param pixmap The picture
     */
    void setPixmap(QPixmap pixmap) { m_pixmap = pixmap; }

    // [== COOKING LOGIC ==]

    /**
     * @brief getIngredeints Getter
     * @return All ingredients
     */
    QList<Ingredient> getIngredeints();

    /**
     * @brief getTasks Getter
     * @param step number the step is at
     * @return A map of ingredient and its amount
     */
    map<Ingredient, int> getTasks(int step) const;

    /**
     * @brief getTasksText Getter
     * @param step number of the step is at
     * @return The text for this step
     */
    map<Ingredient, QString> getTasksText(int step) const;

    /**
     * @brief getDisplayText Getter
     * @param step the number of the step is at
     * @return Get the text of this step
     */
    QString getDisplayText(int step) const { return m_recipeDisplayText[step]; }

    /**
     * @brief isCompleted Checks if a step is completed
     * @param step The step is at
     * @return The number of step we are at currently
     */
    int isCompleted(int step) const { return step == m_recipeTasks.count(); }

    /**
     * @brief addTask Adds tasks to a proecss
     * @param text The task text
     * @param tasks A task
     */
    void addTask(QString text, map<Ingredient, tuple<int, QString>> tasks);

private:
    // [== RECIPE DEFINITIONS ==]
    QString m_name;

    QString m_description;
    QString m_learnMore;
    int m_difficulty;
    QPixmap m_pixmap;

    // [== COOKING LOGIC ==]
    QList<map<Ingredient, tuple<int, QString>>> m_recipeTasks;
    QList<QString> m_recipeDisplayText;

};


#endif // RECIPENEW_H
