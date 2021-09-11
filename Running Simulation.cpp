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

#define ENABLE_CLEAR_SCREEN // Enable or disable ENABLE_CLEAR_SCREEN operation
#define ENABLE_SETTINGS     // Enable or disable menu module
#define ENABLE_ENDLESS_LOOP // Enable or disable the program to end when there is a winner

// MUST CHOOSE 1 and ONLY 1
#define REFRESH_POS_MANUAL // Manually press any key to refresh the runner position
// #define REFRESH_POS_TIMER // Using chrono time function to time the program
// #define REFRESH_POS_SLEEP  // Using Sleep() function to halt the program

#pragma region Movement, Weather And Path Constants
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Movement value
const double WALK = -2.00;       // Default value is -2.00
const double CRAWL = -2.00;      // Default value is -2.00
const double SLEEP = 0.00;       // Default value is 0.00
const double JOG = 0.50;         // Default value is 0.50
const double SPRINT = 0.60;      // Default value is 0.60
const double RUN = 1.50;         // Default value is 1.50
const double FAST_SPRINT = 1.67; // Default value is 1.67

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Weather condition value
const int OVERCAST = 3;    // ID = 1
const int CLOUDY = 2;      // ID = 2
const int CLEAR = 1;       // ID = 3
const int SUNNY = 1;       // ID = 4
const int FOG = -1;        // ID = 5
const int SHOWER = -2;     // ID = 6
const int HEAVY_RAIN = -3; // ID = 7

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Path condition value
const int FLAT = 3;      // ID = 1
const int DOWN_HILL = 2; // ID = 2
const int GRASS = 1;     // ID = 3
const int SANDY = 1;     // ID = 4
const int ROCKY = -1;    // ID = 5
const int UP_HILL = -2;  // ID = 6
const int MUDDY = -3;    // ID = 7

const int MIN_RUNNER_COUNT = 2;           // Default value is 2
const int MAX_RUNNER_COUNT = 9;           // Default value is 9
const int MIN_TRACK_LENGTH = 10;          // Default value is 10
const int MAX_TRACK_LENGTH = 100;         // Default value is 100
const int MIN_REFRESH_INTERVAL = 250;     // Default value is 250
const int MIN_WEATHER_DURATION = 1;       // Default value is 1
const int MAX_WEATHER_DURATION = 5;       // Default value is 5
const int MIN_PATH_CONDITION_COUNT = 2;   // Default value is 2
const int MAX_PATH_CONDITION_COUNT = 10;  // Default value is 10
const int MIN_PATH_CONDITION_LENGTH = 7;  // Dedault value is 7
const int MAX_PATH_CONDITION_LENGTH = 25; // Default value is 10
#pragma endregion Movement, Weather And Path Constants

#pragma region Override Constants

// #define POSITION_OVERRIDE // Enable to manually override the position of each runner in the array
#ifdef POSITION_OVERRIDE
const double POSITION[MAX_RUNNER_COUNT] = {60,  // Position value for runner 1
                                           35,  // Position value for runner 2
                                           35,  // Position value for runner 3
                                           60,  // Position value for runner 4
                                           10,  // Position value for runner 5
                                           60,  // Position value for runner 6
                                           43,  // Position value for runner 7
                                           43,  // Position value for runner 8
                                           10}; // Position value for runner 9
#endif

// #define TIME_ACTIVITY_OVERRIDE // Enable to manually override the activity and action data
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
    {{10, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}}                        // Time % and Action value for Runner 9 [.][.][0] = time % ; [.][.][1] = activity
};
#endif

// #define WEATHER_CONTDITION_OVERRIDE // Enable to manually override the weather condition
#ifdef WEATHER_CONTDITION_OVERRIDE
int WeatherIndex_Counter = 0;
const string WEATHER_NAME[7] = {"Clear",               // Weather 1 Name
                                "Clear",               // Weather 2 Name
                                "Clear",               // Weather 3 Name
                                "Clear",               // Weather 4 Name
                                "Clear",               // Weather 5 Name
                                "Clear",               // Weather 6 Name
                                "Clear"};              // Weather 7 Name
const int WEATHER_VALUE_DURATION[7][2] = {{CLEAR, 1},  // Weather 1 Value {Value, Duration}
                                          {CLEAR, 1},  // Weather 2 Value {Value, Duration}
                                          {CLEAR, 1},  // Weather 3 Value {Value, Duration}
                                          {CLEAR, 1},  // Weather 4 Value {Value, Duration}
                                          {CLEAR, 1},  // Weather 5 Value {Value, Duration}
                                          {CLEAR, 1},  // Weather 6 Value {Value, Duration}
                                          {CLEAR, 1}}; // Weather 7 Value {Value, Duration}
#endif

// #define PATH_CONDITION_OVERRIDE // Enable to manually override the path condition factor
#ifdef PATH_CONDITION_OVERRIDE
const int PATH_COUNT = 3;
const string PATH_NAME[PATH_COUNT] = {"Grass ",      // Path 1 Name
                                      "Grass ",      // Path 2 Name
                                      "Grass "};     // Path 3 Name
const int PATH_VALUE[PATH_COUNT] = {GRASS,           // Path 1 Value
                                    GRASS,           // Path 2 Value
                                    GRASS};          // Path 3Value
const int PATH_ID_LENGTH[PATH_COUNT][2] = {{3, 20},  // Path 1 ID and Length {ID, Length}
                                           {3, 20},  // Path 2 ID and Length {ID, Length}
                                           {3, 20}}; // Path 3 ID and Length {ID, Length}

#endif
#pragma endregion Override Values

// Variable that can be changed in Menu module
int Runner_Count = 2;        // Default value is 2
int Track_Length = 60;       // Default value is 60
int Refresh_Interval = 1000; // Default value is 1000

// Variables to store and keep track of the winner and gotcha runner
int Winner_Count = 0;                            // Counter for winner
int Winner[MAX_RUNNER_COUNT] = {0};              // Winners' index [Winning Runner Number]
int Gotcha_Count = 0;                            // Counter for gotcha
int Gotcha_Group_Count = 0;                      // Counter for gotcha group
int Gotcha_Group[MAX_RUNNER_COUNT / 2][2] = {0}; // Gotcha's index [Position Group][0:Position of gotcha, 1:Count of runner]
double Pos_Sample = 0;                           // Position sample for searching
double Runner_Pos[MAX_RUNNER_COUNT] = {0};       // To store Runner's position
double Runner_Pos_Sort[MAX_RUNNER_COUNT] = {0};  // Mirror of runner's position use for sorting and searching

#ifdef ENABLE_SETTINGS
bool inputError, valueError;        // Variables for error flags
string userIn_Option, userIn_Value; // String to store the user input
#endif

struct Movement_Conditioner
{
    int Duration;                                                      // Duration for weather
    int Condition_Value[MAX_TRACK_LENGTH / MIN_PATH_CONDITION_LENGTH]; // Condition of weather
    int Length[MAX_TRACK_LENGTH / MIN_PATH_CONDITION_LENGTH][2];       // Array for each path condition's length
    int Condition_Count;                                               // Path Condition Count
    string Condition_Name;                                             // String of the condition name
} Weather = {0}, Path = {0};

#pragma region FUNCTIONS
#pragma region Processing
int randomInt(int, int);
int Generate_Weather();
void Generate_Path();
double Generate_Action();
void Generate_Runner_Movement();
void CheckGotcha();
#pragma endregion Processing

#pragma region Output
void ShowSimHeader();
void ShowWeather();
void ShowRunnerPos();
void ShowPosMark();
void ShowPath();
void ShowGotchaWinner();
void ShowOutputs();
#pragma endregion Output

#ifdef ENABLE_SETTINGS
void ShowMenu();
void ShowSubMenu();
bool CheckInput(string);
bool CheckValue();
void UpdateValues();
void MenuModule();
#endif
#pragma endregion FUNCTIONS

// ================================================================================================================================================================================ The main program
void main()
{
    srand(time(NULL));                                                                                        // Initialize Seed for rand function
    SetConsoleTitleA("BACS1014-Assignment ( Running Man Simulator ) Code by Cheah Pin Chee & Lim Ler Shean"); // Set Console Title

Restart:
#ifdef ENABLE_SETTINGS // ================= Menu Module
    MenuModule();
#endif

#ifdef ENABLE_CLEAR_SCREEN
    system("cls"); // Clear the console screen
#endif
    Generate_Path(); // Generate path condition once for the whole simulation
    ShowOutputs();   // Display all interface
    system("pause"); // Promt the user for starting the race
START:
#ifdef ENABLE_CLEAR_SCREEN
    system("cls"); // Clear the console screen
#endif

#ifdef REFRESH_POS_TIMER        // Initialize and get a time stamp with chrono high resolution clock if Timer operation is selected
    high_resolution_clock hrc;  // Use hrc as high_resolution_clock
    auto timeStamp = hrc.now(); // Get a time stamp
#endif
    Generate_Runner_Movement(); // Refresh each runner's position and update Winners at once
    ShowOutputs();              // Display all interface

#ifndef ENABLE_ENDLESS_LOOP // Disable the algorithm to end the program when winner count is more than 0
    string userIn_Restart;  // String to store the user input
    if (Winner_Count >= 1)  // Check if the winner count is more than 0
    {
        cout << "\nThe simulation has ended!\n"
             << "Restart the Simulation? Y: Restart | (Anything other than 'Y'): End program: "; // Show the program end message
        string userInput_Restart;                                                                // String to store the user input for restarting the simulator
        getline(cin, userInput_Restart);                                                         // Read user's input
        if (userInput_Restart == "y" || userInput_Restart == "Y")
        {
            fill(begin(Runner_Pos), end(Runner_Pos), 0);           // Clear the position array
            fill(begin(Runner_Pos_Sort), end(Runner_Pos_Sort), 0); // Clear the sorted position array
            fill(begin(Winner), end(Winner), 0);                   // Clear the winner array
            Winner_Count = 0;                                      // Reset the winner count
            Gotcha_Group_Count = 0;                                // Reset the Gotcha Group Count
            goto Restart;                                          // Return to main menu
        }
        else
        {
            cout << "The program will be terminated. Thank you for using the simulator.\n";
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
// ================================================================================================================================================================================ The main program

#pragma region FUNCTIONS
#pragma region Processing
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
        return minInt + (rand() % (maxInt - minInt + 1));
    }
}

// #    Function 	:   Generate_Weather
// #    Description	:   Generate weather condition and the duration of the weather
// #    Argument	:   n/a
// #    Return		:   Generated weahter value
int Generate_Weather()
{
    Weather.Duration = randomInt(MIN_WEATHER_DURATION, MAX_WEATHER_DURATION); // Generate a random duration for the weather

    switch (randomInt(1, 7)) // Generate random weather condition
    {
    case 1:
        Weather.Condition_Value[0] = OVERCAST;
        Weather.Condition_Name = "Overcast";
        break;
    case 2:
        Weather.Condition_Value[0] = CLOUDY;
        Weather.Condition_Name = "Cloudy";
        break;
    case 3:
        Weather.Condition_Value[0] = CLEAR;
        Weather.Condition_Name = "Clear";
        break;
    case 4:
        Weather.Condition_Value[0] = SUNNY;
        Weather.Condition_Name = "Sunny";
        break;
    case 5:
        Weather.Condition_Value[0] = FOG;
        Weather.Condition_Name = "Foggy";
        break;
    case 6:
        Weather.Condition_Value[0] = SHOWER;
        Weather.Condition_Name = "Shower";
        break;
    case 7:
        Weather.Condition_Value[0] = HEAVY_RAIN;
        Weather.Condition_Name = "Heavy Rain";
        break;
    default:
        break;
    }
    return Weather.Condition_Value[0]; // Return the weather condition value
}

// #    Function 	:   Generate_Path
// #    Description	:   Generate values for path lengths for each path condition
// #    Argument	:   n/a
// #    Return		:   n/a
void Generate_Path()
{
#ifdef PATH_CONDITION_OVERRIDE
    Path.Condition_Count = PATH_COUNT;             // Set path condition count
    for (int i = 0; i < Path.Condition_Count; i++) // Loop Through each path condition
    {
        Path.Length[i][0] = PATH_ID_LENGTH[i][0]; // Set ID
        Path.Length[i][1] = PATH_ID_LENGTH[i][1]; // Set Length
        Path.Condition_Value[i] = PATH_VALUE[i];  // Set Value
    }
#else
    Path.Condition_Count = 0; // Reset path condition count
    int temp = 0;             // Temporary variable to store the random number

    for (int Path_Sum = 0; Path_Sum < Track_Length; Path.Condition_Count++) // Loop until the sum of the path length is equal to the track length
    {
        do
        {
            Path.Length[Path.Condition_Count][0] = randomInt(1, 7);                                 // Get the path condition ID
        } while (Path.Length[Path.Condition_Count][0] == Path.Length[Path.Condition_Count - 1][0]); // Loop again if the path condition id is same as previous one

        temp = randomInt(MIN_PATH_CONDITION_LENGTH, MAX_PATH_CONDITION_LENGTH); // Generate a random int as path length

        if ((Track_Length - (Path_Sum + temp)) < MIN_PATH_CONDITION_LENGTH) // If the generated path length is too big for the last part
        {
            Path.Length[Path.Condition_Count][1] = (Track_Length - Path_Sum); // Merge both path lenght together (the remaining length)
            Path_Sum += (Track_Length - Path_Sum);                            // Update sum of path
        }
        else
        {
            Path.Length[Path.Condition_Count][1] = temp; // Set the path lenght to the generated value
            Path_Sum += temp;                            // Update sum of path
        }
    }
#endif
    return;
}

// #    Function 	:   Generate_Action
// #    Description	:   generate random number and select the action tied to the number
// #    Argument	:   n/a
// #    Return		:   action value
double Generate_Action()
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

// #    Function 	:   Generate_Runner_Movement
// #    Description	:   refresh the runner pos array with movement values
// #    Argument	:   n/a
// #    Return		:   n/a
void Generate_Runner_Movement()
{
    fill(begin(Runner_Pos_Sort), end(Runner_Pos_Sort), 0); // Clear the sorted array
    fill(begin(Winner), end(Winner), 0);                   // Clear the winner array
    Winner_Count = 0;                                      // Reset the winner count

    for (int i = 0; i < Runner_Count; i++) // Loop for all runners
    {
        // -------------------------------------------------------------------------------------------------Add conditioner value
        int tempPath = 0, ConditionerSum = 0; // Use for summing path legth for checking and for total conditioning value
        // cout << "Runner #--------------------------------" << i + 1 << endl;                               // Debug
        // cout << "Original: " << Runner_Pos[i] << "\n\t\tW value : " << Weather.Condition_Value[0] << endl; // Debug
        ConditionerSum += Weather.Condition_Value[0]; // Add weather condition value
        for (int x = 0; x <= Path.Condition_Count; x++)
        {
            tempPath += Path.Length[x][1]; // Sum up the path length value
            // cout << tempPath << endl; // Debug
            if (Runner_Pos[i] < 0)
            {
                break; // Break the loop if the runner position is negative
            }
            else if (Runner_Pos[i] <= tempPath) // If the runner position is in the range
            {
                // cout << "\t\tP Value : " << Path.Condition_Value[x] << endl; // Debug
                ConditionerSum += Path.Condition_Value[x]; // Add the path condition value
                break;                                     // End the loop
            }
        }
        // -------------------------------------------------------------------------------------------------Add conditioner value

#ifndef POSITION_OVERRIDE
        int Time_Sum = 0, Time_Percent = 0;           // Initialize the sum of %time and the %time variable to store generated random value in range
        double prevMovement = 0, currentMovement = 0; // Variable to compare previous and current value
        int Set_Count = 0;                            // Counter for the sets of predefined data processed
        // -------------------------------------------------------------------------------------------------Generate % time for 1 Cycle
        do // Loop for 1 cycle of time
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
                currentMovement = Generate_Action();   // Get a random action
            } while (prevMovement == currentMovement); // Loop if the random action is same as previous action
            prevMovement = currentMovement;            // Update previous action

            Runner_Pos[i] += currentMovement * Time_Percent; // Multiply action and time percent to get movement
#endif
        } while (Time_Sum < 10); // Loop until the %time is 100%
#else
        Runner_Pos[i] = POSITION[i]; // Manually overide
#endif
        // -------------------------------------------------------------------------------------------------Generate % time for 1 Cycle
        // cout << "After A: " << Runner_Pos[i] << endl; // Debug
        Runner_Pos[i] = round(Runner_Pos[i]); // Round up the position value
        Runner_Pos[i] += ConditionerSum;      // Add Conditioner Value
        // cout << "\t\tSum     : " << ConditionerSum << "\nFinal Positon: " << Runner_Pos[i] << endl; // Debug
        // -------------------------------------------------------------------------------------------------Trim the value to max and min
        Runner_Pos[i] = (Runner_Pos[i] < 0) ? 0 : Runner_Pos[i]; // Set to 0 if runner pos is lower than 0
        if (Runner_Pos[i] >= Track_Length)                       // If the runner pos is 60 or greater then
        {
            Runner_Pos[i] = Track_Length; // Set to runner pos to max
            Winner[Winner_Count] = i + 1; // Store the runner's number who is a winner
            Winner_Count += 1;            // Increase winner counter
        }
        // -------------------------------------------------------------------------------------------------Trim the value to max and min

        Runner_Pos_Sort[i] = Runner_Pos[i]; // Copy the runner's position to the sort array
    }
    CheckGotcha(); // Search through all runners for Gotcha
    return;
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
    return;
}

#pragma endregion Processing

#pragma region Output

// #    Function 	:   ShowSimHeader
// #    Description	:   Print the header for the simulator
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowSimHeader()
{
    cout << "\n# ==== BANG !!! AND AWAY THEY GO !!! ==== #\n\n"; // Print the started message
    cout << "============================\n"
         << "# (Running Man Simulator ) #\n"
         << "============================\n\n";
    return;
}

// #    Function 	:   ShowWeather
// #    Description	:   Generate and display the weather information
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowWeather()
{
    Weather.Duration--;
#ifndef WEATHER_CONTDITION_OVERRIDE
    int prevWeather = 0, currentWeather = 0; // Variable to compare previous and current value
    if (Weather.Duration <= 0)               // When duration of the counter turns 0 or below 0, generate new weather
    {
        do
        {
            currentWeather = Generate_Weather(); // Get random weather
        } while (prevWeather == currentWeather); // Loop if the random weather is same as previous weather
        prevWeather = currentWeather;            // Update previous weather
    }
    cout << "Weather  : " << Weather.Condition_Name << "\tDuration: " << Weather.Duration << endl; // Display the weather information
#else
    if (Weather.Duration < 0)        // Accounting for the frist time
    {
        Weather.Duration = WEATHER_VALUE_DURATION[WeatherIndex_Counter][1]; // Set the durration
    }
    Weather.Condition_Name = WEATHER_NAME[WeatherIndex_Counter];                                   // Set the weather name
    Weather.Condition_Value[0] = WEATHER_VALUE_DURATION[WeatherIndex_Counter][0];                  // Set the weather value
                                                                                                   //Reduce the duration by 1 per for each cycle
    cout << "Weather  : " << Weather.Condition_Name << "\tDuration: " << Weather.Duration << endl; // Display the weather information
    if (Weather.Duration == 0)
    {
        WeatherIndex_Counter++;                                                        // Increase the index
        WeatherIndex_Counter = (WeatherIndex_Counter == 7) ? 0 : WeatherIndex_Counter; // Recycle the index
    }
#endif
    return;
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
    return;
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
    cout << "\n"; // End the line after printing the postion mark
    return;
}

// #    Function 	:   ShowPath
// #    Description	:   Print the path condition labeling
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowPath()
{
    cout << "Path Cond: |";                        // Print the label
    for (int i = 0; i < Path.Condition_Count; i++) // For each path conditions
    {
        switch (Path.Length[i][0]) // Base on the value
        {
        case 1:                             // Set the value and name of the condition to FLAT
            Path.Condition_Value[i] = 3;    // Retrieve actual value base on ID
            Path.Condition_Name = "Flat  "; // Change the name of the path condition
            break;
        case 2:                             // Set the value and name of the condition to DOWN HILL
            Path.Condition_Value[i] = 2;    // Retrieve actual value base on ID
            Path.Condition_Name = "D.Hill"; // Change the name of the path condition
            break;
        case 3:                             // Set the value and name of the condition to GRASS
            Path.Condition_Value[i] = 1;    // Retrieve actual value base on ID
            Path.Condition_Name = "Grass "; // Change the name of the path condition
            break;
        case 4:                             // Set the value and name of the condition to SANY
            Path.Condition_Value[i] = 1;    // Retrieve actual value base on ID
            Path.Condition_Name = "Sandy "; // Change the name of the path condition
            break;
        case 5:                             // Set the value and name of the condition to ROCKY
            Path.Condition_Value[i] = -1;   // Retrieve actual value base on ID
            Path.Condition_Name = "Rocky "; // Change the name of the path condition
            break;
        case 6:                             // Set the value and name of the condition to UP HILL
            Path.Condition_Value[i] = -2;   // Retrieve actual value base on ID
            Path.Condition_Name = "U.Hill"; // Change the name of the path condition
            break;
        case 7:                             // Set the value and name of the condition to MUDDY
            Path.Condition_Value[i] = -3;   // Retrieve actual value base on ID
            Path.Condition_Name = "Muddy "; // Change the name of the path condition
            break;
        default:
            break;
        }
        cout << Path.Condition_Name; // Print the condition name
        for (int x = 1; x < Path.Length[i][1] - 6; x++)
        {
            cout << " "; // Print whitespace for the space between each different path condition
        }
        cout << "|"; // Print seperating line at the last of this path condition
    }
    cout << endl; // End the line after printing the Path Conditions
    return;
}

// #    Function 	:   ShowGotchaWinner
// #    Description	:   print the message for gotcha and winners
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowGotchaWinner()
{
    cout << endl;
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
        cout << "\n"; // End the line after printing all the gotcha position and runner
    }
    // ====================================================================================SHOW WINNERS
    if (Winner_Count == 1) // Check if there is only 1 winner
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n"; // Print the seperating line for the winner
        cout << "CONGRATULATION! Runner #" << Winner[0] << " WON!!!\n";           // Print the winner and the winning message
    }
    else if (Winner_Count > 1) // Else there is more than 1 winner
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WINNER!\n";                            // PPrint the seprating line for the winner
        cout << "KUDOS to those Runner!!! We have " << Winner_Count << " winners! It is a TIE!\nRunner #: "; // Print the multiple winner message and how many runer win at once
        for (int i = 0; i < Winner_Count; i++)                                                               // Loop through for all the winner
        {
            cout << Winner[i] << ((i == Winner_Count - 1) ? " WON!!!" : ((i < Winner_Count - 2) ? ", " : ", and ")); // Print the winning runner's number
        }
        cout << "\n"; // End the line after printing all the winning runner's number
    }
    cout << endl;
    return;
}

// #    Function 	:   ShowOutputs
// #    Description	:   print all the interface item
// #    Argument	:   n/a
// #    Return		:   n/a
void ShowOutputs()
{
    ShowSimHeader();    // Print the header
    ShowWeather();      // Display the weather condition
    ShowRunnerPos();    // Print the runners position
    ShowPosMark();      // Show the position marking
    ShowPath();         // Display the path condition
    ShowGotchaWinner(); // Show the message for Gotcha and Winners
    return;
}

#pragma endregion Output

#ifdef ENABLE_SETTINGS
// #    Function 	:   ShowMenu
// #    Description	:   print the menu of options to chagne values or to start the race
// #    Argument	:   *errorFlag: keep track for the input error
// #    Return		:   n/a
void ShowMenu()
{
#ifdef ENABLE_CLEAR_SCREEN
    system("cls"); // Clear the screen once if the clear screen operation is enabled
#endif
    // Print the menu and options available
    cout << "=========================================\n"
         << "# ( Running Man Simulator - Main Menu ) #\n"
         << "=========================================\n"
         << "\n-------------------------------------\n\n"
         << "[1] Set Runner Number         Current: " << Runner_Count << endl
         << "[2] Set Track Length          Current: " << Track_Length << endl
         << "[3] Set Refresh Interval(ms)  Current: " << Refresh_Interval << endl
         << "[0] START THE RACE!\n"
         << "\n-------------------------------------\n\n"
         << ((inputError == true) ? "Invalid input. Please enter listed options only.\n\n" : "") // Check for error flag and decide to print the input error message
         << "Enter an option: ";
    return;
}

// #    Function 	:   ShowSubMenu
// #    Description	:   print the sub menu for changing the values
// #    Argument	:   menu: Select the menu type; errorFlag1: keep track for invalid input; errorFlag2: keep track for out of range values input
// #    Return		:   n/a
void ShowSubMenu()
{
#ifdef ENABLE_CLEAR_SCREEN
    system("cls"); // Clear the screen once if the clear screen operation is enabled
#endif
    //  Variables to store the strings and the error message
    string valueError1, valueError_Min, valueError_Max,
        valueError2 = " or more than ",
        valueError3 = "!\n";

    if (inputError == true) // If the input is invalid
    {
        valueError = false; // Turn off the flag for value not in range
    }

    // Header
    cout << "========================================\n"
         << "# ( Running Man Simulator - Settings ) #\n"
         << "========================================\n"
         << "\n-------------------------------------\n\n";

    // Decide the message content for different menu
    switch (userIn_Option[0])
    {
    case '1': // [1] Set Runner Number
        cout << "[1] ( Edit Runner Number )\n"
             << "Current Value: " << Runner_Count << endl;
        valueError1 = "The runner number cannot be less than ";
        valueError_Min = to_string(MIN_RUNNER_COUNT);
        valueError_Max = to_string(MAX_RUNNER_COUNT);
        break;

    case '2': // [2] Set Track Length
        cout << "[2] ( Set Track Length )\n"
             << "Current Value: " << Track_Length << endl;
        valueError1 = "The track length cannot be less than ";
        valueError_Min = to_string(MIN_TRACK_LENGTH);
        valueError_Max = to_string(MAX_TRACK_LENGTH);
        break;

    case '3': // [3] Set Refresh Interval(ms)
        cout << "[3] ( Set Refresh Interval(ms) )\n"
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
    cout << "\n-------------------------------------\n\n"
         << ((inputError == true) ? "Invalid Input. Please enter integers ONLY.\n\n" : "")
         << ((valueError == true) ? valueError1 : "")
         << "\nEnter a value: ";
    return;
}

// #    Function 	:   CheckInput
// #    Description	:   check the input from the user for the main menu
// #    Argument	:   *input: variable to store the input; *errorFlag: variable to keep track for input error; type: "menu" or "value" type for selecting checking algorithm
// #    Return		:   true for invalid; false for valid
bool CheckInput(string type = "")
{
    regex options;        // Initiate varaible to store the regex sample
    if (type == "option") // If menu type is selected
    {
        options = "([0123]{1})";                                           // Set the sample for menu as the regex sample
        getline(cin, userIn_Option);                                       // Read the user input
        inputError = (regex_match(userIn_Option, options)) ? false : true; // Check the input with sample
    }
    else if (type == "value") // If the value type is selected
    {
        options = "([0-9]+)";                                             // Set the sample for value as the regex sample
        getline(cin, userIn_Value);                                       // Read the user input
        inputError = (regex_match(userIn_Value, options)) ? false : true; // Check the input with sample
    }
    return inputError; // Return the error flag status
}

// #    Function 	:   CheckValue
// #    Description	:   check the values entered by the user and see if it is in range or not
// #    Argument	:   *input: variable to store the input; menu: char to select which value range to check; *errorFlag: variable to keep for values validity
// #    Return		:   true for out of range; false for valu in range
bool CheckValue()
{
    switch (userIn_Option[0]) // Sweitch case to decide value range to check base on menu
    {
    case '1': // [1] Set Runner Number
        valueError = ((stoi(userIn_Value) < MIN_RUNNER_COUNT) || (stoi(userIn_Value) > MAX_RUNNER_COUNT)) ? true : false;
        break;

    case '2': // [2] Set Track Length
        valueError = ((stoi(userIn_Value) < MIN_TRACK_LENGTH) || (stoi(userIn_Value) > MAX_TRACK_LENGTH)) ? true : false;
        break;

    case '3': // [3] Set Refresh Interval(ms)
        valueError = (stoi(userIn_Value) < MIN_REFRESH_INTERVAL) ? true : false;
        break;

    default:
        valueError = true;
        break;
    }
    return valueError; // Return the error flag status for value in range or not in range
}

// #    Function 	:   CheckInput
// #    Description	:   check the input from the user for the main menu
// #    Argument	:   option: select which option to update; value: the value to be stored for the selected option
// #    Return		:   n/a
void UpdateValues()
{
    switch (userIn_Option[0]) // Switch case to select which variable should the value be stored in
    {
    case '1': // Runner Count
        Runner_Count = stoi(userIn_Value);
        break;
    case '2': // Track Length
        Track_Length = stoi(userIn_Value);
        break;
    case '3': // Refresh Interval
        Refresh_Interval = stoi(userIn_Value);
        break;
    default:
        break;
    }
    return;
}

// #    Function 	:   MenuModule
// #    Description	:   whole menu function including showing menu checking inpui, value and updating values
// #    Argument	:   option: select which option to update; value: the value to be stored for the selected option
// #    Return		:   n/a
void MenuModule()
{
    do // The Menu
    {
        // ==================================== Main Menu
        do
        {
            ShowMenu();                 // Show the Main Menu
        } while (CheckInput("option")); // Loop to show the main menu until input is valid

        if (userIn_Option[0] == '0') // Exit the loop if it is 0
            break;
        // ==================================== Sub Menu
        do
        {
            ShowSubMenu();
        } while (CheckInput("value") || CheckValue()); // Check the input and loop if invalid

        UpdateValues(); // Update Values
    } while (1);        // Loop back to main menu if the input is not 0
    return;
}
#endif
#pragma endregion FUNCTIONS