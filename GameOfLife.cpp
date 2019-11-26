#include <cmath>
#include <iostream>
#include <time.h>
#include <windows.h>
#include <iomanip>

using namespace std;

const int MAXGEN = 5;
const int m = 10;
const int n = 10;

void initialize(int grid[n][m])                 //Has the user set the starting values for the array filling it one row at a time
{

    cout << "Please enter the values for the cells from left to right starting with the topmost row. (1=alive, 0=dead)" << endl << "(this simulation is set to be a 10x10 matrix)" << endl;

    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            cin >> grid[i][j];
        }
        cout << endl;
    }
}

void initialize2(int grid [n][m])               //Randomly generates a 2D array based on a desired percentage density
{
    int density;
    cout << "Enter the density of the population as a number between 1 and 100: "; cin >> density;

    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            if ((rand() % 100 + 1)<=density)    //Random number generatior that generates a number between 1 and 100 then evaluates if it is below the desired density and sets the cell to alive if it is
            {
                grid [i][j]=1;
            }
            else
            {
                grid [i][j]=0;
            }
        }
    }
}

int countneighbours(int grid[n][m],int x,int y)    //Evaluates the cells surrounding the target cell and checks how many are alive
{
    int sum=0;

    for (int i=(x-1); i<=(x+1); i++)
    {
        for (int j=(y-1); j<=(y+1); j++)
        {
            if (i!=x || j!=y)                       //Ensures that the target cell is not counted to be it's own neighbour
            {
                if (i<0 && j>=0)                    //Wraps the bottom row of the array back onto the top
                {
                    sum+= grid[n-1][j%m];           //[j%m] wraps the first column next to the last in the first row
                }
                else if (i>=0 && j<0)               //Wraps the last column next to the first
                {
                    sum+= grid[i%n][m-1];           //[i%n] wraps the top row back to the bottom in the first column
                }
                else if (i<0 && j<0)                //Wraps the bottom right corner next to the top left
                {
                    sum+= grid[n-1][m-1];
                }
                else if (0<=i && i<=n && 0<=j && j<=m) //Treats all other cases as normal except for the bottom and right edges where they are wrapped similarly to the above situations
                {
                    sum+= grid[i%n][j%m];
                }
            }
        }
    }

    return sum;
}

void reproduce(int grid[n][m], bool &stable)     //Uses the countneighbours function to evaluate what state a cell should be in the following generation as well as evaluating whether or not the array has reached a stable state
{
    int lastgrid[n][m];

    for (int a=0; a<n; a++)     //Makes a copy of the input array for evaluation
    {
        for (int b=0; b<m; b++)
        {
            lastgrid[a][b]=grid[a][b];
        }
    }

    for (int i=0; i<n; i++)                                                                                                 //Evaluates the 4 rules to the simulation
    {
        for (int j=0; j<m; j++)
        {
            if (countneighbours(lastgrid,i,j)<2 && lastgrid[i][j]==1)                                                       //Kills a cell if it has fewer than 2 neighbours
            {
                grid[i][j]=0;
            }
            else if (countneighbours(lastgrid,i,j)>3 && lastgrid[i][j]==1)                                                  //Kills a cell if it has more than 3 neighbours
            {
                grid[i][j]=0;
            }
            else if (2<=(countneighbours(lastgrid,i,j)) && (countneighbours(lastgrid,i,j))<=3 && lastgrid[i][j]==1)         //Keeps a cell alive if it has 2 or 3 neighbours
            {
                grid[i][j]=1;
            }
            else if ((countneighbours(lastgrid,i,j)==3 && lastgrid[i][j]==0))                                               //Sets a dead cell to alive if it has exactly 3 neighbours
            {
                grid[i][j]=1;
            }
        }
    }


    int a=0;
    int b=0;

    do                          //Evaluates whether or not the past and current generations are identical and closes once it finds a difference between the two, also setting the 'stable' variable to false
    {                           //(note: this has no effect on states that repeat eachother after a certain number of generations)
        do
        {
            if ((lastgrid[a][b])==(grid[a][b]))
            {
                stable = true;
            }
            else
            {
                stable = false;
            }

            b++;

        } while (b<m && stable==true);

        b=0;
        a++;

    } while (a<n && stable==true);
}

bool alldead(int grid [n][m])           //Checks to see whether there are still live cells in the array (treats any non-zero sum as a live state)
{
    int sum=0;

     for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            sum+= grid [i][j];
        }
    }
    if (sum==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void print(int grid [n][m])             //Prints the array with some spacing to make it easier to read
{
    for (int i=0; i<n; i++)
    {
        cout << endl;
        for (int j=0; j<m; j++)
        {
            cout << " " << grid [i][j];
        }
    }
}

int main()
{
    int grid [n][m];
    initialize2(grid);
    cout << endl << "Initial population: " << endl;
    print(grid);

    int gen = 1;

    bool stable = false;
    bool dead = false;

    while (gen <= MAXGEN && !dead && !stable)            //Runs and displays the result of the reproduce function as long as there are live cells that haven't reached a stable state until the desired number of generations have elapsed
    {


        reproduce(grid,stable);
        dead=alldead(grid);

        if (stable)                //Displays a message stating that the array has reached a stable state
            {
                cout << endl << endl << "The population is stable" << endl;
            }

        if (dead)                  //Displays a message stating that the array has died after printing the dead array and the generation
            {
                cout << endl << endl << "Generation = " << gen << endl;
                print(grid);
                cout << endl << endl << "The population is dead" << endl;
            }

        if (!stable && !dead)       //Displays the current generation and the array
            {
                cout << endl << endl << "Generation = " << gen << endl;
                print(grid);
                gen++;
            }
    }
}
