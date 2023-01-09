#ifndef SMEM_SMEM_HPP
#define SMEM_SMEM_HPP

#include <eco/extstl/fixed_string.hpp>

#include <concepts>
#include <cstdint>
#include <expected>
#include <fmt/format.h>
#include <functional>
#include <memory>
#include <type_traits>

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

namespace eco::smem {

enum struct access_type : std::uint8_t {
  read = 0,         ///< read only access
  read_write,       ///< both read and write access
  read_write_share, ///< read and write access allowing shared ownership of mem
};

namespace detail {
namespace bi = boost::interprocess;

[[nodiscard]] inline std::expected<bi::shared_memory_object, bi::error_code_t>
create_boost_smem(const char *name, access_type a_t) noexcept {
  try {
    switch (a_t) {
      using enum access_type;
    case read:
      return bi::shared_memory_object{bi::open_only, name,
                                      bi::mode_t::read_only};
    case read_write: // todo permissions
      return bi::shared_memory_object{bi::create_only, name,
                                      bi::mode_t::read_write};
    case read_write_share:
      return bi::shared_memory_object{bi::open_or_create, name,
                                      bi::mode_t::read_write};
    }
  } catch (bi::interprocess_exception const &ex) {
    return std::unexpected(ex.get_error_code());
  }
  return std::unexpected(bi::error_code_t::other_error);
}

} // namespace detail

namespace concepts {

template <typename structure_t>
concept serializable_structure = requires {
  std::is_trivial_v<structure_t>;
  // todo Check if fixed in size
};
} // namespace concepts

template <access_type a_t, concepts::serializable_structure structure_t,
          extstl::basic_fixed_string name>
class shared_memory {
public:
  // todo filewatcher if access_type::read and shared memory nonexistent
  shared_memory()
      : data_{}, data_placebo_{}, smem_{detail::create_boost_smem(name.data(),
                                                                  a_t)} {
    if constexpr (a_t == access_type::read_write) {
      if (!smem_.has_value() &&
          smem_.error() ==
              boost::interprocess::error_code_t::already_exists_error) {
        boost::interprocess::shared_memory_object::remove(name.data());
        smem_ = detail::create_boost_smem(name.data(), a_t);
      }
      if (is_open())
        smem_.value().truncate(sizeof(structure_t));
    }
    if (is_open()) {
      namespace bi = boost::interprocess;
      bi::mode_t mode{bi::read_write};
      if constexpr (a_t == access_type::read) {
        mode = bi::read_only;
      }
      // Map the shared memory segment into this process's address space
      region_ = {smem_.value(), mode};

      if (region_.get_size() != sizeof(structure_t))
        throw std::runtime_error{fmt::format(
            "Shared memory size: {} inconsistent with struct size: {}",
            region_.get_size(), sizeof(structure_t))};

      data_ = std::unique_ptr<typename decltype(data_)::element_type,
                              typename decltype(data_)::deleter_type>(
          static_cast<structure_t *>(region_.get_address()));
      if (data_ == nullptr)
        throw std::runtime_error{"Shared memory mapping returned null pointer"};
    }
  }
  ~shared_memory() noexcept {
    if constexpr (a_t == access_type::read_write) {
      namespace bi = boost::interprocess;
      if (is_open())
        bi::shared_memory_object::remove(name.data());
    }
    // todo what to do for read_write_shared? remove or not? reference count
    // memory?
  }

  /// \brief check if shared memory is open meaning okay
  [[nodiscard]] constexpr bool is_open() const noexcept {
    return smem_.has_value();
  }

  [[nodiscard]] constexpr structure_t const &read() const noexcept {
    if (data_)
      return *data_;
    return data_placebo_;
  }

  constexpr boost::interprocess::error_code_t
  write(structure_t const &value) noexcept { // todo also accept rvalue
    if constexpr (a_t == access_type::read) {
      []<bool flag = false>() {
        static_assert(flag, "Cannot write with only read privileges");
      }
      ();
    }
    if (!smem_.has_value()) {
      return smem_.error();
    }
    if (data_ == nullptr) {
      return boost::interprocess::error_code_t::io_error;
    }
    *data_ = value;
    return boost::interprocess::error_code_t::no_error;
  }

private:
  struct do_nothing {
    template <class T> void operator()(T *) {} // do nothing
  };
  template <class T> using non_owning_ptr = std::unique_ptr<T, do_nothing>;

  non_owning_ptr<structure_t> data_{};
  structure_t data_placebo_{};
  std::expected<boost::interprocess::shared_memory_object,
                boost::interprocess::error_code_t>
      smem_{};
  boost::interprocess::mapped_region region_{};
};

} // namespace eco::smem

#endif // SMEM_SMEM_HPP
