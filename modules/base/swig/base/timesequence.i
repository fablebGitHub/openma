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

namespace ma
{
  SWIG_TYPEMAP_OUT_CONSTRUCTOR(ma, TimeSequence)
  
  %nodefaultctor;
  class TimeSequence : public Node
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma, TimeSequence, SWIGTYPE)
    SWIG_EXTEND_DEEPCOPY(ma, TimeSequence)
  
    typedef enum : int {
      Unknown,
      Reconstructed,
      Marker,
      Angle,
      Force,
      Moment,
      Power,
      Scalar,
      Pose,
      Analog,
      Other
    } Type;
    static const std::array<double,2> InfinityRange;
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    ~TimeSequence();
    double sampleRate() const;
    void setSampleRate(double value);
    const std::vector<unsigned>& dimensions() const;
    unsigned samples() const;
    unsigned components() const;
    size_t elements() const;
    double duration() const;
    int type() const;
    void setType(int value);
    const std::string& unit() const;
    void setUnit(const std::string& value);
    double startTime() const;
    void setStartTime(double value);
    double scale() const;
    void setScale(double value);
    double offset() const;
    void setOffset(double value);
    const std::array<double,2>& range() const;
    void setRange(const std::array<double,2>& value);
    /*
    const double* data() const;
    double* data();
    template <typename... Is> double data(unsigned sample, Is... indices) const;
    template <typename... Is> double& data(unsigned sample, Is... indices);
    */
    void resize(unsigned samples);
  };
  %clearnodefaultctor;
};