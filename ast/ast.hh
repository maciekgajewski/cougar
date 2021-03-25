#pragma once

#include <memory>
#include <vector>

namespace Cougar {

class Type;

namespace Ast {

enum class Access { Private, Public };

class INode {
public:
  virtual ~INode();

  virtual void dump(int indent = 0) const = 0;
};

class Module : public INode {
public:
  void addNode(std::unique_ptr<INode> node);
  void dump(int indent = 0) const override;

private:
  std::vector<std::unique_ptr<INode>> mNodes;
};

class FunctionArg : public INode {
  void dump(int indent = 0) const override;

private:
  std::string mName;
  Type *mType;
};

class FunctionDefinition : public INode {
public:
  FunctionDefinition(Access access, const Type &retType,
                     const std::string &name)
      : mAccess(access), mReturnType(retType), mName(name) {}

  void dump(int indent = 0) const override;

private:
  Access mAccess;
  const Type &mReturnType;
  std::string mName;
  std::vector<std::unique_ptr<FunctionArg>> mArgs;
  std::vector<std::unique_ptr<INode>> mBody;
};

class Expression : public INode {
  void dump(int indent = 0) const override;
};

class ReturnStatement : public INode {
  void dump(int indent = 0) const override;

private:
  std::unique_ptr<Expression> mExpression;
};

} // namespace Ast

} // namespace Cougar