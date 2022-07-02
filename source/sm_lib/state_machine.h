
#ifndef STATE_MACHINE_DOT_H
#define STATE_MACHINE_DOT_H


#include <cstdint>

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <array>
#include <unordered_set>
#include <vector>
#include <queue>

#include "return.h"

using std::string;
using std::set;
using std::map;
using std::array;
using std::unordered_set;
using std::vector;
using std::queue;
using std::cout;
using std::endl;


/*-------------------------------------------------------\
|  III N   N TTTTT EEEE RRRR  FFFF  A    CCC EEEE  SSSS  |
|   I  NN  N   T   E    R   R F    A A  C    E    S      |
|   I  N N N   T   EEE  RRRR  FFF AAAAA C    EEE   SSS   |
|   I  N  NN   T   E    R  R  F   A   A C    E        S  |
|  III N   N   T   EEEE R   R F   A   A  CCC EEEE SSSS   |
\_______________________________________________________*/

//-------------------------------------
//  Base interface for finite state
//  machines. Contains only typedefs.
//-------------------------------------
class fsm
{
public:
  typedef uint8_t  letter_t;
  typedef int32_t  state_t;

private:

protected:
};

//------------------------------
//  Interface for classes that
//  can build state machines
//------------------------------
class fsm_builder: public fsm
{
public:
  virtual state_t GetNewState( void ) = 0;
  virtual ret_t   SetTransition(state_t from, letter_t ch, state_t to) = 0;
  virtual state_t StartState( void ) = 0;
  virtual void    SetAccept( state_t st ) = 0;

private:

protected:
  
};

//-----------------------------------
//  interface for synthesizing code
//  to implement a statemachine
//-----------------------------------
class fsm_synthesizer: public fsm
{
public:
  virtual ret_t  SetTransition(state_t from, letter_t ch, state_t to) = 0;

private:

protected:
  
};

//------------------------------
//  interface for classes that
//  can run state machines
//------------------------------
class fsm_runner: public fsm
{
public:
  virtual void    Reset( void ) = 0;
  virtual void    Step( letter_t x ) = 0;
  virtual void    Step( string str ) = 0;
  virtual bool    AcceptP( void ) = 0;
  virtual bool    RejectP( void ) = 0;
  virtual int32_t GetAcceptLength( void ) = 0;
private:

protected:
  
};


/*-------------------------------------------------------------------------------------\
|  III M   M PPPP  L    EEEE M   M EEEE N   N TTTTT   A   TTTTT III  OOO  N   N  SSS   |
|   I  MM MM P   P L    E    MM MM E    NN  N   T    A A    T    I  O   O NN  N S      |
|   I  M M M PPPP  L    EEE  M M M EEE  N N N   T   AAAAA   T    I  O   O N N N  SSS   |
|   I  M   M P     L    E    M   M E    N  NN   T   A   A   T    I  O   O N  NN     S  |
|  III M   M P     LLLL EEEE M   M EEEE N   N   T   A   A   T   III  OOO  N   N  SSS   |
\_____________________________________________________________________________________*/


//------------------------------------------------
//  a class that can build and run statemachines
//------------------------------------------------
class state_machine: public fsm_builder, public fsm_runner
{
public:
  static const letter_t RESET   = 0xff;

  state_machine()
  {
    int j;

    state_table.push_back(trans_row_t()); // start state (0)
    state_table.push_back(trans_row_t()); // sink reject (1)
    for(j=0; j<256; ++j) {
      state_table[0][j] = SINK_REJECT_STATE;
      state_table[1][j] = SINK_REJECT_STATE;
    }
    start_state = 0;
    next_available_state = 2;
    Reset();
  }

  state_t GetNewState( void )
  {
    state_table.push_back(trans_row_t()); // add a new row to the state table
    state_t st = next_available_state++;
    for(int j=0; j<256; ++j) state_table[st][j] = SINK_REJECT_STATE;
    return st;
  }

  ret_t SetTransition(state_t from, letter_t ch, state_t to)
  {
    state_table[from][ch] = to;
    return RET_OK;
  }

  ret_t SetEpsilon(state_t from, state_t to)
  {
    if ( epsilons.find(from) == epsilons.end() ) {
      epsilons[from] = set<int32_t>();
    }
    epsilons[from].insert(to);
    return RET_OK;
  }

  void Reset( void )
  {
    //cout << "Resetting...\n";
    current_state = start_state;
    step_count = 0;
    accept_count = 0;
  }

  void Step( letter_t x )
  {
    //cout << "transition from " << current_state << " to " << state_table[current_state][x] << std::endl;
    current_state = state_table[current_state][x];
    if (current_state != SINK_REJECT_STATE) {
      ++step_count;
      if (AcceptP()) accept_count = step_count;
    }
  }

  void Step( string str )
  {
    for( auto x : str ) Step( x );
  }

  state_t StartState( void )
  {
    return start_state;
  }

  void SetAccept( state_t st )
  {
    accept_states.insert(st);
  }

  bool AcceptP( void )
  {
    return accept_states.end() != accept_states.find(current_state);
  }

  bool RejectP( void )
  {
    return current_state == SINK_REJECT_STATE;
  }

  int32_t GetAcceptLength( void )
  {
    return accept_count;
  }

  state_machine Nfa2Dfa( void )
  {
    state_machine new_sm;
    map<state_t, set<int32_t>> state_set_map;
    queue<int32_t> to_expand;

    state_set_map[new_sm.StartState()] = ExpandNfaNode( StartState() );
    to_expand.push(new_sm.StartState());

    state_set_map[1] = set<int32_t>();
    state_set_map[1].insert(1);

    while(to_expand.size()) {
      int32_t       index_to_expand = to_expand.front();
      set<int32_t>  set_to_expand = state_set_map[index_to_expand];
      to_expand.pop();

      for(letter_t j=0x20; j<0x80; ++j) {

        set<int32_t> step_state=StepSet(j,set_to_expand);

        bool new_p=true;
        state_t state_to_use = -1;
        for(auto x=state_set_map.begin(); new_p && x != state_set_map.end(); ++x) {
          if (step_state == x->second) {
            new_p=false;
            state_to_use = x->first;
          }
        }
        if (new_p) {
          state_to_use = new_sm.GetNewState();
          state_set_map[state_to_use] = step_state;
          to_expand.push(state_to_use);
          for(state_t accept : accept_states) {
            if (step_state.find(accept) != step_state.end()) {
              new_sm.SetAccept(state_to_use);
              break;
            }
          }
        }
        new_sm.SetTransition(index_to_expand,j,state_to_use);
      }
    }

    //cout << "<begin DFA>\n";
    //cout << new_sm;
    //cout << "<end>\n";

    return new_sm;
  }


  state_machine Optimize( void ) // only works with a DFA
  {
    vector<set<state_t>>          state_sets;
    map<state_t,int32_t>          back_map;
    
    state_sets.push_back(set<state_t>());  // all reject states except sink  (index 0)
    state_sets.push_back(set<state_t>());  // sink reject state              (index 1)
    state_sets.push_back(set<state_t>());  // all accept states              (index 2)

    // load up the state vector
    for(state_t st=0; st<next_available_state; ++st) {
      if (accept_states.find(st) != accept_states.end()) {
        state_sets[2].insert(st);
        back_map[st]=2;
      } else {
        if (st==1) {
          state_sets[1].insert(st);
          back_map[st]=1;
        } else {
          state_sets[0].insert(st);
          back_map[st]=0;
        }
      }
    }

    // move incompatible states to a new group
    bool changes;
    do {
      changes=false;
      vector<set<state_t>>  old_state_sets(state_sets);
      map<state_t,int32_t>  old_back_map(back_map);

      back_map.clear();
      for(uint32_t j=0; j<state_sets.size(); ++j) {
        state_sets[j].clear();
        int32_t st = *(old_state_sets[j].begin());
        state_sets[j].insert(st);
        back_map[st]=j;
      }
#if 1
  cout << "-------------------------------------------------\n";
  for(uint32_t j=0; j < old_state_sets.size(); ++j) {
    cout << j << ": ";
    for(auto st : old_state_sets[j]) cout << " " << st;
    cout << endl;
  }
  cout << "-------------------------------------------------\n";
#endif
      for(uint32_t j=0; j < old_state_sets.size(); ++j) {

        for(int32_t st : old_state_sets[j]) {

          bool state_placed=false;
          for(uint32_t k=j; k < state_sets.size()  &&  !state_placed; ++k) {
            if (IsCompatible(st,state_sets[k],old_back_map,accept_states)) {
              state_placed=true;
              state_sets[k].insert(st);
              back_map[st] = k;
            }
          }
          if (!state_placed) {
            changes=true;
            state_sets.push_back(set<state_t>());
            state_sets.back().insert(st);
            back_map[st] = state_sets.size()-1;
          }
        }
      }
    } while(changes);

#if 0
    cout << "-------------------------------------------------\n";
    for(uint32_t j=0; j < state_sets.size(); ++j) {
      cout << j << ": ";
      for(auto st : state_sets[j]) cout << " " << st;
      cout << endl;
    }
    cout << "-------------------------------------------------\n";
#endif

    state_machine sm;
#if 0
    cout << "start state is " << start_state << " --> " << back_map[start_state] << endl;
#endif
    // here we need to ensure that the start state is 0 (should be true already)

    for(uint32_t j=2; j<state_sets.size(); ++j) sm.GetNewState();
    for(uint32_t j=0; j<state_sets.size(); ++j) {
      state_t st = *(state_sets[j].begin());
      for(int32_t ch=0; ch<256; ++ch) {
        if (state_table[st][ch] != 1) sm.SetTransition(j,ch,back_map[state_table[st][ch]]);
      }
    }
    for(state_t st : accept_states) {
      sm.SetAccept(back_map[st]);
    }
#if 1
    cout << "<begin opt>\n";
    cout << sm;
    cout << "<end>\n";
#endif
    return sm;
  }

private:

  bool IsCompatible(state_t state, set<state_t>& group, map<state_t,int32_t>& bm, unordered_set<state_t> ac_st);
  set<int32_t> StepSet( int32_t ch, set<int32_t> &st );
  set<int32_t> ExpandNfaNode( state_t st );
  void ExpandNfaNodeRecurse( state_t st, set<int32_t> &nodes );

  static const uint32_t SINK_REJECT_STATE = 0x01;

  typedef array<int32_t, 256> trans_row_t;
  
  state_t  start_state;
  state_t  next_available_state;
  int32_t  step_count;
  int32_t  accept_count;
  vector<trans_row_t> state_table;
  unordered_set<state_t> accept_states;
  map<int32_t,set<int32_t>> epsilons;

  state_t  current_state;
protected:

  friend std::ostream & operator << (std::ostream &out, state_machine &sm);
};


#endif // STATE_MACHINE_DOT_H
