//
// Created by zhou on 24-2-21.
//

#ifndef SHARED_MEMORY_SHM_MANAGER_H
#define SHARED_MEMORY_SHM_MANAGER_H
#include <cstdint>
#include <cstddef>
#include <cstring>

namespace zzq {

    class shm_manager
    {
    public:
        shm_manager()=delete;
        ~shm_manager()=delete;
        static int init();
        static int deinit();
        static int register_producer(
            const char *name, uint32_t width, uint32_t height, uint32_t pix_bytes
        );
        static int register_consumer(
            const char *name, uint32_t width, uint32_t height, uint32_t pix_bytes
        );
        static std::size_t produce_data(
            const char *name, const void *buf_ptr, std::size_t buf_len
        );
        static std::size_t consume_data(
            const char *name, void *buf_ptr, std::size_t buf_len
        );
        static std::size_t consume_latest_data(
            const char *name, void *buf_ptr, std::size_t buf_len
        );
        static std::size_t set_common_data(
            const char *name, const char *sub_name, const void *buf_ptr, std::size_t buf_len
        );
        static std::size_t get_common_data(
            const char *name, const char *sub_name, void *buf_ptr, std::size_t buf_len
        );
    };

} // zzq

#endif //SHARED_MEMORY_SHM_MANAGER_H
