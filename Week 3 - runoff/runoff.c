#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    //check if the name provided is valid
    for (int i = 0; i < candidate_count; i++)
    {
        //if a valid name is found, record the preference, and return true
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }

    //if a valid name is not found, return false
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    //iterate through preferences array
    for (int i = 0; i < voter_count; i++)
    {
        //for each voter, iterate through their preferences
        //until a candidate is found that has not been eliminated
        for (int j = 0; j < candidate_count; j++)
        {
            //when you've found a candidate that has not been elminated
            //increment their votes, and move to the next voter
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int mostVotes = 0;
    int winnerIndex = -1;

    //search the candidates array for the candidate with the most votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > mostVotes)
        {
            mostVotes = candidates[i].votes;
            winnerIndex = i;
        }
    }

    //if the winning candidate holds a majority; print the name and return true
    if (candidates[winnerIndex].votes > voter_count / 2)
    {
        printf("%s\n", candidates[winnerIndex].name);
        return true;
    }

    //if no one holds a majority, return false
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int minVote = MAX_VOTERS;

    //iterate through the candidate array and find the lowest number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        //make sure the candidate we're checking has not been eliminated before counting their votes
        //otherwise we'll get 0 back which is guaranteed to be the lowest number
        if (!candidates[i].eliminated && candidates[i].votes < minVote)
        {
            minVote = candidates[i].votes;
        }
    }

    return minVote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //iterate through the candidates again
    for (int i = 0; i < candidate_count; i++)
    {
        //if ANY candidates have a vote count different than the current minimum,
        //we can assume there's no tie, so break and return false
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            return false;
        }
    }

    //if we've made it through the whole list, and all candidates vote count
    //is equal to our current minimum, there is a tie and we should return true
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //iterate through the candidates array
    for (int i = 0; i < candidate_count; i++)
    {
        //if a candidate hasn't been eliminated yet, and they're currently in last place
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            //eliminate them
            candidates[i].eliminated = true;
        }
    }

    return;
}