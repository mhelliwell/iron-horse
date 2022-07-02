


#ifndef TOKENIZER_DOT_H
#define TOKENIZER_DOT_H



#include <cstdint>

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <deque>
#include <map>

#include "state_machine.h"
#include "return.h"

using std::cout;
using std::endl;
using std::string;
using std::pair;
using std::list;
using std::next;
using std::prev;
using std::map;
using std::deque;



/*--------------------------------------------------------------------\
|                                                                     |
|  TTTTTTT  OOO   K   K  EEEEE  N     N  III  ZZZZZ  EEEEE  RRRRR     |
|     T    O   O  K  K   E      NN    N   I       Z  E      R    R    |
|     T    O   O  K K    E      N N   N   I      Z   E      R    R    |
|     T    O   O  KK     EEE    N  N  N   I     Z    EEE    RRRRR     |
|     T    O   O  K K    E      N   N N   I    Z     E      R  R      |
|     T    O   O  K  K   E      N    NN   I   Z      E      R   R     |
|     T     OOO   K   K  EEEEE  N     N  III  ZZZZZ  EEEEE  R    R    |
|                                                                     |
\____________________________________________________________________*/
class tokenizer
{
public:
  typedef pair<string,int32_t> token_t;

  void SetErrorToken(int32_t err)
  {
    error_ident = err;
  }

  //-----------------------------
  //      SetTokenString
  // creates a state machine to
  // detect the given string
  // and binds it to the given
  // identifier.
  //-----------------------------
  void SetTokenString(string str, int32_t ident)
  {
    state_machine sm;

    fsm::state_t s1 = sm.StartState();
    for(auto c : str) {
      fsm::state_t s_new = sm.GetNewState();
      sm.SetTransition(s1,c,s_new);
      s1 = s_new;
    }
    sm.SetAccept(s1);
    token_sensors.emplace_back(fsm_entry_t(sm,ident));
  }


  //-----------------------------
  //      SetTokenRegEx
  // creates a state machine to
  // detect the given regular
  // expresion and binds it to
  // the given identifier.
  //-----------------------------
  void SetTokenRegEx(string str, int32_t ident)
  {
    regex_tk_list_t  tokens;
    // cout << "regex: " << str << endl;

    // cout << "^^^^^^^^00000" << endl;

    StringToRegexTokens(str, tokens, tokens.begin());

#if 0
    int c=0;
    for(auto x : tokens) {
      cout << c++ << "  ";
      if (x.second == REGEX_TOKEN_STRING)        cout << "         string: ";
      if (x.second == REGEX_TOKEN_LEFT_PAREN)    cout << "     Left Paren: ";
      if (x.second == REGEX_TOKEN_RIGHT_PAREN)   cout << "    Right Paren: ";
      if (x.second == REGEX_TOKEN_LEFT_BRACKET)  cout << "  Right Bracket: ";
      if (x.second == REGEX_TOKEN_RIGHT_BRACKET) cout << "   Left Bracket: ";
      if (x.second == REGEX_TOKEN_STAR)          cout << "           Star: ";
      if (x.second == REGEX_TOKEN_PLUS)          cout << "           Plus: ";
      if (x.second == REGEX_TOKEN_MINUS)         cout << "          Minus: ";
      if (x.second == REGEX_TOKEN_V_BAR)         cout << "   Virtical Bar: ";
      cout << x.first << endl;
    }
#endif

    auto left   = tokens.begin();
    auto middle = next(left);
    auto right  = next(middle);
    while( right != tokens.end() ) {
      if ( left->second == REGEX_TOKEN_LEFT_BRACKET &&
           middle->second == REGEX_TOKEN_STRING &&
         right->second == REGEX_TOKEN_RIGHT_BRACKET ) {
        tokens.emplace(left, "", REGEX_TOKEN_LEFT_PAREN);
        auto new_left=prev(left);
        StringToRegexTokens(macros[middle->first], tokens, left);
        tokens.emplace(left, "", REGEX_TOKEN_RIGHT_PAREN);
        tokens.erase(left,next(right));
        left   = next(new_left);
        middle = next(left);
        right  = next(middle);
      } else {
        ++left;
        ++middle;
        ++right;
      }
    }


#if 0
    int c1=0;
    for(auto x : tokens) {
      cout << c1++ << "  ";
      if (x.second == REGEX_TOKEN_STRING)        cout << "         string: ";
      if (x.second == REGEX_TOKEN_LEFT_PAREN)    cout << "     Left Paren: ";
      if (x.second == REGEX_TOKEN_RIGHT_PAREN)   cout << "    Right Paren: ";
      if (x.second == REGEX_TOKEN_LEFT_BRACKET)  cout << "  Right Bracket: ";
      if (x.second == REGEX_TOKEN_RIGHT_BRACKET) cout << "   Left Bracket: ";
      if (x.second == REGEX_TOKEN_STAR)          cout << "           Star: ";
      if (x.second == REGEX_TOKEN_PLUS)          cout << "           Plus: ";
      if (x.second == REGEX_TOKEN_MINUS)         cout << "          Minus: ";
      if (x.second == REGEX_TOKEN_V_BAR)         cout << "   Virtical Bar: ";
      cout << x.first << endl;
    }
#endif

    state_machine sm;
    fsm::state_t final = sm.GetNewState();
    RecursiveConstructRegex(sm.StartState(),final,sm,tokens);
    sm.SetAccept(final);
    //cout << "^^^^^^^^^^^^^" << endl;
    //cout << sm;
    //cout << "^^^^^^^^^^^^^" << endl;
    token_sensors.emplace_back(fsm_entry_t(sm.Nfa2Dfa().Optimize(),ident));
  }



  bool    TokenP( void ) { return !remaining; }
  void    Reset( void )
  {
    for(auto x = token_sensors.begin(); x != token_sensors.end();  ++x) {
      x->first.Reset();
    }
    remaining=token_sensors.size();
  }

  void SetMacro(string s1, string s2)
  {
    macros[s1]=s2;
  }


  void Enter( fsm::letter_t const &c )
  {
    char_pipe.push_back(c);
    StepAll(c);
  }
  
  //token_t GetToken( void );
  token_t GetToken( void )
  {
    //cout << "enter get token" << endl;
    string bad_chars="";
    int32_t best = 0;
    typename list<fsm_entry_t>::iterator best_token;
    for(auto x = token_sensors.begin(); x != token_sensors.end();  ++x) {
      int32_t len = x->first.GetAcceptLength();

      //cout << len << endl;
      if (len > best) {
        best = len;
        best_token = x;
      }
    }
    if (best) {
      string tk="";
      while(best--) {
        tk.append(1,char_pipe.front());
        char_pipe.pop_front();
      }
      Reset();
      for(auto x : char_pipe) StepAll(x);
      return token_t(tk,best_token->second);
    } else {
      //cout << "char_pipe=" << char_pipe.size() << endl;
      if (char_pipe.size()) {
        bad_chars.append(1,char_pipe.front());
        char_pipe.pop_front();
        Reset();
        for(auto x : char_pipe) StepAll(x);
        return token_t(bad_chars, error_ident);
      }
      return token_t("@empty", error_ident);
    }
  }


private:
  typedef pair<state_machine,int32_t>             fsm_entry_t;
  typedef list<pair<string,int32_t>>              regex_tk_list_t;
  typedef list<pair<string,int32_t>>::iterator    regex_tk_list_t_iter;

  void StringToRegexTokens(string &str, regex_tk_list_t &tk, regex_tk_list_t_iter it)
  {
    cout << "regex: " << str << endl;
    //regex_tk_list_t_iter it;
  
    string temp="";
    for(auto ch : str) {
      if (ch == '(') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_LEFT_PAREN);
      } else if (ch == ')') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_RIGHT_PAREN);
      } else if (ch == '{') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_LEFT_BRACKET);
      } else if (ch == '}') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_RIGHT_BRACKET);
      } else if (ch == '*') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_STAR);
      } else if (ch == '+') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_PLUS);
      } else if (ch == '-') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_MINUS);
      } else if (ch == '|') {
        if (temp.size()>0) { tk.emplace(it, temp, REGEX_TOKEN_STRING); temp = ""; }
        tk.emplace(it, "", REGEX_TOKEN_V_BAR);
      } else {
        temp += ch;
      } 
    }
    if (temp.size()>0) { tk.emplace(it++, temp, REGEX_TOKEN_STRING); temp = ""; }
  }


  void RecursiveConstructRegex(fsm::state_t start, fsm::state_t finish, state_machine &sm, regex_tk_list_t &tk)
  {
    // case 1: nothing but a string
    if (tk.size() == 1 && tk.begin()->second == REGEX_TOKEN_STRING) {
      for(auto c : tk.begin()->first) {
        fsm::state_t s_new = sm.GetNewState();
        sm.SetTransition(start,c,s_new);
        start = s_new;
      }
      sm.SetEpsilon(start,finish);
      return;
    }

    // cases that start with a left parenthesis
    if (tk.size() > 1  &&  tk.begin()->second == REGEX_TOKEN_LEFT_PAREN) {
      auto match = next(tk.begin());
      int32_t count = 1;
      while( match != tk.end() && count > 0 ) {
        if (match->second == REGEX_TOKEN_LEFT_PAREN) {
          ++count;
          ++match;
        } else if (match->second == REGEX_TOKEN_RIGHT_PAREN) {
          if (--count) ++match;
        } else {
          ++match;
        }
      }
      if (count) {
        cout << "@@@1 unmatched parenthesis in regex\n";
        return;
      }
      auto match_plus = next(match);

      // case 2: matching parentheses on the ends
      if (match_plus == tk.end()) {
        tk.erase(match);
        tk.erase(tk.begin());
        return RecursiveConstructRegex(start, finish, sm, tk);
      }

      // case 3 and 4: star, plus, and minus expression
      if (next(match_plus) == tk.end()) {
        // case 3:
        if (match_plus->second == REGEX_TOKEN_PLUS) {
          fsm::state_t s2 = sm.GetNewState();
          fsm::state_t f2 = sm.GetNewState();
          sm.SetEpsilon(start,s2);
          sm.SetEpsilon(f2,finish);
          sm.SetEpsilon(f2,s2);
          tk.erase(match_plus);
          tk.erase(match);
          tk.erase(tk.begin());
          RecursiveConstructRegex(s2, f2, sm, tk);
          return;
        }
        // case 4:
        if (match_plus->second == REGEX_TOKEN_MINUS) {
          fsm::state_t s2 = sm.GetNewState();
          fsm::state_t f2 = sm.GetNewState();
          sm.SetEpsilon(start,s2);
          sm.SetEpsilon(f2,finish);
          sm.SetEpsilon(s2,f2);
          tk.erase(match_plus);
          tk.erase(match);
          tk.erase(tk.begin());
          RecursiveConstructRegex(s2, f2, sm, tk);
          return;
        }
        // case 5:
        if (match_plus->second == REGEX_TOKEN_STAR) {
          fsm::state_t s2 = sm.GetNewState();
          fsm::state_t f2 = sm.GetNewState();
          sm.SetEpsilon(start,s2);
          sm.SetEpsilon(f2,finish);
          sm.SetEpsilon(s2,f2);
          sm.SetEpsilon(f2,s2);
          tk.erase(match_plus);
          tk.erase(match);
          tk.erase(tk.begin());
          RecursiveConstructRegex(s2, f2, sm, tk);
          return;
        }
      }
    }

    // case 6: virtical bar (or) expression
    auto v_bar = tk.begin();
    if (v_bar->second == REGEX_TOKEN_V_BAR) {
      cout << "regex cannot start with a virtical bar\n";
      return;
    }
    int32_t count=0;
    do {
      if (v_bar->second == REGEX_TOKEN_LEFT_PAREN)  ++count;
      if (v_bar->second == REGEX_TOKEN_RIGHT_PAREN) --count;
      ++v_bar;
    } while(v_bar != tk.end()  &&  !(count == 0  &&  v_bar->second == REGEX_TOKEN_V_BAR));
    if (v_bar->second == REGEX_TOKEN_V_BAR) {
      if (next(v_bar) == tk.end()) {
        cout << "regex cannot end with a virtical bar\n";
        return;
      }
      list<pair<string,int32_t>> last_half;
      last_half.splice(last_half.begin(),tk,next(v_bar),tk.end());
      tk.erase(v_bar);
      fsm::state_t s1 = sm.GetNewState();
      fsm::state_t f1 = sm.GetNewState();
      fsm::state_t s2 = sm.GetNewState();
      fsm::state_t f2 = sm.GetNewState();
      sm.SetEpsilon(start,s1);
      sm.SetEpsilon(start,s2);
      sm.SetEpsilon(f1,finish);
      sm.SetEpsilon(f2,finish);
      RecursiveConstructRegex(s1, f1, sm, tk);
      RecursiveConstructRegex(s2, f2, sm, last_half);
      return;
    }

    // case 7: this must be a concatination
    if (tk.begin()->second == REGEX_TOKEN_STRING) {
      list<pair<string,int32_t>> first_part;
      first_part.splice(first_part.begin(),tk,tk.begin());
      fsm::state_t mid = sm.GetNewState();
      RecursiveConstructRegex(start, mid, sm, first_part);
      RecursiveConstructRegex(mid, finish, sm, tk);
      return;
    }
    if (tk.begin()->second == REGEX_TOKEN_LEFT_PAREN) {
      auto match = next(tk.begin());
      int32_t count = 1;
      while( match != tk.end() && count > 0 ) {
        if (match->second == REGEX_TOKEN_LEFT_PAREN) {
          ++count;
        } else if (match->second == REGEX_TOKEN_RIGHT_PAREN) {
          --count;
        }
        ++match;
      }
      if (count) {
        cout << "@@@ unmatched parenthesis in regex\n";
      }
      if (match->second == REGEX_TOKEN_PLUS
          ||  match->second == REGEX_TOKEN_MINUS
          ||  match->second == REGEX_TOKEN_STAR) ++match;
      list<pair<string,int32_t>> last_half;
      last_half.splice(last_half.begin(),tk,match,tk.end());
      fsm::state_t mid = sm.GetNewState();
      RecursiveConstructRegex(start, mid, sm, tk);
      RecursiveConstructRegex(mid, finish, sm, last_half);
      return;
    }
  }

  
  void StepAll( fsm::letter_t const &c )
  {
    //cout << "Enter:" << c << std::endl;
    for(auto x = token_sensors.begin(); x != token_sensors.end();  ++x) {
      if (!x->first.RejectP()) {
        x->first.Step(c);
        if (x->first.RejectP()) {
          --remaining;
        }
      }
    }
  }

  int32_t remaining;
  list<fsm_entry_t>    token_sensors;
  deque<fsm::letter_t> char_pipe;
  map<string,string>   macros;
  int32_t error_ident;


  static const int32_t REGEX_TOKEN_STRING;
  static const int32_t REGEX_TOKEN_LEFT_PAREN;
  static const int32_t REGEX_TOKEN_RIGHT_PAREN;
  static const int32_t REGEX_TOKEN_LEFT_BRACKET;
  static const int32_t REGEX_TOKEN_RIGHT_BRACKET;
  static const int32_t REGEX_TOKEN_STAR;
  static const int32_t REGEX_TOKEN_PLUS;
  static const int32_t REGEX_TOKEN_MINUS;
  static const int32_t REGEX_TOKEN_V_BAR;

protected:
};



#endif // TOKENIZER_DOT_H
