#include <eco/smem/smem.hpp>

#include <utility>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/// @brief C Documentation for reference
/// @param S_IRUSR: read permission for the owner of the file
/// @param S_IWUSR: write permission for the owner of the file
/// @param S_IXUSR: execute permission for the owner of the file
/// @param S_IRGRP: read permission for the group owner of the file
/// @param S_IWGRP: write permission for the group owner of the file
/// @param S_IXGRP: execute permission for the group owner of the file
/// @param S_IROTH: read permission for others (everyone else)
/// @param S_IWOTH: write permission for others (everyone else)
/// @param S_IXOTH: execute permission for others (everyone else)
///
/// @param O_RDONLY: Open the file for reading only.
/// @param O_WRONLY: Open the file for writing only.
/// @param O_RDWR: Open the file for reading and writing.
/// @param O_APPEND: Append to the file (write operations append data to the end
/// of the file).
/// @param O_CREAT: Create the file if it does not exist.
/// @param O_EXCL: When used with O_CREAT, fail if the file already exists.
/// @param O_TRUNC: Truncate the file to zero length if it exists and is opened
/// for writing.
/// @param O_CLOEXEC: Set the close-on-exec flag for the file descriptor.
/// @param O_SYNC: Write operations on the file will block until the data has
/// been physically written to the underlying hardware.
/// @param O_DSYNC: Write operations on the file will block until the data has
/// been physically written to the underlying hardware, and metadata changes
/// (e.g. timestamps) will also be flushed.
/// @param O_NONBLOCK: Open the file in non-blocking mode.

namespace eco::smem {

  namespace detail {
    [[nodiscard]] constexpr file_descriptor smem_open(std::string_view name,
                                                      access_type a_t) noexcept {
      int flags{};
      mode_t mode{};
      switch (a_t) {
        using
        enum access_type;
        case read: {
          flags = O_RDONLY;
          mode = S_IRUSR | S_IRGRP | S_IROTH;
          break;
        }
        case write: {
          flags = O_WRONLY | O_CREAT | O_EXCL;
          mode = S_IWUSR | S_IWGRP | S_IWOTH;
          break;
        }
        case read_write: {
          flags = O_RDWR | O_CREAT | O_EXCL;
          mode = S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH;
          break;
        }
        case write_share: {
          flags = O_WRONLY | O_CREAT;
          mode = S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH;
          break;
        }
        case read_write_share: {
          flags = O_RDWR | O_CREAT;
          mode = S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH;
          break;
        }
      }
      // Create shared mem file at '/dev/shm/<SHM-NAME>'
      int shm_fd = shm_open(name.data(), flags, mode);
      if (shm_fd == -1) {
        return std::unexpected(static_cast<utils::errno_e>(errno));
      }
      return shm_fd;
    }

  } // namespace detail

} // namespace eco::smem
