#include "includes/malloc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

// Hardcoded zone boundaries for testing (to avoid exposing internal functions)
#define TEST_TINY_MAX 256    // Up to 256 bytes for TINY
#define TEST_SMALL_MAX 4096  // Up to 4KB for SMALL 
// LARGE is anything > 4KB

// Test helper functions
void test_basic_malloc_free()
{
    printf("\n=== BASIC MALLOC/FREE TEST ===\n");
    
    printf("Testing basic functionality with TINY allocation (64 bytes)\n");
    
    printf("Initial memory state:\n");
    show_alloc_mem();
    
    // Test 1: Basic TINY allocation
    void *ptr1 = malloc(64);  // TINY size
    printf("\nAfter malloc(64) - TINY:\n");
    show_alloc_mem();
    
    assert(ptr1 != NULL);
    
    // Test 2: Write/read data
    const char *test_string = "Hello World from malloc!";
    strcpy((char*)ptr1, test_string);
    printf("Data written and verified: %s\n", (char*)ptr1);
    assert(strcmp((char*)ptr1, test_string) == 0);
    
    // Test 3: Free
    free(ptr1);
    printf("\nAfter free() - memory should be cleaned:\n");
    show_alloc_mem();
    
    printf("✅ BASIC TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_tiny_allocations()
{
    printf("\n=== TINY ALLOCATIONS TEST ===\n");
    
    printf("Testing TINY allocations (1 to %d bytes)\n", TEST_TINY_MAX);
    
    void *ptrs[20];
    
    printf("Initial state:\n");
    show_alloc_mem();
    
    // Allocate many TINY chunks
    for (int i = 0; i < 20; i++)
    {
        size_t size = 1 + (i * TEST_TINY_MAX) / 20;  // 1 byte to TEST_TINY_MAX distributed
        if (size > TEST_TINY_MAX) size = TEST_TINY_MAX;
        
        ptrs[i] = malloc(size);
        assert(ptrs[i] != NULL);
        
        // Write unique data (safely within bounds)
        if (size > 0) {
            memset(ptrs[i], 'A' + (i % 26), size);
        }
        
        printf("TINY alloc %d: %zu bytes\n", i, size);
    }
    
    printf("\nAfter 20 TINY allocations (1-%d bytes each):\n", TEST_TINY_MAX);
    show_alloc_mem();
    
    // Verify data integrity
    for (int i = 0; i < 20; i++)
    {
        size_t size = 1 + (i * TEST_TINY_MAX) / 20;
        if (size > TEST_TINY_MAX) size = TEST_TINY_MAX;
        
        char *data = (char*)ptrs[i];
        for (size_t j = 0; j < size; j++)
        {
            assert(data[j] == 'A' + (i % 26));
        }
    }
    printf("Data integrity verified for all TINY allocations\n");
    
    // Free every other pointer
    for (int i = 0; i < 20; i += 2)
    {
        free(ptrs[i]);
    }
    
    printf("\nAfter freeing every other pointer:\n");
    show_alloc_mem();
    
    // Allocate again (should reuse freed blocks)
    for (int i = 0; i < 1; i++)
    {
        size_t size = TEST_TINY_MAX / 2;  // Use half of max TINY size
        void *new_ptr = malloc(size);
        memset(new_ptr, 'R', size);  // 'R' for reused
        //free(new_ptr);
    }
    
    printf("\nAfter reuse test:\n");
    show_alloc_mem();
    
    // Free remaining pointers
    for (int i = 1; i < 20; i += 2)
    {
        free(ptrs[i]);
    }
    
    printf("\nAfter cleanup:\n");
    show_alloc_mem();
    
    printf("✅ TINY TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_small_allocations()
{
    printf("\n=== SMALL ALLOCATIONS TEST ===\n");
    
    printf("Testing SMALL allocations (%d to %d bytes)\n", TEST_TINY_MAX + 1, TEST_SMALL_MAX);
    
    printf("Initial state before SMALL allocations:\n");
    show_alloc_mem();
    
    void *ptrs[10];
    
    // Allocate SMALL chunks
    for (int i = 0; i < 10; i++)
    {
        size_t size = TEST_TINY_MAX + 1 + (i * (TEST_SMALL_MAX - TEST_TINY_MAX - 1)) / 10;
        if (size > TEST_SMALL_MAX) size = TEST_SMALL_MAX;
        
        ptrs[i] = malloc(size);
        assert(ptrs[i] != NULL);
        
        // Fill with pattern
        memset(ptrs[i], 0x42 + (i % 10), size);
        printf("SMALL alloc %d: %zu bytes\n", i, size);
    }
    
    printf("\nAfter 10 SMALL allocations (%d-%d bytes each):\n", 
           TEST_TINY_MAX + 1, TEST_SMALL_MAX);
    show_alloc_mem();
    
    // Verify patterns
    for (int i = 0; i < 10; i++)
    {
        size_t size = TEST_TINY_MAX + 1 + (i * (TEST_SMALL_MAX - TEST_TINY_MAX - 1)) / 10;
        if (size > TEST_SMALL_MAX) size = TEST_SMALL_MAX;
        
        unsigned char *data = (unsigned char*)ptrs[i];
        for (size_t j = 0; j < size; j++)
        {
            assert(data[j] == (unsigned char)(0x42 + (i % 10)));
        }
    }
    printf("Small allocation patterns verified\n");
    
    // Free all
    for (int i = 0; i < 10; i++)
    {
        free(ptrs[i]);
    }
    
    printf("\nAfter freeing all SMALL allocations:\n");
    show_alloc_mem();
    
    printf("✅ SMALL TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_large_allocations()
{
    printf("\n=== LARGE ALLOCATIONS TEST ===\n");
    
    printf("Testing LARGE allocations (> %d bytes)\n", TEST_SMALL_MAX);
    
    printf("Initial state before LARGE allocations:\n");
    show_alloc_mem();
    
    // Test LARGE allocations (>TEST_SMALL_MAX)
    size_t large_size1 = TEST_SMALL_MAX + 1;
    size_t large_size2 = TEST_SMALL_MAX * 2;
    size_t large_size3 = TEST_SMALL_MAX * 5;
    
    void *large1 = malloc(large_size1);
    void *large2 = malloc(large_size2);
    void *large3 = malloc(large_size3);
    
    printf("\nAfter 3 LARGE allocations (%zu, %zu, %zu bytes):\n", 
           large_size1, large_size2, large_size3);
    show_alloc_mem();
    
    assert(large1 != NULL);
    assert(large2 != NULL);
    assert(large3 != NULL);
    
    // Write large data patterns
    memset(large1, 0xAA, large_size1);
    memset(large2, 0xBB, large_size2);
    memset(large3, 0xCC, large_size3);
    
    printf("\nAfter writing data patterns:\n");
    show_alloc_mem();
    
    // Verify large patterns (sample verification for performance)
    unsigned char *data1 = (unsigned char*)large1;
    unsigned char *data2 = (unsigned char*)large2;
    unsigned char *data3 = (unsigned char*)large3;
    
    // Check first 1000 bytes and last 100 bytes to verify integrity
    for (size_t i = 0; i < 1000 && i < large_size1; i++) 
        assert(data1[i] == 0xAA);
    for (size_t i = large_size1 - 100; i < large_size1; i++) 
        assert(data1[i] == 0xAA);
    
    for (size_t i = 0; i < 1000 && i < large_size2; i++) 
        assert(data2[i] == 0xBB);
    for (size_t i = large_size2 - 100; i < large_size2; i++) 
        assert(data2[i] == 0xBB);
    
    for (size_t i = 0; i < 1000 && i < large_size3; i++) 
        assert(data3[i] == 0xCC);
    for (size_t i = large_size3 - 100; i < large_size3; i++) 
        assert(data3[i] == 0xCC);
    
    printf("Large allocation patterns verified (sampled)\n");
    
    free(large1);
    free(large2);
    free(large3);
    
    printf("\nAfter freeing all LARGE allocations:\n");
    show_alloc_mem();
    
    printf("✅ LARGE TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_mixed_allocations()
{
    printf("\n=== MIXED ALLOCATIONS TEST ===\n");
    
    printf("Testing mixed allocation types:\n");
    printf("- TINY: %d bytes\n", TEST_TINY_MAX / 2);
    printf("- SMALL: %d bytes\n", (TEST_TINY_MAX + TEST_SMALL_MAX) / 2);
    printf("- LARGE: %d bytes\n", TEST_SMALL_MAX * 2);

    printf("Initial state:\n");
    show_alloc_mem();
    
    void *tiny = malloc(TEST_TINY_MAX / 2);          // TINY allocation
    void *small = malloc((TEST_TINY_MAX + TEST_SMALL_MAX) / 2);  // SMALL allocation  
    void *large = malloc(TEST_SMALL_MAX * 2);        // LARGE allocation
    
    printf("\nAfter mixed allocations (TINY:%d, SMALL:%d, LARGE:%d):\n", 
           TEST_TINY_MAX / 2, (TEST_TINY_MAX + TEST_SMALL_MAX) / 2, TEST_SMALL_MAX * 2);
    show_alloc_mem();
    
    assert(tiny != NULL);
    assert(small != NULL);
    assert(large != NULL);
    
    // Write and verify data safely
    memset(tiny, 'T', TEST_TINY_MAX / 2);
    memset(small, 'S', (TEST_TINY_MAX + TEST_SMALL_MAX) / 2);
    memset(large, 'L', TEST_SMALL_MAX * 2);
    
    // Verify integrity
    char *tiny_data = (char*)tiny;
    char *small_data = (char*)small;
    char *large_data = (char*)large;
    
    for (size_t i = 0; i < TEST_TINY_MAX / 2; i++)
        assert(tiny_data[i] == 'T');
    for (size_t i = 0; i < (TEST_TINY_MAX + TEST_SMALL_MAX) / 2; i++)
        assert(small_data[i] == 'S');
    for (size_t i = 0; i < 1000; i++)  // Sample check for large
        assert(large_data[i] == 'L');
    
    printf("Mixed allocation data integrity verified\n");

    free(tiny);
    free(small);
    free(large);
    
    printf("\nAfter freeing mixed allocations:\n");
    show_alloc_mem();

    printf("✅ MIXED TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_stress()
{
    printf("\n=== STRESS TEST ===\n");
    
    printf("Stress testing with zone boundaries:\n");
    printf("- TINY: 1 to %d bytes\n", TEST_TINY_MAX);
    printf("- SMALL: %d to %d bytes\n", TEST_TINY_MAX + 1, TEST_SMALL_MAX);
    printf("- LARGE: > %d bytes\n", TEST_SMALL_MAX);
    
    printf("Initial state before stress test:\n");
    show_alloc_mem();
    
    void *ptrs[1000];
    size_t sizes[1000];
    
    // Random allocations using zone boundaries
    for (int i = 0; i < 1000; i++)
    {
        int type = i % 3;
        if (type == 0) {
            // TINY allocation
            sizes[i] = 1 + (i % TEST_TINY_MAX);
        } else if (type == 1) {
            // SMALL allocation
            sizes[i] = TEST_TINY_MAX + 1 + (i % (TEST_SMALL_MAX - TEST_TINY_MAX));
        } else {
            // LARGE allocation
            sizes[i] = TEST_SMALL_MAX + 1 + (i % (TEST_SMALL_MAX * 2));
        }
        
        ptrs[i] = malloc(sizes[i]);
        if (i % 100 == 0) printf("Stress allocation %d/1000 (size: %zu)\n", i, sizes[i]);
        
        if (ptrs[i]) {
            // Write pattern based on allocation type
            unsigned char pattern = (unsigned char)(i % 256);
            memset(ptrs[i], pattern, sizes[i] > 1000 ? 1000 : sizes[i]);  // Limit for performance
        }
    }
    
    printf("\nAfter 1000 stress allocations:\n");
    show_alloc_mem();
    
    // Verify some random ones (sample for performance)
    for (int i = 0; i < 1000; i += 50)
    {
        if (ptrs[i]) {
            unsigned char *data = (unsigned char*)ptrs[i];
            unsigned char expected = (unsigned char)(i % 256);
            size_t check_size = sizes[i] > 1000 ? 1000 : sizes[i];
            
            for (size_t j = 0; j < 10 && j < check_size; j++)
            {
                assert(data[j] == expected);
            }
        }
    }
    printf("Stress test data integrity verified (sampled)\n");
    
    // Free randomly
    for (int i = 0; i < 1000; i++)
    {
        if (i % 3 == 0 && ptrs[i]) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
    
    printf("\nAfter freeing 1/3 of allocations randomly:\n");
    show_alloc_mem();
    
    // Free remaining
    for (int i = 0; i < 1000; i++)
    {
        if (ptrs[i]) {
            free(ptrs[i]);
        }
    }
    
    printf("\nAfter freeing all remaining allocations:\n");
    show_alloc_mem();
    
    printf("✅ STRESS TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_show_alloc_mem()
{
    printf("\n=== SHOW_ALLOC_MEM TEST ===\n");
    
    printf("Testing memory visualization with zone boundaries\n");
    
    printf("Initial state:\n");
    show_alloc_mem();
    
    // Allocate some test data using zone boundaries
    void *tiny1 = malloc(TEST_TINY_MAX / 3);
    void *tiny2 = malloc(TEST_TINY_MAX * 2 / 3);
    void *small1 = malloc((TEST_TINY_MAX + TEST_SMALL_MAX) / 2);
    void *large1 = malloc(TEST_SMALL_MAX + 1000);
    void *large2 = malloc(TEST_SMALL_MAX * 2);
    
    strcpy((char*)tiny1, "tiny data 1");
    strcpy((char*)tiny2, "tiny data 2 - longer");
    strcpy((char*)small1, "small allocation data");
    strcpy((char*)large1, "large allocation 1");
    strcpy((char*)large2, "large allocation 2");
    
    printf("\nAfter allocations (T:%d, T:%d, S:%d, L:%d, L:%d):\n",
           TEST_TINY_MAX / 3, TEST_TINY_MAX * 2 / 3, (TEST_TINY_MAX + TEST_SMALL_MAX) / 2, 
           TEST_SMALL_MAX + 1000, TEST_SMALL_MAX * 2);
    show_alloc_mem();
    
    // Free some blocks
    free(tiny1);
    free(large1);
    
    printf("\nAfter freeing tiny1 and large1:\n");
    show_alloc_mem();
    
    // Allocate to test reuse
    void *reused = malloc(TEST_TINY_MAX / 4);
    strcpy((char*)reused, "reused block");
    
    printf("\nAfter reusing freed block (%d bytes):\n", TEST_TINY_MAX / 4);
    show_alloc_mem();
    
    // Clean up
    free(tiny2);
    free(small1);
    free(large2);
    free(reused);
    
    printf("\nAfter cleaning up:\n");
    show_alloc_mem();
    
    printf("✅ SHOW_ALLOC_MEM TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_realloc()
{
    printf("\n=== REALLOC TEST ===\n");
    
    printf("Testing realloc with zone transitions:\n");
    printf("- TINY boundary: %d bytes\n", TEST_TINY_MAX);
    printf("- SMALL boundary: %d bytes\n", TEST_SMALL_MAX);
    
    printf("Initial state before realloc tests:\n");
    show_alloc_mem();
    
    // Test 1: NULL pointer (should act like malloc)
    void *ptr1 = realloc(NULL, TEST_TINY_MAX / 2);
    printf("\nAfter realloc(NULL, %d):\n", TEST_TINY_MAX / 2);
    show_alloc_mem();
    assert(ptr1 != NULL);
    strcpy((char*)ptr1, "Initial TINY data");
    
    // Test 2: Zero size (should act like free)
    void *ptr2 = realloc(ptr1, 0);
    printf("\nAfter realloc(ptr, 0) - should free:\n");
    show_alloc_mem();
    assert(ptr2 == NULL);
    
    // Test 3: TINY expansion within same type
    void *ptr3 = malloc(TEST_TINY_MAX / 4);
    strcpy((char*)ptr3, "TINY data");
    printf("\nOriginal TINY allocation (%d bytes):\n", TEST_TINY_MAX / 4);
    show_alloc_mem();
    
    ptr3 = realloc(ptr3, TEST_TINY_MAX * 3 / 4);  // Still TINY
    printf("\nAfter TINY expansion (%d->%d bytes):\n", TEST_TINY_MAX / 4, TEST_TINY_MAX * 3 / 4);
    show_alloc_mem();
    assert(ptr3 != NULL);
    assert(strcmp((char*)ptr3, "TINY data") == 0);
    printf("Data preserved after TINY expansion\n");
    
    // Test 4: TINY shrinking
    ptr3 = realloc(ptr3, TEST_TINY_MAX / 8);
    printf("\nAfter TINY shrinking (%d->%d bytes):\n", TEST_TINY_MAX * 3 / 4, TEST_TINY_MAX / 8);
    show_alloc_mem();
    assert(ptr3 != NULL);
    assert(strncmp((char*)ptr3, "TINY data", 9) == 0);
    printf("Data preserved after TINY shrinking\n");
    
    // Test 5: TINY to SMALL transition
    size_t small_size = (TEST_TINY_MAX + TEST_SMALL_MAX) / 2;
    ptr3 = realloc(ptr3, small_size);  // TINY -> SMALL
    printf("\nAfter TINY->SMALL transition (%d->%zu bytes):\n", TEST_TINY_MAX / 8, small_size);
    show_alloc_mem();
    assert(ptr3 != NULL);
    assert(strncmp((char*)ptr3, "TINY data", 9) == 0);
    strcpy((char*)ptr3 + 10, " + SMALL extension");
    printf("Data preserved after type transition\n");
    
    // Test 6: SMALL to LARGE transition
    size_t large_size = TEST_SMALL_MAX * 2;
    ptr3 = realloc(ptr3, large_size);  // SMALL -> LARGE
    printf("\nAfter SMALL->LARGE transition (%zu->%zu bytes):\n", small_size, large_size);
    show_alloc_mem();
    assert(ptr3 != NULL);
    assert(strncmp((char*)ptr3, "TINY data", 9) == 0);
    strcpy((char*)ptr3 + 30, " + LARGE data here");
    printf("Data preserved after SMALL->LARGE transition\n");
    
    // Test 7: LARGE to SMALL transition
    size_t back_to_small = TEST_SMALL_MAX / 2;
    ptr3 = realloc(ptr3, back_to_small);  // LARGE -> SMALL
    printf("\nAfter LARGE->SMALL transition (%zu->%zu bytes):\n", large_size, back_to_small);
    show_alloc_mem();
    assert(ptr3 != NULL);
    assert(strncmp((char*)ptr3, "TINY data", 9) == 0);
    printf("Data preserved after LARGE->SMALL transition\n");
    
    // Test 8: Multiple reallocations with memory state tracking
    for (int i = 0; i < 5; i++)
    {
        size_t new_size;
        if (i % 3 == 0) new_size = TEST_TINY_MAX / 2 + i * 10;     // TINY
        else if (i % 3 == 1) new_size = TEST_SMALL_MAX / 2 + i * 100;  // SMALL
        else new_size = TEST_SMALL_MAX + i * 1000;                 // LARGE
        
        ptr3 = realloc(ptr3, new_size);
        printf("\nMulti-realloc %d - size %zu:\n", i, new_size);
        show_alloc_mem();
        assert(ptr3 != NULL);
        assert(strncmp((char*)ptr3, "TINY data", 9) == 0);
        
        // Add data pattern
        if (new_size > 50) {
            memset((char*)ptr3 + 50, 'A' + i, 50);
        }
    }
    printf("Multiple reallocations successful\n");
    
    // Test 9: Same size (should return same pointer)
    void *old_ptr = ptr3;
    size_t current_size = TEST_SMALL_MAX / 2;
    ptr3 = realloc(ptr3, current_size);
    printf("\nSame size realloc test - should be identical pointers:\n");
    show_alloc_mem();
    printf("Old ptr: %p, New ptr: %p\n", old_ptr, ptr3);
    
    // Cleanup
    free(ptr3);
    printf("\nAfter final cleanup:\n");
    show_alloc_mem();
    printf("✅ REALLOC TEST PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

void test_edge_cases()
{
    printf("\n=== EDGE CASES TEST ===\n");
    
    printf("Testing edge cases and boundary conditions\n");
    
    printf("Initial state before edge case tests:\n");
    show_alloc_mem();
    
    // Test NULL free
    free(NULL);
    printf("\nAfter free(NULL) - should be unchanged:\n");
    show_alloc_mem();
    printf("free(NULL) handled correctly\n");
    
    // Test zero malloc
    void *zero_ptr = malloc(0);
    printf("\nAfter malloc(0):\n");
    show_alloc_mem();
    if (zero_ptr) {
        printf("malloc(0) returned non-NULL pointer\n");
        free(zero_ptr);
    } else {
        printf("malloc(0) returned NULL\n");
    }
    
    // Test boundary allocations
    printf("\nTesting boundary allocations:\n");
    
    // Exactly at TINY boundary
    void *tiny_max = malloc(TEST_TINY_MAX);
    printf("malloc(%d) - max TINY allocation:\n", TEST_TINY_MAX);
    show_alloc_mem();
    
    // Exactly at SMALL boundary start
    void *small_min = malloc(TEST_TINY_MAX + 1);
    printf("malloc(%d) - min SMALL allocation:\n", TEST_TINY_MAX + 1);
    show_alloc_mem();
    
    // Exactly at SMALL boundary
    void *small_max = malloc(TEST_SMALL_MAX);
    printf("malloc(%d) - max SMALL allocation:\n", TEST_SMALL_MAX);
    show_alloc_mem();
    
    // Exactly at LARGE boundary start
    void *large_min = malloc(TEST_SMALL_MAX + 1);
    printf("malloc(%d) - min LARGE allocation:\n", TEST_SMALL_MAX + 1);
    show_alloc_mem();
    
    // Test data integrity at boundaries
    if (tiny_max) {
        memset(tiny_max, 0xAA, TEST_TINY_MAX);
        free(tiny_max);
    }
    if (small_min) {
        memset(small_min, 0xBB, TEST_TINY_MAX + 1);
        free(small_min);
    }
    if (small_max) {
        memset(small_max, 0xCC, 1000);  // Sample write for performance
        free(small_max);
    }
    if (large_min) {
        memset(large_min, 0xDD, 1000);  // Sample write for performance
        free(large_min);
    }
    
    // Test very large malloc
    size_t huge_size = 1024 * 1024;  // 1MB
    void *huge = malloc(huge_size);
    printf("\nAfter malloc(%zu) - 1MB allocation:\n", huge_size);
    show_alloc_mem();
    if (huge) {
        strcpy((char*)huge, "HUGE ALLOCATION TEST DATA");
        assert(strcmp((char*)huge, "HUGE ALLOCATION TEST DATA") == 0);
        printf("1MB allocation successful and verified\n");
        free(huge);
        printf("\nAfter freeing 1MB allocation:\n");
        show_alloc_mem();
    } else {
        printf("1MB allocation failed (system limits?)\n");
    }
    
    // Test double free protection (if implemented)
    void *test_ptr = malloc(100);
    free(test_ptr);
    printf("\nTesting double free (should be handled gracefully):\n");
    free(test_ptr);  // This should not crash
    show_alloc_mem();
    
    printf("✅ EDGE CASES PASSED\n");
    printf("Press ENTER to continue...");
    getchar();
}

int main()
{
    printf("🐕 KÖPEK GİBİ MALLOC TEST BAŞLIYOR! 🐕\n");
    printf("================================================\n");
    printf("Zone boundaries for testing:\n");
    printf("- TINY: 1 to %d bytes\n", TEST_TINY_MAX);
    printf("- SMALL: %d to %d bytes\n", TEST_TINY_MAX + 1, TEST_SMALL_MAX);
    printf("- LARGE: > %d bytes\n", TEST_SMALL_MAX);
    printf("================================================\n");
    
    test_basic_malloc_free();
    
    printf("\n💾 Memory state after BASIC test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_tiny_allocations();
    
    printf("\n💾 Memory state after TINY test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_small_allocations();
    
    printf("\n💾 Memory state after SMALL test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_large_allocations();
    
    printf("\n💾 Memory state after LARGE test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_mixed_allocations();
    
    printf("\n💾 Memory state after MIXED test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_show_alloc_mem();
    
    printf("\n💾 Memory state after SHOW_ALLOC_MEM test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_realloc();
    
    printf("\n💾 Memory state after REALLOC test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_stress();
    
    printf("\n💾 Memory state after STRESS test:\n");
    show_alloc_mem();
    printf("================================================\n");
    
    test_edge_cases();
    
    printf("\n💾 FINAL Memory state after all tests:\n");
    show_alloc_mem();
    printf("\n================================================\n");
    printf("🎉 TÜM TESTLER BAŞARIYLA GEÇTİ! 🎉\n");
    printf("🐕 KÖPEK GİBİ TEST TAMAMLANDI! 🐕\n");
    
    return 0;
}