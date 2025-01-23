// Copyright © Opera Norway AS. All rights reserved.
// This file is an original work developed by Opera.
#ifndef __YYSLOT_H__
#define __YYSLOT_H__

#include "YYStd.h"

//#define DBG_SLOT_ALLOC_UNIQUE	// gives every object a unique slot which isn't reused (for debugging purposes)

template <typename T > struct YYSlot
{
	T**			m_ppEntries;
	int			m_nCapacity;
	int			m_nCount;
	int			m_lastFreed;

	int*		m_freeSlotStack;
	int			m_slotStackSize;

	YYSlot( int _size ) {
		m_ppEntries = (T**)YYAlloc( _size*sizeof(T*) );
		memset( m_ppEntries, 0, sizeof(T*)*_size );
		m_nCapacity = _size;
		m_nCount = 0;
		m_lastFreed = 0;

		m_freeSlotStack = (int*)YYAlloc(_size * 2 * sizeof(int));	// allocate enough space for a single range for every slot
		m_slotStackSize = 2;
		m_freeSlotStack[0] = 0;
		m_freeSlotStack[1] = _size - 1;		
	} // end YYSlot

	~YYSlot() {
		YYFree( m_ppEntries );
		m_ppEntries = NULL;
		m_nCapacity = 0;
		m_nCount = 0;
		m_lastFreed = 0;

		YYFree(m_freeSlotStack);
		m_slotStackSize = 0;
	} // end destructor

	int allocSlot(T* _pEntry) {

		ERROR_IF_NOT_MAIN_THREAD("You are allocating a struct/array on a thread - this can also be caused by putting an object\array into a data structure (i.e. a ds_map) on a thread");	// This is called whenever a new object is allocated, which isn't safe to do on any thread other than the main one

		extern int g_GCrangestart;
		extern int g_GCrangeend;
		// Some extra faff required to make sure we take account of the garbage collector's window of consideration
		// as we can't allocate anything new in there while the GC is doing its thing		
#ifdef DBG_SLOT_ALLOC_UNIQUE
		// check to see if we should increase capacity
		if (m_lastFreed >= m_nCapacity) {
			int newsize = (m_nCapacity * 3) / 2;
			m_ppEntries = (T**)YYRealloc(m_ppEntries, newsize * sizeof(T*));
			memset(&m_ppEntries[m_nCapacity], 0, sizeof(T*) * (newsize - m_nCapacity));
			m_freeSlotStack = (int*)YYRealloc(m_freeSlotStack, newsize * 2 * sizeof(int));
			m_freeSlotStack[m_slotStackSize++] = m_nCapacity;
			m_freeSlotStack[m_slotStackSize++] = newsize - 1;			
			m_nCapacity = newsize;
		} // end if

		int nRet = m_lastFreed;
		m_lastFreed++;		// just using this to keep track of the current value

		m_ppEntries[nRet] = _pEntry;
		++m_nCount;
		return nRet;
#else
		int extraspaceneeded = 0;
		if (g_GCrangeend > g_GCrangestart)
			extraspaceneeded = g_GCrangeend - g_GCrangestart;

		// check to see if we should increase capacity
		if (m_nCount >= (m_nCapacity - extraspaceneeded)) {
			int newsize = ((m_nCapacity + extraspaceneeded) * 3) / 2;
			m_ppEntries = (T**)YYRealloc(m_ppEntries, newsize * sizeof(T*));
			memset(&m_ppEntries[m_nCapacity], 0, sizeof(T*)*(newsize - m_nCapacity));
			m_freeSlotStack = (int*)YYRealloc(m_freeSlotStack, newsize * 2 * sizeof(int));
			m_freeSlotStack[m_slotStackSize++] = m_nCapacity;
			m_freeSlotStack[m_slotStackSize++] = newsize - 1;			
			m_nCapacity = newsize;
		} // end if

		  // find a free slot             
		int nRet = -1;
		while ((m_slotStackSize > 0) && (nRet == -1))
		{
			// We can't just assume the first entry from the top of the stack is fine
			// as the garbage collector may have compacted the slot list

			// We store the slot stack as a series of min\max pairs spread across two entries in the array
			int slotVal;
			nRet = slotVal = m_freeSlotStack[m_slotStackSize - 2];
			//m_slotStackSize--;
			if ((nRet >= g_GCrangestart) && (nRet < g_GCrangeend))
			{
				nRet = -1;
			}
			else if (m_ppEntries[nRet] != NULL)
			{
				nRet = -1;
			}
			
			slotVal++;
			if (slotVal > m_freeSlotStack[m_slotStackSize - 1])
			{
				// This range has all been used so pop it off the top of the stack
				m_slotStackSize -= 2;
			}
			else
			{
				// We still have some space in this range
				m_freeSlotStack[m_slotStackSize - 2] = slotVal;
			}
		}
		if (nRet == -1)
		{
			int firstphasemax = yymin(m_nCapacity, g_GCrangestart);
			for (int n = m_lastFreed; n < firstphasemax; n++)
			{
				if (m_ppEntries[n] == NULL) {
					nRet = n;
					m_lastFreed = n + 1;
					break;
				} // end if
			}
			if (nRet == -1)
			{
				int secondphasemin = yymax(m_lastFreed, g_GCrangeend);
				int secondphasenum = (m_nCapacity - extraspaceneeded) - (secondphasemin - g_GCrangeend);
				for (int n = secondphasemin, num = secondphasenum; num > 0; --num, ++n) {
					if (n >= m_nCapacity) n = 0;
					if (m_ppEntries[n] == NULL) {
						nRet = n;
						m_lastFreed = n + 1;
						break;
					} // end if
				} // end for            
			}
			if (nRet == -1)
			{
				int last = yymin(m_nCapacity, m_lastFreed); // in case m_lastFreed is off the end of the array
				for (int n = g_GCrangeend; n < last; n++)
				{
					if (m_ppEntries[n] == NULL) {
						nRet = n;
						m_lastFreed = n + 1;
						break;
					} // end if
				}
			}
		}
		m_ppEntries[nRet] = _pEntry;
		m_lastFreed = nRet;
		++m_nCount;
		return nRet;
#endif
	} // end AllocSlot

	void freeSlot( int _slot ) {		
		if (m_ppEntries != NULL) {
			m_ppEntries[_slot] = NULL;
#ifndef DBG_SLOT_ALLOC_UNIQUE
			// Looks crazy but each slot stack entry is a min\max range spread across two entries (this makes clearing\resetting the slot stack way faster)
			m_freeSlotStack[m_slotStackSize++] = _slot;
			m_freeSlotStack[m_slotStackSize++] = _slot;
			m_lastFreed = yymin(_slot, m_lastFreed);	// store the earliest slot freed so we fill up earlier slots first
#endif
			--m_nCount;
		} // end if
	} // end FreeSlot

	void purgeFreeSlotStack()
	{
		m_slotStackSize = 0;
	}

	void addEntriesToFreeSlotStack(int _first, int _last=-1)
	{
		if (_last == -1)
			_last = m_nCapacity - 1;
		else if (_last >= m_nCapacity)
			_last = m_nCapacity - 1;

		if (_last < _first)
			return;

		if (_first < 0)
			_first = 0;

		m_freeSlotStack[m_slotStackSize++] = _first;
		m_freeSlotStack[m_slotStackSize++] = _last;		
	}

	void setLastFreed(int _slot)
	{
#ifndef DBG_SLOT_ALLOC_UNIQUE
		m_lastFreed = _slot;
#endif
	}

	T* getEntry( int _n ) const { return ((_n >= 0) && (_n < m_nCapacity)) ? m_ppEntries[ _n ] : NULL; }
	int capacity( void ) const { return m_nCapacity; }
	int count( void ) const { return m_nCount; }
}; // end template



#endif
