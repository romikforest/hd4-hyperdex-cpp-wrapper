#ifndef HYPERDEX_MAP_HPP_INCLUDED
#define HYPERDEX_MAP_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: map.hpp
// First created: 2015-10-18-10.14
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#ifndef __cplusplus
  #error HyperDex c++ wrapper needs c++ compiler
#endif

#if __cplusplus <= 201103L
  #error HyperDex c++ wrapper needs at least a C++14 compliant compiler
#endif

// C++
#include <initializer_list>
#include <type_traits>

// forest
#include <forest/tools.hpp>
#include <forest/string.hpp>

// hyperdex
#include <hyperdex_cpp/common.hpp>
#include <hyperdex_cpp/base_types.hpp>
#include <hyperdex_cpp/log.hpp>

namespace hyperdex
{
  template <typename, typename> class primitive_data_traits;

  template<hyperdatatype, hyperdatatype> class map_traits;

  template<hyperdatatype> class map_iterator_traits;

  template<typename KeyType, typename DataType,
           template<typename, typename = void> typename PrimitiveTraits = primitive_data_traits>
  class map final
  {
      template<typename T> struct wrapper {};
      using     string_t = std::true_type;
      using non_string_t = std::false_type;

      class test_insert_1
      {
        public:
          test_insert_1(blob_pair const&, blob_pair const&);
      };

      class test_insert_2
      {
        public:
          test_insert_2(blob_pair const&, blob_pair const&);

          template<typename V>
          test_insert_2(blob_pair const&, V&&);
      };

      class test_insert_3
      {
        public:
          test_insert_3(blob_pair const&, blob_pair const&);

          template<typename V>
          test_insert_3(blob_pair const&, V&&);

          template<typename K>
          test_insert_3(K&&, blob_pair const&);
      };

      class key_helper_
      {

          struct key_helper_string
          {
            key_helper_string(map*, string_t, blob_pair const&);
            key_helper_string(map*, string_t, char const*);
          };

          struct key_helper_non_string
          {
            key_helper_non_string(map*, non_string_t, blob_pair const&);
          };

        public:

          key_helper_(map* obj, hyperdex_blob_data, blob_pair const& k) noexcept
          {
            if (!~hyperdex_ds_map_insert_key_string(obj->map_, k.first, k.second, &obj->status_))
              log_ds_status(obj->status_);
          }

          key_helper_(map* obj, string_t, blob_pair const& k) noexcept
          {
            if (!~hyperdex_ds_map_insert_key_string(obj->map_, k.first, k.second, &obj->status_))
              log_ds_status(obj->status_);
          }

          key_helper_(map* obj, string_t, char const* k) noexcept
          {
            if (!~hyperdex_ds_map_insert_key_string(obj->map_, k, strlen(k), &obj->status_))
              log_ds_status(obj->status_);
          }

          template<typename K,
                   typename = std::enable_if_t<!std::is_constructible<key_helper_string, map*, string_t, K&&>::value>>
          key_helper_(map* obj, string_t, K&& k) noexcept
          {
            char const* str;
            size_t size;

            if (~PrimitiveTraits<std::decay_t<K>>
                  ::serialize(obj->arenas_, obj->status_, str, size, std::forward<K>(k)))
              hyperdex_ds_map_insert_key_string(obj->map_, str, size, &obj->status_);
          }

          key_helper_(map* obj, non_string_t, blob_pair const& k) noexcept
          {
            // Size of non string serialized object (hyperdex_int or hyperdex_float) must be appropriate
            assert(k.second == sizeof(typename map_iterator_traits<type>::key_type));

            PrimitiveTraits<typename map_iterator_traits<type>::key_type>
                    ::map_insert_key(obj->arenas_, obj->status_, obj->map_,
                                     *reinterpret_cast<typename map_iterator_traits<type>
                                       ::key_type const*>(k.first));
          }

          template<typename K,
                   typename = std::enable_if_t<!std::is_constructible<key_helper_non_string,
                                                map*, non_string_t, K&&>::value>>
          key_helper_(map* obj, non_string_t, K&& k) noexcept
          {
            using CT = std::conditional_t<
                           std::is_same<std::decay_t<K>,
                                        typename map_iterator_traits<type>::key_type>::value,
                           typename map_iterator_traits<type>::key_type,
                           std::decay_t<KeyType>
                       >;
            PrimitiveTraits<CT>::map_insert_key(obj->arenas_, obj->status_, obj->map_, std::forward<K>(k));
        }

      }; // class key_helper_

      class value_helper_
      {
          struct value_helper_string
          {
            value_helper_string(map*, string_t, blob_pair const&);
            value_helper_string(map*, string_t, char const*);
          };

          struct value_helper_non_string
          {
            value_helper_non_string(map*, non_string_t, blob_pair const&);
          };

        public:

          value_helper_(map* obj, hyperdex_blob_data, blob_pair const& v) noexcept
          {
            if (!~hyperdex_ds_map_insert_val_string(obj->map_, v.first, v.second, &obj->status_))
              log_ds_status(obj->status_);
          }

          value_helper_(map* obj, string_t, blob_pair const& v) noexcept
          {
            if (!~hyperdex_ds_map_insert_val_string(obj->map_, v.first, v.second, &obj->status_))
              log_ds_status(obj->status_);
          }

          value_helper_(map* obj, string_t, char const* v) noexcept
          {
            if (!~hyperdex_ds_map_insert_val_string(obj->map_, v, strlen(v), &obj->status_))
              log_ds_status(obj->status_);
          }

          template<typename V,
                   typename = std::enable_if_t<!std::is_constructible<value_helper_string, map*, string_t, V&&>::value>>
          value_helper_(map* obj, string_t, V&& v) noexcept
          {
            char const* str;
            size_t size;

            if (~PrimitiveTraits<std::decay_t<V>>
                  ::serialize(obj->arenas_, obj->status_, str, size, std::forward<V>(v)))
              hyperdex_ds_map_insert_val_string(obj->map_, str, size, &obj->status_);
          }

          value_helper_(map* obj, non_string_t, blob_pair const& v) noexcept
          {
            // Size of non string serialized object (hyperdex_int or hyperdex_float) must be appropriate
            assert(v.second == sizeof(typename map_iterator_traits<type>::value_type));

            PrimitiveTraits<typename map_iterator_traits<type>::value_type>
                    ::map_insert_val(obj->arenas_, obj->status_, obj->map_,
                                     *reinterpret_cast<typename map_iterator_traits<type>
                                       ::value_type const*>(v.first));
          }

          template<typename V,
                   typename = std::enable_if_t<!std::is_constructible<value_helper_non_string,
                                                map*, non_string_t, V&&>::value>>
          value_helper_(map* obj, non_string_t, V&& v) noexcept
          {
            using CT = std::conditional_t<
                           std::is_same<std::decay_t<V>,
                                        typename map_iterator_traits<type>::value_type>::value,
                           typename map_iterator_traits<type>::value_type,
                           std::decay_t<DataType>
                       >;
            PrimitiveTraits<CT>::map_insert_val(obj->arenas_, obj->status_, obj->map_, std::forward<V>(v));
          }

      }; // class value_helper_

      template<typename K, typename V>
      hyperdex_int_ret insert_helper_(K&& key, V&& val) noexcept
      {
        key_helper_   { this,
                        std::conditional_t<
                          std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value,
                          hyperdex_blob_data,
                          std::conditional_t<
                            PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                            (std::is_same<std::decay_t<K>,
                                          std::decay_t<blob_pair>>::value &&
                             PrimitiveTraits<std::decay_t<KeyType>>::map_key_type
                              == HYPERDATATYPE_STRING),
                            string_t,
                            non_string_t
                          >
                        > {},
                        std::forward<K>(key) };
        if (status_ != HYPERDEX_DS_SUCCESS) return -1;

        value_helper_ { this,
                        std::conditional_t<
                          std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                          hyperdex_blob_data,
                          std::conditional_t<
                            PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                              (std::is_same<std::decay_t<V>,
                                            std::decay_t<blob_pair>>::value &&
                               PrimitiveTraits<std::decay_t<DataType>>::map_val_type
                                == HYPERDATATYPE_STRING),
                            string_t,
                            non_string_t
                          >
                        > {},
                        std::forward<V>(val) };
        if (status_ != HYPERDEX_DS_SUCCESS) return -1;

        return 0;

      } // insert_helper_

      class map_pairs_definition final
      {
          friend class map;

        public:
          FOREST_NONCOPYABLE_CLASS(map_pairs_definition);
          FOREST_NONMOVABLE_CLASS(map_pairs_definition);
          FOREST_NONALLOCATABLE_CLASS(map_pairs_definition);

          map_pairs_definition(blob_pair const& key, blob_pair const& val) noexcept
          : key_type_ { HYPERDATATYPE_STRING }, val_type_ { HYPERDATATYPE_STRING },
            key_ptr_  { key.first  }, val_ptr_ { val.first  },
            key_sz_   { key.second }, val_sz_  { val.second }
          {
            static_assert(std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value ||
                          PrimitiveTraits<std::decay_t<KeyType>>::map_key_type == HYPERDATATYPE_STRING,
                          "Wrong argument type in hyperdex::map constructor. "
                          "Key argument in blob representation available only for hyperdex::map"
                          " HYPERDATATYPE_STRING underlying key type.");

            static_assert(std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value ||
                          PrimitiveTraits<std::decay_t<DataType>>::map_val_type == HYPERDATATYPE_STRING,
                          "Wrong argument type in hyperdex::map constructor. "
                          "Value argument in blob representation available only for hyperdex::map"
                          " HYPERDATATYPE_STRING underlying value type.");
          }

          template<typename ValData,
                   typename = std::enable_if_t<!std::is_constructible<
                   test_insert_1, blob_pair const&, ValData&&>::value>>
          map_pairs_definition(blob_pair const& key, ValData&& val) noexcept
          : key_type_ { HYPERDATATYPE_STRING },
            val_type_ { PrimitiveTraits<std::decay_t<ValData>>::map_val_type },
            key_ptr_  { key.first  },
            key_sz_   { key.second }
          {
            static_assert(std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value ||
                          PrimitiveTraits<std::decay_t<KeyType>>::map_key_type == HYPERDATATYPE_STRING,
                          "Wrong argument type in hyperdex::map constructor. "
                          "Key argument in blob representation available only for hyperdex::map"
                          " HYPERDATATYPE_STRING underlying key type.");

            static_assert(!std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                           "Wrong constructor type for hyperdex::map. "
                           "Value of hyperdex::map has hyperdex_blob_data type"
                           " and to constructor should be passed blob argument (string with size)"
                           " for it.");

            static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                           PrimitiveTraits<std::decay_t<ValData>>::is_string,
                           "Wrong argument in hyperdex::map constructor. "
                           "hyperdex::map value type must be a string");

            static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<ValData>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::map's one.");

            using VT = std::conditional_t<
                              PrimitiveTraits<std::decay_t<DataType>>::is_string,
                              std::decay_t<ValData>,
                              std::decay_t<DataType>
                            >;

            PrimitiveTraits<VT>::serialize(arenas_,
                                           status_,
                                           val_ptr_, val_sz_,
                                           std::forward<ValData>(val));
          }


          template<typename KeyData,
                   typename = std::enable_if_t<!std::is_constructible<
                   test_insert_2, KeyData&&, blob_pair const&>::value>>
          map_pairs_definition(KeyData&& key, blob_pair const& val) noexcept
          : key_type_ { PrimitiveTraits<std::decay_t<KeyData>>::map_key_type },
            val_type_ { HYPERDATATYPE_STRING },
            val_ptr_  { val.first  },
            val_sz_   { val.second }
          {
            static_assert(std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value ||
                          PrimitiveTraits<std::decay_t<DataType>>::map_key_type == HYPERDATATYPE_STRING,
                          "Wrong argument type in hyperdex::map constructor. "
                          "Value argument in blob representation available only for hyperdex::map"
                          " HYPERDATATYPE_STRING underlying value type.");

            static_assert(!std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value,
                           "Wrong constructor type for hyperdex::map. "
                           "Key of hyperdex::map has hyperdex_blob_data type"
                           " and to constructor should be passed blob argument (string with size)"
                           " for it.");

            static_assert(!PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                           PrimitiveTraits<std::decay_t<KeyData>>::is_string,
                           "Wrong argument in hyperdex::map constructor. "
                           "hyperdex::map key type must be a string");

            static_assert(PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                      std::is_convertible<std::decay_t<KeyData>, std::decay_t<KeyType>>::value,
                      "Type of inserting key is not convertible to type of hyperdex::map's one.");

            using KT = std::conditional_t<
                              PrimitiveTraits<std::decay_t<KeyType>>::is_string,
                              std::decay_t<KeyData>,
                              std::decay_t<KeyType>
                            >;

            PrimitiveTraits<KT>::serialize(arenas_,
                                           status_,
                                           key_ptr_, key_sz_,
                                           std::forward<KeyData>(key));
          }

          template<typename KeyData, typename ValData,
                   typename = std::enable_if_t<!std::is_constructible<
                   test_insert_3, KeyData&&, ValData&&>::value>>
          map_pairs_definition(KeyData&& key, ValData&& val) noexcept
          : key_type_ { PrimitiveTraits<std::decay_t<KeyData>>::map_key_type },
            val_type_ { PrimitiveTraits<std::decay_t<ValData>>::map_val_type }
          {
            static_assert(!std::is_same<std::decay_t<KeyType>,  hyperdex_blob_data>::value ||
                          !std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                           "Wrong constructor type for hyperdex::map. "
                           "Key or value of hyperdex::map have hyperdex_blob_data type"
                           " and to constructor should be passed blob arguments (string with size)"
                           " for them.");

            static_assert(!PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                           PrimitiveTraits<std::decay_t<KeyData>>::is_string,
                           "Inserting to hyperdex::map key type must be a string");

            static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                           PrimitiveTraits<std::decay_t<ValData>>::is_string,
                           "Inserting to hyperdex::map value type must be a string");

            static_assert(PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                      std::is_convertible<std::decay_t<KeyData>, std::decay_t<KeyType>>::value,
                      "Type of inserting key is not convertible to type of hyperdex::map");

            static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<ValData>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::map");

            using KT = std::conditional_t<
                         PrimitiveTraits<std::decay_t<KeyType>>::is_string,
                         std::decay_t<KeyData>,
                         std::decay_t<KeyType>
                       >;

            using VT = std::conditional_t<
                         PrimitiveTraits<std::decay_t<DataType>>::is_string,
                         std::decay_t<ValData>,
                         std::decay_t<DataType>
                       >;

            if (~PrimitiveTraits<KT>::serialize(arenas_,
                                                status_,
                                                key_ptr_, key_sz_,
                                                std::forward<KeyData>(key)))
            {
              PrimitiveTraits<VT>::serialize(arenas_,
                                             status_,
                                             val_ptr_, val_sz_,
                                             std::forward<ValData>(val));
            }
          }

       private:

          mutable hyperdex_ds_returncode status_ {HYPERDEX_DS_SUCCESS};
          mutable arena_vector arenas_ {};

          hyperdatatype key_type_, val_type_;
          char const* key_ptr_ = nullptr;
          char const* val_ptr_ = nullptr;
          size_t key_sz_ = 0, val_sz_ = 0;

      }; // class map_pairs_definition

    public:

      FOREST_NONCOPYABLE_CLASS(map);
      FOREST_NONMOVABLE_CLASS(map);
      FOREST_NONALLOCATABLE_CLASS(map);

      static constexpr bool const is_map = true;
      static constexpr hyperdatatype const type
        = map_traits<std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value ?
                         HYPERDATATYPE_STRING :
                         PrimitiveTraits<std::decay_t<KeyType>>::map_key_type,
                     std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value ?
                         HYPERDATATYPE_STRING :
                         PrimitiveTraits<std::decay_t<DataType>>::map_val_type>::type;


      explicit operator bool () const noexcept { return status_ == HYPERDEX_DS_SUCCESS; }

      hyperdex_ds_returncode status() const noexcept { return status_; }

      map() noexcept
      {
        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          return;
        }
        arenas_.emplace_back(arena, arena_deleter{});

        if (!(map_ = hyperdex_ds_allocate_map(arena)))
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          arenas_.pop_back();
        }
      }

      map(char const* str, size_t size) noexcept // construct from serialized string
      {
        init_str_  = str;
        init_size_ = size;
      }

      map(an_attribute const& attr) noexcept
      : map { attr.a.value, attr.a.value_sz }
      {
        assert(attr.a.datatype == type);
      }

      map(an_attribute const* attr) noexcept
      : map { attr->a.value, attr->a.value_sz }
      {
        assert(attr->a.datatype == type);
      }

      map(hyperdex_client_attribute const& attr) noexcept
      : map { attr.value, attr.value_sz }
      {
        assert(attr.datatype == type);
      }

      map(hyperdex_client_attribute const* attr) noexcept
      : map { attr->value, attr->value_sz }
      {
        assert(attr->datatype == type);
      }

      map(std::initializer_list<map_pairs_definition> args) noexcept
      : map {}
      {
        if (map_)
        {
          for (auto& arg : args)
          {
            if (arg.status_ != HYPERDEX_DS_SUCCESS ||
                !~insert_helper_(blob_pair{arg.key_ptr_, arg.key_sz_}, blob_pair{arg.val_ptr_, arg.val_sz_}))
            {
              log_ds_status(status_);
              return;
            }

          } // for (auto& arg : args)
        }
      }

      map& operator = (hyperdex_client_attribute const& attr) noexcept
      {
        assert(attr.datatype == type);
        clear_();
        init_str_  = attr.value;
        init_size_ = attr.value_sz;
        return *this;
      }

      map& operator = (hyperdex_client_attribute const* attr) noexcept
      {
        assert(attr->datatype == type);
        return operator = (*attr);
      }

      map& operator = (an_attribute const& attr) noexcept
      {
        assert(attr.a.datatype == type);
        return operator = (attr.a);
      }

      map& operator = (an_attribute const* attr) noexcept
      {
        assert(attr->a.datatype == type);
        return operator = (attr->a);
      }

      hyperdex_int_ret insert(blob_pair const& key, blob_pair const& val) noexcept
      {

        static_assert(PrimitiveTraits<std::decay_t<KeyType>>::map_key_type  == HYPERDATATYPE_STRING &&
                      PrimitiveTraits<std::decay_t<DataType>>::map_val_type == HYPERDATATYPE_STRING,
                      "Member function hyperdex_int_ret insert(blob_pair const& key, blob_pair const& val) "
                      "is available only if both hyperdex::map key and value have type HYPERDATATYPE_STRING.");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!map_ && !init_str_to_map_()) return -1;

        return insert_helper_(key, val);
      }

      template<typename V,
               typename = std::enable_if_t<!std::is_constructible<test_insert_1, blob_pair const&, V&&>::value>>
      hyperdex_int_ret insert(blob_pair const& key, V&& val) noexcept
      {

        static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                       PrimitiveTraits<std::decay_t<V>>::is_string,
                       "Inserting to hyperdex::map value type must be a string");

        static_assert(PrimitiveTraits<std::decay_t<KeyType>>::map_key_type == HYPERDATATYPE_STRING,
                      "Member function  template<typename V>"
                      " hyperdex_int_ret insert(blob_pair const& key, V&& val) "
                      "is available only if hyperdex::map key has type HYPERDATATYPE_STRING.");

        static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<V>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::map");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!map_ && !init_str_to_map_()) return -1;

        return insert_helper_(key, std::forward<V>(val));
      }

      template<typename K,
               typename = std::enable_if_t<!std::is_constructible<test_insert_2, K&&, blob_pair const&>::value>>
      hyperdex_int_ret insert(K&& key, blob_pair const& val) noexcept
      {

        static_assert(!PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                       PrimitiveTraits<std::decay_t<K>>::is_string,
                       "Inserting to hyperdex::map key type must be a string");


        static_assert(PrimitiveTraits<std::decay_t<DataType>>::map_val_type == HYPERDATATYPE_STRING,
                      "Member function  template<typename K>"
                      " hyperdex_int_ret insert(K&& key, blob_pair const& val) "
                      "is available only if hyperdex::map value has type HYPERDATATYPE_STRING.");

        static_assert(PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                      std::is_convertible<std::decay_t<K>, std::decay_t<KeyType>>::value,
                      "Type of inserting key is not convertible to type of hyperdex::map");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!map_ && !init_str_to_map_()) return -1;

        return insert_helper_(std::forward<K>(key), val);
      }

      template<typename K, typename V,
               typename = std::enable_if_t<!std::is_constructible<test_insert_3, K&&, V&&>::value>>
      hyperdex_int_ret insert(K&& key, V&& val) noexcept
      {
        static_assert(!std::is_same<std::decay_t<KeyType>,  hyperdex_blob_data>::value ||
                      !std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                      "Method template<typename K, typename V> hyperdex_int_ret insert(K&& key, V&& val) "
                      "is not available for hyperdex::map if key or value have hyperdex_blob_data type.");

        static_assert(!PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                       PrimitiveTraits<std::decay_t<K>>::is_string,
                       "Inserting to hyperdex::map key type must be a string");

        static_assert(!PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                       PrimitiveTraits<std::decay_t<V>>::is_string,
                       "Inserting to hyperdex::map value type must be a string");

        static_assert(PrimitiveTraits<std::decay_t<KeyType>>::is_string ||
                      std::is_convertible<std::decay_t<K>, std::decay_t<KeyType>>::value,
                      "Type of inserting key is not convertible to type of hyperdex::map");

        static_assert(PrimitiveTraits<std::decay_t<DataType>>::is_string ||
                      std::is_convertible<std::decay_t<V>, std::decay_t<DataType>>::value,
                      "Type of inserting value is not convertible to type of hyperdex::map");

        status_ = HYPERDEX_DS_SUCCESS;

        if (!map_ && !init_str_to_map_()) return -1;

        return insert_helper_(std::forward<K>(key), std::forward<V>(val));
      }

      hyperdex_int_ret finalize(char const*& res_str,
                                size_t& res_size)
                                const noexcept
      {
        if (map_)
        {
          hyperdatatype datatype;
          status_  = HYPERDEX_DS_SUCCESS;
          if (!~hyperdex_ds_map_finalize(map_, &status_, &res_str, &res_size, &datatype))
          {
            log_ds_status(status_);
            return -1;
          }
          return 0;
        }
        res_str  = init_str_;
        res_size = init_size_;
        status_  = HYPERDEX_DS_SUCCESS;
        return 0;
      }

      class iterator
      {
          class attribute_helper_
          {
              struct helper_
              {
                helper_(hyperdex_client_attribute&, blob_pair const&, hyperdatatype);
                helper_(hyperdex_client_attribute&, char const*, hyperdatatype);
              };

            public:

              attribute_helper_(hyperdex_client_attribute& attr, blob_pair const& arg, hyperdatatype t) noexcept
              {
                attr = hyperdex_client_attribute{ nullptr, arg.first, arg.second, t };
              }

              attribute_helper_(hyperdex_client_attribute& attr, char const* arg, hyperdatatype t) noexcept
              {
                attr = hyperdex_client_attribute{ nullptr, arg, strlen(arg), t };
              }

              template<typename T,
                       typename = std::enable_if_t<!std::is_constructible<helper_, T, hyperdatatype>::value>>
              attribute_helper_(hyperdex_client_attribute& attr, T const& arg, hyperdatatype t) noexcept
              {
                attr = hyperdex_client_attribute{ nullptr,
                                                  reinterpret_cast<char const*>(&arg),
                                                  sizeof(arg), t };
              }

          };

          template<typename T>
          static
          hyperdex_client_attribute
          attribute_f(T const& arg, hyperdatatype t) noexcept
          {
            hyperdex_client_attribute attr;
            attribute_helper_ { attr, arg, t };
            return attr;
          }

          template<class T>
          void key_iterator_helper(wrapper<T>, hyperdex_client_attribute const& arg) noexcept
          {
            using KT = std::conditional_t<
                         PrimitiveTraits<std::decay_t<KeyType>>::nondeserializable,
                         forest::string,
                         std::decay_t<KeyType>
                       >;

            key_ = PrimitiveTraits<KT>::deserialize( arg );
          }

          void key_iterator_helper(wrapper<hyperdex_blob_data>, hyperdex_client_attribute const& arg) noexcept
          {
            key_.first  = arg.value;
            key_.second = arg.value_sz;
          }

          template<class T>
          void val_iterator_helper(wrapper<T>, hyperdex_client_attribute const& arg) noexcept
          {
            using VT = std::conditional_t<
                         PrimitiveTraits<std::decay_t<DataType>>::nondeserializable,
                         forest::string,
                         std::decay_t<DataType>
                       >;

            val_ = PrimitiveTraits<VT>::deserialize( arg );
          }

          void val_iterator_helper(wrapper<hyperdex_blob_data>, hyperdex_client_attribute const& arg) noexcept
          {
            val_.first  = arg.value;
            val_.second = arg.value_sz;
          }

          void iterator_helper_() noexcept
          {
            typename map_iterator_traits<type>::key_type   key;
            typename map_iterator_traits<type>::value_type value;

            constexpr hyperdatatype const kt = PrimitiveTraits<std::decay_t<KeyType>>::type;
            constexpr hyperdatatype const vt = PrimitiveTraits<std::decay_t<DataType>>::type;

            status_ = map_iterator_traits<type>::next(iter_, key, value);
            if (status_ == 1)
            {
              key_iterator_helper(wrapper<std::decay_t<KeyType>>{}, attribute_f(key, kt));
              val_iterator_helper(wrapper<std::decay_t<DataType>>{}, attribute_f(value, vt));
            }
            else
            {
              if (!~status_) log_malformed_map();
              key_ = decltype(key_) {};
              val_ = decltype(val_) {};
            }

          }

        public:
          iterator(bool pos, char const* init_str = nullptr, size_t init_size = 0) noexcept
          : status_(pos ? 1 : 0)
          {
            if (status_ == 1)
            {
              hyperdex_ds_iterator_init(&iter_, type, init_str, init_size);
              iterator_helper_();
            }
          }

          bool operator!=(const iterator& other) const noexcept
          {
            return (!~status_ ? 0 : status_) != (!~other.status_ ? 0 : other.status_);
          }

          auto operator* () const noexcept
          {
            using KT = std::conditional_t<
                         std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value,
                         blob_pair,
                         std::conditional_t<
                           PrimitiveTraits<std::decay_t<KeyType>>::nondeserializable,
                           forest::string,
                           std::decay_t<KeyType>
                         >
                       >;

            using VT = std::conditional_t<
                         std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
                         blob_pair,
                         std::conditional_t<
                           PrimitiveTraits<std::decay_t<DataType>>::nondeserializable,
                           forest::string,
                           std::decay_t<DataType>
                         >
                       >;

            return std::pair<KT, VT> {key_, val_};
          }

          iterator const& operator++() noexcept
          {
            if (status_ == 1) iterator_helper_();

            return *this;
          }

          hyperdex_int_ret status() const noexcept { return status_; }

        private:
          hyperdex_ds_iterator  iter_ {};
          hyperdex_int_ret status_ { 1 };

          std::conditional_t<
            std::is_same<std::decay_t<KeyType>, hyperdex_blob_data>::value,
            blob_pair,
            std::conditional_t<
              PrimitiveTraits<std::decay_t<KeyType>>::nondeserializable,
              forest::string,
              std::decay_t<KeyType>
            >
          > key_ {};

          std::conditional_t<
            std::is_same<std::decay_t<DataType>, hyperdex_blob_data>::value,
            blob_pair,
            std::conditional_t<
              PrimitiveTraits<std::decay_t<DataType>>::nondeserializable,
              forest::string,
              std::decay_t<DataType>
            >
          > val_ {};

      }; // class iterator

      iterator begin() const noexcept
      {
        if (map_)
        {
          char const* str;
          size_t sz;
          if (!~finalize(str, sz)) return { false };
          return { true, str, sz };
        }
        return { true, init_str_, init_size_ };
      }

      iterator end() const noexcept
      {
        return { false };
      }

    private:

      bool init_str_to_map_() noexcept
      {
        assert(!map_);

        auto arena = hyperdex_ds_arena_create();
        if (!arena)
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          return false;
        }
        arenas_.emplace_back(arena, arena_deleter{});

        if (!(map_ = hyperdex_ds_allocate_map(arena)))
        {
          log_ds_status(status_ = HYPERDEX_DS_NOMEM);
          arenas_.pop_back();
          return false;
        }

        hyperdex_ds_iterator iter;
        hyperdex_ds_iterator_init(&iter, type, init_str_, init_size_);

        hyperdex_int_ret res;

        typename map_iterator_traits<type>::key_type   key;
        typename map_iterator_traits<type>::value_type value;

        while ((res = map_iterator_traits<type>::next(iter, key, value)) == 1)
        {
          if (!~res || !~insert_helper_(key, value))
          {
            log_malformed_map();
            map_ = nullptr;
            arenas_.pop_back();
            return false;
          }
        }

        init_str_  = nullptr;
        init_size_ = 0;

        return true;

      }

      void clear_() noexcept
      {
           arenas_ = arena_vector {};
           status_ = HYPERDEX_DS_SUCCESS;
              map_ = nullptr;
         init_str_ = nullptr;
        init_size_ = 0;
      }

      mutable arena_vector arenas_ {};
      mutable hyperdex_ds_returncode status_ { HYPERDEX_DS_SUCCESS };
      hyperdex_ds_map*         map_  { nullptr };
      mutable char const* init_str_  { nullptr };
      mutable size_t     init_size_  { 0 };

  }; // template class map

  using forest::string;

  template<template<typename, typename = void> typename>
  class basic_document;
  template <typename, typename> class c_string_traits;
  using document = basic_document<c_string_traits>;

  template<hyperdatatype T, template<typename, typename = void> typename>
  class timestamp;

  template <typename, typename> class complex_data_traits;

  using timestamp_second = timestamp<HYPERDATATYPE_TIMESTAMP_SECOND, complex_data_traits>;
  using timestamp_minute = timestamp<HYPERDATATYPE_TIMESTAMP_MINUTE, complex_data_traits>;
  using timestamp_hour   = timestamp<HYPERDATATYPE_TIMESTAMP_HOUR, complex_data_traits>;
  using timestamp_day    = timestamp<HYPERDATATYPE_TIMESTAMP_DAY, complex_data_traits>;
  using timestamp_week   = timestamp<HYPERDATATYPE_TIMESTAMP_WEEK, complex_data_traits>;
  using timestamp_month  = timestamp<HYPERDATATYPE_TIMESTAMP_MONTH, complex_data_traits>;
  using full_timestamp   = timestamp<HYPERDATATYPE_STRING, complex_data_traits>;

  using           string_string_map = map<string, string>;
  using              string_int_map = map<string, hyperdex_int>;
  using            string_float_map = map<string, hyperdex_float>;
  using           string_int128_map = map<string, hyperdex_int128>;
  using      string_long_double_map = map<string, long double>;
  using         string_document_map = map<string, document>;
  using string_timestamp_second_map = map<string, timestamp_second>;
  using string_timestamp_minute_map = map<string, timestamp_minute>;
  using   string_timestamp_hour_map = map<string, timestamp_hour>;
  using    string_timestamp_day_map = map<string, timestamp_day>;
  using   string_timestamp_week_map = map<string, timestamp_week>;
  using  string_timestamp_month_map = map<string, timestamp_month>;
  using   string_full_timestamp_map = map<string, full_timestamp>;
  using             string_blob_map = map<string, hyperdex_blob_data>;

  using           int_string_map = map<hyperdex_int, string>;
  using              int_int_map = map<hyperdex_int, hyperdex_int>;
  using            int_float_map = map<hyperdex_int, hyperdex_float>;
  using           int_int128_map = map<hyperdex_int, hyperdex_int128>;
  using      int_long_double_map = map<hyperdex_int, long double>;
  using         int_document_map = map<hyperdex_int, document>;
  using int_timestamp_second_map = map<hyperdex_int, timestamp_second>;
  using int_timestamp_minute_map = map<hyperdex_int, timestamp_minute>;
  using   int_timestamp_hour_map = map<hyperdex_int, timestamp_hour>;
  using    int_timestamp_day_map = map<hyperdex_int, timestamp_day>;
  using   int_timestamp_week_map = map<hyperdex_int, timestamp_week>;
  using  int_timestamp_month_map = map<hyperdex_int, timestamp_month>;
  using   int_full_timestamp_map = map<hyperdex_int, full_timestamp>;
  using             int_blob_map = map<hyperdex_int, hyperdex_blob_data>;

  using           float_string_map = map<hyperdex_float, string>;
  using              float_int_map = map<hyperdex_float, hyperdex_int>;
  using            float_float_map = map<hyperdex_float, hyperdex_float>;
  using           float_int128_map = map<hyperdex_float, hyperdex_int128>;
  using      float_long_double_map = map<hyperdex_float, long double>;
  using         float_document_map = map<hyperdex_float, document>;
  using float_timestamp_second_map = map<hyperdex_float, timestamp_second>;
  using float_timestamp_minute_map = map<hyperdex_float, timestamp_minute>;
  using   float_timestamp_hour_map = map<hyperdex_float, timestamp_hour>;
  using    float_timestamp_day_map = map<hyperdex_float, timestamp_day>;
  using   float_timestamp_week_map = map<hyperdex_float, timestamp_week>;
  using  float_timestamp_month_map = map<hyperdex_float, timestamp_month>;
  using   float_full_timestamp_map = map<hyperdex_float, full_timestamp>;
  using             float_blob_map = map<hyperdex_float, hyperdex_blob_data>;

  using           int128_string_map = map<hyperdex_int128, string>;
  using              int128_int_map = map<hyperdex_int128, hyperdex_int>;
  using            int128_float_map = map<hyperdex_int128, hyperdex_float>;
  using           int128_int128_map = map<hyperdex_int128, hyperdex_int128>;
  using      int128_long_double_map = map<hyperdex_int128, long double>;
  using         int128_document_map = map<hyperdex_int128, document>;
  using int128_timestamp_second_map = map<hyperdex_int128, timestamp_second>;
  using int128_timestamp_minute_map = map<hyperdex_int128, timestamp_minute>;
  using   int128_timestamp_hour_map = map<hyperdex_int128, timestamp_hour>;
  using    int128_timestamp_day_map = map<hyperdex_int128, timestamp_day>;
  using   int128_timestamp_week_map = map<hyperdex_int128, timestamp_week>;
  using  int128_timestamp_month_map = map<hyperdex_int128, timestamp_month>;
  using   int128_full_timestamp_map = map<hyperdex_int128, full_timestamp>;
  using             int128_blob_map = map<hyperdex_int128, hyperdex_blob_data>;

  using           document_string_map = map<document, string>;
  using              document_int_map = map<document, hyperdex_int>;
  using            document_float_map = map<document, hyperdex_float>;
  using           document_int128_map = map<document, hyperdex_int128>;
  using      document_long_double_map = map<document, long double>;
  using         document_document_map = map<document, document>;
  using document_timestamp_second_map = map<document, timestamp_second>;
  using document_timestamp_minute_map = map<document, timestamp_minute>;
  using   document_timestamp_hour_map = map<document, timestamp_hour>;
  using    document_timestamp_day_map = map<document, timestamp_day>;
  using   document_timestamp_week_map = map<document, timestamp_week>;
  using  document_timestamp_month_map = map<document, timestamp_month>;
  using   document_full_timestamp_map = map<document, full_timestamp>;
  using             document_blob_map = map<document, hyperdex_blob_data>;

  using           timestamp_second_string_map = map<timestamp_second, string>;
  using              timestamp_second_int_map = map<timestamp_second, hyperdex_int>;
  using            timestamp_second_float_map = map<timestamp_second, hyperdex_float>;
  using           timestamp_second_int128_map = map<timestamp_second, hyperdex_int128>;
  using      timestamp_second_long_double_map = map<timestamp_second, long double>;
  using         timestamp_second_document_map = map<timestamp_second, document>;
  using timestamp_second_timestamp_second_map = map<timestamp_second, timestamp_second>;
  using timestamp_second_timestamp_minute_map = map<timestamp_second, timestamp_minute>;
  using   timestamp_second_timestamp_hour_map = map<timestamp_second, timestamp_hour>;
  using    timestamp_second_timestamp_day_map = map<timestamp_second, timestamp_day>;
  using   timestamp_second_timestamp_week_map = map<timestamp_second, timestamp_week>;
  using  timestamp_second_timestamp_month_map = map<timestamp_second, timestamp_month>;
  using   timestamp_second_full_timestamp_map = map<timestamp_second, full_timestamp>;
  using             timestamp_second_blob_map = map<timestamp_second, hyperdex_blob_data>;

  using           timestamp_minute_string_map = map<timestamp_minute, string>;
  using              timestamp_minute_int_map = map<timestamp_minute, hyperdex_int>;
  using            timestamp_minute_float_map = map<timestamp_minute, hyperdex_float>;
  using           timestamp_minute_int128_map = map<timestamp_minute, hyperdex_int128>;
  using      timestamp_minute_long_double_map = map<timestamp_minute, long double>;
  using         timestamp_minute_document_map = map<timestamp_minute, document>;
  using timestamp_minute_timestamp_second_map = map<timestamp_minute, timestamp_second>;
  using timestamp_minute_timestamp_minute_map = map<timestamp_minute, timestamp_minute>;
  using   timestamp_minute_timestamp_hour_map = map<timestamp_minute, timestamp_hour>;
  using    timestamp_minute_timestamp_day_map = map<timestamp_minute, timestamp_day>;
  using   timestamp_minute_timestamp_week_map = map<timestamp_minute, timestamp_week>;
  using  timestamp_minute_timestamp_month_map = map<timestamp_minute, timestamp_month>;
  using   timestamp_minute_full_timestamp_map = map<timestamp_minute, full_timestamp>;
  using             timestamp_minute_blob_map = map<timestamp_minute, hyperdex_blob_data>;

  using           timestamp_hour_string_map = map<timestamp_hour, string>;
  using              timestamp_hour_int_map = map<timestamp_hour, hyperdex_int>;
  using            timestamp_hour_float_map = map<timestamp_hour, hyperdex_float>;
  using           timestamp_hour_int128_map = map<timestamp_hour, hyperdex_int128>;
  using      timestamp_hour_long_double_map = map<timestamp_hour, long double>;
  using         timestamp_hour_document_map = map<timestamp_hour, document>;
  using timestamp_hour_timestamp_second_map = map<timestamp_hour, timestamp_second>;
  using timestamp_hour_timestamp_minute_map = map<timestamp_hour, timestamp_minute>;
  using   timestamp_hour_timestamp_hour_map = map<timestamp_hour, timestamp_hour>;
  using    timestamp_hour_timestamp_day_map = map<timestamp_hour, timestamp_day>;
  using   timestamp_hour_timestamp_week_map = map<timestamp_hour, timestamp_week>;
  using  timestamp_hour_timestamp_month_map = map<timestamp_hour, timestamp_month>;
  using   timestamp_hour_full_timestamp_map = map<timestamp_hour, full_timestamp>;
  using             timestamp_hour_blob_map = map<timestamp_hour, hyperdex_blob_data>;

  using           timestamp_day_string_map = map<timestamp_day, string>;
  using              timestamp_day_int_map = map<timestamp_day, hyperdex_int>;
  using            timestamp_day_float_map = map<timestamp_day, hyperdex_float>;
  using           timestamp_day_int128_map = map<timestamp_day, hyperdex_int128>;
  using      timestamp_day_long_double_map = map<timestamp_day, long double>;
  using         timestamp_day_document_map = map<timestamp_day, document>;
  using timestamp_day_timestamp_second_map = map<timestamp_day, timestamp_second>;
  using timestamp_day_timestamp_minute_map = map<timestamp_day, timestamp_minute>;
  using   timestamp_day_timestamp_hour_map = map<timestamp_day, timestamp_hour>;
  using    timestamp_day_timestamp_day_map = map<timestamp_day, timestamp_day>;
  using   timestamp_day_timestamp_week_map = map<timestamp_day, timestamp_week>;
  using  timestamp_day_timestamp_month_map = map<timestamp_day, timestamp_month>;
  using   timestamp_day_full_timestamp_map = map<timestamp_day, full_timestamp>;
  using             timestamp_day_blob_map = map<timestamp_day, hyperdex_blob_data>;

  using           timestamp_week_string_map = map<timestamp_week, string>;
  using              timestamp_week_int_map = map<timestamp_week, hyperdex_int>;
  using            timestamp_week_float_map = map<timestamp_week, hyperdex_float>;
  using           timestamp_week_int128_map = map<timestamp_week, hyperdex_int128>;
  using      timestamp_week_long_double_map = map<timestamp_week, long double>;
  using         timestamp_week_document_map = map<timestamp_week, document>;
  using timestamp_week_timestamp_second_map = map<timestamp_week, timestamp_second>;
  using timestamp_week_timestamp_minute_map = map<timestamp_week, timestamp_minute>;
  using   timestamp_week_timestamp_hour_map = map<timestamp_week, timestamp_hour>;
  using    timestamp_week_timestamp_day_map = map<timestamp_week, timestamp_day>;
  using   timestamp_week_timestamp_week_map = map<timestamp_week, timestamp_week>;
  using  timestamp_week_timestamp_month_map = map<timestamp_week, timestamp_month>;
  using   timestamp_week_full_timestamp_map = map<timestamp_week, full_timestamp>;
  using             timestamp_week_blob_map = map<timestamp_week, hyperdex_blob_data>;

  using           timestamp_month_string_map = map<timestamp_month, string>;
  using              timestamp_month_int_map = map<timestamp_month, hyperdex_int>;
  using            timestamp_month_float_map = map<timestamp_month, hyperdex_float>;
  using           timestamp_month_int128_map = map<timestamp_month, hyperdex_int128>;
  using      timestamp_month_long_double_map = map<timestamp_month, long double>;
  using         timestamp_month_document_map = map<timestamp_month, document>;
  using timestamp_month_timestamp_second_map = map<timestamp_month, timestamp_second>;
  using timestamp_month_timestamp_minute_map = map<timestamp_month, timestamp_minute>;
  using   timestamp_month_timestamp_hour_map = map<timestamp_month, timestamp_hour>;
  using    timestamp_month_timestamp_day_map = map<timestamp_month, timestamp_day>;
  using   timestamp_month_timestamp_week_map = map<timestamp_month, timestamp_week>;
  using  timestamp_month_timestamp_month_map = map<timestamp_month, timestamp_month>;
  using   timestamp_month_full_timestamp_map = map<timestamp_month, full_timestamp>;
  using             timestamp_month_blob_map = map<timestamp_month, hyperdex_blob_data>;

  using           full_timestamp_string_map = map<full_timestamp, string>;
  using              full_timestamp_int_map = map<full_timestamp, hyperdex_int>;
  using            full_timestamp_float_map = map<full_timestamp, hyperdex_float>;
  using           full_timestamp_int128_map = map<full_timestamp, hyperdex_int128>;
  using      full_timestamp_long_double_map = map<full_timestamp, long double>;
  using         full_timestamp_document_map = map<full_timestamp, document>;
  using full_timestamp_timestamp_second_map = map<full_timestamp, timestamp_second>;
  using full_timestamp_timestamp_minute_map = map<full_timestamp, timestamp_minute>;
  using   full_timestamp_timestamp_hour_map = map<full_timestamp, timestamp_hour>;
  using    full_timestamp_timestamp_day_map = map<full_timestamp, timestamp_day>;
  using   full_timestamp_timestamp_week_map = map<full_timestamp, timestamp_week>;
  using  full_timestamp_timestamp_month_map = map<full_timestamp, timestamp_month>;
  using   full_timestamp_full_timestamp_map = map<full_timestamp, full_timestamp>;
  using             full_timestamp_blob_map = map<full_timestamp, hyperdex_blob_data>;

  using             blob_blob_map = map<hyperdex_blob_data, hyperdex_blob_data>;
  using           blob_string_map = map<hyperdex_blob_data, string>;
  using              blob_int_map = map<hyperdex_blob_data, hyperdex_int>;
  using            blob_float_map = map<hyperdex_blob_data, hyperdex_float>;
  using           blob_int128_map = map<hyperdex_blob_data, hyperdex_int128>;
  using      blob_long_double_map = map<hyperdex_blob_data, long double>;
  using         blob_document_map = map<hyperdex_blob_data, document>;
  using blob_timestamp_second_map = map<hyperdex_blob_data, timestamp_second>;
  using blob_timestamp_minute_map = map<hyperdex_blob_data, timestamp_minute>;
  using   blob_timestamp_hour_map = map<hyperdex_blob_data, timestamp_hour>;
  using    blob_timestamp_day_map = map<hyperdex_blob_data, timestamp_day>;
  using   blob_timestamp_week_map = map<hyperdex_blob_data, timestamp_week>;
  using  blob_timestamp_month_map = map<hyperdex_blob_data, timestamp_month>;
  using   blob_full_timestamp_map = map<hyperdex_blob_data, full_timestamp>;

} // namespace hyperdex

#endif // HYPERDEX_MAP_HPP_INCLUDED
