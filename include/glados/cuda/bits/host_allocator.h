/*
 * This file is part of the GLADOS library.
 *
 * Copyright (C) 2016 Helmholtz-Zentrum Dresden-Rossendorf
 *
 * GLADOS is free software: You can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLADOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with GLADOS. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Date: 14 July 2016
 * Authors: Jan Stephan <j.stephan@hzdr.de>
 */

#ifndef GLADOS_CUDA_BITS_HOST_ALLOCATOR_H_
#define GLADOS_CUDA_BITS_HOST_ALLOCATOR_H_

#include <algorithm>
#include <cstddef>

#ifndef __CUDACC__
#include <cuda_runtime.h>
#endif

#include <glados/bits/memory_layout.h>
#include <glados/bits/memory_location.h>
#include <glados/cuda/exception.h>
#include <glados/cuda/bits/throw_error.h>
#include <glados/cuda/bits/unique_ptr.h>

namespace glados
{
    namespace cuda
    {
        template <class T, memory_layout ml>
        class host_allocator {};

        template <class T>
        class host_allocator<T, memory_layout::pointer_1D>
        {
            public:
                static constexpr auto mem_layout = memory_layout::pointer_1D;
                static constexpr auto mem_location = memory_location::host;
                static constexpr auto alloc_needs_pitch = false;

                using value_type = T;
                using pointer = value_type*;
                using const_pointer = const pointer;
                using size_type = std::size_t;
                using difference_type = std::ptrdiff_t;
                using propagate_on_container_copy_assignment = std::true_type;
                using propagate_on_container_move_assignment = std::true_type;
                using propagate_on_container_swap = std::true_type;
                using is_always_equal = std::true_type;

                template <class Deleter>
                using smart_pointer = unique_ptr<T, Deleter, alloc_needs_pitch, mem_location, true>;

                template <class U>
                struct rebind
                {
                    using other = host_allocator<U, mem_layout>;
                };

                host_allocator() noexcept = default;
                host_allocator(const host_allocator& other) noexcept = default;

                template <class U, memory_layout uml>
                host_allocator(const host_allocator<U, uml>&) noexcept
                {
                    static_assert(std::is_same<T, U>::value && mem_layout == uml, "Attempting to copy incompatible host allocator");
                }

                ~host_allocator() = default;

                auto allocate(size_type n) -> pointer
                {
                    auto p = static_cast<pointer>(nullptr);
                    auto err = cudaMallocHost(reinterpret_cast<void**>(&p), n * sizeof(value_type));
                    if(err != cudaSuccess)
                        detail::throw_error(err);
                    return pointer{p};
                }

                auto deallocate(pointer p, size_type = 0) noexcept -> void
                {
                    auto err = cudaFreeHost(reinterpret_cast<void*>(p));
                    if(err != cudaSuccess)
                        std::exit(err);
                }
        };

        template <class T>
        class host_allocator<T, memory_layout::pointer_2D>
        {
            public:
                static constexpr auto mem_layout = memory_layout::pointer_2D;
                static constexpr auto mem_location = memory_location::host;
                static constexpr auto alloc_needs_pitch = false;

                using value_type = T;
                using pointer = value_type*;
                using const_pointer = const pointer;
                using size_type = std::size_t;
                using difference_type = std::ptrdiff_t;
                using propagate_on_container_copy_assignment = std::true_type;
                using propagate_on_container_move_assignment = std::true_type;
                using propagate_on_container_swap = std::true_type;
                using is_always_equal = std::true_type;

                template <class Deleter>
                using smart_pointer = unique_ptr<T, Deleter, alloc_needs_pitch, mem_location, true>;

                template <class U>
                struct rebind
                {
                    using other = host_allocator<U, mem_layout>;
                };

                host_allocator() noexcept = default;
                host_allocator(const host_allocator& other) noexcept = default;

                template <class U, memory_layout uml>
                host_allocator(const host_allocator<U, uml>&) noexcept
                {
                    static_assert(std::is_same<T, U>::value && mem_layout == uml, "Attempting to copy incompatible host allocator");
                }

                ~host_allocator() = default;

                auto allocate(size_type x, size_type y) -> pointer
                {
                    auto p = static_cast<pointer>(nullptr);
                    auto err = cudaMallocHost(reinterpret_cast<void**>(&p), x * y * sizeof(value_type));
                    if(err != cudaSuccess)
                        detail::throw_error(err);
                    return pointer{p};
                }

                auto deallocate(pointer p, size_type = 0, size_type = 0) noexcept -> void
                {
                    auto err = cudaFreeHost(reinterpret_cast<void*>(p));
                    if(err != cudaSuccess)
                        std::exit(err);
                }
        };

        template <class T>
        class host_allocator<T, memory_layout::pointer_3D>
        {
            public:
                static constexpr auto mem_layout = memory_layout::pointer_3D;
                static constexpr auto mem_location = memory_location::host;
                static constexpr auto alloc_needs_pitch = false;

                using value_type = T;
                using pointer = value_type*;
                using const_pointer = const pointer;
                using size_type = std::size_t;
                using difference_type = std::ptrdiff_t;
                using propagate_on_container_copy_assignment = std::true_type;
                using propagate_on_container_move_assignment = std::true_type;
                using propagate_on_container_swap = std::true_type;
                using is_always_equal = std::true_type;

                template <class Deleter>
                using smart_pointer = unique_ptr<T, Deleter, alloc_needs_pitch, mem_location, true>;

                template <class U>
                struct rebind
                {
                    using other = host_allocator<U, mem_layout>;
                };

                host_allocator() noexcept = default;
                host_allocator(const host_allocator& other) noexcept = default;

                template <class U, memory_layout uml>
                host_allocator(const host_allocator<U, uml>&) noexcept
                {
                    static_assert(std::is_same<T, U>::value && mem_layout == uml, "Attempting to copy incompatible device allocator");
                }

                ~host_allocator() = default;

                auto allocate(size_type x, size_type y, size_type z) -> pointer
                {
                    auto p = static_cast<pointer>(nullptr);
                    auto err = cudaMallocHost(reinterpret_cast<void**>(&p), x * y * z * sizeof(value_type));
                    if(err != cudaSuccess)
                        detail::throw_error(err);
                    return pointer{p};
                }

                auto deallocate(pointer p, size_type = 0, size_type = 0, size_type = 0) noexcept -> void
                {
                    auto err = cudaFreeHost(reinterpret_cast<void*>(p));
                    if(err != cudaSuccess)
                        std::exit(err);
                }
        };

        template <class T1, memory_layout ml1, class T2, memory_layout ml2>
        auto operator==(const host_allocator<T1, ml1>&, const host_allocator<T2, ml2>&) noexcept -> bool
        {
            return true;
        }

        template <class T1, memory_layout ml1, class T2, memory_layout ml2>
        auto operator!=(const host_allocator<T1, ml1>&, const host_allocator<T2, ml2>&) noexcept -> bool
        {
            return false;
        }
    }
}

#endif /* GLADOS_CUDA_BITS_HOST_ALLOCATOR_H_ */
