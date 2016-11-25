/*
 * This file is part of the ddrf library.
 *
 * Copyright (C) 2016 Helmholtz-Zentrum Dresden-Rossendorf
 *
 * ddrf is free software: You can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ddrf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with ddrf. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Date: 06 September 2016
 * Authors: Jan Stephan <j.stephan@hzdr.de>
 */

#include <algorithm>
#include <cstdlib>

#define BOOST_TEST_MODULE CudaPoolAllocator
#include <boost/test/unit_test.hpp>

#include <ddrf/cuda/algorithm.h>
#include <ddrf/cuda/memory.h>
#include <ddrf/cuda/sync_policy.h>
#include <ddrf/memory.h>

BOOST_AUTO_TEST_CASE(cuda_pool_alloc_1d)
{
    constexpr auto szx = 4096;
    constexpr auto dim = szx;

    using device_allocator_type = ddrf::cuda::device_allocator<int, ddrf::memory_layout::pointer_1D>;
    using pool_allocator_type = ddrf::pool_allocator<int, ddrf::memory_layout::pointer_1D, device_allocator_type>;
    auto alloc = pool_allocator_type{};

    auto host_src = ddrf::cuda::make_unique_pinned_host<int>(szx);
    auto host_dst = ddrf::cuda::make_unique_pinned_host<int>(szx);
    auto dev = alloc.allocate_smart(szx);

    auto hs = host_src.get();
    auto hd = host_dst.get();

    std::generate(hs, hs + dim, std::rand);
    std::fill(hd, hd + dim, 0);
    ddrf::cuda::fill(ddrf::cuda::sync, dev, 0, szx);

    ddrf::cuda::copy(ddrf::cuda::sync, dev, host_src, szx);
    ddrf::cuda::copy(ddrf::cuda::sync, host_dst, dev, szx);

    BOOST_CHECK(std::equal(hs, hs + dim, hd));
}

BOOST_AUTO_TEST_CASE(cuda_pool_alloc_2d)
{
    constexpr auto szx = 64;
    constexpr auto szy = 64;
    constexpr auto dim = szx * szy;

    using device_allocator_type = ddrf::cuda::device_allocator<int, ddrf::memory_layout::pointer_2D>;
    using pool_allocator_type = ddrf::pool_allocator<int, ddrf::memory_layout::pointer_2D, device_allocator_type>;
    auto alloc = pool_allocator_type{};

    auto host_src = ddrf::cuda::make_unique_pinned_host<int>(szx, szy);
    auto host_dst = ddrf::cuda::make_unique_pinned_host<int>(szx, szy);
    auto dev = alloc.allocate_smart(szx, szy);
    ddrf::cuda::fill(ddrf::cuda::sync, dev, 0, szx, szy);

    auto hs = host_src.get();
    auto hd = host_dst.get();

    std::generate(hs, hs + dim, std::rand);
    std::fill(hd, hd + dim, 0);

    ddrf::cuda::copy(ddrf::cuda::sync, dev, host_src, szx, szy);
    ddrf::cuda::copy(ddrf::cuda::sync, host_dst, dev, szx, szy);

    BOOST_CHECK(std::equal(hs, hs + dim, hd));
}

BOOST_AUTO_TEST_CASE(cuda_pool_alloc_3d)
{
    constexpr auto szx = 8;
    constexpr auto szy = 8;
    constexpr auto szz = 8;
    constexpr auto dim = szx * szy * szz;

    using device_allocator_type = ddrf::cuda::device_allocator<int, ddrf::memory_layout::pointer_3D>;
    using pool_allocator_type = ddrf::pool_allocator<int, ddrf::memory_layout::pointer_3D, device_allocator_type>;
    auto alloc = pool_allocator_type{};

    auto host_src = ddrf::cuda::make_unique_pinned_host<int>(szx, szy, szz);
    auto host_dst = ddrf::cuda::make_unique_pinned_host<int>(szx, szy, szz);
    auto dev = alloc.allocate_smart(szx, szy, szz);

    auto hs = host_src.get();
    auto hd = host_dst.get();

    std::generate(hs, hs + dim, std::rand);
    std::fill(hd, hd + dim, 0);
    ddrf::cuda::fill(ddrf::cuda::sync, dev, 0, szx, szy, szz);

    ddrf::cuda::copy(ddrf::cuda::sync, dev, host_src, szx, szy, szz);
    ddrf::cuda::copy(ddrf::cuda::sync, host_dst, dev, szx, szy, szz);

    BOOST_CHECK(std::equal(hs, hs + dim, hd));
}
