#pragma once

#include "list.hh"

#include <fmt/format.h>

#include <string_view>

namespace Cougar::Utils {

class QualifiedName {
public:
  QualifiedName(std::string_view name) : mName(name) {}

  void appendScope(std::string_view s) { mScopes.emplace_back(s); }

  std::string_view name() const { return mName; }

  ListView<std::string_view> scopes() const { return mScopes; }

private:
  std::string_view mName;
  List<std::string_view> mScopes;
};

} // namespace Cougar::Utils

template <> struct fmt::formatter<Cougar::Utils::QualifiedName> {
  template <typename ParseContext> constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Cougar::Utils::QualifiedName &v, FormatContext &ctx) {
    auto out = format_to(ctx.out(), "{}", fmt::join(v.scopes(), "::"));
    if (!v.scopes().empty())
      out = format_to(out, "::");
    return format_to(out, v.name());
  }
};
