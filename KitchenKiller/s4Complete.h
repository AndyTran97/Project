#ifndef S4COMPLETE_H
#define S4COMPLETE_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include "recipenew.h"

using namespace std;

namespace Ui {
class s4Complete;
}

class s4Complete : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief The s4Complete class
     * This class is the step 4 of the program's UI
     * @param parent
     */
    explicit s4Complete(QWidget *parent = nullptr);
    ~s4Complete();


public slots:
    void nextPage();
    /**
     * @brief receiveSelectedRecipe This slot is called from s3 which transfer the Recipe to s4
     * @param recipe The recipe that have been selected from s2 and s3
     */
    void receiveSelectedRecipe(RecipeNew *recipe);


signals:
    /**
     * @brief goToPage1
     * Go to Page 1 when clicked Try Again
     */
    void goToPage1();

    /**
     * @brief exitButtonClicked
     * Exit the program when clicked Exit
     */
    void exitButtonClicked();

    /**
     * @brief restartApp
     * Restart the entire app in Model when clicked Try Again
     */
    void restartApp();

private:
    Ui::s4Complete *ui=nullptr;
    RecipeNew *currentRecipe=nullptr;
};

#endif // S4COMPLETE_H





