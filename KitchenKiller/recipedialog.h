#ifndef RECIPEDIALOG_H
#define RECIPEDIALOG_H

#include <QDialog>

namespace Ui {
class RecipeDialog;
}

class RecipeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecipeDialog(QWidget *parent = nullptr);
    ~RecipeDialog();

private:
    Ui::RecipeDialog *ui;
};

#endif // RECIPEDIALOG_H
