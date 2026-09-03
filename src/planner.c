#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "planner.h"

void cstow_planner_init(struct cstow_planner *planner){
    if(planner == NULL) return;

    planner->count = 0;
    planner->capacity = 1;
    planner->actions = NULL;
}

int cstow_planner_add(struct cstow_planner *planner, enum cstow_action_type type, const char *src, const char *dest){
    if(planner == NULL || src == NULL || dest == NULL) return -1;

    if(planner->count == planner->capacity){
        size_t temp_cap = planner->capacity * 2;

        struct cstow_action *temp_actions = realloc(planner->actions, temp_cap * sizeof(struct cstow_action));
        if(temp_actions == NULL){
            perror("realloc");
            return -1;
        }

        planner->capacity = temp_cap;
        planner->actions = temp_actions;
    }

    struct cstow_action *action = &planner->actions[planner->count];

    action->type = type;
    action->src = strdup(src);
    action->dest = strdup(dest);

    if(action->src == NULL || action->dest == NULL){
        free(action->src);
        free(action->dest);

        action->src = NULL;
        action->dest = NULL;

        return -1;
    }

    planner->count += 1;

    return 0;
}

void cstow_plan_destroy(struct cstow_planner *planner){
    if(planner == NULL) return;

    for(size_t i = 0; i < planner->count; i++){
        struct cstow_action *action = &planner->actions[i];
        free(action->src);
        free(action->dest);
    }

    free(planner->actions);
}
