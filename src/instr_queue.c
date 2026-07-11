#include "instr_queue.h"

#include <assert.h>

#include "svec.h"


#define INSTR_QUEUE_MAX_LENGTH 6

typedef struct Node {
    uint8_t data;
    struct Node* next;
} Node;

struct instrqueue{
    Node* head; 
    Node* tail;
    int size;
};

static Node* priv_init_node(uint16_t data);

InstrQueue* iqueue_init(void) {
    InstrQueue* iqueue = xmalloc(sizeof(*iqueue));
    memset(iqueue, 0, sizeof(*iqueue));

    return iqueue;
}

void iqueue_free(InstrQueue* iqueue) {
    if (iqueue == NULL) {
        return;
    }
    iqueue_reset(iqueue);
    free(iqueue);
}

int8_t iqueue_get_size(InstrQueue* iqueue) {
    assert(iqueue != NULL);
    return iqueue->size;
}

void iqueue_enqueue(InstrQueue* iqueue, uint16_t value) {
    if (iqueue == NULL || iqueue->size == INSTR_QUEUE_MAX_LENGTH) {
        return;    
    } 

    Node* node = priv_init_node(value);
    if (iqueue->head == NULL) {
        iqueue->head = iqueue->tail = node;
    }
    else {
        iqueue->tail->next = node; 
        iqueue->tail = node;
    }
    iqueue->size++; 
}


int8_t iqueue_dequeue(InstrQueue* iqueue) {
    assert(iqueue != NULL);
    Node* temp = iqueue->head;
    int retVal = temp->data;

    iqueue->head = iqueue->head->next;
    free(temp);
    
    iqueue->size--;
    return retVal;
}
    
void iqueue_reset(InstrQueue* iqueue) {
    assert(iqueue != NULL);

    Node* current = iqueue->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
}                     

int8_t iqueue_is_full(InstrQueue* iqueue) {
    assert(iqueue != NULL);
    if (iqueue->size == INSTR_QUEUE_MAX_LENGTH) {
        return 1;
    }
    return 0;
}

static Node* priv_init_node(uint16_t data) {
    Node* newNode = xmalloc(sizeof(*newNode));
    newNode->data = data;
    newNode->next = NULL;

    return newNode;
}
