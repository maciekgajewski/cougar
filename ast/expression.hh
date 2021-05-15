#pragma once

#include "node.hh"

#include <variant>

namespace Cougar::Meta {
class TypeInfo;
}

namespace Cougar::Ast {

class Expression;

struct ExStringLiteral {
  std::string_view content;
};

struct ExNumberLiteral {
  std::string_view content;
};

struct ExImplicitCast {
  Meta::TypeInfo *targetType = nullptr;
  Expression *source = nullptr;
};

struct ExConstant {
  Meta::TypeInfo *targetType = nullptr;
  Expression *source = nullptr;

  double value; // TODO use something more complex here
};

class Expression : public NodeOnToken {
public:
  template <typename D>
  Expression(const D &d, const Lexer::Token *tok = nullptr)
      : NodeOnToken(tok), mData(d) {}

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

  void setType(Meta::TypeInfo *t) {
    assert(t);
    assert(!mType);
    mType = t;
  }

  Meta::TypeInfo *type() const { return mType; }

private:
  void doDump(int indent = 0) const override;

  std::variant<ExStringLiteral, ExNumberLiteral, ExImplicitCast, ExConstant>
      mData;
  Meta::TypeInfo *mType = nullptr;
};

// pack of parameters passes to actual function call
class ParamPack : public Node {
public:
  void addParam(Expression *expr) { mParams.emplace_back(expr); }

  Utils::List<Expression *> &params() { return mParams; }
  const Utils::List<Expression *> &params() const { return mParams; }

private:
  void doDump(int indent = 0) const override;
  Utils::List<Expression *> mParams;
};

} // namespace Cougar::Ast