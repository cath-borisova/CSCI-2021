// el_malloc.c: implementation of explicit list allocator functions.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "el_malloc.h"

////////////////////////////////////////////////////////////////////////////////
// Global control functions

// Global control variable for the allocator. Must be initialized in
// el_init().
el_ctl_t el_ctl = {};

// Create an initial block of memory for the heap using mmap(). Initialize the
// el_ctl data structure to point at this block. The initial size/position of
// the heap for the memory map are given in the symbols EL_HEAP_INITIAL_SIZE
// and EL_HEAP_START_ADDRESS.  Initialize the lists in el_ctl to contain a
// single large block of available memory and no used blocks of memory.
int el_init() {
    void *heap = mmap(EL_HEAP_START_ADDRESS, EL_HEAP_INITIAL_SIZE,
                      PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert(heap == EL_HEAP_START_ADDRESS);

    assert(heap == EL_HEAP_START_ADDRESS);

    el_ctl.heap_bytes = EL_HEAP_INITIAL_SIZE; // make the heap as big as possible to begin with
    el_ctl.heap_start = heap; // set addresses of start and end of heap
    el_ctl.heap_end = PTR_PLUS_BYTES(heap, el_ctl.heap_bytes);

    if (el_ctl.heap_bytes < EL_BLOCK_OVERHEAD) {
        fprintf(stderr,"el_init: heap size %ld to small for a block overhead %ld\n",
                el_ctl.heap_bytes,EL_BLOCK_OVERHEAD);
        return 1;
    }

    el_init_blocklist(&el_ctl.avail_actual);
    el_init_blocklist(&el_ctl.used_actual);
    el_ctl.avail = &el_ctl.avail_actual;
    el_ctl.used  = &el_ctl.used_actual;

    // establish the first available block by filling in size in
    // block/foot and null links in head
    size_t size = el_ctl.heap_bytes - EL_BLOCK_OVERHEAD;
    el_blockhead_t *ablock = el_ctl.heap_start;
    ablock->size = size;
    ablock->state = EL_AVAILABLE;
    el_blockfoot_t *afoot = el_get_footer(ablock);
    afoot->size = size;
    el_add_block_front(el_ctl.avail, ablock);
    return 0;
}


// Clean up the heap area associated with the system
void el_cleanup() {
    munmap(el_ctl.heap_start, el_ctl.heap_bytes);
    el_ctl.heap_start = NULL;
    el_ctl.heap_end   = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// Pointer arithmetic functions to access adjacent headers/footers

// Compute the address of the foot for the given head which is at a higher
// address than the head.
el_blockfoot_t *el_get_footer(el_blockhead_t *head) {
    size_t size = head->size;
    el_blockfoot_t *foot = PTR_PLUS_BYTES(head, sizeof(el_blockhead_t) + size);
    return foot;
}

// REQUIRED
// Compute the address of the head for the given foot which is at a
// lower address than the foot.
el_blockhead_t *el_get_header(el_blockfoot_t *foot){
	//get the size of the foot
  	size_t size = foot->size;
  	//compute the address of the head
	el_blockhead_t *head = PTR_MINUS_BYTES(foot, sizeof(el_blockhead_t) + size); //el_blockfoot
	return head;
}

// Return a pointer to the block that is one block higher in memory
// from the given block.  This should be the size of the block plus
// the EL_BLOCK_OVERHEAD which is the space occupied by the header and
// footer. Returns NULL if the block above would be off the heap.
// DOES NOT follow next pointer, looks in adjacent memory.
el_blockhead_t *el_block_above(el_blockhead_t *block) {
    el_blockhead_t *higher = PTR_PLUS_BYTES(block, block->size + EL_BLOCK_OVERHEAD);
    if ((void *) higher >= (void*) el_ctl.heap_end) {
        return NULL;
    } else {
        return higher;
    }
}

// REQUIRED
// Return a pointer to the block that is one block lower in memory
// from the given block.  Uses the size of the preceding block found
// in its foot. DOES NOT follow block->next pointer, looks in adjacent
// memory. Returns NULL if the block below would be outside the heap.
//
// WARNING: This function must perform slightly different arithmetic
// than el_block_above(). Take care when implementing it.
el_blockhead_t *el_block_below(el_blockhead_t *block) {
	//check to see if the current block is at the same address as the start of the heap
	if((void*) block  == (void*) el_ctl.heap_start){
	//if it is, return NULL because then there is no legal block below it 
		return NULL;
	} else {
		//otherwise create a temporary pointer to the blockfoot to get the size
		el_blockfoot_t * size = PTR_MINUS_BYTES(block, sizeof(el_blockfoot_t));
		//compute the address of the blockhead below
		el_blockhead_t *lower = PTR_MINUS_BYTES(block, EL_BLOCK_OVERHEAD + size->size);
		return lower;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Block list operations

// Print an entire blocklist. The format appears as follows.
//
// {length:   2  bytes:  3400}
//   [  0] head @ 0x600000000000 {state: a  size:   128}
//         foot @ 0x6000000000a0 {size:   128}
//   [  1] head @ 0x600000000360 {state: a  size:  3192}
//         foot @ 0x600000000ff8 {size:  3192}
//
// Note that the '@' column uses the actual address of items which
// relies on a consistent mmap() starting point for the heap.
void el_print_blocklist(el_blocklist_t *list) {
    printf("{length: %3lu  bytes: %5lu}\n", list->length,list->bytes);
    el_blockhead_t *block = list->beg;
    for (int i=0 ; i < list->length; i++) {
        printf("  ");
        block = block->next;
        printf("[%3d] head @ %p ", i, block);
        printf("{state: %c  size: %5lu}\n", block->state,block->size);
        el_blockfoot_t *foot = el_get_footer(block);
        printf("%6s", "");          // indent
        printf("  foot @ %p ", foot);
        printf("{size: %5lu}", foot->size);
        printf("\n");
    }
}

// Print out basic heap statistics. This shows total heap info along
// with the Available and Used Lists. The output format resembles the following.
//
// HEAP STATS (overhead per node: 40)
// heap_start:  0x600000000000
// heap_end:    0x600000001000
// total_bytes: 4096
// AVAILABLE LIST: {length:   2  bytes:  3400}
//   [  0] head @ 0x600000000000 {state: a  size:   128}
//         foot @ 0x6000000000a0 {size:   128}
//   [  1] head @ 0x600000000360 {state: a  size:  3192}
//         foot @ 0x600000000ff8 {size:  3192}
// USED LIST: {length:   3  bytes:   696}
//   [  0] head @ 0x600000000200 {state: u  size:   312}
//         foot @ 0x600000000358 {size:   312}
//   [  1] head @ 0x600000000198 {state: u  size:    64}
//         foot @ 0x6000000001f8 {size:    64}
//   [  2] head @ 0x6000000000a8 {state: u  size:   200}
//         foot @ 0x600000000190 {size:   200}
void el_print_stats() {
    printf("HEAP STATS (overhead per node: %lu)\n", EL_BLOCK_OVERHEAD);
    printf("heap_start:  %p\n", el_ctl.heap_start);
    printf("heap_end:    %p\n", el_ctl.heap_end);
    printf("total_bytes: %lu\n", el_ctl.heap_bytes);
    printf("AVAILABLE LIST: ");
    el_print_blocklist(el_ctl.avail);
    printf("USED LIST: ");
    el_print_blocklist(el_ctl.used);
}

// Initialize the specified list to be empty. Sets the beg/end
// pointers to the actual space and initializes those data to be the
// ends of the list.  Initializes length and size to 0.
void el_init_blocklist(el_blocklist_t *list) {
    list->beg = &(list->beg_actual);
    list->beg->state = EL_BEGIN_BLOCK;
    list->beg->size = EL_UNINITIALIZED;
    list->end = &(list->end_actual);
    list->end->state = EL_END_BLOCK;
    list->end->size = EL_UNINITIALIZED;
    list->beg->next = list->end;
    list->beg->prev = NULL;
    list->end->next = NULL;
    list->end->prev = list->beg;
    list->length = 0;
    list->bytes = 0;
}

// REQUIRED
// Add to the front of list; links for block are adjusted as are links
// within list. Length is incremented and the bytes for the list are
// updated to include the new block's size and its overhead.
void el_add_block_front(el_blocklist_t *list, el_blockhead_t *block) {
	//adjusts pointers in linked list to set block between beg and the next block
	block->prev = list->beg;	
	block->next = list->beg->next;
	block->prev->next = block;
	block->next->prev = block;
	//increments the length of the list added to
	list->length += 1;
	//increments the bytes of the list added to
	list->bytes += (block->size + EL_BLOCK_OVERHEAD);
}

// REQUIRED
// Unlink block from the list it is in which should be the list
// parameter. Updates the length and bytes for that list including
// the EL_BLOCK_OVERHEAD bytes associated with header/footer.
void el_remove_block(el_blocklist_t *list, el_blockhead_t *block) {
	//adjusts pointers in linked list to remove the block 
	block->prev->next = block->next;
	block->next->prev = block->prev;
	//decrements the length of the list removed from
	list->length -= 1;
	//decrements the bytes of the list removed from
	list->bytes -= (block->size + EL_BLOCK_OVERHEAD);
}

////////////////////////////////////////////////////////////////////////////////
// Allocation-related functions

// REQUIRED
// Find the first block in the available list with block size of at
// least (size + EL_BLOCK_OVERHEAD). Overhead is accounted so this
// routine may be used to find an available block to split: splitting
// requires adding in a new header/footer. Returns a pointer to the
// found block or NULL if no of sufficient size is available.
el_blockhead_t *el_find_first_avail(size_t size) {
	//creates a temporary pointer (block)
	el_blockhead_t* block = el_ctl.avail->beg->next;
	//loops through the available list
	while(block != el_ctl.avail->end){
		//break out of the loop and return the current block if you find one big enough
		if(block->size >= size + EL_BLOCK_OVERHEAD){
			return block;
		} else{
			//otherwise go to the next block in the list
			block = block->next;
		}
	}
	//if there are no avaiable blocks with a large enough size, return NULL
	return NULL;		
}

// REQUIRED
// Set the pointed to block to the given size and add a footer to it. Creates
// another block above it by creating a new header and assigning it the
// remaining space. Ensures that the new block has a footer with the correct
// size. Returns a pointer to the newly created block while the parameter block
// has its size altered to parameter size. Does not do any linking of blocks.
// If the parameter block does not have sufficient size for a split (at least
// new_size + EL_BLOCK_OVERHEAD for the new header/footer) makes no changes and
// returns NULL.
el_blockhead_t *el_split_block(el_blockhead_t *block, size_t new_size) {
	//make sure the block passed in has a large enough size
	if(block->size >= new_size + EL_BLOCK_OVERHEAD){
		//amount of bytes left in the available list
		size_t remaining = block->size - (new_size + EL_BLOCK_OVERHEAD);
		//set the block to the new size
		block->size = new_size;
		//find the footer of the block, and set it to the new size as well
		el_blockfoot_t *footer = el_get_footer(block);
		footer->size = new_size; 
		//create a new header for the remaining space after changing the size of block
		el_blockhead_t *new_header = el_block_above(block);
		//make sure the new header is not NULL
		if(new_header !=NULL){ 
			//set the new header's size to remaining 
			new_header->size = remaining; 
			//find the footer of the new header and set ts size to remaining as well
			el_blockfoot_t *new_footer = el_get_footer(new_header);
			new_footer->size = remaining;
			//return a pointer to the new header
			return new_header;
		}
	} 
		//if either the block's size is to small or if the new header is NULL, return NULL
		return NULL;
}

// REQUIRED
// Return pointer to a block of memory with at least the given size
// for use by the user. The pointer returned is to the usable space,
// not the block header. Makes use of find_first_avail() to find a
// suitable block and el_split_block() to split it.  Returns NULL if
// no space is available.
void *el_malloc(size_t nbytes) {
	//create a new temporary pointer (block) to the first available block of size nbytes
	el_blockhead_t *block = el_find_first_avail(nbytes);
	//if block is NULL, return NULL
	if(block == NULL){
		return NULL;
	} 
	//otherwise, remove the block from the available list
	el_remove_block(el_ctl.avail, block);
	//create a temporary pointer to the new header block (new_block) returned by el_split_block
	el_blockhead_t *new_block = el_split_block(block, nbytes);
	//if the new block is NULL return NULL
	if(new_block == NULL){
		return NULL;
	}
	//otherwise add the new block to the available list
	el_add_block_front(el_ctl.avail, new_block);
	//and return the original block to the used list
	el_add_block_front(el_ctl.used, block);
	//change the state of the new block to be available
	new_block->state = EL_AVAILABLE;
	//change the state of the original block to be used
	block->state = EL_USED;
	//return a pointer to the available space (offset by sizeof(el_blockhead_t) of the new block)
	return PTR_PLUS_BYTES(block, sizeof(el_blockhead_t));
} 


////////////////////////////////////////////////////////////////////////////////
// De-allocation/free() related functions

// REQUIRED
// Attempt to merge the block lower with the next block in
// memory. Does nothing if lower is NULL or not EL_AVAILABLE and does
// nothing if the next higher block is NULL (because lower is the last
// block) or not EL_AVAILABLE.  Otherwise, locates the next block with
// el_block_above() and merges these two into a single block. Adjusts
// the fields of lower to incorporate the size of higher block and the
// reclaimed overhead. Adjusts footer of higher to indicate the two
// blocks are merged.  Removes both lower and higher from the
// available list and re-adds lower to the front of the available
// list.
void el_merge_block_with_above(el_blockhead_t *lower) {
	//check to make sure lower is not NULL and its state is available
	if(lower != NULL && lower->state == EL_AVAILABLE){
		//create a temporary pointer (above) to hold the blockhead returned by el_block_above(lower)
		el_blockhead_t *above = el_block_above(lower);
		//make sure above is not NULL and its state is available
		if(above != NULL && above->state == EL_AVAILABLE){
			//remove above and lower from the available list
			el_remove_block(el_ctl.avail, lower);
			el_remove_block(el_ctl.avail, above);
			//change lower's size to include above's size and the block overhead
			lower->size += above->size + EL_BLOCK_OVERHEAD;
			//change above's footer's size to be the same as lower's new size
			el_get_footer(above)->size = lower->size; 	
			//add lower to the front of the available list
			el_add_block_front(el_ctl.avail, lower);
		}
		
	}
}

// REQUIRED
// Free the block pointed to by the give ptr.  The area immediately
// preceding the pointer should contain an el_blockhead_t with information
// on the block size. Attempts to merge the free'd block with adjacent
// blocks using el_merge_block_with_above().
void el_free(void *ptr) {
	//create a temporary pointer (block) to the blockhead of the ptr
	el_blockhead_t *block = PTR_MINUS_BYTES(ptr, sizeof(el_blockhead_t));
	//remove the block from the used list
	el_remove_block(el_ctl.used, block);
	//and set its state to available
	block -> state = EL_AVAILABLE;
	//add the block to the front of the available list
	el_add_block_front(el_ctl.avail, block);
	//try to merge the block with the block above by calling el_merge_block_with_above
	el_merge_block_with_above(block);
	//try to merge the block with the block below by calling el_merge_block_with_above with el_block_below(block)
	el_merge_block_with_above(el_block_below(block));

}
