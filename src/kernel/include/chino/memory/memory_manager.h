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
#include <chino/memory.h>

namespace chino::threading
{
class kprocess;
}

namespace chino::memory
{
struct used_page_node
{
    uint32_t start;
    uint32_t count;

    used_page_node *next;

    uint32_t end() noexcept
    {
        return start + count;
    }
};

result<void *, error_code> allocate_pages(threading::kprocess &process, uint32_t pages) noexcept;
void free_pages(threading::kprocess &process, void *base, uint32_t pages) noexcept;

result<void *, error_code> heap_alloc(threading::kprocess &process, size_t bytes) noexcept;
void heap_free(threading::kprocess &process, void *ptr) noexcept;
}
