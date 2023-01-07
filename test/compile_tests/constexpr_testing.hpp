#ifndef SMEM_CONSTEXPR_TESTING_HPP
#define SMEM_CONSTEXPR_TESTING_HPP

#include <cstdint>
#include <concepts>
#include <type_traits>

namespace eco::testing {

  struct test_result {
    enum struct value_type : std::uint8_t {
      succeed, failed
    };
    using
    enum value_type;
    value_type value_{};

    explicit constexpr operator bool() const noexcept {
      return value_ == succeed;
    }

    constexpr test_result() noexcept = default;

    explicit constexpr test_result(bool value) noexcept: value_{value ? succeed : failed} {}

    explicit constexpr test_result(value_type value) noexcept: value_{value} {}

    constexpr test_result &operator|=(test_result rh) noexcept {
      value_ = value_ == failed || rh.value_ == failed ? failed : succeed;
      return *this;
    }

    constexpr test_result operator|(test_result rh) noexcept {
      return test_result{value_ == failed || rh.value_ == failed ? failed : succeed};
    }
  };

  template<typename T, typename... U>
  concept is_any_of = (std::same_as<T, U> || ...);

  template<typename test_result_type>
  concept compatible_test_result = is_any_of<test_result_type, bool, test_result>;

  template<compatible_test_result test_result_type>
  consteval test_result consteval_test(test_result_type expression) {
    if (!expression) {
      throw;
    }
    return test_result{expression};
  }

}

#endif // SMEM_CONSTEXPR_TESTING_HPP
