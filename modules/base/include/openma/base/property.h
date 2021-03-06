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

#ifndef __openma_base_property_h
#define __openma_base_property_h

#include "openma/base/any.h"
#include "openma/base/logger.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include <type_traits>

/**
 * Add a private StaticProperties structure and the methods staticProperty() and setStaticProperty().
 * The classes that can use this macro may need to derive of the class ma::Node (or any inheriting class) but its private implementation must be a based one.
 * @sa Property
 * @relates ma::Node
 * @ingroup openma_base
 */
#define OPENMA_DECLARE_STATIC_PROPERTIES_BASE(class, ...) \
  private: \
  __OPENMA_STATIC_PROPERTIES(__VA_ARGS__); \
  public: \
  virtual bool staticProperty(const char* key, ma::Any* value) const _OPENMA_NOEXCEPT \
  { \
    return class##Private::StaticProperties::visit(this->pint(),key,value); \
  }; \
  virtual bool setStaticProperty(const char* key, const ma::Any* value) _OPENMA_NOEXCEPT \
  { \
    return class##Private::StaticProperties::visit(this->pint(),key,value); \
  }; \
  private:

/**
 * Add a private StaticProperties structure and the methods staticProperty() and setStaticProperty().
 * Compared to the macro OPENMA_DECLARE_STATIC_PROPERTIES_BASE(), this one is for derived private implementation.
 * @sa Property
 * @relates ma::Node
 * @ingroup openma_base
 */
#define OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(derivedclass,baseclass, ...) \
  static_assert(!std::is_same<baseclass,derivedclass>::value,"The base class cannot be the same than the current class."); \
  private: \
  __OPENMA_STATIC_PROPERTIES(__VA_ARGS__); \
  public: \
  virtual bool staticProperty(const char* key, ma::Any* value) const _OPENMA_NOEXCEPT override \
  { \
    return (this->baseclass##Private::staticProperty(key,value) ? true : derivedclass##Private::StaticProperties::visit(this->pint(),key,value)); \
  }; \
  virtual bool setStaticProperty(const char* key, const ma::Any* value) _OPENMA_NOEXCEPT override \
  { \
    return (this->baseclass##Private::setStaticProperty(key,value) ? true : derivedclass##Private::StaticProperties::visit(this->pint(),key,value)); \
  }; \
  private:

#define __OPENMA_STATIC_PROPERTIES(...) \
  struct StaticProperties : ma::__details::_Properties<StaticProperties> \
  { \
    static inline auto make_properties() -> std::add_lvalue_reference<decltype(std::make_tuple(__VA_ARGS__))>::type \
    { \
      static auto props = std::make_tuple(__VA_ARGS__); \
      return props; \
    }; \
  };

namespace ma
{ 
  template <typename T, typename U, U (T::*A)() const, void (T::*M)(U) = nullptr>
  struct Property
  {
    using ValueType = typename std::decay<U>::type;
    
    const char* Label;
    size_t Size;

    template <size_t N>
    _OPENMA_CONSTEXPR Property(const char(&l)[N])
    : Label(l), Size(N-1)
    {};
    
    U get(const T* obj) const _OPENMA_NOEXCEPT
    {
      return (obj->*A)();
    }
    
    void set(T* obj, U value) const _OPENMA_NOEXCEPT
    {
      if (M == nullptr)
        error("No mutator method defined for the static property '%s'.",this->Label);
      else
        (obj->*M)(value);
    }
  };
  
  /**
   * @struct Property openma/base/property.h
   * @brief Define a static propery used in ma::Node class or inheriting classes.
   * @tparam T Class receiving the static property
   * @tparam U Exact type returned/received by the accessor/mutator given to the constructor.
   *
   * A Property object define a static property. Compared to a dynamic one, a static property
   * is linked with a accessor and optionaly a mutator declared in the same class than the property.
   *
   * Because a Node object can store properties with any string as key, a user
   * could decide to call a property with a key corresponding to an existing member (for example, Node's name).
   * What should be the behaviour of this property? Instead of having a possible ambiguity between
   * properties and other members known at compile time (or even data duplication), it was decided to implement a mechanism 
   * of static and dynamic properties transparent to the user. The static properties would
   * reflect some members known at compile time, while dynamic properties are created during run time.
   *
   * @note the type @a U must be exactly the same than the type returned/required by the accessor/mutator with all the qualifiers. For example, if the accessor is const std::string& name() const, and the mutator is void setName(const std::string& name), then @a U as to be set to const std::string& (i.e. const reference to a std::string).
   *
   * A static property is defined only by its constructor. All the other methods are used only internaly.
   * Moreover, this constructor has to be used in the macro OPENMA_DECLARE_STATIC_PROPERTIES_BASE() or OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED() which itself must be included in the declaration of the private implementation part.
   * The difference between the macros OPENMA_DECLARE_STATIC_PROPERTIES_BASE() and OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED() is about the inheriting of a class or not. Code using OpenMA as a third party library should not have access to the private implementation (i.e. *Private classes). So a base private implementation must be firstly done. The first macro must be used in this case.
   * For example:
   * 
   * @code{.unparsed}
   * class TestNodePrivate;
   * 
   * class TestNode : public ma::Node
   * {
   *   OPENMA_DECLARE_PIMPL_ACCESSOR(TestNode) // For the private implementation (pimpl)
   *   
   * public:
   *   TestNode(const std::string& name, Node* parent = nullptr);
   *   
   *   int version() const;
   *   void setVersion(int value);
   *
   * protected:
   *   TestNode(TestNodePrivate& pimpl, const std::string& name, Node* parent);
   *
   * private:
   *   std::unique_ptr<TestNodePrivate> mp_Pimpl; // This is the way to store the private implementation
   * };
   * 
   * class TestNodePrivate
   * {
   *   OPENMA_DECLARE_PINT_ACCESSOR(TestNode) // To have access to the public interface (pint) from the private implementation
   *   
   *   // Declare property(ies) for a base class
   *   OPENMA_DECLARE_STATIC_PROPERTIES_BASE(TestNode
   *     ma::Property<TestNode,int,&TestNode::version,&TestNode::setVersion>("version")
   *   )
   *   
   * public:
   *   TestNodePrivate(TestNode* pint, int version) : Version(version), mp_Pint(pint) {};
   *   int Version;
   *
   * protected:
   *   TestNode* mp_Pint; // Storage of the public interface.
   * };
   *
   * class TestNode2Private;
   * 
   * class TestNode2 : public TestNode
   * {
   *   OPENMA_DECLARE_PIMPL_ACCESSOR(TestNode2) // For the private implementation (pimpl)
   *   
   * public:
   *   TestNode2(const std::string& name, int version, Node* parent = nullptr);
   * };
   * 
   * class TestNode2Private
   * {
   *   OPENMA_DECLARE_PINT_ACCESSOR(TestNode2) // To have access to the public interface (pint) from the private implementation
   *   
   *   // Declare property(ies) for a derived class
   *   OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(TestNode2, TestNode
   *     ma::Property<TestNode,int,&TestNode::version>("version2")
   *   )
   *   
   * public:
   *   TestNode2Private(TestNode* pint, int version) : TestNodePrivate(pint, version) {};
   * };
   *
   * @endcode
   *
   * In this example, The class TestNode will have a new property name "version" which is associated with the accessor TestNode::version and the mutator TestNode::setVersion. Internally, these methods rely on the member TestNodePrivate::Version.
   * If the member is read-only the pointer to the mutation must be set to nullptr (e.g. in the definition of the propoerty version2 for the class TestNode2).
   *
   * @ingroup openma_base
   */
  
  /**
   * @var Property::ValueType
   * Type of the value associated with the property
   */
  /**
   * @var Property::Label
   * Array of characters corresponding to the name of the property.
   */
  /**
   * @var Property::Size
   * Size of the array.
   */
    
  /**
   * @fn template <typename T, typename U> template <std::size_t N, typename A = std::nullptr_t, typename M = std::nullptr_t> constexpr Property<T,U>::Property(const char(&l)[N], A&& a = nullptr, M&& m = nullptr)
   * Constructor.
   */ 
  
  /**
   * @fn U template <typename T, typename U> Property<T,U>::get(const T* obj) const _OPENMA_NOEXCEPT
   * Returns a value from the given object @a obj using the accessor set in the 3rd template argument <c>(U (T::*A)() const)</c>.
   */
  
  /**
   * @fn void template <typename T, typename U> Property<T,U>::set(T* obj, U value) const _OPENMA_NOEXCEPT
   * Sets the given @a value to the given object @a obj using the mutator set in the 4rd template argument <c>void (T::*M)(U)</c>.
   */
   
  namespace __details
  {   
    template <typename Derived>
    struct _Properties
    {
      template <typename Object, typename Value>
      static inline bool visit(Object* obj, const char* key, Value* val)
      {
        const auto& properties = Derived::make_properties();
        return search<0,std::tuple_size<typename std::decay<decltype(properties)>::type>::value-1>(properties,obj,key,val);
      };
  
    private:
      
      template <typename Prop, typename Object, typename Value>
      static inline bool accept(Prop&& prop, Object* obj, const char* key, Value* val)
      {
        if (strncmp(key,prop.Label,prop.Size) == 0)
        {
          val->assign(prop.get(obj));
          return true;
        }
        return false;
      }
      
      template <typename Prop, typename Object, typename Value>
      static inline bool accept(Prop&& prop, Object* obj, const char* key, const Value* val)
      {
        if (strncmp(key,prop.Label,prop.Size) == 0)
        {
          using value_t = typename std::remove_const<typename std::remove_reference<Prop>::type>::type::ValueType;
          prop.set(obj,val->template cast<value_t>());
          return true;
        }
        return false;
      }
      
      template <size_t Index, size_t Size, typename Props, typename Object, typename Value>
      static inline typename std::enable_if<Index != Size, bool>::type search(Props&& properties, Object* obj, const char* key, Value* val)
      {
        if (accept(std::get<Index>(properties),obj,key,val))
          return true;
        return search<Index+1,Size>(std::forward<Props>(properties),obj,key,val);
      };
      
      template <size_t Index, size_t Size, typename Props, typename Object, typename Value>
      static inline typename std::enable_if<Index == Size, bool>::type search(Props&& properties, Object* obj, const char* key, Value* val)
      {
        return accept(std::get<Size>(properties),obj,key,val);
      };
    };

    /**
     * @struct Properties openma/base/property.h
     * @brief Template interface used to manage static properties defined for a class. 
     * @tparam Derived Name of the derived class to create a CRTP (Curiously recurring template pattern).
     *
     * The goal of this interface is to visit each static properties defined for an object and 
     * see if the given key exist. If it is the case, then the associated given value is accessed 
     * or modified depending the context. All this part is transparent for the user which will use
     * the method Node::property() and Node::setProperty().
     *
     * @note This class should not be used directly but instead the macro OPENMA_DECLARE_STATIC_PROPERTIES().
     */
  
    /**
     * @fn template <typename Derived> template <typename Object, typename Value> static inline bool _Properties<Derived>::visit(Object* obj, const char* key, Value* val)
     * Visit known static properties defined in @a Derived and if one of them corresponds to the @a key, then the assocated accessor and/or mutator is applied on the callable object @a obj combined with the value @a val.
     */
  
    /**
     * @fn template <typename Derived> template <typename Prop, typename Object, typename Value> static inline bool _Properties<Derived>::accept(Prop&& prop, Object* obj, const char* key, Value* val)
     * Check if the given @a label correspond to the name of the property. 
     * If it is the case, then the content of the property is modified by using the 
     * content of the given @a value. Internally, the associated mutator is used with the
     * callable object @a obj. The content of the @a value object is casted to the type 
     * requested by the mutator.
     */

    /**
     * @fn template <typename Derived> template <typename Prop, typename Object, typename Value> static inline bool _Properties<Derived>::accept(Prop&& prop, Object* obj, const char* key, const Value* val)
     * Check if the given @a label correspond to the name of the property. 
     * If it is the case, then the content of the property is copied to the given @a value.
     * Internally, the associated accessor is used with the callable object @a obj.
     */
  };
};

#endif // __openma_base_property_h