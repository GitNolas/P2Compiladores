#define _GNU_SOURCE /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "palabrasReservadas.h"
#include "valoresAuxiliares.h"
#include "tablaSimbolos.h"
#include <ctype.h>

typedef struct entry_s entry_t;

hashtable_t *tabla;

typedef struct hashtable_s hashtable_t;

/* Create a new hashtable. */
hashtable_t *ht_create(int size) {

    hashtable_t *hashtable = NULL;
    int i;

    if (size < 1) return NULL;

    /* Allocate the table itself. */
    if ((hashtable = malloc(sizeof ( hashtable_t))) == NULL) {
        return NULL;
    }

    /* Allocate pointers to the head nodes. */
    if ((hashtable->table = malloc(sizeof ( entry_t *) * size)) == NULL) {
        return NULL;
    }
    for (i = 0; i < size; i++) {
        hashtable->table[i] = NULL;
    }

    hashtable->size = size;

    return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash(hashtable_t *hashtable, char *key) {

    unsigned long int hashval=0;
    int i = 0;
    int x=strlen(key);
    /* Convert our string to an integer */
    while (hashval < ULONG_MAX && (i < x)) {
        hashval = hashval << 8;
        hashval += key[ i ];
        i++;
    }

    return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair(char *key, int value) {
    entry_t *newpair;

    if ((newpair = malloc(sizeof ( entry_t))) == NULL) {
        return NULL;
    }

    if ((newpair->key = strdup(key)) == NULL) {
        return NULL;
    }


    newpair->value=value;
    newpair->next = NULL;

    return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set(hashtable_t *hashtable, char *key, int value) {
    int bin = 0;
    entry_t *newpair = NULL;
    entry_t *next = NULL;
    entry_t *last = NULL;

    bin = ht_hash(hashtable, key);

    next = hashtable->table[ bin ];

    while (next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
        last = next;
        next = next->next;
    }

    /* There's already a pair.  Let's replace that string. */
    if (next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {

        next->value = value;

        /* Nope, could't find it.  Time to grow a pair. */
    } else {
        newpair = ht_newpair(key, value);

        /* We're at the start of the linked list in this bin. */
        if (next == hashtable->table[ bin ]) {
            newpair->next = next;
            hashtable->table[ bin ] = newpair;

            /* We're at the end of the linked list in this bin. */
        } else if (next == NULL) {
            last->next = newpair;

            /* We're in the middle of the list. */
        } else {
            newpair->next = next;
            last->next = newpair;
        }
    }
}

/* Retrieve a key-value pair from a hash table. */
int ht_get(hashtable_t *hashtable, char *key) {
    int bin = 0;
    entry_t *pair;

    bin = ht_hash(hashtable, key);

    /* Step through the bin, looking for our value. */
    pair = hashtable->table[ bin ];
    while (pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if (pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
        return -1;

    } else {
        return pair->value;
    }
}
void imprimirTaboa(){
    printf("\033[32m--------INICIO TÁBOA DE SÍMBOLOS--------\n");
    for(int i=0;i<tabla->size;i++){
        if(tabla->table[i]!= NULL){
            printf("################################\n");
            printf("%d: Key->%s         Value->%d\n", i,tabla->table[i]->key, tabla->table[i]->value);
        }
    }
    printf("--------FIN TÁBOA DE SÍMBOLOS--------\n\033[0m");
}
void inicializarTablaSimbolos() {
    tabla = ht_create(65536);
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen("palabrasReservadas.h", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int i = 0, j = 0;
    char delim[] = "//";
    char *ptr;
    char *key;
    int value = PACKAGE;
    while ((read = getline(&line, &len, fp)) != -1) {

        strtok(line, delim);
        ptr = strtok(NULL, delim);
        key = (char*) malloc(strlen(ptr));
        for (int i = 0; i < strlen(ptr); i++) {
            if (isalpha(ptr[i])) {
                key[i] = ptr[i];
            }
        }
        ht_set(tabla, key, value);
        memset(key, '\0', strlen(key));
        free(key);
        value++;
    }
    fclose(fp);
    imprimirTaboa();
    if (line) {
        free(line);
    }
}

int comprobarTabla(char *key) {
    return (ht_get(tabla,key)!=-1);
}
int returnTabla(char *key) {
  if(!comprobarTabla(key)){
    ht_set(tabla, key, ID);
  }
  return ht_get(tabla,key);
}

void insertarTabla(char *key, int value){
    ht_set(tabla, key, value);
}
