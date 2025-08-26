#ifndef S0TITLE_H
#define S0TITLE_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>


namespace Ui {
class s0Title;
}

/**
 * @brief The s0Title class Represents the welcome page
 */
class s0Title : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief s0Title Constructor
     * @param parent Parents widget
     */
    explicit s0Title(QWidget *parent = nullptr);
    ~s0Title();

public slots:
    /**
     * @brief nextPage Goes to the next page
     */
    void nextPage();

    /**
     * @brief aboutPage Goes to the about page
     */
    void aboutPage();

signals:

    /**
     * @brief goToPage1 Goes to the pantry page
     */
    void goToPage1();


private:
    Ui::s0Title *ui=nullptr;
};

#endif // S0TITLE_H
