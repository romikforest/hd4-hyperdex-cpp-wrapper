#ifndef TEST31_HPP_INCLUDED
#define TEST31_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test31.hpp
// First created: 2015-10-30-14.15
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/hyperdex.hpp>

bool check_time(char const* p, size_t n)
{
  forest::string s { p + 7, n - 7 };
  if (s[0] <= '5' && s[0] >= '0') return true;
  return false;
}

void test31()
{
  std::cout << "Test 31: Google macaroons 2." << std::endl;

  using forest::string;

  char const* secret   = "super secret password";


  hyperdex::google_macaroon m  { secret, "long beach", "say some secret you want" };
  if (!m) { LOG(error) << "Macaroon is not created."; return; }

  //m = m.add_first_party_caveat("op = read");
  m = m.add_first_party_caveat("account = 3735928559");
  m = m.add_first_party_caveat("time < 3");
  //m = m.add_first_party_caveat("time < 8");
  m = m.add_first_party_caveat("email = alice@example.org");

  if (!m) { LOG(error) << "Error while adding caveats."; return; }

  string s = m.serialize();

  std::cout << "Serialized macaroon: " << s << std::endl;



  std::cout << "Inspected macaroon: " << m.inspect() << std::endl;

  std::cout << "From macaroon: location: " << m.location() << std::endl
            << "Identifier: " << m.identifier() << std::endl
            << "Signature: " << m.signature() << std::endl;;

  hyperdex::google_macaroon m2 {};
  m2.deserialize(s);

  std::cout << "Inspected macaroon after deserialization: " << m2.inspect() << std::endl;

  hyperdex::google_macaroon_verifier v {};

  std::cout << "Ver: " << v.verify(m2, secret) << std::endl;

  std::cout << "satisfy_exact('account = 3735928559'): " << v.satisfy_exact("account = 3735928559") << std::endl
            << "satisfy_exact('email = alice@example.org'): " << v.satisfy_exact("email = alice@example.org") << std::endl;
            //<< "satisfy_exact('email = alice@example.com'): " << v.satisfy_exact("email = alice@example.com") << std::endl;
  std::cout << "satisfy_general(check_time): " << v.satisfy_general(check_time) << std::endl;
  std::cout << "Ver: " << v.verify(m2, secret) << std::endl;
}


#endif // TEST31_HPP_INCLUDED


