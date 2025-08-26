#ifndef FOODLIBRARY_H
#define FOODLIBRARY_H

#include "ingredient.h"
#include "recipenew.h"
#include<vector>
using namespace std;

///
/// \brief Represents a food library with ingredients and recipes.
///
class FoodLibrary
{
public:

    /**
     * @brief FoodLibrary Constructor
     */
    FoodLibrary();

    /**
     * @brief getAllIngredients Getter
     * @return  A collection of ingredients
     */
    vector<Ingredient*> getAllIngredients()const {return allIngredients;}

    /**
     * @brief getAllRecipesNew Getter
     * @return All recipes
     */
    QList<RecipeNew*> getAllRecipesNew() const {return m_recipes; }

    /**
     * @brief getIngredientByName Getter
     * @param name The name of the ingredient
     * @return An ingredient
     */
    Ingredient* getIngredientByName(const QString& name) const;

    /**
     * @brief getRecipeByName Getter
     * @param recipeName The name of a recipe
     * @return A recipe
     */
    RecipeNew* getRecipeByName(const QString& recipeName) const;

private:
    vector<Ingredient*> allIngredients;
    QList<RecipeNew*> m_recipes;

    /**
     * @brief initialRecipes Initialize the recipes
     */
    void initialRecipes();

};

#endif // FOODLIBRARY_H
