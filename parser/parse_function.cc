#include "parser.hh"

#include "ast/function.hh"
#include "ast/type.hh"

namespace Cougar::Parser {

using namespace Ast;
using namespace Utils;

using Lexer::Token;
using Lexer::TokenType;

// TODO: move to own file
Ast::TypeNode *Parser::parseType(TokenIterator &it) {
  if (it->type != TokenType::Identifier) {
    mDiag.error(it->location, "Expected type");
    return nullptr;
  }

  TypeNode *typeNode = Zone::make<TypeName>(it->content, &*it);
  ++it;

  // maybe pointer
  while (it->type == TokenType::Asterix) {
    typeNode = Zone::make<PointerTo>(typeNode, &*it);
    ++it;
  }

  return typeNode;
}

Ast::FunctionDeclaration *Parser::parseFunction(TokenIterator &it,
                                                Ast::Access access) {

  // return type
  Ast::TypeNode *returnType = parseType(it);
  if (!returnType) {
    mDiag.error(it->location, "Expected function return type");
    return nullptr;
  }

  // function name
  if (it->type != TokenType::Identifier) {
    mDiag.error(it->location, "Expected function name");
    return nullptr;
  }

  FunctionDeclaration *fun =
      Zone::make<FunctionDeclaration>(access, it->content, returnType, &*it);
  ++it;

  // args
  if (it->type != TokenType::ParentOpen) {
    mDiag.error(it->location, "Expected '(' here");
    return nullptr;
  }
  ++it;

  while (true) {

    // arg type
    if (it->type != TokenType::Identifier) {
      mDiag.error(it->location, "Expected argument type");
      return nullptr;
    }

    TypeNode *argType = parseType(it);
    if (!argType) {
      mDiag.error(it->location, "Expected arg type");
      return nullptr;
    }

    // maybe arg name
    std::string_view argName;
    if (it->type == TokenType::Identifier) {
      argName = it->content;
      ++it;
    }

    fun->addArg(argType, argName);

    // comma or closing parent
    if (it->type == TokenType::Comma) {
      ++it;
      continue;
    }

    if (it->type == TokenType::ParentClose) {
      ++it;
      break;
    }

    mDiag.error(it->location, "Expected ',' or ')' here");
  }

  // not - see if this is function defintion or declaration
  // TODO
  if (it->type != TokenType::Semicolon) {
    mDiag.error(it->location, "Function defintions not impelemented yet :(");
    return nullptr;
  }
  ++it;
  return fun;
}

} // namespace Cougar::Parser