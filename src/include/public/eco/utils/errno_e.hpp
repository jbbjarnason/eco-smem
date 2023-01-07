#ifndef SMEM_ERRNO_E_HPP
#define SMEM_ERRNO_E_HPP

namespace eco::utils {

  enum class errno_e {
    success,
    operation_not_permitted,
    no_such_file_or_directory,
    no_such_process,
    interrupted_system_call,
    input_output_error,
    no_such_device_or_address,
    arg_list_too_long,
    exec_format_error,
    bad_file_descriptor,
    no_child_processes,
    resource_temporarily_unavailable,
    cannot_allocate_memory,
    permission_denied,
    bad_address,
    block_device_required,
    device_or_resource_busy,
    file_exists,
    invalid_cross_device_link,
    no_such_device,
    not_a_directory,
    is_a_directory,
    invalid_argument,
    too_many_open_files_in_system,
    too_many_open_files,
    inappropriate_ioctl_for_device,
    text_file_busy,
    file_too_large,
    no_space_left_on_device,
    illegal_seek,
    read_only_file_system,
    too_many_links,
    broken_pipe,
    numerical_argument_out_of_domain,
    numerical_result_out_of_range,
    resource_deadlock_avoided,
    filename_too_long,
    no_lock_available,
    function_not_implemented,
    directory_not_empty,
    too_many_levels_of_symbolic_links,
    unknown_error = -1
  };

}
#endif // SMEM_ERRNO_E_HPP
