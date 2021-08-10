// Template of UDF description

// ##################
// #    Function 	:   function name lah
// #    Description	:   Bla Bla bla
// #    Argument	:   what you pass to the function
// #    Return		:   what the function return
// ##################

// IMPORT library or header used in the program
#include <chrono> // Needed for the clock fucntion
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <time.h>

#include <Windows.h>

using namespace std;
using namespace std::chrono;

#define FREQUENCY 1
#define TOTAL_SQUARE_COUNT 60

#define DEBUG
// DEFINE constants used in the program

// ##################
// #    Function 	:   randomInt
// #    Description	:   Generate random integer in range of min and max value
// #    Argument	:   minInt: minimum range ; maxInt: maximum maximum range
// #    Return		:   generated random integer in range
// ##################
int randomInt(int minInt = 0, int maxInt = 0)
{
    if (minInt == 0 && maxInt == 1)
    {
        // Generate 0 and 1 at random
        return int(round(double(rand() % 10) / 10));
    }
    else
    {
        // Generate number between min and max
        return rand() % maxInt + minInt;
    }
}

double Action()
{
    int Action_Code = randomInt(1, 7); // Randome Action Code from 1 to 7

    switch (Action_Code)
    {

    case 1: // Walk
    case 2: // Crawl
        return -2.00;
        break;

    case 3: // Sleep
        return 0.00;
        break;

    case 4: // Jog
        return 0.50;
        break;

    case 5: // Sprint
        return 0.60;
        break;

    case 6: // Run
        return 1.50;
        break;

    case 7: // Fast Sprint
        return 1.67;
        break;

    default:
        break;
    }
}

void UpdateRunnerPos(double *runner)
{
    int Time_Sum = 0, Time_Percent = 0;
    double prevAction = 0, theAction = 0;
    do
    {
        Time_Percent = randomInt(1, 10 - Time_Sum); // Generate a random percent of a cycle
        Time_Sum += Time_Percent;                   // Update counter that check if a cycle is complete

        //++++++++++++++++++++++++++++++++++++++++++++++++++ Prevent repeating Action for different consecutive time percent
        do
        {
            theAction = Action();          // Get a random action
        } while (prevAction == theAction); // Loop if the random action is same as previous action
        prevAction = theAction;            // Update the previous action
        //++++++++++++++++++++++++++++++++++++++++++++++++++ Prevent repeating Action for different consecutive time percent

        *runner += theAction * Time_Percent;

    } while (Time_Sum < 10);
}

void main()
{

    srand(time(NULL)); // Initialize Seed for rand function

    high_resolution_clock hrc;

    // DECLARE variables
    int Timer_1 = 0, Timer_2 = 0;              // Keep track of how long the Runner 1,2 takes to complete
    double Pos_Runner_1 = 0, Pos_Runner_2 = 0; // Keep track of the Runner 1,2 position
    int duration;

START:
    UpdateRunnerPos(&Pos_Runner_1);
    UpdateRunnerPos(&Pos_Runner_2);

    // INPUT from user

    // PROCESS the user input computing the desired output

    // OUTPUT the final result

    // Loop for 1 Cycle

    // Start the timer
    // auto timeStamp = hrc.now();

    // cout << randomInt(0, 1) << "\t"
    cout << "\t" << setprecision(2) << fixed << showpoint
         << "Runner 1 Pos: " << Pos_Runner_1
         << "\nRunner 2 Pos: " << Pos_Runner_2
         << "\n\t"
         << "End"
         << "\t\n";
    // auto currentTime = hrc.now();

    // Catch the program to loop until defined interval
    // do
    // {
    //     auto currentTime = hrc.now();
    //     duration = duration_cast<milliseconds>(currentTime - timeStamp).count();
    // } while (duration < 1000 / FREQUENCY);

// cout << "Elapsed time in milliseconds: "
//      << chrono::duration_cast<chrono::milliseconds>(end - start).count()
//      << " ms" << endl;

// cout << "Elapsed time in seconds: "
//      << chrono::duration_cast<chrono::seconds>(end - start).count()
//      << " sec";

// LOOP the program if desired
#ifdef DEBUG
    system("pause");
    system("cls");
    goto START;
#endif
}
