/*----------------------------------------------------------------------------\
|  Copyright 2022 by the ->->->->->->->->->->->->->->->                       |
|                                                                             |
|  III  RRRRR    OOOO   N     N      H     H   OOOO   RRRRR    SSSS  EEEEE    |
|   I   R    R  O    O  NN    N      H     H  O    O  R    R  S      E        |
|   I   R    R  O    O  N N   N      H     H  O    O  R    R  S      E        |
|   I   RRRRR   O    O  N  N  N      HHHHHHH  O    O  RRRRR    SSS   EEEE     |
|   I   R  R    O    O  N   N N      H     H  O    O  R  R        S  E        |
|   I   R   R   O    O  N    NN      H     H  O    O  R   R       S  E        |
|  III  R    R   OOOO   N     N      H     H   OOOO   R    R  SSSS   EEEEE    |
|                                                                             |
|                                      ->->->->->->->->->->->->->-> project   |
+-----------------------------------------------------------------------------+
|                                                               _             |
|                                 _\_____\__        _          | |            |
|                ================|__________|__!___/ \_________|_|___/^       |
|    }           |               | |[ ] [ ] |      |        |        |        |
|    ============|               | |  101   |      |        |        |101{    |
|    |   I R O N   H O R S E     | |--------|----/ |--------|-_| |_-\\        |
|    |                           | |      /\|/\  /\|/\  /\|/\|     | \\_      |
|   @|==============101==========| | /|\ |--o========o=======|_____|/|\\\     |
|      (o)===(o)        (o)===(o)    \|/  \/|\/  \/|\/  \/|\/       \|/ |\    |
|                                                                             |
\____________________________________________________________________________*/


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
  static const int32_t ERROR_ID          = 0;    // used by lex to return errors
  static const int32_t TYPE              = 1;    // the token is a primative type
  static const int32_t WHITESPACE        = 2;    // the token is whitespace
  static const int32_t LEFT_PAREN        = 3;    // the token is '('
  static const int32_t RIGHT_PAREN       = 4;    // the token is ')'
  static const int32_t LEFT_BRACKET      = 5;    // the token is '['
  static const int32_t RIGHT_BRACKET     = 6;    // the token is ']'
  static const int32_t LEFT_BRACE        = 7;    // the token is '{'
  static const int32_t RIGHT_BRACE       = 8;    // the token is '}'
  static const int32_t COMMA             = 9;    // the token is ','
  static const int32_t PERIOD            = 10;   // the token is ','
  static const int32_t SEMICOLON         = 11;   // the token is ';'

  //-----------------
  // strict keywords
  //-----------------
  static const int32_t KW_AS             = 12;
  static const int32_t KW_BREAK          = 13;
  static const int32_t KW_CONST          = 14;
  static const int32_t KW_CONTINUE       = 15;
  static const int32_t KW_CRATE          = 16;
  static const int32_t KW_ELSE           = 17;
  static const int32_t KW_ENUM           = 18;
  static const int32_t KW_EXTERN         = 19;
  static const int32_t KW_FALSE          = 20;
  static const int32_t KW_FN             = 21;
  static const int32_t KW_FOR            = 22;
  static const int32_t KW_IF             = 23;
  static const int32_t KW_IMPL           = 24;
  static const int32_t KW_IN             = 25;
  static const int32_t KW_LET            = 26;
  static const int32_t KW_LOOP           = 27;
  static const int32_t KW_MATCH          = 28;
  static const int32_t KW_MOD            = 29;
  static const int32_t KW_MOVE           = 30;
  static const int32_t KW_MUT            = 31;
  static const int32_t KW_PUB            = 32;
  static const int32_t KW_REF            = 33;
  static const int32_t KW_RETURN         = 34;
  static const int32_t KW_SELFVALUE      = 35;
  static const int32_t KW_SELFVALUE      = 36;
  static const int32_t KW_STATIC         = 37;
  static const int32_t KW_STRUCT         = 38;
  static const int32_t KW_SUPER          = 39;
  static const int32_t KW_TRAIT          = 40;
  static const int32_t KW_TRUE           = 41;
  static const int32_t KW_TYPE           = 42;
  static const int32_t KW_UNSAFE         = 43;
  static const int32_t KW_USE            = 44;
  static const int32_t KW_WHERE          = 45;
  static const int32_t KW_WHILE          = 46;

  //-------------------------------
  // strict keywords added in 2018
  //-------------------------------
  static const int32_t KW_ASYNC          = 47;
  static const int32_t KW_AWAIT          = 48;
  static const int32_t KW_DYN            = 49;

  //-------------------
  // reserved keywords
  //-------------------
  static const int32_t KW_ABSTRACT       = 50;
  static const int32_t KW_BECOME         = 51;
  static const int32_t KW_BOX            = 52;
  static const int32_t KW_DO             = 53;
  static const int32_t KW_FINAL          = 54;
  static const int32_t KW_MACRO          = 55;
  static const int32_t KW_OVERRIDE       = 56;
  static const int32_t KW_PRIV           = 57;
  static const int32_t KW_TYPEOF         = 58;
  static const int32_t KW_UNSIZED        = 59;
  static const int32_t KW_VIRTUAL        = 60;
  static const int32_t KW_YIELD          = 61;

  //---------------------------------
  // reserved keywords added in 2018
  //---------------------------------
  static const int32_t KW_TRY            = 62;

  //---------------
  // weak keywords
  //---------------
  static const int32_t KW_UNION          = 63;
  static const int32_t KW_STATICLIFETIME = 64;

  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;
  static const int32_t KW_      = ;


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
    tk.SetMacro("ALPHA",     "{ALPHA_CAP}|{ALPHA_LIT}");
    tk.SetMacro("ALPHA_NUM", "{ALPHA}|{DIGIT}");


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
    tk.SetTokenString("=",OPERATOR);
    tk.SetTokenString("<=",OPERATOR);
    tk.SetTokenString(">=",OPERATOR);
    tk.SetTokenString("-=",OPERATOR);
    tk.SetTokenString("+=",OPERATOR);
    tk.SetTokenString("&&",OPERATOR);
    tk.SetTokenString("||",OPERATOR);
    tk.SetTokenString("==",OPERATOR);
    tk.SetTokenString("!=",OPERATOR);
    tk.SetTokenString(";",SEMICOLON);
    tk.SetTokenString(".",PERIOD);
    tk.SetTokenRegEx("0|{DIGIT_NZ}{DIGIT}*", INTEGER);
    tk.SetTokenRegEx("{ALPHA}{ALPHA_NUM}*", IDENTIFIER);
    //tk.SetTokenRegEx("0|((1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*)(.(0|1|2|3|4|5|6|7|8|9)*(1|2|3|4|5|6|7|8|9)+)-", FLOAT);
    //tk.SetTokenRegEx("0|((23)(45))", INTEGER);
    //tk.SetTokenRegEx("abc(d|e)*cbaf+", IDENTIFIER);
    //tk.SetTokenRegEx("(xyz)+(xjk|mn)+(a)-", LEFT_BRACKET);
    //tk.SetTokenRegEx("(a)*((b|bb)-(a)+)*(b|bb)-", LEFT_BRACKET);
    //tk.SetTokenRegEx("(ab)-(cd)-", LEFT_BRACKET);
    //tk.SetTokenRegEx("(0)-|1(01*0)*1|((0)-|1(01*0)*1)(0|1(01*0)*1)*((0)-|1(01*0)*1)", LEFT_BRACKET);

    //tk.SetTokenRegEx("@(0|1(0(1)*0)*1|(0|1(0(1)*0)*1)-(0|1(0(1)*0)*1)*(0|1(0(1)*0)*1)-)", LEFT_BRACKET);

    //tk.SetTokenRegEx("@(ab)+", LEFT_BRACKET);
    //tk.SetTokenRegEx("(01*0)*1|(0|1(01*0)*1)-", LEFT_BRACKET);
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

      if (r.first == "@empty") {
        std::string str="";
        if (GetNextLine(str)) {
          r.first = "@done";
          return r;
        } else {
          for(auto x : str) tk.Enter((fsm::letter_t)x);
          return GetToken();
        }
      } else {
        return r;
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

