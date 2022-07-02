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

#include "lex.h"



std::ostream & operator << (std::ostream &out, lex::lx_token_t &t)
{
  string s;

  switch( t.second ) {
  case lex::ERROR_ID:         s = "Error             :"; break;
  case lex::KEYWORD:          s = "Keyword           :"; break;
  case lex::TYPE:             s = "Type Name         :"; break;
  case lex::WHITESPACE:       s = "White Space       :"; break;
  case lex::LEFT_PAREN:       s = "Left Parentheses  :"; break;
  case lex::RIGHT_PAREN:      s = "Right Parentheses :"; break;
  case lex::LEFT_BRACKET:     s = "Left Bracket      :"; break;
  case lex::RIGHT_BRACKET:    s = "Right Bracket     :"; break;
  case lex::LEFT_BRACE:       s = "Left Brace        :"; break;
  case lex::RIGHT_BRACE:      s = "Right Brace       :"; break;
  case lex::INTEGER:          s = "Integer           :"; break;
  case lex::FLOAT:            s = "Float             :"; break;
  case lex::SEMICOLON:        s = "Semicolon         :"; break;
  case lex::COMMA:            s = "Comma             :"; break;
  case lex::PERIOD:           s = "Period            :"; break;
  case lex::IDENTIFIER:       s = "Identifier        :"; break;
  default:                    s = "<no name defined> :"; break;
  }

  s += " ";

  if (t.first == " ") {
    s += "<space>";
  } else if (t.first == "\n") {
    s += "<new line>";
  } else if (t.first == "\r") {
    s += "<return>";
  } else if (t.first == "\t") {
    s += "<tab>";
  } else if (t.first == "\f") {
    s += "<form feed>";
  } else {
    s += t.first;
  }

  out << s;

  return out;
}

