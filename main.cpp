// Nicholas Kong
// Operating System Lab 1
// Due Date: February 9, 2020


#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <regex>
#include <sstream>
#include <time.h>

// %F %X
// istringstream split lto get each individual wrod that seperated by white space.

using namespace std;


string proc_stat, proc_uptime, proc_mem; // This will hold the information read from file
string system_info; // This holds file contents
string save_btime, save_uptime; // Will save the btime and uptime contents
string btime = "btime", cpu = "cpu ", mem_total = "MemTotal", mem_avail = "MemAvailable";
string save_Memory_Total, save_Memory_Avaliable, save_CPU; // Saves memory info and cup info
string abc = "[abcdefghijklmnopqrstuvwxyz ]"; // Used in regex to get rid of letters.


// Function that opens /proc/stat file
void open_proc_stat()
{
    // cout << "HERE AT PROC STAT" << endl; // Test to see if function call works.
    ifstream myFile("/proc/stat");
    if(myFile.is_open()) // If the file is open, retrive the information
    {
        while (getline(myFile, proc_stat))
        {
            system_info = proc_stat;
            if (system_info.find(btime) != string::npos)
            {
                save_btime = system_info;
            }
            if (system_info.find(cpu) != string::npos)
            {
                save_CPU = system_info;
                
            }
        }
        myFile.close(); // Closes the file
    }
    else // If the file does not open, display error
    {
        cout << "Error, the '/proc/stat' file could not be opened!" << endl;
    }
}


void open_proc_uptime()
{
    // cout << "HERE AT PROC UPTIME" << endl; // Test to see if function call works.
    ifstream myFile("/proc/uptime");
    if(myFile.is_open())
    {
        while(getline(myFile, proc_uptime))
        {
            save_uptime = proc_uptime;
        }
        myFile.close();
    }
    else
    {
        cout << "Error, the '/proc/uptime' file could not be opened!" << endl;
    }
    
}


void open_proc_meminfo()
{
    // cout << "HERE AT PROC INFO << endl; // Test to see if function call works.
    ifstream myFile("/proc/meminfo");
    if(myFile.is_open())
    {
        while(getline(myFile, proc_mem))
        {
            system_info = proc_mem;
            if(system_info.find(mem_total) != string::npos)
            {
                save_Memory_Total = system_info;
            }
            if(system_info.find(mem_avail) != string::npos)
            {
                save_Memory_Avaliable = system_info;
            }
        }
    }
}


void time_function(int second)
{
    time_t btime = second;
    struct tm  time_change;
    char buffer[80];
    
    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    time_change = *localtime(&btime);
    strftime(buffer, sizeof(buffer), "%a %Y-%m-%d %H:%M:%S %Z", &time_change);
    printf("%s\n", buffer);
}



void time_conversion(int ex)
{
    //int ex = 31536000;
    int day, hour, minute, second;
    // For day conversion we get (24 hours in a day * 3600 seconds in an hour).
    day = ex / (24 * 3600);
    ex = ex % (24 * 3600);
    
    // For hour conversion we get (3600 seconds in an hour).
    hour = ex / 3600;
    ex %= 3600;
    
    // For minute conversion we get (60 seconds in a minute).
    minute = ex / 60;
    ex %= 60;
    
    // Seconds is here.
    second = ex;
    
    // Prints the amount of seconds in form
    cout << day << " Days " << hour << " Hours " << minute << " Minutes " << second << " Seconds"<< endl;
    
    cout << day << ":" << hour << ":" << minute << ":" << second << endl;
}


// Gets only the number from the text
void get_seconds(string text)
{
    string numberOnly;
    const regex letters(abc); // Removes all alphabetical letters and space
    
    stringstream result; // Needed for string to int conversion
    regex_replace(std::ostream_iterator<char>(result), text.begin(), text.end(), letters, ""); // Removes letters
    
    //cout << result.str();
    numberOnly = result.str(); // Places the all removed letters in another string
    
    // Convert the string into an integer
    stringstream now_integer(numberOnly);
    int time_seconds = 0;
    now_integer >> time_seconds;

    // Passes the integer of seconds into time conversion.
    time_function(time_seconds);
}

// Prints the CPU time spent in user mode and system mode.
void remove_split_string_CPU(string str)
{
    string word = "";
    string word1;
    int count = 0; // Counts the number of words
    for (auto &&x : str) // For loop will iterate through the string character by character.
    {
        // If it reaches a white space, the word is complete.
        if (x == ' ')
        {
            //cout << word << endl;
            word1 = word;
            word = "";
            count ++;
            //cout << count;
        }
        
        // Read and connect each charater until a white space is hit.
        else
        {
            word = word + x;
            //cout << count;
        }
        
        // If the second or fourth word is read, print it to the screen.
        if(count == 3 || count == 6)
        {
            if(count == 3)
            {
                cout << "Time spent in user mode: " << word1 << " seconds" << endl;
                count++;
            }
            if(count == 6)
            {
                cout << "Time spent in system mode: " << word1 << " seconds" << endl;
                count++;
            }
        }
        
    }
    //cout << word << endl;
}


// Prints the CPU time spent in user mode and system mode.
void remove_split_string_uptime(string str)
{
    string word = "";
    string word1;
    int count = 0; // Counts the number of words
    for (auto &&x : str) // For loop will iterate through the string character by character.
    {
        // If it reaches a white space, the word is complete.
        if (x == ' ')
        {
            //cout << word << endl;
            word1 = word;
            word = "";
            count ++;
            //cout << count;
        }
        
        // Read and connect each charater until a white space is hit.
        else
        {
            word = word + x;
            //cout << count;
        }
        
        // If the second or fourth word is read, print it to the screen.
        if(count == 1)
        {
            if(count == 1)
            {
                cout << "Time since system was last booted: ";
                
                stringstream result; // Needed for string to int conversion
                // Convert the string into an integer
                stringstream now_integer(word1);
                int time_seconds = 0;
                now_integer >> time_seconds;
                
                time_conversion(time_seconds); // Calls to get the day conversion.
                
                count++;
            }
        }
        
    }
    //cout << word << endl;
}


int main ()
{
    // Part A of the Lab1 below.
    cout << "\nPart A of lab1 is listed below." << endl << endl;
    // utsname is in variable System_Info which will be used to pass its address to uname
    struct utsname System_Info;
    
    uname(&System_Info); // Needed to get the current name of the system information.
    
    printf("System Name - %s\n", System_Info.sysname); // Prints the system name to screen
    printf("Node Name - %s\n", System_Info.nodename); // Prints the Node name to screen
    printf("Release - %s\n", System_Info.release); // Prints the Release info to screen
    printf("Version - %s\n", System_Info.version); // Prints the Version info to screen
    printf("Machine - %s\n", System_Info.machine); // Prints the Machine info to screen
    
    // Part B of the lab1 below.
    cout << "\nPart B of lab1 is listed below." << endl << endl;
    
    
    open_proc_stat(); // Function call to get btime and cpu
    open_proc_uptime(); // Function call to get the uptime in system and in idle time in seconds
    open_proc_meminfo(); // Function call to get the memory total and memory avaliable
    
    // btime printed in correct format
    // cout << save_btime << endl; // Checks to see if correct thing is printed
    
    cout << "Time when system is last booted: ";
    get_seconds(save_btime); // This will convert seconds to year months days, etc...
    
    // Prints the CPU in time of seconds
    // cout << save_CPU << endl; // Checks to see if the correct stuff is printed
    remove_split_string_CPU(save_CPU); // Prints the time CPU spent in user mode and in system mode.
    
    // Prints the uptime in seconds
    // cout << "uptime: " << save_uptime << endl; // Makes sure correct info was retrieved.
    remove_split_string_uptime(save_uptime);
    
    // Prints the memory below
    cout << save_Memory_Total << endl;
    cout << save_Memory_Avaliable << endl;
    
    return 0;
}
