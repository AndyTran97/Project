
#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QString>
#include <QPixmap>
#include <QString>
#include<QVector>
#include <Box2D/Box2D.h>
#include <QObject>

using std::is_permutation;

/**
 * @brief The ActionsPerformed enum Represents cooking actions
 */
enum ActionsPerformed { CUT, BOIL, MIX, FRY};

/**
 * @brief The FoodCategory enum Represents food category
 */
enum FoodCategory { STAPLE, MEAT, VEGETABLE, CONDIMENT };

/**
 * @brief The FoodTags enum Represents Food tags for allergy
 */
enum FoodTags { NUT, VEGAN, Dairy, SUGAR , Gluten};

class Ingredient
{
public:

    /**
     * @brief Ingredient Constructor
     */
    Ingredient();

    /**
     * @brief Ingredient Constructor
     * @param name The name of the ingredient
     */
    Ingredient(const QString& name);

    /**
     * @brief Ingredient Constructor
     * @param name The name of the ingredient
     * @param actions The cooking actions performed on the ingredient
     */
    Ingredient( const QString& name,  QList<ActionsPerformed> actions);

    /**
     * @brief Ingredient Constructor
     * @param name The name of the ingredient
     * @param cate The food catrgory the ingredient has
     * @param tags The allergy tags this ingredient has
     * @param actions The actions performed on this ingredient
     */
    Ingredient(QString name, FoodCategory cate, QVector<FoodTags> tags,
               QList<ActionsPerformed> actions);

    // Getter methods

    /**
     * @brief getName Getter
     * @return The name
     */
    QString getName() const{return name; }

    /**
     * @brief getPixmap Getter
     * @return A pixmap of the ingredient
     */
    QPixmap getPixmap() const{return pixmap;}

    /**
     * @brief getCate Getter
     * @return The ingredient's category
     */
    FoodCategory getCate() const{return cate;}

    /**
     * @brief getTags Getter
     * @return The food tags of the ingredient
     */
    QVector<FoodTags> getTags() const{return tags;}

    /**
     * @brief getActions Getter
     * @return The actions performed on the ingredient
     */
    QList<ActionsPerformed> getActions() const{return actions;}

    //Setters

    /**
     * @brief setPixmap Setter
     */
    void setPixmap(){
        QString path = ":/sprites/icons//";
        path += name;
        path+= ".png";
        QPixmap pix (path);
        pixmap = pix;
    }

    /**
     * @brief setPiece reset pixmap to piece image
     */
    void setPiece(){
        QString path = ":/sprites/icons//";
        path += name;
        path += "_piece.png";
        QPixmap pix (path);
        pixmap = pix;
    }

    /**
     * @brief setCate Setter
     * @param newCate New food category
     */
    void setCate(FoodCategory newCate){cate = newCate;}

    /**
     * @brief setTags Setter
     * @param newTags Food tags
     */
    void setTags(const QVector<FoodTags> &newTags){tags= newTags;}

    /**
     * @brief setActions Setter
     * @param newActions Actions performed
     */
    void setActions(const QList<ActionsPerformed> &newActions){actions = newActions;}

    QList<ActionsPerformed> actions;

    /**
     * @brief operator == overload operator
     * @param other Other ingredient
     * @return true if they have same name and same number of actions; otherwise, false
     */
    bool operator==(const Ingredient& other) const;

    /**
     * @brief operator < overload operator
     * @param other Other ingredient
     * @return True this ingredient has the same name and has less actions; otherwise, false
     */
    bool operator<(const Ingredient& other) const;


private:
    QString name;
    FoodCategory cate;
    QVector<FoodTags> tags;
    QPixmap pixmap;

};

#endif // INGREDIENT_H
