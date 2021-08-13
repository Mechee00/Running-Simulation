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
#include <math.h>
#include <iomanip>
#include <time.h>

#include <Windows.h>

using namespace std;
using namespace std::chrono;

#define TOTAL_SQUARE_COUNT 40

// Set the delayh duration, CHOOSE ONLY 1 between DEBUG_MANUAL_UPDATE or TIMER or SLEEP
#define DELAY_DURATION 1000
// #define DEBUG_MANUAL_UPDATE
#define TIMER
// #define SLEEP

// Overide random position with static value
// #define DEBUG_MANUAL_RUNNER_POS
// #define DEBUG_SHOW_POS_VALUE
#define MANUAL_RUNNER_1_POS 57
#define MANUAL_RUNNER_2_POS 57

// Movement value
#define Walk -2.00
#define Crawl -2.00
#define Sleep 0.00
#define Jog 0.50
#define Sprint 0.60
#define Run 1.50
#define Fast_Sprint 1.67;

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
        return Walk;
        break;
    case 2: // Crawl
        return Crawl;
        break;

    case 3: // Sleep
        return Sleep;
        break;

    case 4: // Jog
        return Jog;
        break;

    case 5: // Sprint
        return Sprint;
        break;

    case 6: // Run
        return Run;
        break;

    case 7: // Fast Sprint
        return Fast_Sprint;
        break;

    default:
        break;
    }
}

void UpdateRunnerPos(double *runner)
{
    int Time_Sum = 0, Time_Percent = 0;
    double prevAction = 0, theAction = 0;

    do // Loop for finding activity and percent of time  for 1 cycle
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

        *runner += theAction * Time_Percent; // Multiply action and time percent to get movement

    } while (Time_Sum < 10);

    *runner = (*runner < 0) ? 0 : *runner;                                   // Set to 0 if runner pos is lower than 0
    *runner = (*runner > TOTAL_SQUARE_COUNT) ? TOTAL_SQUARE_COUNT : *runner; // Set to max if runner pos is greater than max
}

void ShowRunnerPos(double *runnerPos, string runner_num)
{
    cout << "Runner " << runner_num << " : ";
    for (int i = 0; i <= TOTAL_SQUARE_COUNT; i++)
    {
        string pos;
        if (i == *runnerPos)
        {
            pos = runner_num;
        }
        else
        {
            pos = '_';
        }
        cout << pos;
    }
    cout << endl;
}

void ShowPosMark(double *runner1_var, double *runner2_var)
{
    cout << "Position : ";
    for (int i = 0; i <= TOTAL_SQUARE_COUNT; i++)
    {
        if (i == 0) // Marker line pos 0
        {
            cout << 0;
        }
        else if (i == TOTAL_SQUARE_COUNT) // Marker line pos 60
        {
            cout << TOTAL_SQUARE_COUNT;
        }
        else if ((*runner1_var == *runner2_var) && (*runner1_var == i)) // Marker line both runner same pos
        {
            cout << "GOTCHA!";
            i += 6;
        }
        else // Marker line default
        {
            cout << ((i % 5 == 0) ? "^" : ".");
        }
    }
    cout << "\n\n";
}

bool ShowWinner(double *runner1, double *runner2)
{
    // Check if there is a winner or a TIE
    if ((*runner1 == TOTAL_SQUARE_COUNT) && (*runner2 == TOTAL_SQUARE_COUNT))
    {
        cout << "\n===== IT IS A TIE! =====\n"; // Both runner arrived at 60 at once !
        return true;
    }
    else if (*runner1 == TOTAL_SQUARE_COUNT)
    {
        cout << "\n===== # RUNNER 1 WINS! # =====\n"; // Runner 1 arrived at 60!
        return true;
    }
    else if (*runner2 == TOTAL_SQUARE_COUNT)
    {
        cout << "\n===== # RUNNER 2 WINS! # =====\n";
        return true;
    }
    else
    {
        return false;
    }
}

void main()
{
    srand(time(NULL)); // Initialize Seed for rand function

    high_resolution_clock hrc;

    // DECLARE variables
    double Pos_Runner_1 = 0, Pos_Runner_2 = 0; // Keep track of the Runner 1,2 position
    int duration;

    cout << "++++++++++ (Running Competition Simulator )++++++++++\n";
    cout << "\nThe race is about to start!\n\n";
    ShowRunnerPos(&Pos_Runner_1, "1");
    ShowRunnerPos(&Pos_Runner_2, "2");
    ShowPosMark(&Pos_Runner_1, &Pos_Runner_2);
    cout << endl;
    system("pause");
START:
    system("cls");
#ifdef TIMER
    auto timeStamp = hrc.now();
#endif

#ifdef DEBUG_MANUAL_RUNNER_POS
    Pos_Runner_1 = MANUAL_RUNNER_1_POS;
    Pos_Runner_2 = MANUAL_RUNNER_2_POS;
#else
    UpdateRunnerPos(&Pos_Runner_1);
    UpdateRunnerPos(&Pos_Runner_2);

    Pos_Runner_1 = round(Pos_Runner_1);
    Pos_Runner_2 = round(Pos_Runner_2);
#endif

    cout << "++++++++++ (Running Competition Simulator )++++++++++\n"
         << "!!!!!!!!!!!!!!!!!!!! ( BANG !!! ) !!!!!!!!!!!!!!!!!!!!\n"
         << "!!!!!!!!!!!!!! ( AND AWAY THEY GO !!! ) !!!!!!!!!!!!!!\n\n";

    ShowRunnerPos(&Pos_Runner_1, "1");
    ShowRunnerPos(&Pos_Runner_2, "2");
    ShowPosMark(&Pos_Runner_1, &Pos_Runner_2);

#ifdef DEBUG_SHOW_POS_VALUE
    cout << "\nRunner 1 Pos: " << Pos_Runner_1
         << "\nRunner 2 Pos: " << Pos_Runner_2
         << endl;
#endif

    if (ShowWinner(&Pos_Runner_1, &Pos_Runner_2))
    {
        cout << "\nThe Race has ended.\n\n";
        system("pause");
        return;
    }

#ifdef DEBUG_MANUAL_UPDATE
    system("pause");
#endif

#ifdef TIMER
    // Catch the program to loop until defined interval
    do
    {
        auto currentTime = hrc.now();
        duration = duration_cast<milliseconds>(currentTime - timeStamp).count();
    } while (duration < DELAY_DURATION);
#endif

#ifdef SLEEP
    Sleep(DELAY_DURATION);
#endif

    goto START;
}
