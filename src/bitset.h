/*  Bit Set Definitions */

/*
 *  Used for dominator analysis and else.
 *
 */

#ifndef BITSET_H
#define BITSET_H

#include <assert.h>
#include <stdio.h>

/*
 *  The bit_set class is a bit vector representation for a set of
 *  integers.  The member "bits" is the bit vector that is used to
 *  represent integers greater than or equal to "first" but strictly
 *  less than "last".  To improve efficiency, first and last are always
 *  rounded down and up to the nearest multiple of bits in a long,
 *  respectively.
 */

class bit_set {
    friend class bit_set_iter;

private:
    /* We make explicit copy constructor and assignment operator and
     * make them private to foil C++'s automatic default versions. */
    bit_set(const bit_set &);
    bit_set &operator=(const bit_set &);

    int first, last;
    long *bits;

    long operator[](int i);

public:
    bit_set() : first(0), last(0), bits(0) { }
    bit_set(int f, int l, bool no_clear = false);
    ~bit_set()				{ delete [] bits; }

    void expand(int f, int l, bool no_clear = false);
    int lb()				{ return first; }
    int ub()				{ return last; }
    void clear();				/* clear all the bits to 0 */
    void universal();			/* set all bits to 1 */
    void add(int e);			/* set bit e to 1 */
    void remove(int e);			/* reset bit e to 0 */
    void invert();				/* invert all bits */
    bool contains(int e);
    void set_union(bit_set *l, bit_set *r);
    void set_intersect(bit_set *l, bit_set *r);
    void copy(bit_set *s);
    void transfer(bit_set *src, bool del = true);
    bit_set &operator=(bit_set &b)		{ copy(&b); return *this; }
    void operator+=(bit_set &r);	/* bit-wise OR */
    void operator*=(bit_set &r);	/* bit-wise AND */
    void operator-=(bit_set &r);	/* bit-wise subtraction */
    bool operator==(bit_set &r);	/* bit-wise comparison */
    bool operator!=(bit_set &b)	{ return !(*this == b); }
    bool operator<=(bit_set &r);	/* subset of the bits? */
    bool operator^(bit_set &r);		/* test for non-empty intersection */
    bool is_empty();				/* all zeros? */
    bool is_universal();			/* all ones? */
    int count();					/* count the 1 bits */
    void print(FILE *fp = stdout, char *fmt = "%d,");
};


/*
 *  Bitset iterator.  This iterator works a little differently -- you
 *  must call is_empty() once before each call to step().
 */

class bit_set_iter {
protected:
    /* We make explicit copy constructor and assignment operator and
     * make them protected to foil C++'s automatic default versions. */
    bit_set_iter(const bit_set_iter &);
    bit_set_iter &operator=(const bit_set_iter &);

    bit_set *s;
    int w_offset, b_offset;		/* word and bit offsets in s->bits */
    int w_first;			/* s->first + (w_offset * word size) */
public:
    bit_set_iter() :s(0), w_offset(0), b_offset(0), w_first(0) { }
    bit_set_iter(bit_set *st);

    void reset(bit_set *st)		{ s = st; reset(); }
    void reset();
    int step()				{ return w_first + b_offset++; }
    bool is_empty();
};


#endif /* BITSET_H */

