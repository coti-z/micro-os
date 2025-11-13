#include "memory.h"
#include "io.h"

/* Memory allocator by Kernighan and Ritchie,
 * The C programming Language, 2nd ed.  Section 8.7.
 */

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

/* Heap allocation */
static uint8_t heap[HEAP_SIZE];
static uint heap_used = 0;

void
memory_init(void)
{
  printf("Initializing memory allocator...\n");
  base.s.ptr = freep = &base;
  base.s.size = 0;
  printf("Heap initialized: 0x%p - 0x%p (%d KB)\n",
         (void *)heap, (void *)(heap + HEAP_SIZE), HEAP_SIZE / 1024);
}

static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if (nu < 4096)
    nu = 4096;

  /* Check if we have enough space */
  if (heap_used + nu * sizeof(Header) > HEAP_SIZE)
    return 0;

  p = (char *)heap + heap_used;
  heap_used += nu * sizeof(Header);

  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}

/* Set memory to value */
void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    unsigned char val = (unsigned char)c;

    for (size_t i = 0; i < n; i++) {
        p[i] = val;
    }

    return s;
}
