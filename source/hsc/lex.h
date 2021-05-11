
#include <cstdint>

#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "return.h"
#include "tokenizer.h"


using std::ifstream;


class token_id
{
public:
  static const int32_t ERROR_ID       = 0;    // used by lex to return errors
  static const int32_t TYPE           = 1;    // the token is a primative type
  static const int32_t KEYWORD        = 2;    // the token is a reserved keyword
  static const int32_t WHITESPACE     = 3;    // the token is whitespace
  static const int32_t LEFT_PAREN     = 4;    // the token is '('
  static const int32_t RIGHT_PAREN    = 5;    // the token is ')'
  static const int32_t LEFT_BRACKET   = 6;    // the token is '['
  static const int32_t RIGHT_BRACKET  = 7;    // the token is ']'
  static const int32_t LEFT_BRACE     = 8;    // the token is '{'
  static const int32_t RIGHT_BRACE    = 9;    // the token is '}'
  static const int32_t COMMA          = 10;   // the token is ','
  static const int32_t PERIOD         = 11;   // the token is ','
  static const int32_t SEMICOLON      = 12;   // the token is ';'
  static const int32_t OPERATOR       = 13;   // the token is an operator: *, /, +, -
  static const int32_t IDENTIFIER     = 14;   // the token is an identifier: abc, a12, _test, etc.
  static const int32_t INTEGER        = 15;   // the token is an integer
  static const int32_t FLOAT          = 16;   // the token is a floating point number

private:
protected:
};


/*--------------------------------------------\
|                                             |
|  L      EEEEE  XX   XX                      |
|  L      E       X   X                       |
|  L      E        X X                        |
|  L      EEE       X      Lexical Analizer   |
|  L      E        X X                        |
|  L      E       X   X                       |
|  LLLLL  EEEEE  XX   XX                      |
|                                             |
\____________________________________________*/
class lex : private token_id
{
public:
  typedef tokenizer::token_t lx_token_t;

  lex()
  {
    tk.SetErrorToken(ERROR_ID);

    tk.SetMacro("DIGIT",     "0|1|2|3|4|5|6|7|8|9");
    tk.SetMacro("DIGIT_NZ",  "1|2|3|4|5|6|7|8|9");
    tk.SetMacro("ALPHA_CAP", "A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z");
    tk.SetMacro("ALPHA_LIT", "a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z");
    tk.SetMacro("ALPHA",     "{ALPHA_CAP}{ALPHA_LIT}");
    tk.SetMacro("ALPHA_NUM", "{ALPHA}{DIGIT}");


    tk.SetTokenString(" ", WHITESPACE);
    tk.SetTokenString("\r", WHITESPACE);
    tk.SetTokenString("\n", WHITESPACE);
    tk.SetTokenString("\t", WHITESPACE);
    tk.SetTokenString("@", TYPE);
    tk.SetTokenString("def_object",KEYWORD);
    tk.SetTokenString("def_function",KEYWORD);
    tk.SetTokenString("def_procedure",KEYWORD);
    tk.SetTokenString("def_filter",KEYWORD);
    tk.SetTokenString("def_type",KEYWORD);
    tk.SetTokenString("return",KEYWORD);
    tk.SetTokenString("interface:",KEYWORD);
    tk.SetTokenString("internal:",KEYWORD);
    tk.SetTokenString("(",LEFT_PAREN);
    tk.SetTokenString(")",RIGHT_PAREN);
    tk.SetTokenString("[",LEFT_BRACKET);
    tk.SetTokenString("]",RIGHT_BRACKET);
    tk.SetTokenString("{",LEFT_BRACE);
    tk.SetTokenString("}",RIGHT_BRACE);
    tk.SetTokenString(",",COMMA);
    tk.SetTokenString("+",OPERATOR);
    tk.SetTokenString("-",OPERATOR);
    tk.SetTokenString("/",OPERATOR);
    tk.SetTokenString("*",OPERATOR);
    tk.SetTokenString("<",OPERATOR);
    tk.SetTokenString(">",OPERATOR);
    tk.SetTokenString("<=",OPERATOR);
    tk.SetTokenString(">=",OPERATOR);
    tk.SetTokenString("&&",OPERATOR);
    tk.SetTokenString("||",OPERATOR);
    tk.SetTokenString("==",OPERATOR);
    tk.SetTokenString("!=",OPERATOR);
    tk.SetTokenString(";",SEMICOLON);
    tk.SetTokenString(".",PERIOD);
    //tk.SetTokenSpecialIdent("integer",INTEGER);
    tk.SetTokenRegEx("0|{DIGIT_NZ}{DIGIT}*)", INTEGER);
    tk.SetTokenRegEx("0|((1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*)(.(0|1|2|3|4|5|6|7|8|9)*(1|2|3|4|5|6|7|8|9)+)-", FLOAT);
    //tk.SetTokenRegEx("0|((23)(45))", INTEGER);
    //tk.SetTokenRegEx("abc(d|e)*cbaf+", IDENTIFIER);
    tk.SetTokenRegEx("(xyz)+(xjk|mn)+(a)-", LEFT_BRACKET);
    tk.SetTokenRegEx("(a)*((b|bb)-(a)+)*(b|bb)-", LEFT_BRACKET);
  }

  void OpenFile(std::string file_name)
  {
    input_files.emplace_back(ifstream(file_name));
  }

  int32_t GetNextLine(std::string &str)
  {
    auto fs=input_files.rbegin();
    if (*fs) {
      getline(*fs,str);
      return RET_OK;
    } else {
      fs->close();
      return RET_DONE;
    }
  }


  lx_token_t GetToken( void )
  //typename tokenizer::token_t GetToken( void )
  {
    lx_token_t r;

    r = tk.GetToken();
    if (r.second == ERROR_ID) {
      std::string str="";
      if (GetNextLine(str)) {
        r.first = "done";
        return r;
      } else {
        for(auto x : str) tk.Enter((fsm::letter_t)x);
        return GetToken();
      }
    }
    return r;
  }
  
private:
  tokenizer tk;

  list<ifstream> input_files;

protected:

  friend std::ostream & operator << (std::ostream &out, lex::lx_token_t &t);
};


std::ostream & operator << (std::ostream &out, lex::lx_token_t &t);

