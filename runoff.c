//by cjoshi
#include <stdio.h>
#include <string.h>

#include "runoff.h"

// preferences[i][j] is jth preference for voter i
int preferences[kMaxVoters][kMaxCandidates];

// array of candidates
Candidate_t candidates[kMaxCandidates];

// numbers of voters and candidates
int voter_count;
int candidate_count;

int main(int argc, char *argv[])
{
    // check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > kMaxCandidates)
    {
        printf("Maximum number of candidates is %i\n", kMaxCandidates);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = 0;
    }

    printf("Number of voters: ");
    scanf("%i", &voter_count);

    if (voter_count > kMaxVoters)
    {
        printf("Maximum number of voters is %i\n", kMaxVoters);
        return 3;
    }

    // keep asking for votes
    for (int i = 0; i < voter_count; i++)
    {

        // ask for each ranking
        for (int j = 0; j < candidate_count; j++)
        {
            char name[50];
            printf("Rank %i: ", j + 1);
            scanf("%s", name);

            // record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // keep holding runoffs until winner exists
    while (1)
    {
        // calculate votes given remaining candidates
        tabulate();

        // check if election has been won
        bool won = print_winner();
        if (won)
            break;

        // eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // if tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                    printf("%s\n", candidates[i].name);
            }
            break;
        }

        // eliminate anyone with minimum number of votes
        eliminate(min);

        // reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// record preference if vote is valid
bool vote(int voter, int rank, char* name)
{
    bool vote = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            vote = 1;
            break;
        }
    }
    return vote;
}

// tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == 0)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }

    }
    return;
}

// print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        char* current = candidates[i].name;
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", current);
            return 1;
        }
    }
    return 0;
}

// return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == 0 && candidates[i].votes < min)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == 0 && candidates[i].votes != min)
        {
            return 0;
        }
    }
    return 1;
}

// eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = 1;
        }
    }
    return;
}
