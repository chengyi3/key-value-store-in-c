#include "kvstore.h"


// Exercise 4: finish implementing kvstore
// TODO: define your synchronization variables here
// (hint: don't forget to initialize them)


pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
/* read a key from the key-value store.
 *
 * if key doesn't exist, return NULL.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */
char* kv_read(kvstore_t *kv, char *key) {
    pthread_mutex_lock(&mu);
    int i;
    
    for (i = 0; i < TABLE_MAX; i++) {
        if (kv->keys[i].stat == 1 && !strcmp(kv->keys[i].key, key)) {
            printf("[INFO] read key exist [%s]\n", key);
            pthread_mutex_unlock(&mu);
            return kv->values[i];
        }
    }
    


    // delete this later cuz printing is slow
    printf("[INFO] read key not exist [%s]\n", key);
    pthread_mutex_unlock(&mu);
    return NULL;
}
//int s = sizeof(kv->keys)/sizeof(key_entry_t);


/* write a key-value pair into the kv-store.
 *
 * - if the key exists, overwrite the old value.
 * - if key doesn't exit,
 *     -- insert one if the number of keys is smaller than TABLE_MAX
 *     -- return failure if the number of keys equals TABLE_MAX
 * - return 0 for success; return 1 for failures.
 *
 * notes:
 * - the input "val" locates on stack, you must copy the string to
 *   kv-store's own memory. (hint: use malloc)
 * - the "val" is a null-terminated string. Pay attention to how many bytes you
 *   need to allocate. (hint: you need an extra to store '\0').
 * - Read "man strlen" and "man strcpy" to see how they handle string length.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */

int kv_write(kvstore_t *kv, char *key, char *val) {
    pthread_mutex_lock(&mu);
    
    int i;
    int insert = -1;
    for (i = 0; i < TABLE_MAX; i++) {
        if (kv->keys[i].stat == 1) {
            if (!strcmp(kv->keys[i].key, key)) {
                char * r = malloc(sizeof(val) + 1);
                strcpy(r,val);
                kv->values[i] = r;
                printf("[INFO] write key[%s]=val[%s]\n", key, val);
                pthread_mutex_unlock(&mu);
                return 0;
            }
        } else {
            insert = i;
        }
    }
    if (insert != -1) {
        strcpy(kv->keys[insert].key, key);
        char * r = malloc(sizeof(val) + 1);
        strcpy(r,val);
        kv->values[insert] = r;
        kv->keys[insert].stat = 1;
        printf("[INFO] write key[%s]=val[%s]\n", key, val);
        pthread_mutex_unlock(&mu);
        return 0;
        
    }
    
    pthread_mutex_unlock(&mu);
    return 1;
    
}
//int s = sizeof(kv->keys)/sizeof(key_entry_t);


/* delete a key-value pair from the kv-store.
 *
 * - if the key exists, delete it.
 * - if the key doesn't exits, do nothing.
 *
 * NOTE: kv-store must be implemented as a monitor.
 */
void kv_delete(kvstore_t *kv, char *key) {
    pthread_mutex_lock(&mu);
    int i;
    for (i = 0; i < TABLE_MAX; i++) {
        if (kv->keys[i].stat == 1 && !strcmp(kv->keys[i].key, key)) {
            kv->keys[i].stat = 0;
            strcpy(kv->keys[i].key, "");
            free(kv->values[i]);
            pthread_mutex_unlock(&mu);
        }
    }
    pthread_mutex_unlock(&mu);
}
    /* TODO: your code here */

    // delete this later cuz printing is slow
    //printf("[INFO] delete key[%s]\n", key);


//int s = sizeof(kv->keys)/sizeof(key_entry_t);

// print kv-store's contents to stdout
// note: use any format that you like; this is mostly for debugging purpose
void kv_dump(kvstore_t *kv) {
    pthread_mutex_lock(&mu);
    int i;
    for (i = 0; i < TABLE_MAX; i++) {
        if (kv->keys[i].stat == 1) {
            printf("key[%s]=val[%s]\n", kv->keys[i].key, kv->values[i]);
        }
    }
    pthread_mutex_unlock(&mu);
}
