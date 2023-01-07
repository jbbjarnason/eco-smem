#include "constexpr_testing.hpp"
#include <eco/smem/smem.hpp>
#include <utility>
#include <fcntl.h>

namespace eco::smem {

  static_assert(std::to_underlying(access_type::read) == O_RDONLY);
  static_assert(std::to_underlying(access_type::write) == O_WRONLY);
  static_assert(std::to_underlying(access_type::read_write) == O_RDWR);

} // namespace eco::smem