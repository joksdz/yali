#include "map.h"
#include <stdio.h>


#define TOMBSTONE ((char *)1)

unsigned long hash(string key) {
    unsigned long h = 2166136261UL;
    for (int i = 0; i < key.len; i++) {
        h ^= (unsigned char)key.data[i];
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

// a slot is empty if key.data is NULL, a tombstone if key.data is TOMBSTONE
static Entry *findEntry(Entry *entries, int capacity, string key) {
    unsigned long idx = hash(key) % capacity;
    Entry *tombstone = NULL;

    for (;;) {
        Entry *e = &entries[idx];
        if (e->key.data == NULL) {
            return tombstone != NULL ? tombstone : e;
        } else if (e->key.data == TOMBSTONE) {
            if (tombstone == NULL) tombstone = e;
        } else if (strCmp(e->key, key) == 0) {
            return e;
        }
        idx = (idx + 1) % capacity;
    }
}

static void growMap(Map *m) {
    int newCapacity = m->capacity == 0 ? 8 : m->capacity * 2;
    Entry *newEntries = malloc(newCapacity * sizeof(Entry));
    for (int i = 0; i < newCapacity; i++) {
        newEntries[i].key.data = NULL;
        newEntries[i].key.len = 0;
    }

    int newCount = 0;
    for (int i = 0; i < m->capacity; i++) {
        Entry *e = &m->entries[i];
        if (e->key.data == NULL || e->key.data == TOMBSTONE) continue;
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

TokenType getValFromMap(Map *m, string key) {
    if (m->count == 0) return TOKEN_NONE;
    Entry *e = findEntry(m->entries, m->capacity, key);
    if (e->key.data == NULL || e->key.data == TOMBSTONE) return TOKEN_NONE;
    return e->val;
}

void insertMap(Map *m, string key, TokenType value) {
    if (m->count + 1 > m->capacity * 0.75) {
        growMap(m);
    }

    Entry *e = findEntry(m->entries, m->capacity, key);
    bool isNewKey = (e->key.data == NULL || e->key.data == TOMBSTONE);
    e->key = key;
    e->val = value;
    if (isNewKey) m->count++;
}

void delMap(Map *m, string key) {
    if (m->count == 0) return;
    Entry *e = findEntry(m->entries, m->capacity, key);
    if (e->key.data == NULL) return; // not found
    e->key.data = TOMBSTONE;
    e->key.len = 0;
}

void dispMap(Map *m) {
    for (int i = 0; i < m->capacity; i++) {
        if (m->entries[i].key.data != NULL && m->entries[i].key.data != TOMBSTONE) {
            printf("%.*s\n", m->entries[i].key.len, m->entries[i].key.data);
        }
    }
}
