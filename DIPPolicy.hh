/*
***************************************************
Changes are in line 73 - 95
***************************************************
 * Copyright (c) 2007 Mark D. Hill and David A. Wood
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MEM_RUBY_SYSTEM_DIPPOLICY_HH__
#define __MEM_RUBY_SYSTEM_DIPPOLICY_HH__

#include "mem/ruby/system/AbstractReplacementPolicy.hh"

/* Simple DIP insertion policy */

using namespace std;

class DIPPolicy : public AbstractReplacementPolicy
{
  public:
    DIPPolicy(Index num_sets, Index assoc);
    ~DIPPolicy();

    void touch(Index set, Index way, Time time);
    void DIPtouch(Index set, Index way, Time time);
    Index getVictim(Index set) const;
};

inline
DIPPolicy::DIPPolicy(Index num_sets, Index assoc)
    : AbstractReplacementPolicy(num_sets, assoc)
{
}

inline
DIPPolicy::~DIPPolicy()
{
}

inline void
DIPPolicy::touch(Index set, Index index, Time time)
{
    assert(index >= 0 && index < m_assoc);
    assert(set >= 0 && set < m_num_sets);
//    L2RefCount++;
//    std::cout << "L2 Ref Count is " << L2RefCount << endl;
    m_last_ref_ptr[set][index] = time;
}

inline void
DIPPolicy::DIPtouch(Index set, Index index, Time time)
{
    assert(index >= 0 && index < m_assoc);
    assert(set >= 0 && set <m_num_sets);
    
    Time time1, smallest_time;
    //Index smallest_index;

//    smallest_index = 0;
    smallest_time = m_last_ref_ptr[set][0];

    for (unsigned i = 0; i < m_assoc; i++) {
  time1 = m_last_ref_ptr[set][0];

	if (time1 < smallest_time) {
//	    smallest_index = i;
	    smallest_time = time1; //
	}
    }

    m_last_ref_ptr[set][index] = smallest_time - 1; //This makes the line LRU as no other line has a smaller time at this point.
} 

inline Index
DIPPolicy::getVictim(Index set) const
{
    //  assert(m_assoc != 0);
    Time time, smallest_time;
    Index smallest_index;

  //  L2MissCount++;
  //  std::cout << "L2 Miss Count is " << L2MissCount << endl;
    smallest_index = 0;
    smallest_time = m_last_ref_ptr[set][0];

    for (unsigned i = 0; i < m_assoc; i++) {
        time = m_last_ref_ptr[set][i];
        // assert(m_cache[cacheSet][i].m_Permission !=
        //     AccessPermission_NotPresent);

        if (time < smallest_time) {
            smallest_index = i;
            smallest_time = time;
        }
    }

    //  DEBUG_EXPR(CACHE_COMP, MedPrio, cacheSet);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, smallest_index);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, m_cache[cacheSet][smallest_index]);
    //  DEBUG_EXPR(CACHE_COMP, MedPrio, *this);

    return smallest_index;
}

#endif // __MEM_RUBY_SYSTEM_LRUPOLICY_HH__
