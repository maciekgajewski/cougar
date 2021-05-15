#pragma once

#include "list.hh"

#include <fmt/format.h>

#include <string_view>

namespace Cougar::Utils {

class Qualification {
public:
  ListView<std::string_view> scopes() const { return mScopes; }
  void appendScope(std::string_view s) { mScopes.emplace_back(s); }
  bool empty() const { return mScopes.empty(); }

private:
  List<std::string_view> mScopes;
};

class QualifiedName {
public:
  QualifiedName() = default;
  QualifiedName(std::string_view name) : mName(name) {}
  QualifiedName(std::string_view name, Qualification q)
      : mName(name), mQualification(q) {}

  std::string_view name() const { return mName; }
  void setName(std::string_view n) { mName = n; }

  const Qualification &qualification() const { return mQualification; }

private:
  std::string_view mName;
  Qualification mQualification;
};

} // namespace Cougar::Utils

template <> struct fmt::formatter<Cougar::Utils::QualifiedName> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Utils::QualifiedName &v, FormatContext &ctx) {
    auto out =
        format_to(ctx.out(), "{}", fmt::join(v.qualification().scopes(), "::"));
    if (!v.qualification().empty())
      out = format_to(out, "::");
    return format_to(out, v.name());
  }
};
