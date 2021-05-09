

#include <cstdint>

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <memory>
#include <array>
#include <string>
#include <utility>
#include <list>
#include <deque>
#include <queue>
#include <fstream>
#include <map>

#include "return.h"


#include "state_machine.h"


std::ostream & operator << (std::ostream &out, state_machine &sm)
{
  for(int32_t j=0, s=sm.state_table.size(); j<s; ++j ) {
    auto row = sm.state_table[j];
    map<int32_t,set<int32_t>> going_to;
    for(int32_t k=0; k<256; ++k) {
      int32_t g = row[k];
      if (going_to.find(g) == going_to.end()) {
        going_to[g] = set<int32_t>();
      }
      going_to[g].insert(k);
    }
    cout << j << ":";
    for(auto const& x : going_to) {
      if (x.first != 1) {   // if this is not going to the sink state...
        cout << " (";
        bool first_time=true;
        for(auto const& y : x.second) {
          if (first_time) first_time=false; else cout << ",";
          cout << (char)y;
        }
        cout << ")->" << x.first;
      }
    }
    if (going_to.find(j) != going_to.end()  &&  going_to[j].size() == 256) cout << " [sink]";
    if (sm.accept_states.find(j) != sm.accept_states.end()) cout << " [accept]";
    cout << endl;
  }
  return out;
}






bool state_machine::IsCompatible(state_t state, set<state_t>& group, map<state_t,int32_t>& bm, unordered_set<state_t> ac_st)
{
  if ((ac_st.find(state) == ac_st.end()) != (ac_st.find(*group.begin()) == ac_st.end())) return false;
  for(state_t s : group)
    if (s != state)
      for(int32_t ch=0; ch<256; ++ch)
        if (bm[state_table[state][ch]] != bm[state_table[s][ch]]) return false;
  return true;
}


set<int32_t> state_machine::StepSet( int32_t ch, set<int32_t> &st )
{
  set<int32_t> s;
  for(int32_t x : st) {
    auto t = ExpandNfaNode(state_table[x][ch]);
    s.insert(t.begin(), t.end());
  }
  return s;
}

set<int32_t> state_machine::ExpandNfaNode( state_t st )
{
  set<int32_t> s;
  ExpandNfaNodeRecurse( st, s );
  return s;
}

void state_machine::ExpandNfaNodeRecurse( state_t st, set<int32_t> &nodes )
{
  nodes.insert(st);
  if ( epsilons.find(st) != epsilons.end() ) {
    auto x=epsilons[st];
    for(auto s : x) {
      if ( nodes.find(s) == nodes.end() ) ExpandNfaNodeRecurse(s,nodes);
    }
  }
}

