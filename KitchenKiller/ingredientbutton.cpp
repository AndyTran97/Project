#include "ingredientbutton.h"

IngredientButton::IngredientButton(Ingredient ingredient, int size, QWidget *parent)
    : QPushButton(parent),
      ingredient(ingredient),
      selected(false),
      selectable(true)
{
    setFixedSize(size, size);

    QPixmap pixmap = ingredient.getPixmap();
    QIcon icon(pixmap);
    setIcon(icon); // maybe ingredient does not need a pixmap? convert to icon for you?
    setIconSize(QSize(40, 40));
    // setIconSize(this->size());

    connect(this, &QPushButton::clicked, this, [this]() {
        if (selectable) {
            setSelected(!getSelected());
            emit onSelected(getSelected());
        }
    });

    //
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

bool IngredientButton::event(QEvent *event) {
    if (event->type() == QEvent::HoverEnter) {
        QToolTip::showText(cursor().pos(), ingredient.getName(), this);
        return true;
    } else if (event->type() == QEvent::HoverLeave) {
        QToolTip::hideText();
        return true;
    }

    return QPushButton::event(event);
}

Ingredient IngredientButton::getIngredient() const {
    return ingredient;
}

bool IngredientButton::getSelected() const {
    return selected;
}

void IngredientButton::setSelected(bool selected) {
    this->selected = selected;
    if (selected) {
        setStyleSheet("QPushButton {border: 2px solid rgba(0, 255, 0, 0.8);}");
    } else {
        setStyleSheet("QPushButton {};");
    }
}

void IngredientButton::setSelectable(bool value) {
    selectable = value;
}

