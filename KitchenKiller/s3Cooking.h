#ifndef S3COOKING_H
#define S3COOKING_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include<Qlist>
#include<QVector>
#include<QGraphicsView>
#include<QGraphicsScene>
#include"render.h"
#include<random>

namespace Ui {
class s3Cooking;
}

class s3Cooking : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief s3Cooking This class is the step 3 of the program's UI
     * @param parent
     */
    explicit s3Cooking(QWidget *parent = nullptr);
    ~s3Cooking();

public slots:
    /**
     * @brief nextPage A slot to move to s4
     */
    void nextPage();

    /**
     * @brief enableFinalized Called from Model that the steps were all completed and enable the Finalized button
     */
    void enableFinalized();

    /**
     * @brief recieveSelectedIngredients A slot to receive the Recipe were sent from s2
     */
    void recieveSelectedRecipe(RecipeNew* recipe);

signals:
    /**
     * @brief goToPage4 signal to go to s4
     */
    void goToPage4();

private:
    Ui::s3Cooking *ui;
    Render* render = new Render();

    //Ruini
    QMap<QString, QPixmap> xmaps;

};

#endif // S3COOKING_H
