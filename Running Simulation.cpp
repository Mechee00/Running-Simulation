// IMPORT libraries and headers used in the program
#include <algorithm> // For sort function
#include <chrono>    // For chrono high resolution clockclock fucntion
#include <iostream>  // For cout
#include <math.h>    // For round function
#include <regex>     // For regex checking function
#include <stdlib.h>  // For the rand function
#include <string>    // For to_string function
#include <time.h>    // For srand fucntion
#include <Windows.h> // For cls, pause and SLEEP

using namespace std;         // Use for cout, cin, etc
using namespace std::chrono; // Use for highresolutionclock.

// #define CLEARSCREEN // Enable or disable clearscreen operation
#define ENABLE_MENU // Enable or disable menu module

// #define REFRESH_POS_MANUAL
#define REFRESH_POS_TIMER
// #define REFRESH_POS_SLEEP

// Movement value
#define WALK -2.00        // Default value -2.00
#define CRAWL -2.00       // Default value is -2.00
#define SLEEP 0.00        // Default value is 0.00
#define JOG 0.50          // Default value is 0.50
#define SPRINT 0.60       // Default value is 0.60
#define RUN 1.50          // Default value is 1.50
#define FAST_SPRINT 1.67; // Default value is 1.670

#define MIN_RUNNER_COUNT 2       // Default value is 2
#define MAX_RUNNER_COUNT 9       // Default value is 9
#define MIN_TRACK_LENGTH 10      // Default value is 10
#define MAX_TRACK_LENGTH 100     // Default value is 100
#define MIN_REFRESH_INTERVAL 250 // Default value is 250

int Runner_Count = 2;        // Default value is 2
int Track_Length = 60;       // Default value is 60
int Refresh_Interval = 1000; // Default value is 1000

int Winner_Index = 0;                      // Winner index
int Winner_Count = 0;                      // Counter for winner
int Winner[MAX_RUNNER_COUNT] = {0};        // Winners' index [Winner's Position]
int Gotcha_Count = 0;                      // Counter for gotcha
int Gotcha_Group_Count = 0;                // Counter for gotcha group
int Gotcha_Group_Index = 0;                // Gotcha index counter
int Gotcha_Group[MAX_RUNNER_COUNT / 2][2]; // Gotcha's index [Position Group][0:Position of gotcha, 1:Count of runner]

double Pos_Sample = 0; // Position sample for searching

double Runner_Pos[MAX_RUNNER_COUNT] = {0};      // To store Runner's position
double Runner_Pos_Sort[MAX_RUNNER_COUNT] = {0}; // Mirror of runner's position use for sorting and searching

#pragma region Done

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
    switch (randomInt(1, 7)) // Random Action Code from 1 to 7
    {
    case 1: // WALK
        return WALK;
        break;
    case 2: // CRAWL
        return CRAWL;
        break;

    case 3: // SLEEP
        return SLEEP;
        break;

    case 4: // JOG
        return JOG;
        break;

    case 5: // SPRINT
        return SPRINT;
        break;

    case 6: // RUN
        return RUN;
        break;

    case 7: // FAST SPRINT
        return FAST_SPRINT;
        break;

    default:
        break;
    }
}

void UpdateRunnerPos()
{
    fill(begin(Runner_Pos_Sort), end(Runner_Pos_Sort), 0);
    fill(begin(Winner), end(Winner), 0);
    Winner_Count = 0;
    Winner_Index = 0;

    for (int i = 0; i < Runner_Count; i++) // Loop for all the runners
    {
        int Time_Sum = 0, Time_Percent = 0;
        double prevAction = 0, theAction = 0;
        do // Loop for 1 cycle of time
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

            Runner_Pos[i] += theAction * Time_Percent; // Multiply action and time percent to get movement

        } while (Time_Sum < 10); // Loop until the time is 100%

        Runner_Pos[i] = (Runner_Pos[i] < 0) ? 0 : Runner_Pos[i]; // Set to 0 if runner pos is lower than 0
        Runner_Pos[i] = round(Runner_Pos[i]);

        if (Runner_Pos[i] >= Track_Length) // If the runner pos is 40 or greater then
        {
            Runner_Pos[i] = Track_Length; // Set to runner pos to max
            Winner_Count += 1;            // Increase winner counter
            Winner[Winner_Index] = i + 1;
            Winner_Index++; // Increase the index number for next winner
        }
        Runner_Pos_Sort[i] = Runner_Pos[i];
    }
}

void ShowRunnerPos()
{
    for (int i = 0; i < Runner_Count; i++) // For the runners
    {
        cout << "Runner " << to_string(i + 1) << " : ";
        for (int x = 0; x <= Track_Length; x++) // For the track lengths
        {
            string pos;
            if (x == Runner_Pos[i])
            {
                pos = to_string(i + 1);
            }
            else
            {
                pos = '_';
            }
            cout << pos;
        }
        cout << endl;
    }
}

void ShowPosMark(int catched_up_pos = 0)
{
    cout << "Square   : 0";
    for (int i = 1; i <= Track_Length; i++)
    {
        if (i == Track_Length) // Marker line pos 60
        {
            cout << to_string(Track_Length);
        }
        else // Marker line default
        {
            // cout << ((i % 5 == 0) ? "^" : ".");
            cout << ((i % 10 == 5) ? "^" : ((i % 10 == 0) ? to_string(i / 10) : "-"));
        }
    }
    cout << "\n\n";
}

#pragma endregion Done

void ShowGotchaWinner()
{

    // ====================================================================================SHOW GOTCHA
    if (Gotcha_Group_Count >= 1)
    {

        cout << "**************************************************** GOTCHA!\n";
        cout << "FINALLY!!! " << Gotcha_Group_Count << " GOTCHA!!";
        cout << endl;
        for (int i = 0; i < Gotcha_Group_Count; i++) // Loop through each gotcha group
        {
            cout << "At square " << Gotcha_Group[i][0] << ", runner #: ";
            int x = 0;

            for (int y = 0; y < Runner_Count; y++) // Count all runner
            {
                if (x == Gotcha_Group[i][1])
                    break;
                if (Runner_Pos[y] == Gotcha_Group[i][0])
                {
                    x += 1;
                    cout << y + 1 << ((x == Gotcha_Group[i][1]) ? " GOTCHA!!!" : ((x < Gotcha_Group[i][1] - 1) ? ", " : ", and "));
                }
            }
            cout << endl;
        }
        cout << endl
             << endl;
    }
    // ====================================================================================SHOW WINNERS
    if (Winner_Count == 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n";
        cout << "CONGRATULATION! Runner #" << Winner[0] << " Wins!!!\n\n";
    }
    else if (Winner_Count > 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n";
        cout << "KUDOS to those Runner!!! We have " << Winner_Count << " winners! It is a TIE!\nRunner #: ";
        for (int i = 0; i < Winner_Count; i++)
        {
            cout << Winner[i] << ((i == Winner_Count - 1) ? "." : ((i < Winner_Count - 2) ? ", " : ", and "));
        }
        cout << endl
             << endl;
    }
}

void CheckGotcha()
{
    sort(begin(Runner_Pos_Sort), end(Runner_Pos_Sort));
    Gotcha_Group_Count = 0;
    Gotcha_Group_Index = 0;
    // =================================================Check Gotcha
    for (int i = MAX_RUNNER_COUNT - 1; i > 0; i--) // Loop Sorted Runner Position in reverse
    {
        if (Runner_Pos_Sort[i] == 0)
        {
            break;
        }
        Pos_Sample = Runner_Pos_Sort[i]; // Update Pos Sample

        for (int x = i - 1; x >= 0; x--) // Loop runner position with sample
        {
            i = x + 1;
            if (Pos_Sample == Runner_Pos_Sort[x]) // Check the element after the sample if it is the same
            {
                Gotcha_Count += 1; // Increase gotcha count
            }
            else // No more data matching sample
            {
                break;
            }
        }
        // ============================================================================================= Update Gotcha
        if (Gotcha_Count >= 1)
        {
            Gotcha_Group[Gotcha_Group_Index][0] = Pos_Sample;
            Gotcha_Group[Gotcha_Group_Index][1] = Gotcha_Count + 1;
            Gotcha_Group_Index += 1;
            Gotcha_Group_Count += 1;
        }
        Gotcha_Count = 0; // Reset Counter
    }
}

#pragma region Menu Module

void ShowMenu(bool *errorFlag)
{
#ifdef CLEARSCREEN
    system("cls");
#endif
    cout << "=================================================\n"
         << "# ( Running Competition Simulator - Main Menu ) #\n"
         << "=================================================\n"
         << "\n-------------------------------------\n\n"
         << "[R] Set Runner Number         Current: " << Runner_Count << endl
         << "[T] Set Track Length          Current: " << Track_Length << endl
         << "[P] Set Refresh Interval(ms)  Current: " << Refresh_Interval << endl
         << "[S] START THE RACE!\n"
         << "\n-------------------------------------\n\n"
         << ((*errorFlag == true) ? "Invalid input. Please enter R, T, P, or S only.\n\n" : "")
         << "Enter an option: ";
}

void ShowSubMenu(char menu, bool errorFlag1, bool errorFlag2)
{
#ifdef CLEARSCREEN
    system("cls");
#endif
    string valueError1, valueError_Min, valueError_Max,
        valueError2 = " or more than ",
        valueError3 = "!\n";

    if (errorFlag1 == true)
    {
        errorFlag2 = false;
    }

    // Header
    cout << "=================================================\n"
         << "# ( Running Competition Simulator - Sub Menu ) #\n"
         << "=================================================\n"
         << "\n-------------------------------------\n\n";

    // Content
    switch (toupper(menu))
    {
    case 'R': // [R] Set Runner Number
        cout << "[R] ( Edit Runner Number )\n"
             << "Current Value: " << Runner_Count << endl;
        // valueError = "The runner number cannot be less than " + MIN_RUNNER_COUNT + "!\n";
        valueError1 = "The runner number cannot be less than ";
        valueError_Min = to_string(MIN_RUNNER_COUNT);
        valueError_Max = to_string(MAX_RUNNER_COUNT);

        break;

    case 'T': // [T] Set Track Length
        cout << "[T] ( Set Track Length )\n"
             << "Current Value: " << Track_Length << endl;
        valueError1 = "The track length cannot be less than ";
        valueError_Min = to_string(MIN_TRACK_LENGTH);
        valueError_Max = to_string(MAX_TRACK_LENGTH);

        break;

    case 'P': // [P] Set Refresh Interval(ms)
        cout << "[P] ( Set Refresh Interval(ms) )\n"
             << "Current Value: " << Refresh_Interval << endl;
        valueError1 = "The refresh interval cannot be less than ";
        valueError_Min = to_string(MIN_REFRESH_INTERVAL);
        valueError_Max = "";
        valueError2 = "";
        break;

    default:
        break;
    }
    valueError1.append(valueError_Min);
    valueError1.append(valueError2);
    valueError1.append(valueError_Max);
    valueError1.append(valueError3);
    // Footer

    cout
        << "\n-------------------------------------\n\n"
        << ((errorFlag1 == true) ? "Invalid Input. Please enter integers ONLY.\n\n" : "")
        << ((errorFlag2 == true) ? valueError1 : "")
        << "\nEnter a value: ";
}

bool CheckInput(string *input, bool *errorFlag, string type = "menu")
{
    regex options;
    if (type == "menu")
    {
        options = "([rtpsRTPS]{1})";
    }
    else if (type == "value")
    {
        options = "([0-9]+)";
    }

    getline(cin, *input);

    if (regex_match(*input, options)) // Input is valid
    {
        *errorFlag = false;
    }
    else // Input is invalid
    {
        *errorFlag = true;
    }

    return *errorFlag;
}

bool CheckValue(string *input, char menu, bool *errorFlag)
{
    switch (toupper(menu))
    {
    case 'R': // [R] Set Runner Number
        *errorFlag = ((stoi(*input) < MIN_RUNNER_COUNT) || (stoi(*input) > MAX_RUNNER_COUNT)) ? true : false;
        break;

    case 'T': // [T] Set Track Length
        *errorFlag = ((stoi(*input) < MIN_TRACK_LENGTH) || (stoi(*input) > MAX_TRACK_LENGTH)) ? true : false;
        break;

    case 'P': // [P] Set Refresh Interval(ms)
        *errorFlag = (stoi(*input) < MIN_REFRESH_INTERVAL) ? true : false;
        break;

    default:
        *errorFlag = true;
        break;
    }
    return *errorFlag;
}

void UpdateValues(char option, int value)
{
    switch (toupper(option))
    {
    case 'R': // Runner Count
        Runner_Count = value;
        break;

    case 'T': // Track Length
        Track_Length = value;
        break;

    case 'P': // Refresh Interval
        Refresh_Interval = value;
        break;
    default:
        break;
    }
}

void MenuModule()
{
    bool inputError = false, valueError = false;
    string userIn_Option, userIn_Value;
    do // The Menu
    {
        // ================== Main Menu
        do
        {
            ShowMenu(&inputError);                                 // Show the Main Menu
        } while (CheckInput(&userIn_Option, &inputError, "menu")); // Check the input and loop if invalid
        // ================== Main Menu

        if (toupper(userIn_Option[0]) == 'S')
            break;

        // ================== Sub Menu
        do
        {
            ShowSubMenu(userIn_Option[0], inputError, valueError);                                                                 // Show the Sub Menu
        } while ((CheckInput(&userIn_Value, &inputError, "value")) || (CheckValue(&userIn_Value, userIn_Option[0], &valueError))); // Check the input and loop if invalid
        // ================== Sub Menu

        // ================== Update Values
        UpdateValues(userIn_Option[0], stoi(userIn_Value));
        // ================== Update Values

    } while (userIn_Option[0] != 'S'); // Loop back to main menu if the input is not S
}

#pragma endregion Menu Module

void main()
{
    srand(time(NULL)); // Initialize Seed for rand function

#ifdef ENABLE_MENU // ================= Menu Module
    MenuModule();
#endif

    cout << "====================================\n"
         << "# (Running Competition Simulator ) #\n"
         << "====================================\n\n"
         << "\nThe race is about to start!\n\n";
    ShowRunnerPos();
    ShowPosMark();
    cout << endl;
    system("pause");
    cout << "BANG !!!\nAND AWAY THEY GO !!!\n\n";

START:
#ifdef CLEARSCREEN
    system("cls");
    cout << "====================================\n"
         << "# (Running Competition Simulator ) #\n"
         << "====================================\n\n";
    cout << "BANG !!!\nAND AWAY THEY GO !!!\n\n";
#else
    cout << endl;
    cout << "====================================\n"
         << "# (Running Competition Simulator ) #\n"
         << "====================================\n\n";
#endif
#ifdef REFRESH_POS_TIMER
    high_resolution_clock hrc;
    auto timeStamp = hrc.now();
#endif

    UpdateRunnerPos();  // Refresh each runner's position and update Winners at once
    ShowRunnerPos();    // Print the runners position
    ShowPosMark();      // Show the position marking
    CheckGotcha();      // Search through all runners for Gotcha
    ShowGotchaWinner(); // Show the message for Gotcha and Winners!

    if (Winner_Count >= 1)
    {
        cout << "\nThe Simulation has ended!\n\n";
        system("pause");
        return;
    }

#ifdef REFRESH_POS_MANUAL
    system("pause");
#endif

#ifdef REFRESH_POS_TIMER
    int duration;
    do // Catch the program to loop until defined interval
    {
        auto currentTime = hrc.now();
        duration = duration_cast<milliseconds>(currentTime - timeStamp).count();
    } while (duration < Refresh_Interval);
#endif

#ifdef REFRESH_POS_SLEEP
    Sleep(Refresh_Interval); // Stop the process for n amount of milliseconds
#endif

    goto START;
}