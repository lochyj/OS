#include "kernel/system/vmem/heap.h"

static signed int find_smallest_hole(addr size, unsigned char page_align, struct heap_t* heap)
{
	/* Find the smallest hole that will fit */
	unsigned int iterator = 0;
	while (iterator < heap->index.size)
	{
		struct header_t* header_t = (struct header_t*)lookup_ordered_array(iterator, &heap->index);
		/* If the user has request the memory be page-aligned */
		if (page_align > 0)
		{
			/* Page-align the starting point of this header_t */
			addr location = (addr)header_t;
			signed int offset = 0;
			if ((location+sizeof(struct header_t)) & 0xFFFFF000 != 0)
				offset = 0x1000 /* page size */ - (location+sizeof(struct header_t)) % 0x1000;
			signed int hole_size = (signed int)header_t->size - offset;
			/* Can we fit now? */
			if (hole_size >= (signed int)size)
				break;
		}
		else if (header_t->size >= size)
			break;
		iterator++;
	}

	/* Why did the loop exit? */
	if (iterator == heap->index.size)
		return -1; /* We got to the end and didn't find anything */
	else
		return iterator;
}

static signed char header_t_less_than(void* a, void* b) {
	return (((struct header_t*)a)->size < ((struct header_t*)b)->size)?1:0;
}

heap_t* create_heap(addr start, addr end_addr, addr max, unsigned char supervisor, unsigned char readonly) {
	struct heap_t* heap = (struct heap*)kmalloc(sizeof(struct heap_t));

	/* All of our assumptions are made on start and end being page-aligned */
	ASSERT(start % 0x1000 == 0);
	ASSERT(end_addr % 0x1000 == 0);

	/* Initalize the index */
	heap->index = place_ordered_array((void*)start, INDEX_SIZE, &header_t_less_than);

	/* Shift the start address forward to resemble where we can start putting data */
	start += sizeof(type_t) * INDEX_SIZE;

	/* Make sure the start address is page aligned */
	if (start & 0xFFFFF000 != 0) {
		start &= 0xFFFFF000;
		start += 0x1000;
	}

	/* Write the start, end and max addresses into the heap structure */
	heap->start_address = start;
	heap->end_address = end_addr;
	heap->max_address = max;
	heap->supervisor = supervisor;
	heap->readonly = readonly;

	/* We start off with one large hole in the index */
	struct header_t* hole = (struct header_t*)start;
	hole->size = end_addr - start;
	hole->magic = MAGIC;
	hole->is_hole = 1;
	insert_ordered_array((void*)hole, &heap->index);

	return heap;
}

static void expand(addr new_size, struct heap_t* heap) {
	/* Sanity check */
	ASSERT(new_size > heap->end_address - heap->start_address);

	/* Get the nearest following page boundary */
	if (new_size & 0xFFFFF000 != 0) {
		new_size &= 0xFFFFF000;
		new_size += 0x1000;
	}

	/* Make sure we are not overreaching ourselves */
	ASSERT(heap->start_address + new_size <= heap->max_address);

	/* This should always be on a page boundary */
	addr old_size = heap->end_address - heap->start_address;
	addr i = old_size;

	while (i < new_size) {
		frame_alloc( (struct page*)get_page(heap->start_address+i, 1, kernel_directory),
				(heap->supervisor)?1:0, (heap->readonly)?0:1);
		i += 0x1000; /* page size */
	}

	heap->end_address = heap->start_address+new_size;
}

static addr contract(addr new_size, struct heap_t* heap) {
	/* Sanity check */
	ASSERT(new_size < heap->end_address - heap->start_address);

	/* Get the nearest following page boundary */
	if (new_size & 0x1000)
	{
		new_size &= 0x1000;
		new_size += 0x1000;
	}

	/* Don't contract too far */
	if (new_size < MIN_SIZE)
		new_size = MIN_SIZE;
	addr old_size = heap->end_address - heap->start_address;
	addr i = old_size;
	while (new_size < i)
	{
		frame_free((struct page*)get_page(heap->start_address + i, 0, kernel_directory));
		i -= 0x1000;
	}
	heap->end_address = heap->start_address + new_size;
	return new_size;
}

void* vmalloc(addr size, unsigned char page_align, struct heap_t* heap)
{
	/* Make sure we take the size of the header_t / footer_t into account */
	addr new_size = size + sizeof(struct header_t) + sizeof(struct footer_t);

	/* Find the smallest hole that will fit */
	signed int iterator = find_smallest_hole(new_size, page_align, heap);

	if (iterator == -1) /* If we didn't find a suitable hole */
	{
		/* Save some previous data */
		addr old_length = heap->end_address - heap->start_address;
		addr old_end_address = heap->end_address;

		/* We need to allocate some more space */
		expand(old_length + new_size, heap);
		addr new_length = heap->end_address - heap->start_address;

		/* Find the endmost header_t (not endmost in size, but in location) */
		iterator = 0;
		/* Vars to hold the index of, and value of, the endmost header_t found so far */
		addr idx = -1; addr value = 0x0;
		while (iterator < heap->index.size)
		{
			addr tmp = (addr)lookup_ordered_array(iterator, &heap->index);
			if (tmp > value)
			{
				value = tmp;
				idx = iterator;
			}
			iterator++;
		}

		/* If we didn't find any header_ts, we need to add one */
		if (idx == -1)
		{
			struct header_t* header	= (struct header_t*)old_end_address;
			header->magic			= MAGIC;
			header->size			= new_length - old_length;
			header->is_hole			= 1;
			struct footer_t* footer	= (struct footer_t*)(old_end_address + header->size
							  - sizeof(struct footer_t));
			footer->magic			= MAGIC;
			footer->header_t			= header;
			insert_ordered_array((void*)header, &heap->index);
		}
		else
		{
			/* The last header_t needs adjusting */
			struct header_t* header = lookup_ordered_array(idx, &heap->index);
			header->size += new_length - old_length;
			
			/* Rewrite the footer_t */
			struct footer_t* footer = (struct footer_t*)((addr)header + header->size - sizeof(struct footer_t));
			footer->header_t = header;
			footer->magic = MAGIC;
		}
	}

	struct header_t* orig_hole_header = (struct header_t*)lookup_ordered_array(iterator, &heap->index);
	addr orig_hole_pos = (addr)orig_hole_header;
	addr orig_hole_size = orig_hole_header->size;

	/* Here we work out if we should split the hole we found into two parts.
	 * Is the original hole size - requested hole size less than the overhead for adding a new hole? */
	if (orig_hole_size - new_size < sizeof(struct header_t) + sizeof(struct footer_t))
	{
		/* Then just increase the requested size to the size of the hole we found */
		size += orig_hole_size - new_size;
		new_size = orig_hole_size;
	}

	/* If we need to page-align the data, do it now and make a new hole in front of our block */
	if (page_align && (orig_hole_pos & 0xFFFFF000))
	{
		addr new_location		= orig_hole_pos + 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) - 
					  	  sizeof(struct header_t);
		struct header_t* hole_header = (struct header_t*)orig_hole_pos;
		hole_header->size		= 0x1000 /* page size */ - (orig_hole_pos & 0xFFF) -
						  sizeof(struct header_t);
		hole_header->magic		= MAGIC;
		hole_header->is_hole		= 1;
		struct footer_t* hole_footer	= (struct footer_t*)((addr)new_location - sizeof(struct footer_t));
		hole_footer->magic		= MAGIC;
		hole_footer->header_t		= hole_header;
		orig_hole_pos			= new_location;
		orig_hole_size			= orig_hole_size - hole_header->size;
	}
	else
	{
		/* Else we don't need this hole any more, delete it from the index */
		remove_ordered_array(iterator, &heap->index);
	}

	/* Overwrite the original header_t... */
	struct header_t* block_header_t	= (struct header_t*)orig_hole_pos;
	block_header_t->magic			= MAGIC;
	block_header_t->is_hole			= 0;
	block_header_t->size			= new_size;
	/* ... and the footer_t */
	struct footer_t* block_footer_t	= (struct footer_t*)(orig_hole_pos + sizeof(struct header_t) + size);
	block_footer_t->magic			= MAGIC;
	block_footer_t->header_t			= block_header_t;

	/* We may need to write a new hole after the allocated block.
	 * We do this only if the new hole would have positive size */
	if (orig_hole_size - new_size > 0)
	{
		struct header_t* hole_header_t	= (struct header_t*)(orig_hole_pos + sizeof(struct header_t) + size +
						  sizeof(struct footer_t));
		hole_header_t->magic		= MAGIC;
		hole_header_t->is_hole		= 1;
		hole_header_t->size		= orig_hole_size - new_size;
		struct footer_t* hole_footer_t	= (struct footer_t*)((addr)hole_header_t + orig_hole_size - new_size -
						  sizeof(struct footer_t));
		if ((addr)hole_footer_t < heap->end_address)
		{
			hole_footer_t->magic	= MAGIC;
			hole_footer_t->header_t	= hole_header_t;
		}
		
		/* Put the new hole in the array */
		insert_ordered_array((void*)hole_header_t, &heap->index);
	}

	/* ... and we're done! */
	return (void*)((addr)block_header_t + sizeof(struct header_t));
}

void vfree(void *p, struct heap* heap)
{
	/* Exit gracefully for null pointers */
	if (p == 0)
		return;

	/* Get the header_t and footer_t associated with this pointer */
	struct header_t* header_t = (struct header_t*)((addr)p - sizeof(struct header_t));
	struct footer_t* footer_t = (struct footer_t*)((addr)header_t + header_t->size - sizeof(struct footer_t));

	/* Sanity checks */
	ASSERT(header_t->magic == MAGIC);
	ASSERT(footer_t->magic == MAGIC);

	/* Make us a hole */
	header_t->is_hole = 0;

	/* Do we want to add this header_t to the free holes index */
	char do_add = 1;

	/* Unify left */
	struct footer_t* test_footer_t = (struct footer_t*)((addr)header_t - sizeof(struct footer_t));
	if (test_footer_t->magic == MAGIC && test_footer_t->header_t->is_hole == 1)
	{
		addr cache_size = header_t->size;	/* Cache our current size */
		header_t = test_footer_t->header_t;	/* Rewrite our header_t with the new one */
		footer_t->header_t = header_t;	/* Rewrite our footer_t to point to the new header_t */
		header_t->size += cache_size;	/* Change the size */
		do_add = 0;			/* Since this header_t is already in the index, we don't want to add it */
	}

	/* Unify right */
	struct header_t* test_header_t = (struct header_t*)((addr)footer_t + sizeof(struct footer_t));
	if (test_header_t->magic == MAGIC && test_header_t->is_hole)
	{
		header_t->size += test_header_t->size;	/* Increase our size */
		test_footer_t = (struct footer_t*)((addr)test_header_t + test_header_t->size - sizeof(struct footer_t));
		footer_t = test_footer_t;

		/* Find and remove this kernel from the index */
		unsigned int iterator = 0;
		while ((iterator < heap->index.size) &&
			(lookup_ordered_array(iterator, &heap->index) != (void*)test_header_t))
			iterator++;

		/* Make sure we actually found the item */
		ASSERT(iterator < heap->index.size);

		/* Remove it */
		remove_ordered_array(iterator, &heap->index);
	}

	/* If the footer_t location is the end address, we can contract */
	if ((addr)footer_t + sizeof(struct footer_t) == heap->end_address)
	{
		addr old_length = heap->end_address - heap->start_address;
		addr new_length = contract((addr)header_t - heap->start_address, heap);

		/* Check how big we will be after resizing */
		if (header_t->size - (old_length - new_length) > 0)
		{
			/* We will still exist, so resize us */
			header_t->size -= old_length - new_length;
			footer_t = (struct footer_t*)((addr)header_t + header_t->size - sizeof(struct footer_t));
			footer_t->magic = MAGIC;
			footer_t->header_t = header_t;
		}
		else
		{
			/* We will no longer exist, remove us from the index */
			unsigned int iterator = 0;
			while ((iterator < heap->index.size) &&
				(lookup_ordered_array(iterator, &heap->index) != (void*)test_header_t))
				iterator++;

			/* If we didn't find ourselves, we have nothing to remove */
			if (iterator < heap->index.size)
				remove_ordered_array(iterator, &heap->index);
		}
	}

	/* Add ourselves if we need to */
	if (do_add == 1)
		insert_ordered_array((void*)header_t, &heap->index);
}