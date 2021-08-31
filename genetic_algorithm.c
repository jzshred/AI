/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Genetic Algorithm Example
//
// Author: John Zalles
// Date: September 19th, 2020
// Quote of the day: "Defeat is not the worst of failures. Not to have tried is the true failure."
//                      - George Edward Woodberry
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------------
// Required libraries
//---------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//---------------------------------------------------------------------------------------------------
// Define constants
//---------------------------------------------------------------------------------------------------
#define STATES 10           // no. of US states inside an individual (value can be modified)
#define BORDERS 17          // no. of border pairs (value can be modified after adjusting borderPairs variable)
#define INDIV 100           // no. of individuals in population (value can be modified)
#define COLORS 4            // no. of possible colors (value can be modified after adjusting colors variable)
#define ITERATIONS 1000     // no. of iterations (value can be modified)

//---------------------------------------------------------------------------------------------------
// Variable declarations
//---------------------------------------------------------------------------------------------------
char stateNames[30] = "NC,SC,VA,TN,KY,WV,GA,AL,MS,FL";
char colors[COLORS+1] = { 'R', 'G', 'B', 'W'};  // Red, Green, Blue, White
char initialPopulation[INDIV][STATES+1];
double fitness[INDIV];
int chosenOnes[INDIV];
char crossPopulation[INDIV][STATES+1];
char bestIndividual[STATES+1];
_Bool globalMax = 0;
double maxFitness = 0.0;
int counter = 0;

//---------------------------------------------------------------------------------------------------
// Border pairs (would have to be manually modified to include more states)
//---------------------------------------------------------------------------------------------------
int borderPairs[BORDERS][2] =
    {
     {0, 1},
     {0, 2},
     {0, 3},
     {0, 6},
     {1, 6},
     {2, 3},
     {2, 4},
     {2, 5},
     {3, 4},
     {3, 6},
     {3, 7},
     {3, 8},
     {4, 5},
     {6, 7},
     {6, 9},
     {7, 8},
     {7, 9},
    };

//---------------------------------------------------------------------------------------------------
// Function: randNum - generates a random number within bounds
//---------------------------------------------------------------------------------------------------
int randNum(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

//---------------------------------------------------------------------------------------------------
// Step 1: create initial population
//---------------------------------------------------------------------------------------------------
void createPopulation()
{
    // randomly generate a population of n individuals
    // each "row" represents an individual
    // each "column" represents a state and stores one of k colors

    // create individuals
    for(int j = 0; j < INDIV; ++j)
    {
        for(int i = 0; i < STATES; ++i)
        {
            initialPopulation[j][i] = colors[randNum(0, COLORS-1)];
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 2: calculate fitness function for each individual
//---------------------------------------------------------------------------------------------------
void fitnessFunction()
{
    // fitness function is percentage of non-conflicting pairs of states

    // add up number of conflicts
    for(int j = 0; j < INDIV; ++j)
    {
        for(int i = 0; i < BORDERS; ++i)
        {
            if(initialPopulation[j][borderPairs[i][0]] == initialPopulation[j][borderPairs[i][1]])
                fitness[j] += 1.0;
        }
    }

    // transform result into percentage of non-conflicting pairs of states
    for(int i = 0; i < INDIV; ++i)
    {
        fitness[i] = ((BORDERS - fitness[i])/BORDERS);
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 3: return best individual after last iteration or if a global maximum is found
//---------------------------------------------------------------------------------------------------
void returnBest()
{
    // find individual with the best fitness function
    for(int i = 0; i < INDIV; ++i)
    {
        if(fitness[i] > maxFitness)
        {
            maxFitness = fitness[i];
            strcpy(bestIndividual, initialPopulation[i]);
            if(maxFitness == 1.0)
            {
                globalMax = 1;
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 4: select top individuals based on fitness function
//---------------------------------------------------------------------------------------------------
void chooseIndividuals()
{
    double rank[INDIV-1];

    for(int i = 0; i < INDIV; ++i)
    {
        for(int j = 0; j < (INDIV-1); ++j)
        {
            if(fitness[i] > rank[j])
            {
                // move rank
                for(int k = 0; k < (INDIV-2-j); ++k)
                {
                    rank[INDIV-2-k] = rank[INDIV-3-k];
                }

                // copy rank
                rank[j] = fitness[i];

                // move chosen
                for(int k = 0; k < (INDIV-2-j); ++k)
                {
                    chosenOnes[INDIV-2-k] = chosenOnes[INDIV-3-k];
                }

                // copy chosen
                chosenOnes[j] = i;
                break;
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 5: crossover
//---------------------------------------------------------------------------------------------------
void crossover()
{
    int crossPoint;
    int k, n;

    for(int j = 0; j < INDIV; ++j)
    {
        if(j % 2 == 0)
        {
            crossPoint = randNum(1, STATES-2);  // choose a random crossover point
            k = j / 2;
            n = k + 1;
        }
        else
        {
            k = (j + 1) / 2;
            n = k - 1;
        }

        // crossover individuals
        for(int i = 0; i < crossPoint; ++i)
        {
            crossPopulation[j][i] = initialPopulation[chosenOnes[k]][i];
        }
        for(int i = crossPoint; i < STATES; ++i)
        {
            crossPopulation[j][i] = initialPopulation[chosenOnes[n]][i];
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 6: mutation
//---------------------------------------------------------------------------------------------------
void mutation()
{
    // choose a random US state, and mutate its color
    for(int i = 0; i < INDIV; ++i)
    {
        crossPopulation[i][randNum(0, STATES-1)] = colors[randNum(0, COLORS-1)];
    }

    // copy new population
    for(int j = 0; j < INDIV; ++j)
    {
        strcpy(initialPopulation[j], crossPopulation[j]);
    }

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 7: print results
//---------------------------------------------------------------------------------------------------
void printResults()
{
    printf("-------------------------------------------------------\n");
    printf("After %d iterations, a %s maximum has been found.\n", counter, globalMax == 1 ? "global" : "local");
    printf("Best individual: %s\n", bestIndividual);
    printf("Fitness function: %g\n", maxFitness);
    printf("Number of iterations: %d\n", counter);
    printf("Individuals in population: %d\n", INDIV);
    printf("Number of colors: %d\n", COLORS);
    printf("-------------------------------------------------------\n");

    for(int i = 1; i < (STATES+1); ++i)
    {
        printf("%c%c -- %c\n", stateNames[(i*3)-3], stateNames[(i*3)-2], bestIndividual[i-1]);
    }

    printf("-------------------------------------------------------\n");

    return;
}

//---------------------------------------------------------------------------------------------------
// Step 8: create dot file
//---------------------------------------------------------------------------------------------------
void dotFile()
{
    // declare and initialize variables
    FILE *filePointer = NULL;
    char colorName[10];
    char usState[3];

    // create file "results.dot"
    filePointer = fopen("results.dot", "w+");

    // write first line of the dot file
    fputs("graph {\n", filePointer);

    // write state colors according to best individual
    for(int i = 1; i < (STATES+1); ++i)
    {
        fputs("  ", filePointer);
        usState[0] = stateNames[(i*3)-3];
        usState[1] = stateNames[(i*3)-2];
        usState[2] = '\0';
        fputs(usState, filePointer);
        fputs(" [fillcolor=", filePointer);
        switch(bestIndividual[i-1])
        {
            case 'R':
                strcpy(colorName, "red");
                break;
            case 'G':
                strcpy(colorName, "green");
                break;
            case 'B':
                strcpy(colorName, "blue");
                break;
            case 'W':
                strcpy(colorName, "white");
                break;
        }
        fputs(colorName, filePointer);
        fputs(", style=filled]\n", filePointer);
    }

    // write borders
    for(int j = 0; j < BORDERS; ++j)
    {
        fputs("  ", filePointer);
        usState[0] = stateNames[(((borderPairs[j][0])+1)*3)-3];
        usState[1] = stateNames[(((borderPairs[j][0])+1)*3)-2];
        usState[2] = '\0';
        fputs(usState, filePointer);
        fputs(" -- ", filePointer);
        usState[0] = stateNames[(((borderPairs[j][1])+1)*3)-3];
        usState[1] = stateNames[(((borderPairs[j][1])+1)*3)-2];
        usState[2] = '\0';
        fputs(usState, filePointer);
        fputs("\n", filePointer);
    }

    // write the last line of the dot file
    fputs("}", filePointer);

    // close the file
    fclose(filePointer);
    filePointer = NULL;

    // print comments
    printf("File 'results.dot' created with results.\n");

    return;
}

//---------------------------------------------------------------------------------------------------
// Function: main
//---------------------------------------------------------------------------------------------------
int main()
{
    // use current time as seed for random number generator
    srand(time(0));

    // Step 1: create initial population
    createPopulation();

    for(counter = 0; counter < ITERATIONS && globalMax == 0; ++counter)
    {
        // Step 2: calculate fitness function for each individual
        fitnessFunction();

        // Step 3: return best individual after last iteration or if a global maximum is reached
        returnBest();

        if(globalMax != 1)
        {
            // Step 4: select top individuals based on fitness function
            chooseIndividuals();

            // Step 5: crossover
            crossover();

            // Step 6: mutation
            mutation();
        }
    }

    // Step 7: print results
    printResults();

    // Step 8: create dot file
    dotFile();

    // Finalize program
    return 0;
}
