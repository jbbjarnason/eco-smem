#include <eco/extstl/fixed_string.hpp>
#include "constexpr_testing.hpp"

namespace eco::extstl {

  consteval bool verify_basic_string() {
    testing::test_result tr;
    using namespace std::string_view_literals;
    {
      basic_fixed_string s{"12"};
      tr |= testing::consteval_test(s.view() == "12"sv);
    }
    {
      auto s{fs<wchar_t>("123")};
      tr |= testing::consteval_test(s.view() == L"123"sv);
    }
    {
      auto s{basic_fixed_string{"12"} + basic_fixed_string{"34"} + fs<char>("aBc")};
      tr |= testing::consteval_test(s.view() == "1234aBc"sv);
    }
    {
      using uchar = unsigned char;
      auto s{fs<uchar>("12")};
      constexpr uchar expected[]{uchar('1'), uchar('2'), uchar('\0')};
      tr |= testing::consteval_test(s.view() == std::basic_string_view(&expected[0]));
    }
    return static_cast<bool>(tr);
  }

  static_assert(verify_basic_string());

}

