#ifndef __CSTOW_PLANNER_H
#define __CSTOW_PLANNER_H

#include <stddef.h>

enum cstow_action_type{
    CSTOW_ACTION_CREATE,
    CSTOW_ACTION_REMOVE
};

struct cstow_action{
    char *src;
    char *dest;
    
    enum cstow_action_type type;
};

struct cstow_planner{
    size_t count;
    size_t capacity;
    
    struct cstow_action *actions;
};

int cstow_planner_init(struct cstow_planner *planner);
int cstow_planner_add(struct cstow_planner *planner, enum cstow_action_type type, const char *src, const char *dest);
void cstow_plan_destroy(struct cstow_planner *planner);

#endif
