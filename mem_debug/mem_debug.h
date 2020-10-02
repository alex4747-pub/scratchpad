// Copyright (C) 2020  Aleksey Romanov (aleksey at voltanet dot io)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

// NOLINT(build/header_guard)
//
#ifndef INCLUDE_MEM_DEBUG_H_
#define INCLUDE_MEM_DEBUG_H_

#include <cstddef>

namespace simple {
class MemDebug {
 public:
  // Example counter structure:
  // allocation size,
  // number of calls to new
  // number of calls to delete

  struct Counters {
    Counters(size_t alloc_size, size_t new_count, size_t delete_count) noexcept
        : alloc_size_(alloc_size),
          new_count_(new_count),
          delete_count_(delete_count) {}

    size_t alloc_size_;
    size_t new_count_;
    size_t delete_count_;
  };

  static Counters GetCounters() noexcept;

  // Arm failures provide size and skip count, size zero
  // means no failure armed
  static void ArmNewFailure(size_t size, size_t skip) noexcept;
  static void ArmNothrowFailure(size_t size, size_t skip) noexcept;
  static void ArmArrNewFailure(size_t size, size_t skip) noexcept;
  static void ArmArrNothrowFailure(size_t size, size_t skip) noexcept;
};
}  // namespace simple

#endif  // INCLUDE_MEM_DDEBUG_H_
