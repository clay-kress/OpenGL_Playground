#include "event.h"

void Event_init(Event* e, const char* name) {
    e->name= name;
    e->subs= initList(sizeof(EventFunction), 0);
}

void Event_subscribe(Event* e, EventFunction f) {
    list_push(&e->subs, &f);
    return;
}

void Event_unsubscribe(Event* e, EventFunction f) {
    for (int i= 0; i < list_len(&e->subs); i++) {
        if (*((EventFunction*) list_getPtr(&e->subs, i)) == f) {
            list_remove(&e->subs, i, NULL);
            break;
        }
    }
}

void Event_dispatch(Event* e) {
    for (int i= 0; i < list_len(&e->subs); i++) {
        (*(EventFunction*) list_getPtr(&e->subs, i))(e->passVal);
    }
    return;
}
