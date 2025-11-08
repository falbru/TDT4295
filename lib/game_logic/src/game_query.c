#include "game_query.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char gameQueries[QUERY_COUNT][MAX_QUERY_LENGTH] = {"Lorem", "Ipsum", "Place", "Holder", "Plasstaker"};

void getGameQuery(GameQuery *query)
{
    int random_index = rand() % QUERY_COUNT;
    strcpy(query->queryName, gameQueries[random_index]);
    query->queryIndex = random_index;
}

bool checkGameResult(GameQuery *original_query, GameQuery *result_query, uint8_t result_index)
{
    if (result_index == original_query->queryIndex)
    {
        *result_query = *original_query;
        return true;
    }
    result_query->queryIndex = result_index;
    strcpy(result_query->queryName, gameQueries[result_index]);
    return false;
}