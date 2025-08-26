#include "recipenew.h"

RecipeNew::RecipeNew(QString name) : m_name(name){}

QList<Ingredient> RecipeNew::getIngredeints() {
    // check filter list from s1 maybe?

    QList<Ingredient> ingredientList;
    for (const auto &tasks : m_recipeTasks) {
        for (const auto& [ingredient, count] : tasks) {
            Ingredient raw(ingredient.getName());
            if (!ingredientList.contains(raw)) {
                ingredientList.append(raw);
            }
        }
    }

    return ingredientList;
}

map<Ingredient, int> RecipeNew::getTasks(int step) const {
    map<Ingredient, int> tasksMap;
    for (const auto& [key, value] : m_recipeTasks[step]) {
        tasksMap[key] = std::get<0>(value);
    }
    return tasksMap;
};

map<Ingredient, QString> RecipeNew::getTasksText(int step) const {
    map<Ingredient, QString> tasksMap;
    for (const auto& [key, value] : m_recipeTasks[step]) {
        tasksMap[key] = std::get<1>(value);
    }
    return tasksMap;
};

void RecipeNew::addTask(QString text, map<Ingredient, tuple<int, QString>> tasks) {
    m_recipeTasks.append(tasks);
    m_recipeDisplayText.append(text);
}
