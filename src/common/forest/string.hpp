#ifndef FOREST_STRING_HPP_INCLUDED
#define FOREST_STRING_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: Forest Common Files
// File: string.hpp
// First created: 2015-09-11-21.57
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <forest/alternatives.hpp>

#if FOREST_ALTERNATIVE_FOR_STRING == FAV_STD

#include <string>

namespace forest
{
  using std::string;
  using std::to_string;
  using std::wstring;
}

#else

#error "Wrong alternative for string class!"

#endif // FOREST_ALTERNATIVE_FOR_STRING

#if FOREST_ALTERNATIVE_FOR_STRING_EXT == FAV_NONE

#elif FOREST_ALTERNATIVE_FOR_STRING_EXT == FAV_BOOST

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string/trim_all.hpp>

namespace forest
{
  using boost::algorithm::to_lower;
  using boost::algorithm::to_upper;
  using boost::algorithm::to_lower_copy;
  using boost::algorithm::to_upper_copy;
  using boost::algorithm::is_classified;
  using boost::algorithm::is_space;
  using boost::algorithm::is_alnum;
  using boost::algorithm::is_alpha;
  using boost::algorithm::is_cntrl;
  using boost::algorithm::is_digit;
  using boost::algorithm::is_graph;
  using boost::algorithm::is_lower;
  using boost::algorithm::is_upper;
  using boost::algorithm::is_print;
  using boost::algorithm::is_punct;
  using boost::algorithm::is_xdigit;
  using boost::algorithm::is_any_of;
  using boost::algorithm::is_from_range;
  using boost::algorithm::is_equal;
  using boost::algorithm::is_iequal;
  using boost::algorithm::is_less;
  using boost::algorithm::is_iless;
  using boost::algorithm::is_not_greater;
  using boost::algorithm::is_not_igreater;
  using boost::algorithm::token_compress_mode_type;
  using boost::algorithm::FormatterConcept;
  using boost::algorithm::erase_range_copy;
  using boost::algorithm::erase_range;
  using boost::algorithm::erase_first_copy;
  using boost::algorithm::erase_first;
  using boost::algorithm::ierase_first_copy;
  using boost::algorithm::ierase_first;
  using boost::algorithm::erase_last_copy;
  using boost::algorithm::erase_last;
  using boost::algorithm::ierase_last_copy;
  using boost::algorithm::ierase_last;
  using boost::algorithm::erase_nth_copy;
  using boost::algorithm::ierase_nth_copy;
  using boost::algorithm::erase_nth;
  using boost::algorithm::ierase_nth;
  using boost::algorithm::erase_all_copy;
  using boost::algorithm::ierase_all_copy;
  using boost::algorithm::erase_all;
  using boost::algorithm::ierase_all;
  using boost::algorithm::erase_head_copy;
  using boost::algorithm::erase_head;
  using boost::algorithm::erase_tail_copy;
  using boost::algorithm::erase_tail;
  using boost::algorithm::find;
  using boost::algorithm::find_first;
  using boost::algorithm::ifind_first;
  using boost::algorithm::find_last;
  using boost::algorithm::ifind_last;
  using boost::algorithm::find_nth;
  using boost::algorithm::ifind_nth;
  using boost::algorithm::find_head;
  using boost::algorithm::find_tail;
  using boost::algorithm::find_token;
  using boost::algorithm::find_format_copy;
  using boost::algorithm::find_format;
  using boost::algorithm::find_format_all_copy;
  using boost::algorithm::find_format_all;
  using boost::algorithm::find_iterator;
  using boost::algorithm::make_find_iterator;
  using boost::algorithm::first_finder;
  using boost::algorithm::last_finder;
  using boost::algorithm::nth_finder;
  using boost::algorithm::head_finder;
  using boost::algorithm::tail_finder;
  using boost::algorithm::token_finder;
  using boost::algorithm::range_finder;
  using boost::algorithm::const_formatter;
  using boost::algorithm::identity_formatter;
  using boost::algorithm::empty_formatter;
  using boost::algorithm::dissect_formatter;
  using boost::algorithm::iter_find;
  using boost::algorithm::iter_split;
  using boost::algorithm::join;
  using boost::algorithm::join_if;
  using boost::algorithm::starts_with;
  using boost::algorithm::istarts_with;
  using boost::algorithm::ends_with;
  using boost::algorithm::iends_with;
  using boost::algorithm::contains;
  using boost::algorithm::icontains;
  using boost::algorithm::equals;
  using boost::algorithm::iequals;
  using boost::algorithm::lexicographical_compare;
  using boost::algorithm::ilexicographical_compare;
  using boost::algorithm::all;
  using boost::algorithm::find_regex;
  using boost::algorithm::replace_regex_copy;
  using boost::algorithm::replace_regex;
  using boost::algorithm::replace_all_regex_copy;
  using boost::algorithm::replace_all_regex;
  using boost::algorithm::erase_regex_copy;
  using boost::algorithm::erase_regex;
  using boost::algorithm::erase_all_regex_copy;
  using boost::algorithm::find_all_regex;
  using boost::algorithm::split_regex;
  using boost::algorithm::join_if;
  using boost::algorithm::regex_finder;
  using boost::algorithm::regex_formatter;
  using boost::algorithm::replace_range_copy;
  using boost::algorithm::replace_range;
  using boost::algorithm::replace_first_copy;
  using boost::algorithm::replace_first;
  using boost::algorithm::ireplace_first_copy;
  using boost::algorithm::ireplace_first;
  using boost::algorithm::replace_last_copy;
  using boost::algorithm::replace_last;
  using boost::algorithm::ireplace_last_copy;
  using boost::algorithm::ireplace_last;
  using boost::algorithm::ireplace_nth_copy;
  using boost::algorithm::replace_nth_copy;
  using boost::algorithm::replace_nth;
  using boost::algorithm::ireplace_nth;
  using boost::algorithm::replace_all_copy;
  using boost::algorithm::replace_all;
  using boost::algorithm::ireplace_all_copy;
  using boost::algorithm::ireplace_all;
  using boost::algorithm::replace_head_copy;
  using boost::algorithm::replace_head;
  using boost::algorithm::replace_tail_copy;
  using boost::algorithm::replace_tail;
  using boost::algorithm::find_all;
  using boost::algorithm::ifind_all;
  using boost::algorithm::split;
  using boost::algorithm::trim_left_copy_if;
  using boost::algorithm::trim_left_copy;
  using boost::algorithm::trim_left_if;
  using boost::algorithm::trim_left;
  using boost::algorithm::trim_right_copy_if;
  using boost::algorithm::trim_right_copy;
  using boost::algorithm::trim_right_if;
  using boost::algorithm::trim_right;
  using boost::algorithm::trim_copy_if;
  using boost::algorithm::trim_copy;
  using boost::algorithm::trim_if;
  using boost::algorithm::trim;
  using boost::algorithm::trim_all_copy_if;
  using boost::algorithm::trim_all_copy;
  using boost::algorithm::trim_all_if;
  using boost::algorithm::trim_all;
  using boost::algorithm::trim_fill_copy_if;
  using boost::algorithm::trim_fill_copy;
  using boost::algorithm::trim_fill_if;
  using boost::algorithm::trim_fill;
}

#else

#error "Wrong alternative for string extensions!"

#endif // FOREST_ALTERNATIVE_FOR_STRING_EXT


#ifdef FOREST_ALTERNATIVES_DECLARE_GLOBAL

using forest::string;
using forest::to_string;
using forest::wstring;

#if FOREST_ALTERNATIVE_FOR_STRING_EXT == FAV_BOOST

  using forest::to_lower;
  using forest::to_upper;
  using forest::to_lower_copy;
  using forest::to_upper_copy;
  using forest::is_classified;
  using forest::is_space;
  using forest::is_alnum;
  using forest::is_alpha;
  using forest::is_cntrl;
  using forest::is_digit;
  using forest::is_graph;
  using forest::is_lower;
  using forest::is_upper;
  using forest::is_print;
  using forest::is_punct;
  using forest::is_xdigit;
  using forest::is_any_of;
  using forest::is_from_range;
  using forest::is_equal;
  using forest::is_iequal;
  using forest::is_less;
  using forest::is_iless;
  using forest::is_not_greater;
  using forest::is_not_igreater;
  using forest::token_compress_mode_type;
  using forest::FormatterConcept;
  using forest::erase_range_copy;
  using forest::erase_range;
  using forest::erase_first_copy;
  using forest::erase_first;
  using forest::ierase_first_copy;
  using forest::ierase_first;
  using forest::erase_last_copy;
  using forest::erase_last;
  using forest::ierase_last_copy;
  using forest::ierase_last;
  using forest::erase_nth_copy;
  using forest::ierase_nth_copy;
  using forest::erase_nth;
  using forest::ierase_nth;
  using forest::erase_all_copy;
  using forest::ierase_all_copy;
  using forest::erase_all;
  using forest::ierase_all;
  using forest::erase_head_copy;
  using forest::erase_head;
  using forest::erase_tail_copy;
  using forest::erase_tail;
  using forest::find;
  using forest::find_first;
  using forest::ifind_first;
  using forest::find_last;
  using forest::ifind_last;
  using forest::find_nth;
  using forest::ifind_nth;
  using forest::find_head;
  using forest::find_tail;
  using forest::find_token;
  using forest::find_format_copy;
  using forest::find_format;
  using forest::find_format_all_copy;
  using forest::find_format_all;
  using forest::find_iterator;
  using forest::make_find_iterator;
  using forest::first_finder;
  using forest::last_finder;
  using forest::nth_finder;
  using forest::head_finder;
  using forest::tail_finder;
  using forest::token_finder;
  using forest::range_finder;
  using forest::const_formatter;
  using forest::identity_formatter;
  using forest::empty_formatter;
  using forest::dissect_formatter;
  using forest::iter_find;
  using forest::iter_split;
  using forest::join;
  using forest::join_if;
  using forest::starts_with;
  using forest::istarts_with;
  using forest::ends_with;
  using forest::iends_with;
  using forest::contains;
  using forest::icontains;
  using forest::equals;
  using forest::iequals;
  using forest::lexicographical_compare;
  using forest::ilexicographical_compare;
  using forest::all;
  using forest::find_regex;
  using forest::replace_regex_copy;
  using forest::replace_regex;
  using forest::replace_all_regex_copy;
  using forest::replace_all_regex;
  using forest::erase_regex_copy;
  using forest::erase_regex;
  using forest::erase_all_regex_copy;
  using forest::find_all_regex;
  using forest::split_regex;
  using forest::join_if;
  using forest::regex_finder;
  using forest::regex_formatter;
  using forest::replace_range_copy;
  using forest::replace_range;
  using forest::replace_first_copy;
  using forest::replace_first;
  using forest::ireplace_first_copy;
  using forest::ireplace_first;
  using forest::replace_last_copy;
  using forest::replace_last;
  using forest::ireplace_last_copy;
  using forest::ireplace_last;
  using forest::ireplace_nth_copy;
  using forest::replace_nth_copy;
  using forest::replace_nth;
  using forest::ireplace_nth;
  using forest::replace_all_copy;
  using forest::replace_all;
  using forest::ireplace_all_copy;
  using forest::ireplace_all;
  using forest::replace_head_copy;
  using forest::replace_head;
  using forest::replace_tail_copy;
  using forest::replace_tail;
  using forest::find_all;
  using forest::ifind_all;
  using forest::split;
  using forest::trim_left_copy_if;
  using forest::trim_left_copy;
  using forest::trim_left_if;
  using forest::trim_left;
  using forest::trim_right_copy_if;
  using forest::trim_right_copy;
  using forest::trim_right_if;
  using forest::trim_right;
  using forest::trim_copy_if;
  using forest::trim_copy;
  using forest::trim_if;
  using forest::trim;
  using forest::trim_all_copy_if;
  using forest::trim_all_copy;
  using forest::trim_all_if;
  using forest::trim_all;
  using forest::trim_fill_copy_if;
  using forest::trim_fill_copy;
  using forest::trim_fill_if;
  using forest::trim_fill;

#endif // FOREST_ALTERNATIVE_FOR_STRING_EXT

#endif // FOREST_ALTERNATIVES_DECLARE_GLOBAL


#endif // FOREST_STRING_HPP_INCLUDED
