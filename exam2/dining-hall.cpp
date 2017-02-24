#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

/********************************************************************************************
 * This is the Meal Class. 
 *
 * The type parameter specifies whether the bowl is a Veggie or a Steak meal . 
 * The prep_time parameter specifies the time at which 
 *          the meal was prepared and brought to you for distribution. 
 * Class has a virtual function done_cooking() function 
 *          which is used when the meal is fully prepared and brought for distribution. 
 *          It prints out the type of meal and the prep_time
 * Class has another virtual function happy_feast() function
 *          which is called when a meal is bought by a student. 
 *          It prints the type of meal and an id specific to the meal.
 *******************************************************************************************/

 class Meal {
    public:
        //Member variables
        string type;
        int prep_time;

        //Constructors
        Meal(string t, int p): type(t), prep_time(p) { }

        //Virtual Functions
        virtual void done_cooking() {
            cout << "Meal["<<this->prep_time<<"]["<<this->type<<"]: I'm up for grabs. Yay!" <<endl;;
        }

        virtual void happy_feast() {
            cout <<"Meal: I don't know what I am! Happy feasting on me though!" << endl;
        }
};

/********************************************************************************************
 * This is the VeggieMeal class derived from the Meal class 
 *
 * Every VeggieMeal has a specific vmID. 
 * The VeggieMeal constructor has been implemented for you.
 * You don't need to implement the destructor.
 * Class has the function happy_feast() which has been overridden from the parent class
 *          which is called when a VeggieMeal is purchased by a student. 
 *          It prints the vmID of the VeggieMeal and something that a VeggieMeal has.
 *******************************************************************************************/

class VeggieMeal: public Meal {
    public:
        //Member variables
        int vmID;

        //Constructor
        VeggieMeal(int p, int id) : Meal("Veggie", p), vmID(id){ }
        ~VeggieMeal();        
        //Functions
        virtual void happy_feast() {
            cout << "VeggieMeal["<<this->vmID<<"]: I am filled with veggies and avocados! Nom Nom" << endl;
        }
};

/********************************************************************************************
 * This is the BaconMeal class derived from the Meal class 
 *
 * Every BaconMeal has a specific bmID. 
 * The Steak constructor has been implemented for you.
 * You don't need to implement the destructor.
 * Class has the function happy_feast() which has been overridden from the parent class
 *          which is called when a BaconMeal is purchased by a student. 
 *          It prints the bmID of the BaconMeal and something that a BaconMeal has.
 *******************************************************************************************/
class BaconMeal: public Meal {
    public:
        //Member variables
        int bmID;

        //Constructor
        BaconMeal(int p, int id) : Meal("Bacon", p), bmID(id){ }
        ~BaconMeal();

        //Functions
        virtual void happy_feast() {
            cout << "BaconMeal["<<this->bmID<<"]: I am a Bacon Quiche! Yum!" << endl;
        }
};


/********************************************************************************************
 * YOU HAVE TO IMPLEMENT ONLY THE DINING HALL CLASS FOR WHICH THE SKELETON HAS BEEN GIVEN
 * YOU DON'T NEED TO TOUCH ANY OF THE ABOVE CLASSES.
 *******************************************************************************************/



/********************************************************************************************
 * This is the DiningHall class.  
 *
 * Before you implement any of the functions of this class, 
 *      YOU NEED TO figure out what kind of data structure(s) you will need to simulate this dining hall.
 *
 * We have given you the function definitions. You are not allowed to change any function definitions. 
 * You need to only fill in code wherever it says "Your Code Here"
 *******************************************************************************************/
class DiningHall {
    public:
        //YOUR CODE HERE
        //Declare your data structures for VeggieMeals and BaconMeals
        queue<Meal*>  baconMeals;
        stack<Meal* > veggieMeals;

        void addMeal(Meal* a);
        Meal* giveVeggieMeal();
        Meal* giveBaconMeal();
        Meal* giveAnyMeal();
};


/*******************************************************************************************
 * This function is called when you want to add meals to your dining hall.
 * 
 * Parameters: Pointer of type Meal 
 * Return Type: void
 * *****************************************************************************************/
void DiningHall::addMeal(Meal* a) {
    if (a != NULL) {
        if (a->type == "Veggie") {
            veggieMeals.push(a);
        } else if (a->type == "Bacon") {
            baconMeals.push(a);
        }
    }
}


/*******************************************************************************************
 * This function is called when a student requests for a VeggieMeal.
 * Your job is to -
 *      1. Check if there are Veggie Meals available.
 *      2. If not, cout "No more Veggie Meals" and  return NULL
 *      3. Else, get the most fresh VeggieMeal (the one with the largest/latest prep_time)
 *      4. Call the happy_feast() method on the VeggieMeal that you've chosen.
 *      5. Return the VeggieMeal that you have chosen.
 *
 * Parameters: Nothing
 * Return Value: Pointer of type Meal
 * *****************************************************************************************/
Meal * DiningHall::giveVeggieMeal() {
    if (veggieMeals.empty()) {
        cout << "No more Veggie Meals" << endl;
        return NULL;
    } else {
        Meal* meal = veggieMeals.top();
        meal->happy_feast();
        veggieMeals.pop();
        return meal;
    }
}

/*******************************************************************************************
 * This function is called when a student requests for a BaconMeal.
 * Your job is to -
 *      1. Check if there are Bacon Meals available.
 *      2. If not, cout "No more Bacon Meals" and  return NULL
 *      3. Else, get the earliest prepared BaconMeal (the one with the smallest/earliest prep_time)
 *      4. Call the happy_feast() method on the BaconMeal that you've chosen.
 *      5. Return the BaconMeal that you have chosen.
 *
 * Parameters: Nothing
 * Return Value: Pointer of type Meal
 * *****************************************************************************************/
Meal * DiningHall::giveBaconMeal() {
    if (baconMeals.empty()) {
        cout << "No more Bacon Meals" << endl;
        return NULL;
    } else {
        Meal* meal = baconMeals.front();
        meal->happy_feast();
        baconMeals.pop();
        return meal;
    }
}

/*******************************************************************************************
 * This function is called when a student requests for any meal. Not specifically Veggie or Bacon.
 * Your job is to -
 *      1. Check if there are any more meals available.
 *      2. If not, cout "No more Meals" and  return NULL
 *      3. Else, 
 *          IF there are more Veggie meals than Bacon meals, give out the most fresh veggie meal (one with the latest/largest prep_time)
 *          IF the number of Veggie meals is less than or equal to the number of Bacon meals, return the earliest prepared Bacon Meal (one with the smallest/earliest prep_time)
 *      4. Call the happy_feast() method on the meal that you've chosen.
 *      5. Return the meal that you have chosen.
 *
 * Parameters: Nothing
 * Return Value: Pointer of type Meal
 * *****************************************************************************************/
Meal * DiningHall::giveAnyMeal() {
    if (veggieMeals.empty() && baconMeals.empty()) {
        cout << "No more Meals" << endl;
        return NULL;
    } else if (veggieMeals.size() > baconMeals.size()) {
        Meal* meal = veggieMeals.top();
        meal->happy_feast();
        veggieMeals.pop();
        return meal;
    } else {
        Meal* meal = baconMeals.front();
        meal->happy_feast();
        baconMeals.pop();
        return meal;
    }
}

/*********************************************************************************************
 * A main function has been provided that simulates an dining hall.
 * You may use it to test your code.
 * Currently the main function randomly adds Veggie and Bacon meals and makes them ready for distribution.
 * Also, it randomly asks for meals. whether the student needs a veggie, a bacon or any meal of the two.
 *
 * You may edit the main function to write your own test cases.
 * *******************************************************/        
int main() {

    srand(time(NULL));

    int random;
    int pt = 0, id = 0, count= 0;
    DiningHall dh;
    
    cout << "Adding Meals ----------------------------------------- " << endl;
    for(count = 0; count < 10; count++) {
        random = rand()%2;          //Flip a coin between 0, 1
        if(random == 0) {
            VeggieMeal *v = new VeggieMeal(id, pt);
            dh.addMeal(v);
        } else {
            BaconMeal *b = new BaconMeal(id, pt);
            dh.addMeal(b);
        }
        id++;
        pt++;
    }
    
    cout << "Distributing Meals ---------------------------------------- " << endl;
    Meal *ml;
    for (count = 0; count < 15; count ++) {
        random = rand()%3;          //Choose a random number between 0, 1, 2
        if(random == 0) {
            ml = dh.giveAnyMeal();
        } else if(random == 1) {
            ml = dh.giveVeggieMeal();
        } else {
            ml = dh.giveBaconMeal();
        }
        sleep(0.25);
    }    
    return 0;
}
