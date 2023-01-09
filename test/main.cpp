#include <array>

#include <eco/smem/smem.hpp>
#include <fmt/format.h>

auto main() -> int {
  eco::smem::shared_memory<eco::smem::access_type::read_write, bool, "my_value"> testing;
  fmt::print("value is: {}\n", testing.read());
  testing.write(true);
  fmt::print("value is: {}\n", testing.read());
  return 0;
}
