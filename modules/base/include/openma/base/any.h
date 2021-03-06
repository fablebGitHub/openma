/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __openma_base_any_h
#define __openma_base_any_h

#include "openma/base_export.h"
#include "openma/base/typeid.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <type_traits>
#include <vector>
#include <initializer_list>
#include <cstddef> // size_t

namespace ma
{
  class OPENMA_BASE_EXPORT Any
  {
  public:
    
    // Forward declaration
    struct Storage;
    template <typename S, typename R> struct Helper;
    template <typename... T> struct Conversion;
    template <typename Type, typename From, typename To> struct Register;
    template <typename Type, typename... From, typename... To> struct Register<Type, Conversion<From...>, Conversion<To...>>;
    template <typename Type> struct Unregister;
    
    Any() _OPENMA_NOEXCEPT;
    Any(const Any& other);
    Any(Any&& other) _OPENMA_NOEXCEPT;
    template <typename U, typename D = void*, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(U&& value, D&& dimensions = D{});
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(std::initializer_list<U> values, std::initializer_list<unsigned> dimensions = {});
    ~Any();
    
    std::vector<unsigned> dimensions() const _OPENMA_NOEXCEPT;
    size_t size() const _OPENMA_NOEXCEPT;
    
    bool isValid() const _OPENMA_NOEXCEPT;
    bool isEmpty() const _OPENMA_NOEXCEPT;
    bool isArithmetic() const _OPENMA_NOEXCEPT;
    bool isString() const _OPENMA_NOEXCEPT;
    typeid_t type() const _OPENMA_NOEXCEPT;
    
    void swap(Any& other) _OPENMA_NOEXCEPT;
    
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> bool isEqual(U&& value) const _OPENMA_NOEXCEPT;
    template <typename U, typename D = void*, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> void assign(U&& value, D&& dimensions = D{}) _OPENMA_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> void assign(std::initializer_list<U> values, std::initializer_list<unsigned> dimensions = {}) _OPENMA_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast() const _OPENMA_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast(size_t idx) const _OPENMA_NOEXCEPT;
    
    template<class U> operator U() const _OPENMA_NOEXCEPT;
    
    Any& operator=(const Any& other);
    Any& operator=(Any&& other) _OPENMA_NOEXCEPT;
    
    OPENMA_BASE_EXPORT friend bool operator==(const Any& lhs, const Any& rhs) _OPENMA_NOEXCEPT;
    OPENMA_BASE_EXPORT friend bool operator!=(const Any& lhs, const Any& rhs) _OPENMA_NOEXCEPT;
        
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const A& lhs, const U& rhs) _OPENMA_NOEXCEPT {return lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const U& lhs, const A& rhs) _OPENMA_NOEXCEPT {return rhs.isEqual(lhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const A& lhs, const U& rhs) _OPENMA_NOEXCEPT {return !lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const U& lhs, const A& rhs) _OPENMA_NOEXCEPT {return !rhs.isEqual(lhs);};
    
  private:    
    Storage* mp_Storage;
  };
};

#include "openma/base/any.tpp"

#endif // __openma_base_any_h
