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

// Trivial memory debugging example, so one could do meaningful
// memory debug without diving into your system internals.

// It is simple but could be  expanded as desired to absolutely
// unlimited level.
//
// 1. Collects trivial debug information about c++ memory
//    allocations.
//
// 2. Provides a way to introduce memory allocation failures
//
#include <mem_debug.h>

#include <cassert>
#include <cstring>
#include <iostream>

class TestObj {
 public:
  TestObj() : buf_() {}

  uint8_t const* GetBuf() const { return buf_; }
  uint8_t* GetBuf() { return buf_; }

 private:
  uint8_t buf_[1024];
};

int main(int, char**) {
  // Allocate a few memory blocks
  auto to0 = new TestObj;
  auto to1 = new (std::nothrow) TestObj;
  auto to2 = new TestObj[16];
  auto to3 = new (std::nothrow) TestObj[4];

  delete[] to2;
  to2 = nullptr;

  simple::MemDebug::Counters counters = simple::MemDebug::GetCounters();

  assert(counters.alloc_size_ == 6 * 1024);
  assert(counters.new_count_ == 4);
  assert(counters.delete_count_ == 1);

  simple::MemDebug::ArmArrNothrowFailure(200, 2);

  auto ca0 = new (std::nothrow) char[200];
  assert(ca0 != nullptr);

  auto ca1 = new (std::nothrow) char[200];
  assert(ca1 != nullptr);

  // This one should fail
  auto ca2 = new (std::nothrow) char[200];
  assert(ca2 == nullptr);

  auto ca3 = new (std::nothrow) char[200];
  assert(ca3 != nullptr);

  // Delete everything to avoid unused
  // varaliables warining
  delete[] ca3;
  delete[] ca2;
  delete[] ca1;
  delete[] ca0;
  delete[] to3;
  delete[] to2;
  delete to1;
  delete to0;

  return 0;
}
