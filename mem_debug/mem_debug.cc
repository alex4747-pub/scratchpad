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
#include <cstdlib>
#include <cstring>
#include <new>

// Memory allocation statistics
static size_t mem_alloc_size = 0;
static size_t mem_new_count = 0;
static size_t mem_delete_count = 0;

// Memory allocation failures

// Size of memory allocation to fail
// set to non-zero value to introduce
// failure
static size_t fail_new_size = 0;

// If we do not want to fail on the very
// first allocation set non-zero
static size_t fail_new_skip = 0;

// Same for new-nothrow
static size_t fail_nothrow_size = 0;
static size_t fail_nothrow_skip = 0;

// Same for array
static size_t fail_arr_new_size = 0;
static size_t fail_arr_new_skip = 0;

// Same for arry-nothorow
static size_t fail_arr_nothrow_size = 0;
static size_t fail_arr_nothrow_skip = 0;

void* operator new(std::size_t size) {
  if (fail_new_size) {
    if (fail_new_skip == 0) {
      fail_new_size = 0;  // Do it once
      throw std::bad_alloc();
    }

    fail_new_skip--;
  }

  void* p = malloc(size + sizeof(size_t));
  if (!p) throw std::bad_alloc();
  mem_alloc_size += size;
  mem_new_count++;
  memcpy(p, &size, sizeof(size_t));
  return reinterpret_cast<void*>(reinterpret_cast<char*>(p) + sizeof(size_t));
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
  if (fail_nothrow_size) {
    if (fail_nothrow_skip == 0) {
      fail_nothrow_size = 0;  // Do it once
      return nullptr;
    }

    fail_nothrow_skip--;
  }

  void* p = malloc(size + sizeof(size_t));
  if (!p) {
    return nullptr;
  }
  mem_alloc_size += size;
  mem_new_count++;
  memcpy(p, &size, sizeof(size_t));
  return reinterpret_cast<void*>(reinterpret_cast<char*>(p) + sizeof(size_t));
}

void* operator new[](std::size_t size) {
  if (fail_arr_new_size) {
    if (fail_arr_new_skip == 0) {
      fail_arr_new_size = 0;  // Do it once
      throw std::bad_alloc();
    }

    fail_arr_new_skip--;
  }
  void* p = malloc(size + sizeof(size_t));
  if (!p) throw std::bad_alloc();
  mem_alloc_size += size;
  mem_new_count++;
  memcpy(p, &size, sizeof(size_t));
  return reinterpret_cast<void*>(reinterpret_cast<char*>(p) + sizeof(size_t));
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
  if (fail_arr_nothrow_size) {
    if (fail_arr_nothrow_skip == 0) {
      fail_arr_nothrow_size = 0;  // Do it once
      return nullptr;
    }

    fail_arr_nothrow_skip--;
  }
  void* p = malloc(size + sizeof(size_t));
  if (!p) {
    return nullptr;
  }
  mem_alloc_size += size;
  mem_new_count++;
  memcpy(p, &size, sizeof(size_t));
  return reinterpret_cast<void*>(reinterpret_cast<char*>(p) + sizeof(size_t));
}

void operator delete(void* ptr) noexcept {
  if (ptr == nullptr) {
    return;
  }

  void* p =
      reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) - sizeof(size_t));
  size_t size = 0;
  memcpy(&size, p, sizeof(size_t));

  mem_delete_count++;
  if (size < mem_alloc_size) {
    mem_alloc_size -= size;
  } else {
    mem_alloc_size = 0;
  }

  free(p);
}

void operator delete[](void* ptr) noexcept {
  if (ptr == nullptr) {
    return;
  }
  void* p =
      reinterpret_cast<void*>(reinterpret_cast<char*>(ptr) - sizeof(size_t));
  size_t size = 0;
  memcpy(&size, p, sizeof(size_t));

  mem_delete_count++;
  if (size < mem_alloc_size) {
    mem_alloc_size -= size;
  } else {
    mem_alloc_size = 0;
  }

  free(p);
}

// Control interface
namespace simple {

MemDebug::Counters MemDebug::GetCounters() noexcept {
  return Counters(mem_alloc_size, mem_new_count, mem_delete_count);
}

void MemDebug::ArmNewFailure(size_t size, size_t skip) noexcept {
  fail_new_size = size;
  fail_new_skip = skip;
}

void MemDebug::ArmNothrowFailure(size_t size, size_t skip) noexcept {
  fail_nothrow_size = size;
  fail_nothrow_skip = skip;
}

void MemDebug::ArmArrNewFailure(size_t size, size_t skip) noexcept {
  fail_arr_new_size = size;
  fail_arr_new_skip = skip;
}

void MemDebug::ArmArrNothrowFailure(size_t size, size_t skip) noexcept {
  fail_arr_nothrow_size = size;
  fail_arr_nothrow_skip = skip;
}

}  // namespace simple
