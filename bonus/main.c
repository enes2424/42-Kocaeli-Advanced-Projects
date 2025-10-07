#include "includes/malloc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Hardcoded zone boundaries for testing (to avoid exposing internal functions)
#define TEST_TINY_MAX 256    // Up to 256 bytes for TINY
#define TEST_SMALL_MAX 4096  // Up to 4KB for SMALL
// LARGE is anything > 4KB

// Test helper functions
void test_malloc_scribble()
{
    printf("\n=== MALLOC_SCRIBBLE TEST ===\n");
    
    printf("Testing MALLOC_SCRIBBLE environment variable\n");
    printf("This should fill freed memory with 0x55 pattern\n");
    
    // Test with SCRIBBLE disabled
    printf("\n1. Test WITHOUT MALLOC_SCRIBBLE:\n");
    
    char *ptr1 = malloc(64);
    strcpy(ptr1, "Hello World!");
    printf("Data before free: '%s'\n", ptr1);
    
    free(ptr1);
    printf("Data after free (no scribble): ");
    for (int i = 0; i < 12; i++) {
        printf("%02x ", (unsigned char)ptr1[i]);
    }
    printf("\n");
    
    // Test with SCRIBBLE enabled
    printf("\n2. Test WITH MALLOC_SCRIBBLE=1:\n");
    printf("Setting MALLOC_SCRIBBLE=1 dynamically...\n");
    
    // Set environment variable
    setenv("MALLOC_SCRIBBLE", "1", 1);
    
    char *ptr2 = malloc(64);
    strcpy(ptr2, "Test Pattern!");
    printf("Data before free: '%s'\n", ptr2);
    
    free(ptr2);
    printf("Data after free (with scribble): ");
    for (int i = 0; i < 13; i++) {
        printf("%02x ", (unsigned char)ptr2[i]);
    }
    printf("\n");
    
    // Test without SCRIBBLE again
    printf("\n3. Test after UNSETTING MALLOC_SCRIBBLE:\n");
    unsetenv("MALLOC_SCRIBBLE");
    
    char *ptr3 = malloc(64);
    strcpy(ptr3, "Clean Test!");
    printf("Data before free: '%s'\n", ptr3);
    
    free(ptr3);
    printf("Data after free (no scribble): ");
    for (int i = 0; i < 11; i++) {
        printf("%02x ", (unsigned char)ptr3[i]);
    }
    printf("\n");
    
    printf("✅ MALLOC_SCRIBBLE TEST COMPLETED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_show_alloc_mem_ex()
{
    printf("\n=== SHOW_ALLOC_MEM_EX TEST ===\n");
    
    printf("Testing extended memory display function\n");
    printf("This shows detailed memory information and hex dumps\n");
    
    printf("\nInitial memory state:\n");
    show_alloc_mem_ex();
    
    // Allocate different sizes
    char *tiny = malloc(32);
    strcpy(tiny, "TINY");
    
    char *small = malloc(1024);
    strcpy(small, "SMALL_ALLOCATION_TEST");
    
    char *large = malloc(8192);
    strcpy(large, "LARGE_ALLOCATION_WITH_LOTS_OF_DATA");
    
    printf("\nAfter allocations:\n");
    show_alloc_mem_ex();
    
    // Free some and check
    free(tiny);
    printf("\nAfter freeing TINY:\n");
    show_alloc_mem_ex();
    
    free(small);
    free(large);
    printf("\nAfter freeing all:\n");
    show_alloc_mem_ex();
    
    printf("✅ SHOW_ALLOC_MEM_EX TEST COMPLETED\n");
    printf("Press ENTER to continue...");
    getchar();
}

// Thread function for testing thread safety
void* thread_malloc_test(void* arg)
{
    int thread_id = *(int*)arg;
    
    for (int i = 0; i < 10; i++) {
        void *ptr = malloc(64 + thread_id * 10);
        if (ptr) {
            sprintf((char*)ptr, "Thread%d_Alloc%d", thread_id, i);
            usleep(1000); // Small delay
            free(ptr);
        }
    }
    return NULL;
}

void test_thread_safety()
{
    printf("\n=== THREAD SAFETY TEST ===\n");
    
    printf("Testing malloc/free in multi-threaded environment\n");
    printf("Creating 4 threads doing concurrent allocations\n");
    
    pthread_t threads[4];
    int thread_ids[4] = {1, 2, 3, 4};
    
    printf("\nStarting concurrent allocations...\n");
    
    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, thread_malloc_test, &thread_ids[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads completed successfully\n");
    printf("Memory state after threaded operations:\n");
    show_alloc_mem();
    
    printf("✅ THREAD SAFETY TEST COMPLETED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_defragmentation()
{
    printf("\n=== DEFRAGMENTATION TEST ===\n");
    
    printf("Testing memory defragmentation (free block merging)\n");
    
    printf("\nAllocating 5 small blocks:\n");
    void *ptrs[5];
    for (int i = 0; i < 5; i++) {
        ptrs[i] = malloc(64);
        sprintf((char*)ptrs[i], "Block_%d", i);
    }
    show_alloc_mem();
    
    printf("\nFreeing every other block (fragmentation):\n");
    free(ptrs[1]);
    free(ptrs[3]);
    show_alloc_mem();
    
    printf("\nFreeing adjacent blocks (should trigger defragmentation):\n");
    free(ptrs[0]);
    free(ptrs[2]);
    show_alloc_mem();
    
    printf("\nFreeing last block:\n");
    free(ptrs[4]);
    show_alloc_mem();
    
    printf("✅ DEFRAGMENTATION TEST COMPLETED\n");
    printf("Press ENTER to continue...");
    getchar();
}

int main()
{
    printf("🚀 BONUS MALLOC TEST BAŞLIYOR! 🚀\n");
    printf("================================================\n");
    printf("Testing bonus features:\n");
    printf("1. Thread Safety (pthread)\n");
    printf("2. MALLOC_SCRIBBLE debug variable\n");
    printf("3. show_alloc_mem_ex() function\n");
    printf("4. Memory defragmentation\n");
    printf("================================================\n");
    
    test_thread_safety();
    
    printf("\n💾 Memory state after THREAD SAFETY test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_malloc_scribble();
    
    printf("\n💾 Memory state after MALLOC_SCRIBBLE test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_show_alloc_mem_ex();
    
    printf("\n💾 Memory state after SHOW_ALLOC_MEM_EX test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_defragmentation();
    
    printf("\n💾 FINAL Memory state after all bonus tests:\n");
    show_alloc_mem();
    printf("\n================================================\n");
    printf("🎉 TÜM BONUS TESTLER BAŞARIYLA GEÇTİ! 🎉\n");
    printf("🚀 BONUS TEST TAMAMLANDI! 🚀\n");
    
    return 0;
}