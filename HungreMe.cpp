// HungryMe.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <functional>
#include <fstream>
#include "HungryMe.h"

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

void changeColor(int color)
{
    SetConsoleTextAttribute(hOut, color);
}

void setCursor(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Button
{
public:

    int xPos;
    int yPos;

    int xSize;
    int ySize;

    int color;

    std::string* textTab;

    std::function<void(void)> activateButton;

    static void nothing()
    {}

private:

    int tabSize;

public:

    void setButtonFunction(std::function<void(void)> function) { activateButton = function; }

    Button(int xPosB, int yPosB, int xSizeB, int ySizeB, int colorB)
    {
        xPos = xPosB;
        yPos = yPosB;
        xSize = xSizeB;
        ySize = ySizeB;
        color = colorB % 256;
        tabSize = 0;
        textTab = new std::string[0];
        activateButton = nothing;
    }

    Button()
    {
        xPos = 0;
        yPos = 0;
        xSize = 10;
        ySize = 4;
        color = 74;
        tabSize = 0;
        textTab = new std::string[0];
        activateButton = nothing;
    }

    void addText(std::string text)
    {
        std::string* temp = new std::string[tabSize + 1];;
        std::string* oldTab = textTab;

        for (int i = 0; i < tabSize; i++)
        {
            temp[i] = textTab[i];
        }

        temp[tabSize] = text;

        textTab = temp;
        delete[] oldTab;

        tabSize++;
    }

    std::string returnTabText(int arrayNum)
    {
        return textTab[arrayNum];
    }

    void editTabText(int arrayNum, std::string text)
    {
        textTab[arrayNum] = text;
    }

    void viewButton()
    {
        changeColor(color);
        for (int i = 0; i < ySize; i++)
        {
            setCursor(xPos, yPos + i);
            for (int j = 0; j < xSize; j++)
            {

                if (i < tabSize && (int)textTab[i][j] != 0)
                {
                    std::cout << textTab[i][j];
                }
                else
                {
                    for (int k = j; k < xSize; k++)
                    {
                        std::cout << " ";
                    }
                    j = xSize;
                }
            }
        }
        changeColor(7);
        /**std::cout<<xPos<<" =xPos|";
        std::cout<<yPos<<" =yPos|";
        std::cout<<xSize<<" =xSize|";
        std::cout<<ySize<<" =ySize|";
        std::cout<<color<<" =color|";*/
    }

    bool changeButtonColor(int kolor)
    {
        color = kolor % 256;
        return true;
    }
};

class Interface//Button Agregation
{
private:
    int cursorPosX;
    int cursorPosY;
    int currentButton;
    int tabSize;

public:
    Button* buttonsTab;

    static bool ifBigger(int a, int b)
    {
        if (a > b)
            return true;
        else
            return false;
    }

    static bool ifSmaller(int a, int b)
    {
        if (a < b)
            return true;
        else
            return false;
    }

    void lookForClosest(bool (*statement)(int, int), bool inAxisX)
    {
        int closeX;
        int closeY;
        int button = -1;//-1 tells if button was found
        if (inAxisX)
        {
            for (int i = 0; i < tabSize; i++)
            {
                if ((*statement)(cursorPosX, buttonsTab[i].xPos))/**sprawdza kierunek**/
                {
                    if (button == -1) /**sprawdza znalezienie buttona**/
                    {
                        closeX = buttonsTab[i].xPos;
                        closeY = buttonsTab[i].yPos;
                        button = i;
                    }
                    else
                    {
                        if (((cursorPosX - buttonsTab[i].xPos) * (cursorPosX - buttonsTab[i].xPos) + (cursorPosY - buttonsTab[i].yPos) * (cursorPosY - buttonsTab[i].yPos))
                            < ((cursorPosX - closeX) * (cursorPosX - closeX) + (cursorPosY - closeY) * (cursorPosY - closeY)))/**sprawdza najblizszy button**/
                        {
                            closeX = buttonsTab[i].xPos;
                            closeY = buttonsTab[i].yPos;
                            button = i;
                        }
                    }
                }
            }
            if (button >= 0)
            {
                if ((*statement)(cursorPosX, closeX))
                {
                    cursorPosX = closeX;
                    cursorPosY = closeY;
                    buttonsTab[currentButton].changeButtonColor(buttonsTab[currentButton].color - buttonsTab[currentButton].color % 16 + (buttonsTab[currentButton].color + 8) % 16 + 128);//shows which button is now selected
                    buttonsTab[currentButton].viewButton();
                    buttonsTab[button].changeButtonColor(buttonsTab[button].color - buttonsTab[button].color % 16 + (buttonsTab[button].color + 8) % 16 + 128);//shows which button is now selected
                    buttonsTab[button].viewButton();
                    currentButton = button;
                }
            }
        }
        else
        {
            for (int i = 0; i < tabSize; i++)
            {
                if ((*statement)(cursorPosY, buttonsTab[i].yPos))/**sprawdza kierunek**/
                {
                    if (button == -1) /**sprawdza znalezienie buttona**/
                    {
                        closeX = buttonsTab[i].xPos;
                        closeY = buttonsTab[i].yPos;
                        button = i;
                    }
                    else
                    {
                        if (((cursorPosX - buttonsTab[i].xPos) * (cursorPosX - buttonsTab[i].xPos) + (cursorPosY - buttonsTab[i].yPos) * (cursorPosY - buttonsTab[i].yPos))
                            < ((cursorPosX - closeX) * (cursorPosX - closeX) + (cursorPosY - closeY) * (cursorPosY - closeY)))/**sprawdza najblizszy button**/
                        {
                            closeX = buttonsTab[i].xPos;
                            closeY = buttonsTab[i].yPos;
                            button = i;
                        }
                    }
                }
            }
            if (button >= 0)
            {
                if ((*statement)(cursorPosY, closeY))
                {
                    cursorPosX = closeX;
                    cursorPosY = closeY;
                    buttonsTab[currentButton].changeButtonColor(buttonsTab[currentButton].color - buttonsTab[currentButton].color % 16 + (buttonsTab[currentButton].color + 8) % 16 + 128);//shows which button is now selected
                    buttonsTab[currentButton].viewButton();
                    buttonsTab[button].changeButtonColor(buttonsTab[button].color - buttonsTab[button].color % 16 + (buttonsTab[button].color + 8) % 16 + 128);//shows which button is now selected
                    buttonsTab[button].viewButton();
                    currentButton = button;
                }
            }
        }
    }

public:
    Interface()
    {
        tabSize = 0;
        cursorPosX = 0;
        cursorPosY = 0;
        currentButton = 0;
        buttonsTab = new Button[0];
    }

    void moveCursor(char ch)
    {
        switch (ch)
        {
        case 'w':case 'W':
        {
            lookForClosest(ifBigger, false);
        }
        break;

        case 's':case 'S':
        {
            lookForClosest(ifSmaller, false);
        }
        break;

        case 'a':case 'A':
        {
            lookForClosest(ifBigger, true);
        }
        break;

        case 'd':case 'D':
        {
            lookForClosest(ifSmaller, true);
        }
        case ' ':
        {
            buttonsTab[currentButton].activateButton();
        }
        break;
        }
    }

    void addButton(Button* button)
    {
        Button* temp;
        Button* oldTab = buttonsTab;
        temp = new Button[tabSize + 1];

        if (tabSize == 0)
        {
            currentButton = 0;
            cursorPosX = (*button).xPos;
            cursorPosY = (*button).yPos;
            (*button).changeButtonColor((*button).color - (*button).color % 16 + ((*button).color + 8) % 16 + 128);//highlights current button
        }

        for (int i = 0; i < tabSize; i++)
        {
            temp[i] = buttonsTab[i];
        }

        temp[tabSize] = *button;

        buttonsTab = temp;
        delete[] oldTab;

        tabSize++;
    }

    void removeButton(int tabPos)
    {
        if (tabSize > 0)
        {
            Button* temp;
            Button* oldTab = buttonsTab;
            temp = new Button[tabSize - 1];

            for (int i = 0; i < tabSize - 1; i++)
            {
                if (i >= tabPos)
                {
                    temp[i] = buttonsTab[i + 1];
                }
                else
                {
                    temp[i] = buttonsTab[i];
                }
            }
            buttonsTab = temp;
            delete[] oldTab;

            tabSize--;
        }
    }

    void viewInterface()
    {
        for (int i = 0; i < tabSize; i++)
        {
            buttonsTab[i].viewButton();
        }
    }

};

class Date
{
private:
    int day;
    int month;
    int year;

public:

    Date()
    {
        day = 1;
        month = 1;
        year = 0;
    }
    Date(int dayVar, int monthVar, int yearVar)
    {
        changeDay(dayVar, monthVar, yearVar);
    }
    void changeDay(int dayVar, int monthVar, int yearVar)
    {
        int numOfDays = 0;
        switch (monthVar)
        {
        case 1:case 3:case 5:case 7:case 8: case 10: case 12:
            numOfDays = 31;
                break;
            case 2:
                if (yearVar % 400== 0 || (yearVar % 100!= 0 && yearVar%4==0))
                    numOfDays = 29;
                else
                    numOfDays = 28;
                break;
            case 4:case 6: case 9: case 11:
                numOfDays = 30;
                break;
        }
        day = dayVar % numOfDays;
        month = monthVar % 12;
        year = yearVar;
    }
    int getDay()
    {
        return day;
    }
    int getMonth()
    {
        return month;
    }
    int getYear()
    {
        return year;
    }
};

class Ingridient 
{
    public:
        std::string name;

        float kcal;

        Ingridient()
        {
            name = "skladnik";
            kcal = 0;
        }
        Ingridient(std::string nameVar, float kcalVar)
        {
            name = nameVar;
            kcal = kcalVar;
        }
};

class Recipe
{
public:
    std::string name;

private:
    float kcal;
    std::string description;
    Ingridient** ingridientsTab;
    int* numOfIngr;
    int tabSize;
    //ITACHI

public:
    Recipe()
    {
        name = "recipe";
        tabSize = 0;
    }

    Recipe(std::string nameVar)
    {
        name = nameVar;
        tabSize = 0;
    }
    Recipe(std::string nameVar, std::string descriptionVar)
    {
        name = nameVar;
        description = descriptionVar;
        tabSize = 0;
    }

    void changeDescription(std::string descriptionVar)
    {
        description = name + " ";
        for (int i = 0;i < tabSize; i++)
        {
            description = description + ingridientsTab[i][0].name + " ";
        }
        description = description + descriptionVar;
    }

    std::string getDescription()
    {
        return description;
    }

    void addIngridient(Ingridient* ingridientVar,int howMany)
    {
        for (int i = 0; i < tabSize; i++)
        {
            if (ingridientVar == ingridientsTab[i])
            {
                numOfIngr[i]+=howMany;
            }
            else
            {
                Ingridient** temp=new Ingridient*[tabSize+1];
                Ingridient** oldTab = ingridientsTab;
                int* tempNUM = new int[tabSize + 1];
                int* oldTabNUM = numOfIngr;
                for (int i = 0; i < tabSize; i++)
                {
                    temp[i] = oldTab[i];
                    tempNUM[i] = oldTabNUM[i];
                }

                temp[tabSize] = ingridientVar;
                tempNUM[tabSize] = howMany;

                ingridientsTab = temp;
                numOfIngr = tempNUM;

                delete[] oldTab;
                delete[] oldTabNUM;

            }
        }
    }

    Ingridient* getIngridient(int num)
    {
        return ingridientsTab[num];
    }

    int getNumOfIngr(int num)
    {
        return numOfIngr[num];
    }

    void deleteIngridient(Ingridient* ingridientVar, int howMany)
    {
        for (int i = 0; i < tabSize; i++)
        {
            if (ingridientVar == ingridientsTab[i])
            {
                numOfIngr[i]-=howMany;
                if (numOfIngr[i] <= 0)
                {
                    {
                        Ingridient** temp = new Ingridient * [tabSize - 1];
                        Ingridient** oldTab = ingridientsTab;
                        int* tempNUM = new int[tabSize - 1];
                        int* oldTabNUM = numOfIngr;

                        int i=0;
                        while (ingridientVar != oldTab[i])
                        {
                            temp[i] = oldTab[i];
                            tempNUM[i] = oldTabNUM[i];
                            i++;
                        }
                        while (i < tabSize-1)
                        {
                            temp[i] = oldTab[i + 1];
                            tempNUM[i] = oldTabNUM[i + 1];
                        }

                        ingridientsTab = temp;
                        numOfIngr = tempNUM;

                        delete[] oldTab;
                        delete[] oldTabNUM;

                        tabSize--;

                    }

                }
            }
        }

    }

    int getTabSize()
    {
        return tabSize;
    }

    float getKcal()
    {
        float temp=0;

        for (int i = 0; i < tabSize; i++)
            temp += ingridientsTab[i][0].kcal;

        return temp;
    }
};

class Meal
{
public:

    Date date;
    Recipe* recipe;

private:

    float kcal;

public:

    Meal()
    {

    }
    Meal(Date dateVar,Recipe* recipeVar)
    {
        date = dateVar;
        recipe = recipeVar;
    }
    float getKcal()
    {
        return recipe->getKcal();
    }
    std::string getMealName()
    {
        return recipe->name;
    }
};

class CookingWeek
{
private:
    Recipe* dayTab[7][10];
    int tabSize[7];
public:
    CookingWeek()
    {

    }
    void addRecipe(Recipe* recipeVar,int whichTab)
    {
        if (tabSize[whichTab] < 10)
            dayTab[whichTab][tabSize[whichTab] + 1] = recipeVar;
    }
    void removeRecipe(int whichTab)
    {
        if (tabSize[whichTab] > 1)
            tabSize[whichTab]--;
    }
    Recipe* getRecipe(int day,int meal)
    {
        return dayTab[day][meal];
    }
    int getTabSize(int num)
    {
        return tabSize[num%7];
    }
};
class CookingWeed
{
private:
     int howManyGrams;
     static int howManyGrams2[3];
     static std::fstream xd;
public:
    CookingWeed()
    {
        howManyGrams = 6;
    }
    void smokeSomething()
    {
        howManyGrams--;
    }
    void addGrass()
    {
        trimTheLawn();
    }
    void trimTheLawn()
    {
        howManyGrams++;
    };

};

class Data
{
private:
    Ingridient** ingridientTab;
    Recipe** recipeTab;
    Meal** mealTab;
    CookingWeek** cookingWeekTab;
    int tabSize[4];
    std::ifstream dataIn;
    std::ofstream dataOut;

public:
    Data()
    {
        tabSize[0] = 0;
        tabSize[1] = 0;
        tabSize[2] = 0;
        tabSize[3] = 0;

        readFile();
    }

    void addIngridient(Ingridient* ingridientVar)
    {
        for (int i = 0; i < tabSize[0]; i++)
        {
                Ingridient** temp = new Ingridient * [tabSize[0] + 1];
                Ingridient** oldTab = ingridientTab;

                for (int i = 0; i < tabSize[0]; i++)
                {
                    temp[i] = oldTab[i];
                }

                temp[tabSize[0]] = ingridientVar;

                ingridientTab = temp;
                tabSize[0]++;

                delete[] oldTab;
        }
    }

    void addRecipe(Recipe* recipeVar)
    {
        for (int i = 0; i < tabSize[1]; i++)
        {
            Recipe** temp = new Recipe * [tabSize[1] + 1];
            Recipe** oldTab = recipeTab;

            for (int i = 0; i < tabSize[1]; i++)
            {
                temp[i] = oldTab[i];
            }

            temp[tabSize[1]] = recipeVar;

            recipeTab = temp;
            tabSize[2]++;

            delete[] oldTab;
        }
        
    }

    void addMeal(Meal* mealVar)
    {
        for (int i = 0; i < tabSize[2]; i++)
        {
            Meal** temp = new Meal * [tabSize[2] + 1];
            Meal** oldTab = mealTab;

            for (int i = 0; i < tabSize[2]; i++)
            {
                temp[i] = oldTab[i];
            }

            temp[tabSize[2]] = mealVar;

            mealTab = temp;
            tabSize[2]++;

            delete[] oldTab;
        }
    }

    void addCookingWeek(CookingWeek* cookingWeekVar)
    {
        for (int i = 0; i < tabSize[3]; i++)
        {
            CookingWeek** temp = new CookingWeek * [tabSize[2] + 1];
            CookingWeek** oldTab = cookingWeekTab;

            for (int i = 0; i < tabSize[3]; i++)
            {
                temp[i] = oldTab[i];
            }

            temp[tabSize[3]] = cookingWeekVar;

            cookingWeekTab = temp;
            tabSize[3]++;

            delete[] oldTab;
        }
    }

    void deleteIngridient(int tabNum)
    {
        if (tabSize[0] > 0 && tabNum < tabSize[0])
        {
            {
                Ingridient** temp = new Ingridient * [tabSize[0] - 1];
                Ingridient** oldTab = ingridientTab;

                int i = 0;
                while (i != tabNum)
                {
                    temp[i] = oldTab[i];
                    i++;
                }
                while (i < tabSize[0] - 1)
                {
                    temp[i] = oldTab[i + 1];
                }

                ingridientTab = temp;

                delete[] oldTab;

                tabSize[0]--;

            }
        }
    }
    
    void deleteRecipe(int tabNum)
    {
        if (tabSize[1] > 0 && tabNum < tabSize[1])
        {
            {
                Recipe** temp = new Recipe * [tabSize[1] - 1];
                Recipe** oldTab = recipeTab;

                int i = 0;
                while (i != tabNum)
                {
                    temp[i] = oldTab[i];
                    i++;
                }
                while (i < tabSize[1] - 1)
                {
                    temp[i] = oldTab[i + 1];
                }

                recipeTab = temp;

                delete[] oldTab;

                tabSize[1]--;
            }
        }
    }
    
    void deleteMeal(int tabNum)
    {
        if (tabSize[2] > 0 && tabNum < tabSize[2])
        {
            {
                Meal** temp = new Meal * [tabSize[2] - 1];
                Meal** oldTab = mealTab;

                int i = 0;
                while (i != tabNum)
                {
                    temp[i] = oldTab[i];
                    i++;
                }
                while (i < tabSize[2] - 1)
                {
                    temp[i] = oldTab[i + 1];
                }

                mealTab = temp;

                delete[] oldTab;

                tabSize[2]--;
            }
        }
    }
   
    void deleteCookingWeek(int tabNum)
    {
        if (tabSize[3] > 0 && tabNum < tabSize[3])
        {
            {
                CookingWeek** temp = new CookingWeek * [tabSize[3] - 1];
                CookingWeek** oldTab = cookingWeekTab;

                int i = 0;
                while (i != tabNum)
                {
                    temp[i] = oldTab[i];
                    i++;
                }
                while (i < tabSize[3] - 1)
                {
                    temp[i] = oldTab[i + 1];
                }

                cookingWeekTab = temp;

                delete[] oldTab;

                tabSize[3]--;
            }
        }
    }
    
    Recipe* returnRecipe(int tabNum)
    {
        return recipeTab[tabNum];
    }
    
    Meal* returnMeal(int tabNum)
    {
        return mealTab[tabNum];
    }
    
    Ingridient* returnIngridient(int tabNum)
    {
        return ingridientTab[tabNum];
    }
    
    CookingWeek* returnCookingWeek(int tabNum)
    {
        return cookingWeekTab[tabNum];
    }

    void updateFile()
    {
        dataOut.open("HungryMeDATA.txt");

        //Ingridients
        dataOut << "Ingridients:\n";
        dataOut << tabSize[0]<<"\n";
        for (int i = 0; i < tabSize[0]; i++)
        {
            dataOut <<"ingridient "<< i <<" \n";
            dataOut << (int)ingridientTab[i] << "\n";
            dataOut << ingridientTab[i][0].name<<"\n";
            dataOut << ingridientTab[i][0].kcal <<"\n";
        }
        //Recipes
        dataOut << "Recipes:\n";
        dataOut << tabSize[1] << "\n";
        for (int i = 0; i < tabSize[1]; i++)
        {
            dataOut << "Recipe " << i << " \n";
            dataOut << (int)recipeTab[i] << "\n";
            dataOut << recipeTab[i][0].name << "\n";
            dataOut << recipeTab[i][0].getDescription() << "\n";
            for (int j = 0; j < recipeTab[i][0].getTabSize(); j++)
            {
                dataOut << "ingridient " << j << " \n";
                dataOut << (int)recipeTab[i][0].getIngridient(j) << "\n";
                dataOut << (int)recipeTab[i][0].getNumOfIngr(j) << "\n";
            }
        }
        //Meals
        dataOut << "Meals:\n";
        dataOut << tabSize[2] << "\n";
        for (int i = 0; i < tabSize[0]; i++)
        {
            dataOut << "recipe " << i << " \n";
            dataOut << (int)mealTab[i] << "\n";
            dataOut << (int)mealTab[i][0].recipe<< "\n";
            dataOut << mealTab[i][0].date.getDay() << "\n";
            dataOut << mealTab[i][0].date.getMonth() << "\n";
            dataOut << mealTab[i][0].date.getYear() << "\n";
        }
        //CookingWeeks
        dataOut << "CookingWeeks:\n";
        dataOut << tabSize[3] << "\n";
        for (int i = 0; i < tabSize[3]; i++)
        {
            dataOut << "CookingWeek " << i << " \n";
            for (int j = 0; j < 7; j++)
            {
                dataOut << "CookingWeek " << i << " \n";
                dataOut << cookingWeekTab[i][0].getTabSize(j) << "\n";
                for (int k = 0; k < cookingWeekTab[i][0].getTabSize(j); k++)
                {
                    dataOut << (int)cookingWeekTab[i][0].getRecipe(j,k) << "\n";
                }
            }
        }
        dataOut.close();
    }
    static int lookForAdress(int* array,int adress)
    {
        for (int i = 0;; i++)
        {
            if (array[i] == adress)
                return i;
        }
    }

    void readFile()
    {
        int* ingridientPointer= nullptr;
        int* recipePointer= nullptr;
        int* mealPointer= nullptr;
        int* cookingWeekPointer= nullptr;
        std::string line;
        std::string line2;
        dataIn.open("HungryMeDATA.txt");
        while (getline(dataIn, line))
        {
            if (line == "Ingridients:")
            {
                getline(dataIn, line);
                int tabSize = std::stoi(line);
                ingridientPointer = new int[tabSize];

                for (int i = 0; i < tabSize; i++)
                {
                    getline(dataIn, line);//there is info for txt file user
                    getline(dataIn, line);
                    ingridientPointer[i]= std::stoi(line);
                    getline(dataIn, line);
                    getline(dataIn, line2);
                    ingridientTab[i][0].name=line;
                    ingridientTab[i][0].kcal = std::stoi(line2);
                }


            }

            if (line == "Recipes:")
            {
                getline(dataIn, line);
                int tabSize = std::stoi(line);
                recipePointer = new int[tabSize];
                int temp;

                for (int i = 0; i < tabSize; i++)
                {
                    getline(dataIn, line);//there is info for txt file user
                    getline(dataIn, line);
                    recipePointer[i] = std::stoi(line);
                    getline(dataIn, line);
                    getline(dataIn, line2);
                    addRecipe(new Recipe(line, line2));
                    for (int j = 0; j < recipeTab[i][0].getTabSize(); j++)
                    {
                        getline(dataIn, line);//*ingridient
                        getline(dataIn, line2);// num of ingridients
                     
                        temp = lookForAdress(ingridientPointer, std::stoi(line));
                        recipeTab[i][0].addIngridient(ingridientTab[temp], std::stoi(line2));

                    }
                }


            }

            if (line == "Meals:")// do zrobienia
            {
                getline(dataIn, line);
                int tabSize = std::stoi(line);
                mealPointer = new int[tabSize];
                int temp;
                for (int i = 0; i < tabSize; i++)
                {
                    getline(dataIn, line);//there is info for txt file user
                    getline(dataIn, line);
                    mealPointer[i] = std::stoi(line);

                }

            }

            if (line == "CookingWeeks:")// do zrobienia
            {
                getline(dataIn, line);
                int tabSize = std::stoi(line);
                cookingWeekPointer = new int[tabSize];
                int temp;
                for (int i = 0; i < tabSize; i++)
                {
                    getline(dataIn, line);//there is info for txt file user
                    getline(dataIn, line);
                    cookingWeekPointer[i] = std::stoi(line);

                }

            }
        }
        dataOut.close();
    }

};
class IngridientData
{

};

class RecipeDataData
{

};

class Callendar
{

};

class CookingWeekData
{

};

class Menu
{

};

int main()
{
    Data data;
    data.updateFile();
    data.readFile();
    _getch();
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
