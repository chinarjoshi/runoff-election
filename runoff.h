#ifndef RUNOFF_H
#define RUNOFF_H
#include <stdbool.h>

// enumerated global constants
typedef enum
{
  kMaxVoters = 100,
  kMaxCandidates = 9
} GlobalConstants_t;

// candidates have name, vote count, eliminated status
typedef struct
{
  char* name;
  int votes;
  bool eliminated;
} Candidate_t;

// function prototypes
bool vote(int voter, int rank, char *name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

#endif
