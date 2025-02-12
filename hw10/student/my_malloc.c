/*
 * CS 2110 Homework 10 Spring 2024
 * Author:
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

/* Function Headers
 * Here is a place to put all of your function headers
 * Remember to declare them as static
 */

/* Our freelist structure - our freelist is represented as a singly linked list
 * the freelist is sorted by address;
 */
metadata_t *address_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;



// -------------------- PART 1: Helper functions --------------------

/* The following prototypes represent useful helper functions that you may want
 * to use when writing your malloc functions. We recommend reading over their
 * documentation and prototypes; having a good idea of the kinds of helpers
 * you can use will make it easier to plan your code.
 *
 * None of these functions will be graded individually. However, implementing
 * and using these will make programming easier. We have provided ungraded test
 * cases these functions that you may check your implementations against.
 */


/* HELPER FUNCTION: find_right
 * Given a pointer to a free block, this function searches the freelist for another block to the right of the provided block.
 * If there is a free block that is directly next to the provided block on its right side,
 * then return a pointer to the start of the right-side block.
 * Otherwise, return null.
 * This function may be useful when implementing my_free().
 */
metadata_t *find_right(metadata_t *freed_block) {
    metadata_t *current = address_list;
    while (current != NULL) {
        if (current == (metadata_t *)((char*) freed_block + TOTAL_METADATA_SIZE
            + freed_block->size)) {
                return current;
            }
        current = current->next;
    }
    return NULL;
}

/* GIVEN HELPER FUNCTION: find_left
 * This function is provided for you by the TAs. You do not need to use it, but it may be helpful to you.
 * This function is the same as find_right, but for the other side of the newly freed block.
 * This function will be useful for my_free(), but it is also useful for my_malloc(), since whenever you sbrk a new block,
 * you need to merge it with the block at the back of the freelist if the blocks are next to each other in memory.
 */

metadata_t *find_left(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr && ((uintptr_t) freed_block > (uintptr_t) curr)) {
        uintptr_t right_of_curr = ((uintptr_t) curr) + TOTAL_METADATA_SIZE + curr->size;
        if (right_of_curr == (uintptr_t) freed_block) {
            return curr; // if `right_of_curr` is the freed block, then the `curr` block is directly left of `freed_block`
        }
        curr = curr->next;
    }
    
    return NULL;
}

/* HELPER FUNCTION: merge
 * This function should take two pointers to blocks and merge them together.
 * The most important step is to increase the total size of the left block to include the size of the right block.
 * You should also copy the right block's next pointer to the left block's next pointer. If both blocks are initially in the freelist, this will remove the right block from the list.
 * This function will be useful for both my_malloc() (when you have to merge sbrk'd blocks) and my_free().
 */
 void merge(metadata_t *left, metadata_t *right) {
    // Check for NULL arguments
  if (left == NULL || right == NULL) {
    return;
  }

  // Increase left block size to include right block size
  left->size += right->size + TOTAL_METADATA_SIZE;

  // Update left block's next pointer to point to right block's next block
  left->next = right->next;
 }

/* HELPER FUNCTION: split_block
 * This function should take a pointer to a large block and a requested size, split the block in two, and return a pointer to the new block (the right part of the split).
 * Remember that you must make the right side have the user-requested size when splitting. The left side of the split should have the remaining data.
 * We recommend doing the following steps:
 * 1. Compute the total amount of memory that the new block will take up (both metadata and user data).
 * 2. Using the new block's total size with the address and size of the old block, compute the address of the start of the new block.
 * 3. Shrink the size of the old/left block to account for the lost size. This block should stay in the freelist.
 * 4. Set the size of the new/right block and return it. This block should not go in the freelist.
 * This function will be useful for my_malloc(), particularly when the best-fit block is big enough to be split.
 */
 metadata_t *split_block(metadata_t *block, size_t size) {
    remove_from_addr_list(block);
    metadata_t *ret = (metadata_t *)((char*) block + TOTAL_METADATA_SIZE + block->size - size - TOTAL_METADATA_SIZE);
    block->size -= (size + TOTAL_METADATA_SIZE);
    add_to_addr_list(block);
    ret->size = size;
    return ret;
 }

/* HELPER FUNCTION: add_to_addr_list
 * This function should add a block to freelist.
 * Remember that the freelist must be sorted by address. You can compare the addresses of blocks by comparing the metadata_t pointers like numbers (do not dereference them).
 * Don't forget about the case where the freelist is empty. Remember what you learned from Homework 9.
 * This function will be useful for my_malloc() (mainly for adding in sbrk blocks) and my_free().
 */
 void add_to_addr_list(metadata_t *block) {
    if (block == NULL) {
    return;
  }

  // Handle empty free list case
  if (address_list == NULL) {
    block->next = NULL;
    address_list = block;
    return;
  }

  // Find the insertion position
  metadata_t *prev = NULL;
  metadata_t *curr = address_list;
  while (curr != NULL && (uintptr_t) curr < (uintptr_t) block) {
    prev = curr;
    curr = curr->next;
  }

  // Insert the block before the current block
  if (prev == NULL) {
    // Insert at the beginning
    block->next = address_list;
    address_list = block;
  } else {
    block->next = curr;
    prev->next = block;
  }
}

/* GIVEN HELPER FUNCTION: remove_from_addr_list
 * This function is provided for you by the TAs. You are not required to use it or our implementation of it, but it may be helpful to you.
 * This function should remove a block from the freelist.
 * Simply search through the freelist, looking for a node whose address matches the provided block's address.
 * This function will be useful for my_malloc(), particularly when the best-fit block is not big enough to be split.
 */
void remove_from_addr_list(metadata_t *block) {
  // Check for NULL argument
  if (block == NULL) {
    return;
  }

  // Handle removing the head of the list
  if (block == address_list) {
    address_list = address_list->next;
    return;
  }

  // Find the block before the one to remove
  metadata_t *prev = NULL;
  metadata_t *curr = address_list;
  while (curr != NULL && curr != block) {
    prev = curr;
    curr = curr->next;
  }

  // Block not found in the list
  if (curr == NULL) {
    return;
  }

  // Remove the block from the list
  prev->next = curr->next;
}
/* HELPER FUNCTION: find_best_fit
 * This function should find and return a pointer to the best-fit block. See the PDF for the best-fit criteria.
 * Remember that if you find the perfectly sized block, you should return it immediately.
 * You should not return an imperfectly sized block until you have searched the entire list for a potential perfect block.
 * Note that you should NOT be splitting the best-fit block in this function (do this within my_malloc() instead).
 */
 metadata_t *find_best_fit(size_t size) {
    metadata_t *current = address_list;
    while (current != NULL) {
        if (current->size == size) {
            break;
        }
        else if (current->size > size && current->size - size - TOTAL_METADATA_SIZE > MIN_BLOCK_SIZE) {
            break;
        }
        else {
            current = current->next;
        }
    }
    return current;
 }




// ------------------------- PART 2: Malloc functions -------------------------

/* Before starting each of these functions, you should:
 * 1. Understand what the function should do, what it should return, and what the freelist should look like after it finishes
 * 2. Develop a high-level plan for how to implement it; maybe sketch out pseudocode
 * 3. Check if the parameters have any special cases that need to be handled (when they're NULL, 0, etc.)
 * 4. Consider what edge cases the implementation needs to handle
 * 5. Think about any helper functions above that might be useful, and implement them if you haven't already
 */


/* MALLOC
 * See PDF for documentation
 */
void *my_malloc(size_t size) {
    if (size > SBRK_SIZE - TOTAL_METADATA_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    } else if (size == 0) {
        my_malloc_errno = NO_ERROR;
        return NULL;
    }
    metadata_t *current = find_best_fit(size);
    if (current == NULL) {
        metadata_t *point = (metadata_t *) my_sbrk(SBRK_SIZE);
        if (!point) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }  
        point->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
        point->next = NULL;
        add_to_addr_list(point);
        my_malloc_errno = NO_ERROR;
        return my_malloc(size);
    }    
    else if (current->size == size) {
        metadata_t *ret = current;
        remove_from_addr_list(ret);
        ret->next = NULL;
        my_malloc_errno = NO_ERROR;
        return ret + 1;
    } else {
        metadata_t *ret = split_block(current, size);
        ret->next = NULL;
        my_malloc_errno = NO_ERROR;
        return ret + 1;
    }
    return (NULL);
}

/* FREE
 * See PDF for documentation
 */
void my_free(void *ptr) {
    if (ptr == NULL) {
        my_malloc_errno = NO_ERROR;
        return;
    }
    metadata_t *addr = (metadata_t *) ((char*) ptr - sizeof(metadata_t));
    metadata_t *leftm = find_left(addr);
    metadata_t *rightm = find_right(addr);
    if (leftm && rightm) {
        remove_from_addr_list(leftm);
        remove_from_addr_list(rightm);
        leftm->size += addr->size + TOTAL_METADATA_SIZE;
        leftm->size += rightm->size + TOTAL_METADATA_SIZE;
        add_to_addr_list(leftm);
    } else if (leftm) {
        remove_from_addr_list(leftm);
        leftm->size += addr->size + TOTAL_METADATA_SIZE;
        add_to_addr_list(leftm);
    } else if (rightm) {
        remove_from_addr_list(rightm);
        addr->size += rightm->size + TOTAL_METADATA_SIZE;
        add_to_addr_list(addr);
    } else {
        add_to_addr_list(addr);
    }

    my_malloc_errno = NO_ERROR;
    return;
}

/* REALLOC
 * See PDF for documentation
 */
void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
        return my_malloc(size);
    }   
    metadata_t *addr = (metadata_t *) ((char*) ptr - sizeof(metadata_t));
    if (size == 0) {
        my_free(ptr);
        return NULL;
    }

    metadata_t *toReturn = my_malloc(size);
    if (!toReturn) {
        return NULL;
    }
    int size1 = (addr->size > size) ? size : addr->size;
    memcpy((void *) toReturn, (void *) ptr, size1);
    my_free(ptr);
    return toReturn;
}

/* CALLOC
 * See PDF for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
   void *ret = my_malloc(nmemb * size);
    if (ret == NULL) {
        return NULL;
    }
    
    return memset(ret, 0, nmemb * size);
}
