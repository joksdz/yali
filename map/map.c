#include "map.h"
#include <stdio.h>


#define TOMBSTONE ((char *)1)

unsigned long hash(char *key) {
    unsigned long h = 2166136261UL;
    while (*key) {
        h ^= (unsigned char)*key++;
        h *= 16777619UL;
    }
    return h;
}

void initMap(Map *m) {
    m->count = 0;
    m->capacity = 0;
    m->entries = NULL;
}

void freeMap(Map *m) {
    free(m->entries);
    initMap(m);
}

static Entry *findEntry(Entry *entries, int capacity, char *key) {
    unsigned long idx = hash(key) % capacity;
    Entry *tombstone = NULL;

    for (;;) {
        Entry *e = &entries[idx];
        if (e->key == NULL) {
            return tombstone != NULL ? tombstone : e; 
        } else if (e->key == TOMBSTONE) {
            if (tombstone == NULL) tombstone = e;      // remember first tombstone seen
        } else if (strcmp(e->key, key) == 0) {
            return e;                                   
        }
        idx = (idx + 1) % capacity;
    }
}

static void growMap(Map *m) {
    int newCapacity = m->capacity == 0 ? 8 : m->capacity * 2;
    Entry *newEntries = malloc(newCapacity * sizeof(Entry));
    for (int i = 0; i < newCapacity; i++) {
        newEntries[i].key = NULL;
    }

    int newCount = 0;
    for (int i = 0; i < m->capacity; i++) {
        Entry *e = &m->entries[i];
        if (e->key == NULL || e->key == TOMBSTONE) continue;
        Entry *dest = findEntry(newEntries, newCapacity, e->key);
        dest->key = e->key;
        dest->val = e->val;
        newCount++;
    }

    free(m->entries);
    m->entries = newEntries;
    m->capacity = newCapacity;
    m->count = newCount;
}

TokenType getValFromMap(Map *m, char *key) {
    if (m->count == 0) return TOKEN_NONE;
    Entry *e = findEntry(m->entries, m->capacity, key);
    if (e->key == NULL || e->key == TOMBSTONE) return TOKEN_NONE;
    return e->val;
}

void insertMap(Map *m, char *key, TokenType value) {
    if (m->count + 1 > m->capacity * 0.75) {
        growMap(m);
    }

    Entry *e = findEntry(m->entries, m->capacity, key);
    bool isNewKey = (e->key == NULL || e->key == TOMBSTONE);
    e->key = key;
    e->val = value;
    if (isNewKey) m->count++;
}

void delMap(Map *m, char *key) {
    if (m->count == 0) return;
    Entry *e = findEntry(m->entries, m->capacity, key);
    if (e->key == NULL) return; // not found
    e->key = TOMBSTONE;                                         // no null ,nulling would break probe chains for later entries that collided into this slot
}

void dispMap(Map *m) {
    for (int i = 0; i < m->capacity; i++) {
        if (m->entries[i].key != NULL && m->entries[i].key != TOMBSTONE) {
            printf("%s\n", m->entries[i].key);
        }
    }
}
