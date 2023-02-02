#include <eco/smem/smem.hpp>
#include <eco/extstl/fixed_string.hpp>
#include <fmt/format.h>

#include <string_view>

using std::string_view_literals::operator""sv;

auto main() -> int {
  eco::smem::shared_memory<eco::smem::access_type::read_write, bool, "my_value"> testing_writer;
  fmt::print("writer value is: {}\n", testing_writer.read());
  testing_writer.write(true);
  fmt::print("writer value is: {}\n", testing_writer.read());

  eco::smem::shared_memory<eco::smem::access_type::read, bool, "my_value"> testing_reader;
  fmt::print("reader value is: {}\n", testing_reader.read());
  testing_writer.write(false);
  fmt::print("reader value is: {}\n", testing_reader.read());

  eco::smem::shared_memory<eco::smem::access_type::read_write,
                           eco::extstl::basic_fixed_string<char, 1024>,
                           "my_value">
      testing_writer_string;
  testing_writer_string.write("foo");

  eco::smem::shared_memory<eco::smem::access_type::read,
                           eco::extstl::basic_fixed_string<char, 1024>,
                           "my_value">
      testing_reader_string;

  return 0;
}
