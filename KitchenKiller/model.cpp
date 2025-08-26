#include "model.h"
#include "qdebug.h"

Model::Model(b2World *world, QObject *parent)
    : QObject{parent},
      m_world(world)
{
    cutSound = new QSoundEffect(this);
    cutSound->setSource(QUrl(":/sprites/icons/chopping.wav"));

    boilSound = new QSoundEffect(this);
    boilSound->setSource(QUrl(":/sprites/icons/boiling.wav"));

    frySound = new QSoundEffect(this);
    frySound->setSource(QUrl(":/sprites/icons/sizzling.wav"));

}


void Model::setRecipe(RecipeNew* recipe) {
    // reset tasks
    currentTask = 0;
    trackerTask.clear();

    m_recipe = recipe;
    emit updateDisplayText(getDisplayText());
}


void Model::incrementIngredient(Ingredient* ingredient) {
    if (m_recipe->isCompleted(currentTask)) return;

    map<Ingredient, int> tasks = m_recipe->getTasks(currentTask);

    // if the ingredient is part of current "tasks"
    if (tasks.find(*ingredient) != tasks.end()) {
        // if the ingredient doesn't exist, create a tracker for it
        if (trackerTask.find(*ingredient) == trackerTask.end())
            trackerTask[*ingredient] = 0;

        trackerTask[*ingredient]++;
    }

    if (checkTasks()) {
        currentTask++;

        if (m_recipe->isCompleted(currentTask))
            qDebug() << "WINNER!!!";

        // emit win signal
        // win will clean up model
    }

    emit updateDisplayText(getDisplayText());
}

bool Model::checkTasks() {
    map<Ingredient, int> tasks = m_recipe->getTasks(currentTask);

    for (const auto& [ingredient, count] : tasks) {
        if (trackerTask.find(ingredient) != tasks.end()) {
            if (trackerTask[ingredient] < count) {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

QString Model::getDisplayText() {
    if (m_recipe->isCompleted(currentTask)){
        emit enableFinalized();
        return "Completed!";
    }
    QString displayText = "Step " + QString::number(currentTask + 1);
    displayText += "\n" + m_recipe->getDisplayText(currentTask) + "\n";

    map<Ingredient, int> tasks = m_recipe->getTasks(currentTask);
    map<Ingredient, QString> tasksText = m_recipe->getTasksText(currentTask);

    // get step instructions and add format to them
    for (const auto& [ingredient, count] : tasks) {
        QString text = tasksText[ingredient] + " (%2/%1)";
        text = text.arg(count);

        int progress = 0;
        if (trackerTask.find(ingredient) != tasks.end()) {
            progress = trackerTask[ingredient];
        }
        text = text.arg(progress);

        displayText += "\n- " + text;
    }

    return displayText;
}



// [== COOKING EVENTS ==]
void Model::cut(Shape *shape) {

    Ingredient *ingredient = static_cast<Ingredient*>(shape->getData());

    if (!ingredient) return;
    if (ingredient->actions.contains(CUT)) return;

    ingredient->actions.append(CUT);
    incrementIngredient(ingredient);

    cutSound->play();

    // check if cuttable?
    auto method = [this, shape, ingredient]() {

        b2Vec2 position = shape->getBody()->GetPosition();
        b2Vec2 size = shape->getSize();
        size *= 0.5;

        delete shape;


        for (int i = 1; i <= 4; i++) {


            Ingredient *newIngredient = new Ingredient(ingredient->getName(), ingredient->actions);

            newIngredient->setPiece();

            float x = rand() / (RAND_MAX + 1.);
            x = (x - 0.5) * 2;
            float y = rand() / (RAND_MAX + 1.);
            y = (y - 1) * 2;

            b2Vec2 variance(x, y);
            variance *= 15;

            qDebug() << m_world->IsLocked();

            Shape *copy = nullptr;
            copy = new Shape(m_world, position + variance, size);
            copy->getBody()->SetLinearVelocity(variance);
            copy->setData(newIngredient);
            copy->setStatic(false);
        }

    };

    actionQueue.append(method);
}

void Model::boil(Shape *shape) {

    Ingredient *ingredient = static_cast<Ingredient*>(shape->getData());
       boilSound->play();

    if (!ingredient) return;
    if (ingredient->actions.contains(BOIL)) return;

    ingredient->actions.append(BOIL);
    incrementIngredient(ingredient);

    if (ingredient->actions.contains(BOIL)) {
        auto method = [shape]() {

            delete shape;
        };

        actionQueue.append(method);
    }
}

void Model::fry(Shape *shape) {

    Ingredient *ingredient = static_cast<Ingredient*>(shape->getData());
    frySound->play();

    if (!ingredient) return;
    if (ingredient->actions.contains(FRY)) return;

    ingredient->actions.append(FRY);
    incrementIngredient(ingredient);

    if (ingredient->actions.contains(FRY)) {
        auto method = [shape]() {

            delete shape;
        };

        actionQueue.append(method);
    }
}
