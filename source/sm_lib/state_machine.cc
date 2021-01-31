

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


