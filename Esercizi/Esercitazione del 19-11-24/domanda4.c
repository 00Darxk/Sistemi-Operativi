#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define MEMORY_POOL_SIZE (1024* 1024* 1024)
#define MIN_BLOCK_SIZE sizeof(Block) 

// Esercizio 4: Implementazione di my_malloc() e my_free():
/**
 * Scrivi un programma che implementi le funzioni my_malloc() e my_free(). Queste funzioni 
 * devono comportarsi rispettivamente come le funzioni malloc() e free(), cioè: la prima 
 * deve ritornare un puntatore void ad una area di memoria utilizzabile della dimensione 
 * specificata come parametro; la seconda deve liberare l'area di memoria alla quale fa 
 * riferimento il puntatore passato come parametro. L'intero programma deve essere 
 * realizzato senza utilizzare le funzioni malloc() e free(). E' possibile utilizzare 
 * una sola volta mmap() e munmap(), per le quali sono disponibili le pagine di man.
 */

typedef struct Block{
    size_t size;        // dimensione blocco
    int free;           // blocco libero (1) o occupato (0) 
    struct Block* next; // blocco successivo 
}Block;

static Block* memory_pool = NULL;
static Block* free_list = NULL;

void init_memory_pool(){
    if(memory_pool == NULL){
        memory_pool = (Block*) mmap(NULL, MEMORY_POOL_SIZE, 
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        if(memory_pool == MAP_FAILED){
            pererror("mmap");
            exit(1);
        }

        memory_pool->size = MEMORY_POOL_SIZE - sizeof(Block);
        memory_pool->free = 1;
        memory_pool->next = NULL;
        free_list = memory_pool;
    }
}


void split_block(Block* block, size_t size){
    Block* new_block = (Block*)((char*)block + sizeof(Block) + size);
    new_block->size = block->size - sizeof(Block) - size;
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->free = 0;
    block->next = new_block;
}

void print_blocks(Block* block){
    int i = 0;
    while(block != NULL){
        printf("Blocco %d:\n", i++);
        printf("\tSize: %lu\n", block->size);
        printf("\tFree: %d\n", block->free);
        block = block->next;
    }
    printf("------------------------\n");
}


void* my_alloc(size_t size){
    if(size <= 0)
        return NULL;
    init_memory_pool();

    Block* current = free_list;
    while(current != NULL){
        if(current->free && current->size >= size){
            if(current->size > size + MIN_BLOCK_SIZE)
                split_block(current, size);
            else
                current->free = 0;
            printf("Memoria allocata, nuovo layout:\n");
            print_blocks(free_list);
            return (void*)((char*)current + sizeof(Block));
        }
        current = curent->next;
    }
    return NULL;
}

void merge_blocks(Block* block){
    while(block->next != NULL && block->next->free){
        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
    }
}

void my_free(void* ptr){
    if(ptr == NULL)
        return NULL;
    
    Block* block = (Block*)((char*)block - sizeof(Block));
    block->free = 1;
    merge_blocks(block);
    printf("Memoria de-allocata, nuovo layout:\n");
    print_blocks(free_list);
}

void cleanup_memory_pool(){
    if(memory_pool != NULL){
        if(munmap(memory_pool, MEMORY_POOL_SIZE) == -1)
            pererror("munmap");
        memory_pool = NULL;
        free_list = NULL;
    }
}

int main(){
    char* ptr1 = (char*) my_malloc(100);
    int* array = my_malloc(1000* sizeof(int));

    my_free(array);
    my_free(ptr1);

    cleanup_memory_pool();
}
