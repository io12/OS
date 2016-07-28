#include <ints.h>
#include <klibc.h>
#include <kprintf.h>

#include <liballoc.h>

#define DEBUG	

#define LIBALLOC_MAGIC 0xc001c0de
#define MAXCOMPLETE    5
#define MAXEXP         32
#define MINEXP         8	

#define MODE_BEST      0
#define MODE_INSTANT   1

#define MODE           MODE_BEST

// allowing for 2^MAXEXP blocks
struct boundary_tag* l_freePages[MAXEXP];
i32 l_completePages[MAXEXP];

#ifdef DEBUG
// the real amount of memory allocated
u32 l_allocated = 0;
// the amount of memory in use (malloc'ed)
u32 l_inuse = 0;
#endif

// flag to indicate initialization
i32 l_initialized = 0;
// individual page size
i32 l_pageSize  = 4096;
// minimum number of pages to allocate
i32 l_pageCount = 16;


// ***********   HELPER FUNCTIONS  *******************************

/** Returns the exponent required to manage 'size' amount of memory. 
 *
 *  Returns n where  2^n <= size < 2^(n+1)
 */
i32 getexp(u32 size) {
	i32 shift = MINEXP;

	if (size < (1 << MINEXP)) {
#ifdef DEBUG
		kprintf(PL_SERIAL, "getexp returns -1 for %u less than MINEXP\n", size);
#endif
		// smaller than the quantum
		return -1;
	}

	while (shift < MAXEXP) {
		if ((1 << shift) > size) {
			break;
		}
		shift += 1;
	}

#ifdef DEBUG
	kprintf(PL_SERIAL, "getexp returns %u (%u bytes) for %u size\n", shift - 1,
			(1 << (shift - 1)), size);
#endif

	return shift - 1;	
}

void* liballoc_memcpy(void* s1, const void* s2, size_t n) {
	char* cdest;
	char* csrc;
	u32* ldest = (u32*) s1;
	u32* lsrc  = (u32*) s2;

	while (n >= sizeof(u32)) {
		*ldest++ = *lsrc++;
		n -= sizeof(u32);
	}

	cdest = (char*) ldest;
	csrc  = (char*) lsrc;

	while (n > 0) {
		*cdest++ = *csrc++;
		n -= 1;
	}

	return s1;
}

#ifdef DEBUG
void dump_array() {
	i32 i = 0;
	struct boundary_tag *tag = NULL;

	kprintf(PL_SERIAL, "------ Free pages array ---------\n");
	kprintf(PL_SERIAL, "System memory allocated: %u\n", l_allocated);
	kprintf(PL_SERIAL, "Memory in used (malloc'ed): %u\n", l_inuse);

	for (i = 0; i < MAXEXP; i++) {
		kprintf(PL_SERIAL, "%.2i(%u): ",i, l_completePages[i]);

		tag = l_freePages[i];
		while (tag != NULL) {
			if (tag->split_left  != NULL) kprintf(PL_SERIAL, "*");
			kprintf(PL_SERIAL, "%u", tag->real_size);
			if (tag->split_right != NULL) kprintf(PL_SERIAL, "*");

			kprintf(PL_SERIAL, " ");
			tag = tag->next;
		}
		kprintf(PL_SERIAL, "\n");
	}

	kprintf(PL_SERIAL, "'*' denotes a split to the left/right of a tag\n");
}
#endif

void insert_tag(struct boundary_tag *tag, i32 index) {
	i32 realIndex;

	if (index < 0) {
		realIndex = getexp(tag->real_size - sizeof(struct boundary_tag));
		if (realIndex < MINEXP) realIndex = MINEXP;
	}
	else {
		realIndex = index;
	}

	tag->index = realIndex;

	if (l_freePages[realIndex] != NULL) {
		l_freePages[realIndex]->prev = tag;
		tag->next = l_freePages[realIndex];
	}

	l_freePages[realIndex] = tag;
}

void remove_tag(struct boundary_tag *tag) {
	if (l_freePages[tag->index] == tag) {
		l_freePages[tag->index] = tag->next;
	}

	if (tag->prev != NULL) {
		tag->prev->next = tag->next;
	}
	if (tag->next != NULL) {
		tag->next->prev = tag->prev;
	}

	tag->next = NULL;
	tag->prev = NULL;
	tag->index = -1;
}

struct boundary_tag* melt_left(struct boundary_tag *tag) {
	struct boundary_tag *left = tag->split_left;

	left->real_size   += tag->real_size;
	left->split_right  = tag->split_right;

	if (tag->split_right != NULL) {
		tag->split_right->split_left = left;
	}

	return left;
}

struct boundary_tag* absorb_right(struct boundary_tag *tag) {
	struct boundary_tag *right = tag->split_right;

	// remove right from free pages
	remove_tag(right);

	tag->real_size   += right->real_size;

	tag->split_right  = right->split_right;
	if (right->split_right != NULL) {
		right->split_right->split_left = tag;
	}

	return tag;
}

struct boundary_tag* split_tag(struct boundary_tag* tag) {
	u32 remainder = tag->real_size - sizeof(struct boundary_tag) - tag->size;
	struct boundary_tag *new_tag = (struct boundary_tag*) ((u32) tag +
			sizeof(struct boundary_tag) + tag->size);	

	new_tag->magic = LIBALLOC_MAGIC;
	new_tag->real_size = remainder;	

	new_tag->next = NULL;
	new_tag->prev = NULL;

	new_tag->split_left = tag;
	new_tag->split_right = tag->split_right;

	if (new_tag->split_right != NULL) {
		new_tag->split_right->split_left = new_tag;
	}
	tag->split_right = new_tag;

	tag->real_size -= new_tag->real_size;

	insert_tag(new_tag, -1);

	return new_tag;
}

// ***************************************************************

struct boundary_tag* allocate_new_tag(u32 size) {
	u32 pages;
	u32 usage;
	struct boundary_tag *tag;

	// this is how much space is required
	usage  = size + sizeof(struct boundary_tag);

	// perfect amount of space
	pages = usage / l_pageSize;
	if ((usage % l_pageSize) != 0) {
		pages += 1;
	}

	// make sure it's >= the minimum size
	if (pages < l_pageCount) {
		pages = l_pageCount;
	}

	tag = (struct boundary_tag*) liballoc_alloc(pages);

	if (tag == NULL) {
		// OUTOFMEMORY
		return NULL;
	}

	tag->magic       = LIBALLOC_MAGIC;
	tag->size        = size;
	tag->real_size   = pages * l_pageSize;
	tag->index       = -1;

	tag->next        = NULL;
	tag->prev        = NULL;
	tag->split_left  = NULL;
	tag->split_right = NULL;

#ifdef DEBUG
	kprintf(PL_SERIAL, "Resource allocated %X of %u pages (%u bytes) for %u "
			"size.\n", (u32) tag, pages, pages * l_pageSize, size);

	l_allocated += pages * l_pageSize;

	kprintf(PL_SERIAL, "Total memory usage = %u KB\n",  (int) ((l_allocated / (1024))));
#endif

	return tag;
}

void *malloc(size_t size) {
	i32 index;
	void *ptr;
	struct boundary_tag *tag = NULL;

	liballoc_lock();

	if (l_initialized == 0) {
#ifdef DEBUG
		kprintf(PL_SERIAL, "%s\n","liballoc initializing.");
#endif
		memset(l_freePages, 0, MAXEXP);
		memset(l_completePages, 0, MAXEXP);
		l_initialized = 1;
	}

	index = getexp(size) + MODE;
	if (index < MINEXP) {
		index = MINEXP;
	}

	// Find one big enough.
	tag = l_freePages[index];
	while (tag != NULL) {
		// If there's enough space in this tag.
		if ((tag->real_size - sizeof(struct boundary_tag))
				>= (size + sizeof(struct boundary_tag))) {
#ifdef DEBUG
			kprintf(PL_SERIAL, "Tag search found %u >= %u\n",(tag->real_size -
						sizeof(struct boundary_tag)), (size
						+ sizeof(struct boundary_tag)));
#endif
			break;
		}

		tag = tag->next;
	}

	// No page found. Make one.
	if (tag == NULL) {
		if ((tag = allocate_new_tag(size)) == NULL) {
			liballoc_unlock();
			return NULL;
		}

		index = getexp(tag->real_size - sizeof(struct boundary_tag));
	}
	else {
		remove_tag(tag);

		if ((tag->split_left == NULL) && (tag->split_right == NULL)) {
			l_completePages[index] -= 1;
		}
	}

	// We have a free page. Remove it from the free pages list.
	tag->size = size;

	// Removed... see if we can re-use the excess space.

#ifdef DEBUG
	kprintf(PL_SERIAL, "Found tag with %u bytes available (requested %u bytes, leaving "
			"%u), which has exponent: %u (%u bytes)\n", tag->real_size
			- sizeof(struct boundary_tag), size, tag->real_size - size
			- sizeof(struct boundary_tag), index, 1<<index);
#endif

	// support a new tag + remainder
	u32 remainder = tag->real_size - size - sizeof(struct boundary_tag) * 2;

	if (((int) (remainder) > 0) /*&& ((tag->real_size - remainder) >=
				      (1 << MINEXP))*/) {
		i32 childIndex = getexp(remainder);

		if (childIndex >= 0) {
#ifdef DEBUG
			kprintf(PL_SERIAL, "Seems to be splittable: %u >= 2^%u .. %u\n",
					remainder, childIndex, (1<<childIndex));
#endif
			struct boundary_tag *new_tag = split_tag(tag); 
#ifdef DEBUG
			kprintf(PL_SERIAL, "Old tag has become %u bytes, new tag is now %u "
					"bytes (%u exp)\n", tag->real_size,
					new_tag->real_size, new_tag->index);
#endif
		}
	}

	ptr = (void*) ((u32) tag + sizeof(struct boundary_tag));

#ifdef DEBUG
	l_inuse += size;
	kprintf(PL_SERIAL, "malloc: %X,  %u, %u\n", (u32) ptr, (int) l_inuse / 1024,
			(int) l_allocated / 1024);
	dump_array();
#endif

	liballoc_unlock();
	return ptr;
}

void free(void* ptr) {
	i32 index;
	struct boundary_tag *tag;

	if (ptr == NULL) {
		return;
	}

	liballoc_lock();

	tag = (struct boundary_tag*) ((u32) ptr - sizeof(struct boundary_tag));

	if (tag->magic != LIBALLOC_MAGIC) {
		liballoc_unlock();
		return;
	}

#ifdef DEBUG
	l_inuse -= tag->size;
	kprintf(PL_SERIAL, "free: %X, %u, %u\n", (u32) ptr, (int) l_inuse / 1024,
			(int) l_allocated / 1024);
#endif

	// MELT LEFT...
	while ((tag->split_left != NULL) && (tag->split_left->index >= 0)) {
#ifdef DEBUG
		kprintf(PL_SERIAL, "Melting tag left into available memory. Left was %u, "
				"becomes %u (%u)\n", tag->split_left->real_size,
				tag->split_left->real_size + tag->real_size,
				tag->split_left->real_size);
#endif
		tag = melt_left(tag);
		remove_tag(tag);
	}

	// MELT RIGHT...
	while ((tag->split_right != NULL) && (tag->split_right->index >= 0)) {
#ifdef DEBUG
		kprintf(PL_SERIAL, "Melting tag right into available memory. This was was %u, "
				"becomes %u (%u)\n", tag->real_size,
				tag->split_right->real_size + tag->real_size,
				tag->split_right->real_size);
#endif
		tag = absorb_right(tag);
	}

	// Where is it going back to?
	index = getexp(tag->real_size - sizeof(struct boundary_tag));
	if (index < MINEXP) {
		index = MINEXP;
	}

	// A whole, empty block?
	if ((tag->split_left == NULL) && (tag->split_right == NULL)) {
		if (l_completePages[index] == MAXCOMPLETE) {
			// Too many standing by to keep. Free this one.
			u32 pages = tag->real_size / l_pageSize;

			if ((tag->real_size % l_pageSize) != 0) {
				pages += 1;
			}
			if (pages < l_pageCount) {
				pages = l_pageCount;
			}

			liballoc_free(tag, pages);

#ifdef DEBUG
			l_allocated -= pages * l_pageSize;
			kprintf(PL_SERIAL, "Resource freeing %X of %u pages\n",
					(u32) tag, pages);
			dump_array();
#endif

			liballoc_unlock();
			return;
		}

		// increase the count of complete pages
		l_completePages[index] += 1;
	}

	// ..........

	insert_tag(tag, index);

#ifdef DEBUG
	kprintf(PL_SERIAL, "Returning tag with %u bytes (requested %u bytes), which has "
			"exponent: %u\n", tag->real_size, tag->size, index); 
	dump_array();
#endif

	liballoc_unlock();
}

void* calloc(size_t nobj, size_t size) {
	i32 real_size;
	void *p;

	real_size = nobj * size;

	p = malloc(real_size);

	memset(p, 0, real_size);

	return p;
}

void* realloc(void *p, size_t size) {
	void *ptr;
	struct boundary_tag *tag;
	i32 real_size;

	if (size == 0) {
		free(p);
		return NULL;
	}
	if (p == NULL) {
		return malloc(size);
	}

	liballoc_lock();
	tag = (struct boundary_tag*) ((u32) p - sizeof(struct boundary_tag));
	real_size = tag->size;
	liballoc_unlock();

	if (real_size > size) {
		real_size = size;
	}

	ptr = malloc(size);
	liballoc_memcpy(ptr, p, real_size);
	free(p);

	return ptr;
}
