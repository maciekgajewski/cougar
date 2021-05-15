#include "code_generator.hh"

#include "ast/expression.hh"

#include "meta/type_info.hh"

#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace Cougar::LlvmCodeGenerator {

llvm::Value *CodeGenerator::generateExpression(Ast::Expression *e) {
  return e->visit([this](auto &data) { return generateExpression(data); });
}

llvm::Value *CodeGenerator::generateExpression(Ast::ExStringLiteral &strLit) {

  llvm::ArrayType *arrayType = llvm::ArrayType::get(
      llvm::Type::getInt8Ty(*mContext), strLit.content.size() + 1);

  llvm::GlobalVariable *strConst = new llvm::GlobalVariable(
      *mModule,                                        // module
      arrayType,                                       // type
      true,                                            // is const
      llvm::GlobalValue::LinkageTypes::PrivateLinkage, // linkage
      nullptr,                                         // initializer
      llvm::Twine("str_const")                         // name
  );

  llvm::Constant *c =
      llvm::ConstantDataArray::getString(*mContext, strLit.content);
  strConst->setInitializer(c);
  strConst->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);

  // generate "GEP" (getelementpopinter) instruction
  return mBuilder->CreateConstGEP2_64(arrayType, strConst, 0, 0);
}

llvm::Value *CodeGenerator::generateExpression(Ast::ExNumberLiteral &) {
  throw std::logic_error("Unable to generate code for number literal");
}

llvm::Value *CodeGenerator::generateExpression(Ast::ExImplicitCast &) {
  throw std::logic_error("Unable to generate code for implicit cast");
}

llvm::Value *CodeGenerator::generateExpression(Ast::ExConstant &c) {
  assert(c.targetType);

  llvm::Type *type = toLlvm(c.targetType);

  if (type->isIntegerTy()) {
    return llvm::ConstantInt::get(type, std::uint64_t(c.value),
                                  type->isSingleValueType());
  } else if (type->isFloatingPointTy()) {
    return llvm::ConstantFP::get(*mContext, llvm::APFloat(c.value));
  } else {
    throw std::logic_error(
        fmt::format("Unable to generate code for constant of type '{}'",
                    c.targetType->prettyName()));
  }
}

} // namespace Cougar::LlvmCodeGenerator