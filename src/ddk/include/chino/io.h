// MIT License
//
// Copyright (c) 2020 SunnyCase
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once
#include "object.h"
#include <chino/threading.h>
#include <gsl/gsl-lite.hpp>

namespace chino::io
{
#ifdef _MSC_VER
#define EXPORT_DRIVER __declspec(allocate(".CHINO_DRV$C")) static const ::chino::io::driver
#else
#error "Unsupported compiler"
#endif

struct driver;

enum class device_type
{
    misc = 0,
};

class device_property
{
public:
    constexpr device_property(const void *data, int len) noexcept
        : data_(data), len_(len) {}

    size_t len() const noexcept { return len_; }
    uint32_t uint32(size_t index) const noexcept;
    std::string_view string(size_t index) const noexcept;

private:
    const void *data_;
    int len_;
};

class device_descriptor
{
public:
    constexpr device_descriptor(void *fdt, int node) noexcept
        : fdt_(fdt), node_(node) {}

    result<device_property, error_code> property(std::string_view name) const noexcept;

private:
    void *fdt_;
    int node_;
};

struct device_id
{
    std::string_view compatible;
    void *data;
};

typedef result<void, error_code> (*driver_add_device_t)(driver &drv, const device_descriptor &device_desc);

struct driver_operations
{
    driver_add_device_t add_device;
};

struct driver
{
    std::string_view name;
    driver_operations operations;
    gsl::span<const device_id> match_table;
};

struct device : ob::object
{
    const driver *drv;
    device_type type;
    threading::sched_spinlock syncroot;
};

struct file : ob::object
{
    device *dev;
    fpos_t offset;
    threading::sched_spinlock syncroot;
};
}
