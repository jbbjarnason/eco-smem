#ifndef SMEM_SMEM_HPP
#define SMEM_SMEM_HPP

#include <eco/extstl/fixed_string.hpp>
#include <eco/utils/errno_e.hpp>

#include <cstdint>
#include <expected>
#include <filesystem>

namespace eco::smem {

  enum struct access_type : std::uint8_t {
    read = 0,         ///< read only access
    write,            ///< write only access
    read_write,       ///< both read and write access
    write_share,      ///< write only access allowing shared ownership of memory
    read_write_share, ///< read and write access allowing shared ownership of
    /// memory
  };

  namespace detail {
    using file_descriptor = std::expected<int, utils::errno_e>;

    [[nodiscard]] constexpr file_descriptor smem_open(std::string_view,
                                                      access_type) noexcept;

  } // namespace detail

  namespace concepts {
    template<typename structure_t>
    concept serializable_structure = requires {
      structure_t::serialize;
      structure_t::size;
    };
  } // namespace concepts

// TODO use std::filesystem::path_view proposal
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2022/p1030r5.pdf Need to
// or write a test to validate the path is correctly formatted at compile time
// or write a concept which verifies it hmm
  template<access_type a_t, concepts::serializable_structure structure_t,
          extstl::basic_fixed_string name,
          extstl::basic_fixed_string path = "/var/eco/smem/">
  class shared_memory {
  public:
    constexpr shared_memory() = default;

  private:
    structure_t data;
  };

/// @brief reader
  template<concepts::serializable_structure structure_t,
          extstl::basic_fixed_string name>
  class shared_memory<access_type::read, structure_t, name> {
  public:
    enum struct read_error_code {
      nonexisting = 0,
      permission,
      invalid_size,
      name_too_long,
      too_many_files_open,
      unknown
    };

    [[nodiscard]] constexpr std::expected<structure_t const &, read_error_code>
    read() const noexcept {
      return {};
    }
  };

} // namespace eco::smem

#endif // SMEM_SMEM_HPP
