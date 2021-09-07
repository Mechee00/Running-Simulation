// IMPORT libraries and headers used in the program
#include <algorithm> // For sort function
#include <chrono>    // For chrono high resolution clock fucntion
#include <iostream>  // For cout
#include <math.h>    // For round function
#include <regex>     // For regex checking function
#include <stdlib.h>  // For the rand function
#include <string>    // For to_string function
#include <time.h>    // For srand fucntion
#include <Windows.h> // For cls, pause and SLEEP

using namespace std;         // Use for cout, cin, etc
using namespace std::chrono; // Use for highresolutionclock.

#define CLEARSCREEN // Enable or disable clearscreen operation
#define ENABLE_MENU // Enable or disable menu module
// #define NOENDING    // Enable or disable the program to end when there is a winner
// #define POSITION_OVERRIDE      // Enable to manually override the position of each runner in the array
// #define TIME_ACTIVITY_OVERRIDE // Enable to manually override the activity and action data

// MUST CHOOSE 1 and ONLY 1
// #define REFRESH_POS_MANUAL // Manualy press any key to refresh the runner position
#define REFRESH_POS_TIMER // Using chrono time fucntion to time the program
// #define REFRESH_POS_SLEEP  // Using Sleep() funciton to halt the program

// Movement value
const double WALK = -2.00;       // Default value -2.00
const double CRAWL = -2.00;      // Default value is -2.00
const double SLEEP = 0.00;       // Default value is 0.00
const double JOG = 0.50;         // Default value is 0.50
const double SPRINT = 0.60;      // Default value is 0.60
const double RUN = 1.50;         // Default value is 1.50
const double FAST_SPRINT = 1.67; // Default value is 1.670

const int MIN_RUNNER_COUNT = 2;       // Default value is 2
const int MAX_RUNNER_COUNT = 9;       // Default value is 9
const int MIN_TRACK_LENGTH = 10;      // Default value is 10
const int MAX_TRACK_LENGTH = 100;     // Default value is 100
const int MIN_REFRESH_INTERVAL = 250; // Default value is 250

#ifdef POSITION_OVERRIDE
const double POSITION[MAX_RUNNER_COUNT] = {0,  // Position value for runner 1
                                           1,  // Position value for runner 2
                                           2,  // Position value for runner 3
                                           3,  // Position value for runner 4
                                           4,  // Position value for runner 5
                                           5,  // Position value for runner 6
                                           6,  // Position value for runner 7
                                           7,  // Position value for runner 8
                                           8}; // Position value for runner 9

#endif

#ifdef TIME_ACTIVITY_OVERRIDE
const double ACTIVITY_TIME[MAX_RUNNER_COUNT][10][2] = {
    {{6, FAST_SPRINT}, {1, CRAWL}, {2, SLEEP}, {1, SPRINT}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, // Time % and Action value for Runner 1 [.][.][0] = time % ; [.][.][1] = activity
    {{6, FAST_SPRINT}, {1, CRAWL}, {2, SLEEP}, {1, SPRINT}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}, // Time % and Action value for Runner 2 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 3 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 4 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 5 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 6 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 7 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 8 [.][.][0] = time % ; [.][.][1] = activity
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},                       // Time % and Action value for Runner 9 [.][.][0] = time % ; [.][.][1] = activity
};
#endif

// Variable that can be changed in Menu module
int Runner_Count = 2;        // Default value is 2
int Track_Length = 60;       // Default value is 60
int Refresh_Interval = 1000; // Default value is 1000

// Variables to store and keep track of the winner and gotcha runner
int Winner_Count = 0;                           // Counter for winner
int Winner[MAX_RUNNER_COUNT] = {0};             // Winners' index [Winning Runner Number]
int Gotcha_Count = 0;                           // Counter for gotcha
int Gotcha_Group_Count = 0;                     // Counter for gotcha group
int Gotcha_Group[MAX_RUNNER_COUNT / 2][2];      // Gotcha's index [Position Group][0:Position of gotcha, 1:Count of runner]
double Pos_Sample = 0;                          // Position sample for searching
double Runner_Pos[MAX_RUNNER_COUNT] = {0};      // To store Runner's position
double Runner_Pos_Sort[MAX_RUNNER_COUNT] = {0}; // Mirror of runner's position use for sorting and searching

#pragma region Core Function

// #    Function 	:   randomInt
// #    Description	:   generate random integer in range of min and max value
// #    Argument	:   minInt: minimum range ; maxInt: maximum maximum range
// #    Return		:   generated random integer in range
int randomInt(int minInt = 0, int maxInt = 0)
{
    if (minInt == 0 && maxInt == 1) // Check if the desired range is between 0 and 1
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

// #    Function 	:   randomAction
// #    Description	:   generate random number and select the action tied to the number
// #    Argument	:   n/a
// #    Return		:   action value
double randomAction()
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

// #    Function 	:   UpdateRunnerPos
// #    Description	:   refresh the runner pos array with movement values
// #    Argument	:   n/a
// #    Return		:   n/a
void UpdateRunnerPos()
{
    fill(begin(Runner_Pos_Sort), end(Runner_Pos_Sort), 0); // Clear the sorted array
    fill(begin(Winner), end(Winner), 0);                   // Clear the winner array
    Winner_Count = 0;                                      // Reset the winner count

    for (int i = 0; i < Runner_Count; i++) // Loop for all runners
    {
#ifndef POSITION_OVERRIDE
        int Time_Sum = 0, Time_Percent = 0;           // Initialize the sum of %time and the %time variable to store generated random value in range
        double prevMovement = 0, currentMovement = 0; // Variable to track if the current generated action is same as previous
        int Set_Count = 0;                            // Counter for the sets of predefined data processed
        do                                            // Loop for 1 cycle of time
        {
#ifdef TIME_ACTIVITY_OVERRIDE
            Runner_Pos[i] += ACTIVITY_TIME[i][Set_Count][0] * ACTIVITY_TIME[i][Set_Count][1]; // Calculate the position based on preset data
            Time_Sum += ACTIVITY_TIME[i][Set_Count][0];                                       // Add up the time sum counter
            Set_Count++;                                                                      // Increase the counter for next set of data
#else
            Time_Percent = randomInt(1, 10 - Time_Sum); // Generate a random %time of a cycle
            Time_Sum += Time_Percent;                   // Update counter that check if a cycle is complete

            // Prevent repeating Action for different consecutive time percent
            do
            {
                currentMovement = randomAction();      // Get a random action
            } while (prevMovement == currentMovement); // Loop if the random action is same as previous action
            prevMovement = currentMovement;            // Update the previous action

            Runner_Pos[i] += currentMovement * Time_Percent; // Multiply action and time percent to get movement
#endif
        } while (Time_Sum < 10); // Loop until the %time is 100%
#else
        Runner_Pos[i] = POSITION[i]; // Manually overi
#endif
        Runner_Pos[i] = (Runner_Pos[i] < 0) ? 0 : Runner_Pos[i]; // Set to 0 if runner pos is lower than 0
        Runner_Pos[i] = round(Runner_Pos[i]);                    // Round the runner position after calculation to make a whole number

        if (Runner_Pos[i] >= Track_Length) // If the runner pos is 60 or greater then
        {
            Runner_Pos[i] = Track_Length; // Set to runner pos to max
            Winner[Winner_Count] = i + 1; // Store the runner's number who is a winner
            Winner_Count += 1;            // Increase winner counter
        }
        Runner_Pos_Sort[i] = Runner_Pos[i]; // Copy the runner's position to the sort array
    }
}

// #    Function 	:   CheckGotcha
// #    Description	:   search all the runner array for gotcha
// #    Argument	:   n/a
// #    Return		:   n/a
void CheckGotcha()
{
    sort(begin(Runner_Pos_Sort), end(Runner_Pos_Sort)); // Sort the array in accending order
    Gotcha_Group_Count = 0;                             // Reset the Gotcha Group Count

    // =================================================Check Gotcha
    // Concept of taking n term value to test the value of n+1 term
    for (int i = MAX_RUNNER_COUNT - 1; i > 0; i--) // Loop Sorted Runner Position in reverse order
    {
        if (Runner_Pos_Sort[i] == 0) // If the current runner position value is 0
        {
            break; // Break the loop and end the search
        }
        Pos_Sample = Runner_Pos_Sort[i]; // Update Position Sample

        for (int x = i - 1; x >= 0; x--) // Loop runner position with position sample
        {
            i = x + 1;                            // Sync with the outer loop
            if (Pos_Sample == Runner_Pos_Sort[x]) // Check if the Sample position is same with the next position value
            {
                Gotcha_Count += 1; // Increase gotcha count if the sample value match with the next value
            }
            else // No position value that match with the sample value
            {
                break; // End the loop
            }
        }
        // ============================================================================================= Update Gotcha
        if (Gotcha_Count >= 1) // If there is a gotcha or more
        {
            Gotcha_Group[Gotcha_Group_Count][0] = Pos_Sample;       // Store the position value where gotcha occured
            Gotcha_Group[Gotcha_Group_Count][1] = Gotcha_Count + 1; // Store how many gotcha occured at that position
            Gotcha_Group_Count += 1;                                // Update gotcha group count
        }
        Gotcha_Count = 0; // Reset gotcha counter for next position value
    }
}

// #    Function 	:   ShowSimHeader
// #    Description	:   Print the header for the simulator
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowSimHeader()
{
    cout << "====================================\n"
         << "# (Running Competition Simulator ) #\n"
         << "====================================\n\n";
}

// #    Function 	:   ShowRunnerPos
// #    Description	:   print the position for all the runner registered
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowRunnerPos()
{
    for (int i = 0; i < Runner_Count; i++) // Loop for runner 1 to Runner_Count
    {
        cout << "Runner " << to_string(i + 1) << " : "; // Print Runner and the Runner number
        for (int x = 0; x <= Track_Length; x++)         // Loop and print the line for the track length and the position of the runner
        {
            cout << ((x == Runner_Pos[i]) ? to_string(i + 1) : "_"); // Print the runner's number if the current track position match with the runner's positio value or print the "_" as the track
        }
        cout << endl; // End the line at the end of printing the whole track length
    }
}

// #    Function 	:   ShowPosMark
// #    Description	:   print the position marking for refference
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowPosMark()
{
    cout << "Square   : 0";                 // Position mark starting line
    for (int i = 1; i <= Track_Length; i++) // Loop for the track length
    {
        // Print the track length at the end of the track length or the marking for the track
        cout << ((i == Track_Length) ? to_string(Track_Length) : ((i % 10 == 5) ? "^" : ((i % 10 == 0) ? to_string(i / 10) : "-")));
        // cout << ((i == Track_Length) ? to_string(Track_Length) : ((i % 5 == 0) ? "^" : ".")); // Alternative Mark
    }
    cout << "\n\n"; // End the line after printing the postion mark
}

// #    Function 	:   ShowGotchaWinner
// #    Description	:   print the message for gotcha and winners
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowGotchaWinner()
{
    // ====================================================================================SHOW GOTCHA
    if (Gotcha_Group_Count >= 1) // Check if there is gotcha
    {
        cout << "**************************************************** GOTCHA!\n"; // Print the seperating line for Gotcha
        cout << "FINALLY!!! " << Gotcha_Group_Count << " GOTCHA!!\n";             // PPrint the gotcha message showing how many group of gotcha
        for (int i = 0; i < Gotcha_Group_Count; i++)                              // Loop through each gotcha group
        {
            cout << "At square " << Gotcha_Group[i][0] << ", runner #: "; // Print message for the gotcha position
            int x = 0;                                                    // Counter to keep track how many gotcha runner's has been found
            for (int y = 0; y < Runner_Count; y++)                        // Sequential search the runner's position
            {
                if (x == Gotcha_Group[i][1]) // If the gotcha runner count has reached, stop the sequential search loop
                    break;
                if (Runner_Pos[y] == Gotcha_Group[i][0]) // If the runner position is equal to the gotcha position value
                {
                    x += 1;                                                                                                         // Increase the counter for gotcha runner found
                    cout << y + 1 << ((x == Gotcha_Group[i][1]) ? " GOTCHA!!!" : ((x < Gotcha_Group[i][1] - 1) ? ", " : ", and ")); // Print the gotcha runner number
                }
            }
            cout << "\n"; // Print end line after finish printing all the runner gotcha at i position
        }
        cout << "\n\n"; // End the line after printing all the gotcha position and runner
    }
    // ====================================================================================SHOW WINNERS
    if (Winner_Count == 1) // Check if there is only 1 winner
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n"; // Print the seperating line for the winner
        cout << "CONGRATULATION! Runner #" << Winner[0] << " Wins!!!\n\n";        // Print the winner and the winning message
    }
    else if (Winner_Count > 1) // Else there is more than 1 winner
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n";                            // PPrint the seprating line for the winner
        cout << "KUDOS to those Runner!!! We have " << Winner_Count << " winners! It is a TIE!\nRunner #: "; // Print the multiple winner message and how many runer win at once
        for (int i = 0; i < Winner_Count; i++)                                                               // Loop through for all the winner
        {
            cout << Winner[i] << ((i == Winner_Count - 1) ? "." : ((i < Winner_Count - 2) ? ", " : ", and ")); // Print the winning runner's number
        }
        cout << "\n\n"; // End the line after printing all the winning runner's number
    }
}

#pragma endregion Core Function

#pragma region Menu Module

// #    Function 	:   ShowMenu
// #    Description	:   print the menu of options to chagne values or to start the race
// #    Argument	:   *errorFlag: keep track for the input error
// #    Return		:   n/a
void ShowMenu(bool *errorFlag)
{
#ifdef CLEARSCREEN
    system("cls"); // Clear the screen once if the clear screen operation is enabled
#endif
    // Print the menu and options available
    cout << "=================================================\n"
         << "# ( Running Competition Simulator - Main Menu ) #\n"
         << "=================================================\n"
         << "\n-------------------------------------\n\n"
         << "[R] Set Runner Number         Current: " << Runner_Count << endl
         << "[T] Set Track Length          Current: " << Track_Length << endl
         << "[P] Set Refresh Interval(ms)  Current: " << Refresh_Interval << endl
         << "[S] START THE RACE!\n"
         << "\n-------------------------------------\n\n"
         << ((*errorFlag == true) ? "Invalid input. Please enter R, T, P, or S only.\n\n" : "") // Check for error flag and decide to print the input error message
         << "Enter an option: ";
}

// #    Function 	:   ShowSubMenu
// #    Description	:   print the sub menu for changing the values
// #    Argument	:   menu: Select the menu type; errorFlag1: keep track for invalid input; errorFlag2: keep track for out of range values input
// #    Return		:   n/a
void ShowSubMenu(char menu, bool errorFlag1, bool errorFlag2)
{
#ifdef CLEARSCREEN
    system("cls"); // Clear the screen once if the clear screen operation is enabled
#endif
    //  Variables to store the strings and the error message
    string valueError1, valueError_Min, valueError_Max,
        valueError2 = " or more than ",
        valueError3 = "!\n";

    if (errorFlag1 == true) // If the input is valid
    {
        errorFlag2 = false; // Turn off the flag for value not in range so it is always printing only 1 type of error message
    }

    // Header
    cout << "=================================================\n"
         << "# ( Running Competition Simulator - Sub Menu ) #\n"
         << "=================================================\n"
         << "\n-------------------------------------\n\n";

    // Decide the message content for different menu
    switch (toupper(menu))
    {
    case 'R': // [R] Set Runner Number
        cout << "[R] ( Edit Runner Number )\n"
             << "Current Value: " << Runner_Count << endl;
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
    // Combine all the strings
    valueError1.append(valueError_Min);
    valueError1.append(valueError2);
    valueError1.append(valueError_Max);
    valueError1.append(valueError3);

    // Print out the rest of the message and the combined string
    cout
        << "\n-------------------------------------\n\n"
        << ((errorFlag1 == true) ? "Invalid Input. Please enter integers ONLY.\n\n" : "")
        << ((errorFlag2 == true) ? valueError1 : "")
        << "\nEnter a value: ";
}

// #    Function 	:   CheckInput
// #    Description	:   check the input from the user for the main menu
// #    Argument	:   *input: variable to store the input; *errorFlag: variable to keep track for input error; type: "menu" or "value" type for selecting checking algorithm
// #    Return		:   true for invalid; false for valid
bool CheckInput(string *input, bool *errorFlag, string type = "menu")
{
    regex options;      // Initiate varaible to store the regex sample
    if (type == "menu") // If menu type is selected
    {
        options = "([rtpsRTPS]{1})"; // Set the sample for menu as the regex sample
    }
    else if (type == "value") // If the value type is selected
    {
        options = "([0-9]+)"; // Set the sample for value as the regex sample
    }

    getline(cin, *input); // Read the user input

    if (regex_match(*input, options)) // If the input has mateched the regex
    {
        *errorFlag = false; // input is valid and turn off the error flag
    }
    else // User input does not match with regex sample
    {
        *errorFlag = true; // Turn on the error flag as input is invalid
    }

    return *errorFlag; // Return the error flag status
}

// #    Function 	:   CheckValue
// #    Description	:   check the values entered by the user and see if it is in range or not
// #    Argument	:   *input: variable to store the input; menu: char to select which value range to check; *errorFlag: variable to keep for values validity
// #    Return		:   true for out of range; false for valu in range
bool CheckValue(string *input, char menu, bool *errorFlag)
{
    switch (toupper(menu)) // Sweitch case to decide value range to check base on menu
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
    return *errorFlag; // Return the error flag status for value in range or not in range
}

// #    Function 	:   CheckInput
// #    Description	:   check the input from the user for the main menu
// #    Argument	:   option: select which option to update; value: the value to be stored for the selected option
// #    Return		:   n/a
void UpdateValues(char option, int value)
{
    switch (toupper(option)) // Switch case to select which variable should the value be stored in
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

// #    Function 	:   MenuModule
// #    Description	:   whole menu function including showing menu checking inpui, value and updating values
// #    Argument	:   option: select which option to update; value: the value to be stored for the selected option
// #    Return		:   n/a
void MenuModule()
{
    bool inputError = false, valueError = false; // Error flag of invalid input for menu and the value for sub menu
    string userIn_Option, userIn_Value;          // String to store the user input for menu option and value
    do                                           // The Menu
    {
        // ==================================== Main Menu
        do
        {
            ShowMenu(&inputError);                                 // Show the Main Menu
        } while (CheckInput(&userIn_Option, &inputError, "menu")); // Loop to show the main menu until input is valid

        if (toupper(userIn_Option[0]) == 'S') // Exit the loop if it is S
            break;
        // ==================================== Sub Menu
        do
        {
            ShowSubMenu(userIn_Option[0], inputError, valueError);                                                                 // Show the Sub Menu
        } while ((CheckInput(&userIn_Value, &inputError, "value")) || (CheckValue(&userIn_Value, userIn_Option[0], &valueError))); // Check the input and loop if invalid
        // ==================================== Update Values
        UpdateValues(userIn_Option[0], stoi(userIn_Value));
    } while (userIn_Option[0] != 'S'); // Loop back to main menu if the input is not S
}

#pragma endregion Menu Module

void main() // The main program
{
    srand(time(NULL));                                                                                                // Initialize Seed for rand function
    SetConsoleTitleA("BACS1014-Assignment ( Running Competition Simulator ) Code by Cheah Pin Chee & Lim Ler Shean"); // Set Console Title
Restart:
#ifdef ENABLE_MENU // ================= Menu Module
    MenuModule();
#endif

    ShowSimHeader();                              // Print the header
    cout << "\nThe race is about to start!\n\n";  // Starting message
    ShowRunnerPos();                              // Show the runner's initial position at 0
    ShowPosMark();                                // Show the marking for the positons
    system("pause");                              // Promt the user for starting the race
    cout << "BANG !!!\nAND AWAY THEY GO !!!\n\n"; // Print the started message

START:
#ifdef CLEARSCREEN                                // The message for crealscreen operation is selected
    system("cls");                                // Clear the system screen
    ShowSimHeader();                              // Print the header
    cout << "BANG !!!\nAND AWAY THEY GO !!!\n\n"; // Print the started mesage
#else                                             // The message for no clearscreen
    cout << endl;                    // Print end line
    ShowSimHeader();                 // Print the header
#endif

#ifdef REFRESH_POS_TIMER        // Initialize and get a time stamp with chrono high resolution clock if Timer operation is selected
    high_resolution_clock hrc;  // Use hrc as high_resolution_clock
    auto timeStamp = hrc.now(); // Get a time stamp
#endif

    UpdateRunnerPos();  // Refresh each runner's position and update Winners at once
    ShowRunnerPos();    // Print the runners position
    ShowPosMark();      // Show the position marking
    CheckGotcha();      // Search through all runners for Gotcha
    ShowGotchaWinner(); // Show the message for Gotcha and Winners!

#ifndef NOENDING           // Disable the algorithm to end the program when winner count is more than 0
    if (Winner_Count >= 1) // Check if the winner count is more than 0
    {
        cout << "\nThe Simulation has ended!\n"
             << " Restart the Simulation? Y: Restart | (Anything other than 'Y'): End program: "; // Show the program end message
        string userInput_Restart;                                                                 // String to store the user input for restarting the simulator
        getline(cin, userInput_Restart);                                                          // Read user's input
        if (userInput_Restart == "y" || userInput_Restart == "Y")
        {
            system("cls"); // Clear the screen for simulator
            goto Restart;  // Return to main menu
        }
        else
        {
            system("pause"); // Prompt user to input any key to continue
            return;          // End the main() function
        }
    }
#endif

#ifdef REFRESH_POS_MANUAL // Promt the user for the next loop to refresh the runner's position and more when manual operation is selected
    system("pause");
#endif

#ifdef REFRESH_POS_TIMER // Loop and check if the difference between current time and time stamp is greater than the predefined interval when timer operation is selected
    int duration;
    do // Catch the program to loop until pre-defined interval
    {
        auto currentTime = hrc.now();                                            // Get the current time
        duration = duration_cast<milliseconds>(currentTime - timeStamp).count(); // Calculate the duration elapsed since the initial time stamp
    } while (duration < Refresh_Interval);                                       // Check if the duration elapsed is greater than the refresh interval
#endif

#ifdef REFRESH_POS_SLEEP     // Intstruct the program to halt for predefined interval if the sleep operation is selected
    Sleep(Refresh_Interval); // Stop the process for n amount of milliseconds
#endif
    goto START; // Loop back to the point labelled with START
}