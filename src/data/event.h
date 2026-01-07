#ifndef EVENT_DEF
#define EVENT_DEF

#include "list.h"

#include <stdio.h>

typedef void (*EventFunction) (void*);
typedef struct Event {
    void* passVal;
    const char* name;
    List subs;                                                      // Of EventFunctions
} Event;

void Event_init(Event* e, const char* name);

void Event_subscribe(Event* e, EventFunction f);
void Event_unsubscribe(Event* e, EventFunction f);
void Event_dispatch(Event* e);

#endif
