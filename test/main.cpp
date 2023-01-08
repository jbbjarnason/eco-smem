#include <array>

#include <eco/smem/smem.hpp>

struct smem_bool {
  static constexpr std::size_t size{ 1 };
  static constexpr void serialize(smem_bool const& the_struct, std::array<std::byte, size> & container) {
    container.at(0) = std::bit_cast<std::byte>(the_struct.value);
  }
  bool value;
};

auto main() -> int {
  eco::smem::shared_memory<eco::smem::access_type::read_write, smem_bool, "my_value"> testing;

  return 0;
}
