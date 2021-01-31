
#include <cstdint>

#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "return.h"
#include "tokenizer.h"

#include "lex.h"

using std::cout;
using std::endl;
using std::string;
using std::list;
using std::ifstream;





/*---------------------------------------------------------------------------------\
|                                                                                  |
|       class structure                                                            |
|                                                                                  |
|                                                                                  |
|        l_type                            describes a datatype                    |
|          |                                                                       |
|          +----value                      a value is data type + a number         |
|          |                                                                       |
|          +----variable                   a variable is a data type + a name      |
|                                                                                  |
|                                                                                  |
|        node (protected node_infra)       interface for a parse tree node         |
|          |                                                                       |
|          +----function                                                           |
|          |                                                                       |
|          +----assignment                                                         |
|                                                                                  |
|                                                                                  |
|        parser (private node_infra)       the main parser                         |
|                                                                                  |
|                                                                                  |
|                                                                                  |
\_________________________________________________________________________________*/

class l_type
{
public:
  static const int32_t TYPE_INT8    = 0;
  static const int32_t TYPE_UINT8   = 1;
  static const int32_t TYPE_INT16   = 2;
  static const int32_t TYPE_UINT16  = 3;
  static const int32_t TYPE_INT32   = 4;
  static const int32_t TYPE_UINT32  = 5;
  static const int32_t TYPE_INT64   = 6;
  static const int32_t TYPE_UINT64  = 7;
  static const int32_t TYPE_BOOL    = 8;
  static const int32_t TYPE_STRING  = 9;
  static const int32_t TYPE_DERIVED = 10;

private:
  int32_t type_id;        // set to one of the type IDs defined in this class
  int32_t derived_index;  // if the type_id is TYPE_DERIVED, this points to class structure

protected:
};


class value : protected l_type
{
public:

private:
  int32_t value;

protected:
};


class variable : protected l_type
{
public:
private:
  std::string name;
protected:
};


class name_space : private l_type
{
public:

  bool IsVariable(string str)
  {
    return true;
  }

  int32_t ResovleType(string str)
  {
    if (str == "int8_t")   return TYPE_INT8;
    if (str == "uint8_t")  return TYPE_UINT8;
    if (str == "int16_t")  return TYPE_INT16;
    if (str == "uint16_t") return TYPE_UINT16;
    if (str == "int32_t")  return TYPE_INT32;
    if (str == "uint32_t") return TYPE_UINT32;
    if (str == "int64_t")  return TYPE_INT64;
    if (str == "uint64_t") return TYPE_UINT64;
    if (str == "bool")     return TYPE_BOOL;
    if (str == "string")   return TYPE_STRING;
    return -1;
  }

private:

protected:
};


class node_infra
{
public:
  typedef unique_ptr<class node> u_node_ptr;

  static const int32_t FUNCTION_NODE           = 0;
  static const int32_t ASSIGNMENT_NODE         = 1;
  static const int32_t PLUS_NODE               = 2;
  static const int32_t MINUS_NODE              = 3;
  static const int32_t MULTIPLY_NODE           = 4;
  static const int32_t DIVIDE_NODE             = 5;
  static const int32_t GRATER_THAN_NODE        = 6;
  static const int32_t LESS_THAN_NODE          = 7;
  static const int32_t GRATER_THAN_EQUAL_NODE  = 8;
  static const int32_t LESS_THAN_EQUAL_NODE    = 9;
  static const int32_t EQUAL_NODE              = 10;
  static const int32_t NOT_EQUAL_NODE          = 11;
  static const int32_t LOGICAL_OR_NODE         = 12;
  static const int32_t LOGICAL_AND_NODE        = 13;
  static const int32_t BITWISE_OR_NODE         = 14;
  static const int32_t BITWISE_AND_NODE        = 15;
  static const int32_t BITWISE_XOR_NODE        = 16;

private:
protected:
};



class node : protected node_infra
{
public:
  virtual value* Evaluate( void ) = 0;

  node(int32_t nt) { node_type = nt; }

private:
  int32_t node_type;

protected:
};


class function_node : public node
{
public:
  void SetName(string str);

  function_node():node(FUNCTION_NODE) {} // call the the base constructor with the node type

private:
  std::string name;
  std::list<variable> argument_list;
  l_type return_type;

protected:
};


class assignment_node : public node
{
public:
  void SetName(string str);

  assignment_node():node(ASSIGNMENT_NODE) {} // call the the base constructor with the node type

  value* Evaluate( void )
  {
    return new value();
  }

private:
  variable variable_to_set;

protected:
};



class parser : private node_infra
{
public:
  parser(lex &x) { my_lex = &x; }

  u_node_ptr ParseStatement()
  {
    lex::lx_token_t r;
    name_space ns();

    tk_list.push_back(my_lex->GetToken());
    tk_list.push_back(my_lex->GetToken());

    // check if this is a type declaration
    //if (ResolveType(tk_list.front()) >= 0) return ParseTypeDeclaration();

    do {
      r = my_lex->GetToken();
      cout << r << std::endl;
    } while(r.second);

    return std::make_unique<assignment_node>();
  }

private:
  lex *my_lex;
  deque<lex::lx_token_t> tk_list;

  u_node_ptr ParseTypeDeclaration()
  {
    
    return std::make_unique<assignment_node>();
  }

protected:
};



/*------------------------------------------\
|                                           |
| MM   MM    A    III  N    N               |
| M M M M   A A    I   NN   N               |
| M  M  M  A   A   I   N N  N               |
| M     M  AAAAA   I   N  N N               |
| M     M  A   A   I   N   NN               |
| M     M  A   A  III  N    N               |
|                                           |
\__________________________________________*/
int main(int argc, char **argv)
{
  lex lx;
  string str;
  lex::lx_token_t r;

  parser pr(lx);
  lx.OpenFile("test.cf");

#if 1
  while(!lx.GetNextLine(str)) {
    cout << str << std::endl;
  }

  do {
    r = lx.GetToken();
    cout << r << std::endl;
  } while(r.second);
#endif

  pr.ParseStatement();
  return 0;
}




