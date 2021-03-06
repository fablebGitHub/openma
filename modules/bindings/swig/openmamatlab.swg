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
  
%include "openma.swg"
  
%fragment("OpenMA", "header", fragment="OpenMA_ToList")
%{
  template<typename T>
  mxArray* to_double_matrix(const std::vector<T>* in)
  {
    mxArray* out = mxCreateDoubleMatrix(in->empty() ? 0 : 1, in->size(), mxREAL);
    if (out != nullptr)
    {
      double* data = mxGetPr(out);
      for (size_t i = 0, len = in->size() ; i < len ; ++i)
        data[i] = in->operator[](i);
    }
    return out;
  };
%};

%fragment("OpenMA_ToList", "header")
%{
  template <typename T>
  mxArray* to_list(const std::vector<T*>* in, swig_type_info* type)
  {
    mxArray* out = mxCreateCellMatrix(in->empty() ? 0 : 1, in->size());
    if (out != nullptr)
    {
      for (size_t i = 0, len = in->size() ; i < len ; ++i)
      {
        mxArray* elt = SWIG_NewPointerObj(SWIG_as_voidptr(in->operator[](i)), type, 1 |  0 );
        if (elt == nullptr) break;
        _ma_refcount_incr(in->operator[](i));
        mxSetCell(out, i, elt);
      }
    }
    return out;
  };
%}