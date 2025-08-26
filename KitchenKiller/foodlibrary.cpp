#include "foodLibrary.h"
#include "recipenew.h"
#include <QDebug>
FoodLibrary::FoodLibrary() {
  initialRecipes();

  // STAPLE
  {
    Ingredient *riceNoodles =
        new Ingredient("RiceNoodles", STAPLE, {VEGAN}, {});
    allIngredients.push_back(riceNoodles);

//    Ingredient *eggNoodles =
//        new Ingredient("EggNoddles", STAPLE, {}, {});
//    allIngredients.push_back(eggNoodles);
  }

  // MEAT
  {
    Ingredient *beef = new Ingredient("Beef", MEAT, {}, {});
    allIngredients.push_back(beef);

    Ingredient *pork = new Ingredient("Pork", MEAT, {}, {});
    allIngredients.push_back(pork);

    Ingredient *chicken = new Ingredient("Chicken", MEAT, {}, {});
    allIngredients.push_back(chicken);

    Ingredient *shrimp = new Ingredient("Shrimp", MEAT, {}, {});
    allIngredients.push_back(shrimp);
  }

  // VEGET
  {
    Ingredient *broccoli = new Ingredient("Broccoli", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(broccoli);

    Ingredient *tomato = new Ingredient("Tomato", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(tomato);

    Ingredient *carrot = new Ingredient("Carrot", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(carrot);

    Ingredient *cabbage = new Ingredient("Cabbage", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(cabbage);

    Ingredient *greenonion = new Ingredient("GreenOnion", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(greenonion);

    Ingredient *ginger = new Ingredient("Ginger", VEGETABLE, {VEGAN}, {});
    allIngredients.push_back(ginger);


  }

  // CONDIMENT
  {
    Ingredient *peanut = new Ingredient("Peanut", CONDIMENT, {VEGAN, NUT}, {});
    allIngredients.push_back(peanut);

    Ingredient *pepper = new Ingredient("Pepper", CONDIMENT, {VEGAN}, {});
    allIngredients.push_back(pepper);

    Ingredient *soysauce = new Ingredient("SoySauce", CONDIMENT, {VEGAN}, {});
    allIngredients.push_back(soysauce);

//    Ingredient *fishsauce = new Ingredient("FishSauce", CONDIMENT, {}, {});
//    allIngredients.push_back(fishsauce);


  }

  // [== RECIPES ==]

  // Chow Mein
  {
    RecipeNew *stirFryNoddle = new RecipeNew("Chǎo miàn");
    stirFryNoddle->setPixmap(QPixmap(":/sprites/icons/chao mian.png"));
    stirFryNoddle->setDescription(
        "1. Chop bell peppers, onions, and carrots. \n "
        "2. Slice the chicken or beef. \n"
        "3. Heat oil in a wok or large frying pan and stir-fry the chicken or "
        "beef. \n"
        "4. Add the chopped vegetables and stir-fry for a few minutes. \n"
        "5. Cook the egg noodles according to package instructions. \n"
        "6. Add the cooked stirFryNoddles and soy sauce to the wok or frying "
        "pan, mix well and serve.");

    stirFryNoddle->setLearnMore(
        "<p>What is Chow Mein?</p>"
        "<p>- Chow Mein is a Chinese stir-fried noodle dish that is popular in "
        "many countries. It typically consists of egg noodles, vegetables, and "
        "a choice of meat or seafood, all stir-fried together with a savory "
        "sauce.</p>"
        "<p>Where did Chow Mein emerge?</p>"
        "<p>- Chow Mein originated in China, specifically from the northern "
        "regions, where wheat-based dishes like noodles are more common.</p>"
        "<p>When did Chow Mein become popular?</p>"
        "<p>- Chow Mein gained popularity in the United States and other "
        "countries with the spread of Chinese immigrants in the early 20th "
        "century.</p>"
        "<p>Fun fact:</p>"
        "<p>- The name Chow Mein means 'stir-fried noodles' in Chinese "
        "(Mandarin).</p>");

    stirFryNoddle->setDifficulty(2);

    stirFryNoddle->addTask(
        "Salted the water",
        {{Ingredient("Salt", {BOIL}), {1, "Put salt in the boiling pot"}}});
    stirFryNoddle->addTask(
        "Boil noddle and start the pan",
        {{Ingredient("RiceNoodles", {BOIL}),
          {1, "Boil the rice noodles until soft"}},
         {Ingredient("Garlic", {CUT}), {2, "Mince garlic"}},
         {Ingredient("Oil", {FRY}), {1, "Oil the frying pan"}}});
    stirFryNoddle->addTask(
        "Put ",
        {{Ingredient("RiceNoodles", {FRY}), {1, "Fry the rice noodles"}},
         {Ingredient("Garlic", {CUT, FRY}), {8, "Fry the minced garlic"}}});
    stirFryNoddle->addTask(
        "Almost done!",
        {{Ingredient("Pepper", {FRY}), {1, "Add pepper to the frying pan"}},
         {Ingredient("ChiliYum", {FRY}),
          {1, "Add chili yum to the frying pan"}},
         {Ingredient("SoySauce", {FRY}),
          {1, "Add soy sauce to the frying pan"}}});

    m_recipes.append(stirFryNoddle);
  }

  // Pho
  {
    RecipeNew *pho = new RecipeNew("Phở");
    pho->setPixmap(QPixmap(":/sprites/icons/pho.png"));

    pho->setDescription(
        "1. Chop broccoli, carrot and cabbage. \n "
        "2. Chop all the proteins including beef or chicken. \n"
        "3. Put all the chopped meat to boiling water to cook for 5 hours. \n"
        "4. Add all the vegetables into the soup and cook 30 minutes or more. "
        "\n"
        "5. Cook the noodle in the soup for 5 minutes. And add some soysauce. "
        "\n"
        "6. Pour all content to a bowl. ");

    pho->setLearnMore(
        "<p>What is Phở?</p>"
        "<p>- Phở is a Vietnamese soup dish consisting of broth, rice noodles "
        "(bánh phở), herbs, and meat (usually beef (phở bò), sometimes chicken "
        "(phở gà)). Phở is a popular food in Vietnam, where it is served in "
        "households, street-stalls, and restaurants country-wide.</p>"
        "<p>Where did Phở emerge?</p>"
        "<p>- Residents of the city of Nam Định were the first to create "
        "Vietnamese traditional phở</p>"
        "<p>When did Phở become popular?</p>"
        "<p>- Originated in the early 20th century in Northern Vietnam, and "
        "was popularized throughout the world by refugees after the Vietnam "
        "War.</p>"
        "<p>Fun fact:</p>"
        "<p>- Because phở's origins are poorly documented, there is "
        "disagreement over the cultural influences that led to its development "
        "in Vietnam, as well as the etymology of the name.</p>"
        "<p>- The Hanoi (northern) and Saigon (southern) styles of pho differ "
        "by noodle width, sweetness of broth, and choice of herbs and "
        "sauce.</p>");

    pho->setDifficulty(4);

    pho->addTask("Chop beef and chicken.",
                 {
                     {Ingredient("Beef", {CUT}), {1, "Chop beef"}},
                     {Ingredient("Chicken", {CUT}), {1, "Chop chicken"}},
                 });
    pho->addTask(
        "Put chopped meats into boiling pot for 5 hours.",
        {
            {Ingredient("Beef", {CUT, BOIL}), {4, "Boil chopped beef"}},
            {Ingredient("Chicken", {CUT, BOIL}), {4, "Boil chopped chicken"}},
        });

    pho->addTask("Chop broccoli, carrot, and cabbage.",
                 {
                     {Ingredient("Broccoli", {CUT}), {1, "Chop broccoli"}},
                     {Ingredient("Carrot", {CUT}), {1, "Chop carrot"}},
                     {Ingredient("Cabbage", {CUT}), {1, "Chop cabbage"}},
                 });
    pho->addTask(
        "Add chopped vegetables into boiling pot for 30 minutes or more.",
        {
            {Ingredient("Broccoli", {CUT, BOIL}), {4, "Boil chopped broccoli"}},
            {Ingredient("Carrot", {CUT, BOIL}), {4, "Boil chopped carrot"}},
            {Ingredient("Cabbage", {CUT, BOIL}), {4, "Boil chopped cabbage"}},
        });

    pho->addTask(
        "Boil the rice noodle in the soup for 5 minutes.",
        {
            {Ingredient("RiceNoodles", {BOIL}), {1, "Boil rice noodles"}},
        });
    pho->addTask("Add soysauce to the soup.",
                 {
                     {Ingredient("SoySauce", {BOIL}),
                      {1, "Add soysauce to the boiling pot"}},
                 });

    m_recipes.append(pho);
  }

  //Kuyteav
  {
    RecipeNew* kuyteav = new RecipeNew("Kŭytéav");
kuyteav->setPixmap(QPixmap(":/sprites/icons/kuyteav.png"));
kuyteav->setDescription("1. Prepare the pork broth by simmering pork bones, water, and seasonings. \n "
                        "2. Cook the rice noodles separately according to package instructions. \n"
                        "3. In a separate pan, cook the minced pork and shrimp. \n"
                        "4. Assemble the bowls by placing noodles first, then the cooked minced pork and shrimp. \n"
                        "5. Pour the hot pork broth over the assembled bowls. \n"
                        "6. Garnish with bean sprouts, lime wedges, and fresh herbs. Serve hot. "
                        );

kuyteav->setLearnMore("<p>What is Kuyteav?</p>"
                      "<p>- Kuyteav is a Cambodian noodle soup dish made from rice noodles, minced pork, shrimp, and a rich pork broth. It is typically garnished with bean sprouts, lime wedges, and fresh herbs.</p>"
                      "<p>Where did Kuyteav emerge?</p>"
                      "<p>- Kuyteav has its origins in Cambodia, where it is considered a national dish.</p>"
                      "<p>When did Kuyteav become popular?</p>"
                      "<p>- Kuyteav has been a popular Cambodian dish for centuries and has gained popularity in neighboring countries such as Vietnam and Thailand.</p>"
                      "<p>Fun fact:</p>"
                      "<p>- Kuyteav is often enjoyed as a breakfast dish in Cambodia, but it can be found and enjoyed at any time of the day.</p>"
                      );

kuyteav->setDifficulty(2);

kuyteav->addTask("Prepare the pork broth.", {
                 {Ingredient("Pork", {BOIL}),                      {1, "Boil pork"}},

             });
kuyteav->addTask("Cook the rice noodles separately.", {
                 {Ingredient("RiceNoodles", {BOIL}),                    {1, "Boil rice noodles"}},
             });
kuyteav->addTask("Cook the minced pork and shrimp.", {
                 {Ingredient("Pork", {CUT,FRY}),                      {1, "Fry minced pork"}},
                 {Ingredient("Shrimp", {FRY}),                          {1, "Fry shrimp"}},
             });
 m_recipes.append(kuyteav);
}


  // Bun Bo Hue:
{
 RecipeNew *bunBoHue = new RecipeNew("Bún Bò Huế");
  bunBoHue->setPixmap(QPixmap(":/sprites/icons/bun bo hue.png"));
  bunBoHue->setDescription(
      "1. Prepare lemongrass, garlic, shallots, and chilies. \n "
      "2. Chop beef and pork. \n"
      "3. Create the broth by simmering beef bones and pork bones for 4 hours. "
      "\n"
      "4. Add lemongrass, garlic, shallots, and chilies to the broth and "
      "simmer for 1 hour. \n"
      "5. Cook the rice noodles separately. \n"
      "6. Assemble the dish by adding noodles, meat, and broth to a bowl, then "
      "garnishing with herbs and lime wedges. ");

  bunBoHue->setLearnMore(
      "<p>What is Bún Bò Huế?</p>"
      "<p>- Bún Bò Huế is a popular Vietnamese soup containing rice vermicelli "
      "(bún) and beef (bò). It is a spicy, tangy, and slightly sweet soup with "
      "lemongrass as its main flavor profile.</p>"
      "<p>Where did Bún Bò Huế emerge?</p>"
      "<p>- Bún Bò Huế originated in the city of Huế, the former imperial "
      "capital of Vietnam.</p>"
      "<p>When did Bún Bò Huế become popular?</p>"
      "<p>- Bún Bò Huế became popular in the 20th century and has since become "
      "a popular dish across Vietnam and in Vietnamese communities "
      "worldwide.</p>"
      "<p>Fun fact:</p>"
      "<p>- The dish is named after Huế, which is known for its distinct and "
      "refined cuisine.</p>"
      "<p>- The soup is usually served with a variety of accompaniments, such "
      "as bean sprouts, mint, cilantro, and lime wedges.</p>");

  bunBoHue->setDifficulty(4);

  bunBoHue->addTask("Chop beef and pork.",
                    {
                        {Ingredient("Beef", {CUT}), {1, "Chop beef"}},
                        {Ingredient("Pork", {CUT}), {1, "Chop pork"}},
                    });

  bunBoHue->addTask("Simmer beef and pork for 4 hours.",
                    {
                        {Ingredient("Beef", {CUT, BOIL}), {4, "Boil chopped beef"}},
                        {Ingredient("Pork", {CUT, BOIL}), {4, "Boil chopped pork"}},
                    });

  bunBoHue->addTask(
      "Prepare lemongrass, garlic, and chilies.",
      {
          {Ingredient("Lemongrass", {CUT}), {1, "Chop lemongrass"}},
          {Ingredient("Garlic", {CUT}), {1, "Chop garlic"}},
          {Ingredient("Chilies", {CUT}), {1, "Chop chilies"}},
      });
  bunBoHue->addTask(
      "Add lemongrass, garlic, and chilies to the broth, and add fish sauce. simmer for 1 hour.",
      {
          {Ingredient("Lemongrass", {CUT, BOIL}),
           {4, "Boil chopped lemongrass"}},
          {Ingredient("Garlic", {CUT, BOIL}), {4, "Boil chopped garlic"}},
          {Ingredient("Chilies", {CUT, BOIL}), {4, "Boil chopped chilies"}},
          {Ingredient("FishSauce", {BOIL}),
           {1, "Add fish sauce to the boiling pot"}},
      });

  bunBoHue->addTask(
      "Cook the rice noodles separately.",
      {
          {Ingredient("RiceNoodles", {BOIL}), {1, "Boil rice noodles"}},
      });

  m_recipes.append(bunBoHue);
}

//YangChunMian
{
RecipeNew* yangChunMian = new RecipeNew("Yángchūn miàn ");
yangChunMian->setPixmap(QPixmap(":/sprites/icons/yang chun.png"));
yangChunMian->setDescription("1. Bring water to a boil in a pot and cook the noodles until al dente. \n "
                             "2. In a separate pot, simmer chicken stock, green onion, ginger, and seasonings. \n"
                             "3. Heat oil in a pan and lightly sauté the vegetables. \n"
                             "4. Drain the noodles and place them in serving bowls. \n"
                             "5. Pour the hot chicken broth over the noodles. \n"
                             "6. Top with sautéed vegetables, shredded chicken, and green onion. Serve immediately. "
                             );

yangChunMian->setLearnMore("<p>What is Yang Chun Mian?</p>"
                           "<p>- Yang Chun Mian is a traditional Chinese noodle dish made with thin egg noodles, a light chicken broth, and various toppings such as vegetables, shredded chicken, and green onions.</p>"
                           "<p>Where did Yang Chun Mian emerge?</p>"
                           "<p>- Yang Chun Mian has its origins in China, where it has been a popular dish for centuries.</p>"
                           "<p>When did Yang Chun Mian become popular?</p>"
                           "<p>- Yang Chun Mian has been a staple dish in China for centuries and continues to be popular in various Chinese communities around the world.</p>"
                           "<p>Fun fact:</p>"
                           "<p>- Yang Chun Mian is considered a comfort food in China and is enjoyed throughout the year, especially during colder months.</p>"
                           );

yangChunMian->setDifficulty(1);

yangChunMian->addTask("Cook rice noodles until al dente.", {
                 {Ingredient("Rice Noodles", {BOIL}),                        {1, "Boil Rice Noodles"}},
             });
yangChunMian->addTask("Simmer chicken stock, green onion, ginger, and seasonings.", {
                 {Ingredient("Chicken", {BOIL}),                   {1, "Boil chicken stock"}},
                 {Ingredient("GreenOnion", {BOIL}),                     {1, "Add green onion"}},
                 {Ingredient("Ginger", {BOIL}),                         {1, "Add ginger"}},
                 {Ingredient("SoySauce", {BOIL}),                     {1, "Add seasonings"}},
             });
yangChunMian->addTask("Sauté the vegetables.", {
                 {Ingredient("Cabbage", {CUT, FRY}),                      {1, "Sauté chopped cabbage "}},
             });

m_recipes.append(yangChunMian);
}
}

void FoodLibrary::initialRecipes() {
    //    Recipe* pho = new Recipe();
    //    //Set all ingredient
    //    pho->setIngredients({broccoli, carrot, cabbage, riceNoodles,beef,
    //    chicken,soysauce}); pho->loadImageFromFile(":/sprites/icons/pho.png");
    //    pho->setCountry(Country::VIETNAM);
    //    pho->setName("Pho");
    //    pho->setDifficulty(3);
    //    pho->setDescription("1. Chop broccoli, carrot and cabbage. \n "
    //                        "2. Chop all the proteins including beef or chicken.
    //                        \n" "3. Put all the chopped meat to boiling water to
    //                        cook for 5 hours. \n" "4. Add all the vegetables
    //                        into the soup and cook 30 minutes or more. \n" "5.
    //                        Cook the noodle in the soup for 5 minutes. And add
    //                        some soysauce. \n" "6. Pour all content to a bowl.
    //                        ");

    //    pho->setLearnMore("<p>What is Phở?</p>"
    //                         "<p>- Phở is a Vietnamese soup dish consisting of
    //                         broth, rice noodles (bánh phở), herbs, and meat
    //                         (usually beef (phở bò), sometimes chicken (phở
    //                         gà)). Phở is a popular food in Vietnam, where it is
    //                         served in households, street-stalls, and
    //                         restaurants country-wide.</p>"
    //                         "<p>Where did Phở emerge?</p>"
    //                         "<p>- Residents of the city of Nam Định were the
    //                         first to create Vietnamese traditional phở</p>"
    //                         "<p>When did Phở become popular?</p>"
    //                         "<p>- Originated in the early 20th century in
    //                         Northern Vietnam, and was popularized throughout
    //                         the world by refugees after the Vietnam War.</p>"
    //                         "<p>Fun fact:</p>"
    //                         "<p>- Because phở's origins are poorly documented,
    //                         there is disagreement over the cultural influences
    //                         that led to its development in Vietnam, as well as
    //                         the etymology of the name.</p>"
    //                         "<p>- The Hanoi (northern) and Saigon (southern)
    //                         styles of pho differ by noodle width, sweetness of
    //                         broth, and choice of herbs and sauce.</p>");

    //    pho->setRecipeTags({RecipeTags::DAIRYFREE, RecipeTags::GLUTENFREE,
    //    RecipeTags::NUTFREE}); recipes.push_back(pho); pho->setTasks(  {
    //                        {Ingredient(QString("Broccoli"),
    //                        {ActionsPerformed::CUT}), 1},
    //                        {Ingredient(QString("Carrort"),
    //                        {ActionsPerformed::CUT}), 1},
    //                        {Ingredient(QString("Cabbage"),
    //                        {ActionsPerformed::CUT}), 1},
    //                        {Ingredient(QString("Beef" ) ,
    //                        {ActionsPerformed::CUT}), 1},
    //                        {Ingredient(QString("Chicken"),
    //                        {ActionsPerformed::CUT}), 1},

    //                        //Boil
    //                        {Ingredient(QString("Beef"      ) ,
    //                        {ActionsPerformed::CUT, ActionsPerformed::BOIL}),
    //                        1}, {Ingredient(QString("Chicken"   ) ,
    //                        {ActionsPerformed::CUT, ActionsPerformed::BOIL}),
    //                        1}, {Ingredient(QString("Broccoli"
    //                        ),{ActionsPerformed::CUT, ActionsPerformed::BOIL}),
    //                        1}, {Ingredient(QString("Carrot"    ),
    //                        {ActionsPerformed::CUT, ActionsPerformed::BOIL}),
    //                        1}, {Ingredient(QString("Cabbage"   ),
    //                        {ActionsPerformed::CUT, ActionsPerformed::BOIL}),
    //                        1},
    //                        {Ingredient(QString("RiceNoodles"),{ActionsPerformed::BOIL}),
    //                        1}, {Ingredient(QString("SoySauce"  )
    //                        ,{ActionsPerformed::BOIL}), 1}

    //                    });
}

Ingredient *FoodLibrary::getIngredientByName(const QString &name) const {
  for (Ingredient *ingredient : allIngredients) {
      if (ingredient->getName() == name) {
          return ingredient;
      }
  }
  return nullptr;
}

RecipeNew *FoodLibrary::getRecipeByName(const QString &recipeName) const {
    for (RecipeNew *recipe : m_recipes) {
      if (recipe->getName() == recipeName) {
          return recipe;
      }
  }
  return nullptr;
}



