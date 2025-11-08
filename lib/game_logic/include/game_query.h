#ifndef GAME_QUERY_H_INCLUDED
#define GAME_QUERY_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

#define QUERY_COUNT 5
#define MAX_QUERY_LENGTH 16

typedef struct
{
    char queryName[MAX_QUERY_LENGTH];
    uint8_t queryIndex;
} GameQuery;

bool checkGameResult(GameQuery *original_query, GameQuery *result_query, uint8_t result_index);
void getGameQuery(GameQuery *query);

#endif