#ifndef TEST30_HPP_INCLUDED
#define TEST30_HPP_INCLUDED

///////////////////////////////////////////////////////////////
// English - Russian UTF-8
//
// Project: HyperDex c++ wrapper
// File: test30.hpp
// First created: 2015-10-30-14.13
//
// Copyright (c) 2015, Koptev Roman (forest_software@mail.ru)
// License: GNU GPL v2 or later
///////////////////////////////////////////////////////////////

#include <hyperdex_cpp/hyperdex.hpp>

#include <unistd.h>

void test30()
{
  std::cout << "Test 30: Google macaroons." << std::endl;

  using forest::string;

  hyperdex::admin admin;
  if (!admin) { LOG(error) << "Error creating hyperdex admin!"; return; }

  char const* desc = "space accounts key account attributes string name, int balance with authorization";
  auto a_ans = admin.add_space_sync(desc);

  if (a_ans) LOG(info)  << "Space is created";
  else       LOG(error) << "Space is not created";

  hyperdex::client client;

  char const* secret   = "super secret password";
  char const* account  = "account number of john smith";
  char const* secret_f = HYPERDEX_ATTRIBUTE_SECRET;

  hyperdex::attributes attrs { { secret_f, secret }, { "name", "John Smith" }, { "balance", 10 } };

  auto ans = client.put_sync("accounts", account, attrs);

  if (ans) LOG(info)  << "Account data added.";
  else   { LOG(error) << "Account data can't be added."; }

//  // Access without authorization:
//
//  ans = client.get_sync("accounts", account, attrs);
//
//  if (ans) LOG(info)  << "Account data received.";
//  else   { LOG(error) << "Account data can't be received."; std::cout << ans << std::endl; return; }
//
//  std::cout << attrs << std::endl;

  // Access with authorization:

  hyperdex::google_macaroon m  { secret, "long beach", "say some secret you want" };
  if (!m) { LOG(error) << "Macaroon is not created."; return; }

  m = m.add_first_party_caveat("op = read");

  hyperdex::full_timestamp tm { std::chrono::system_clock::now() };
  m = m.add_first_party_caveat(string {"time < "} + forest::to_string((long long int)tm.to_seconds() + 30));

  if (!m) { LOG(error) << "Error while adding caveat."; return; }

  string s = m.serialize();

  std::cout << "Serialized macaroon: " << s << std::endl;

  string insp = m.inspect();

  std::cout << "Inspected macaroon: " << insp << std::endl;

  std::cout << "From macaroon: location: " << m.location() << std::endl
            << "Identifier: " << m.identifier() << std::endl
            << "Signature: " << m.signature() << std::endl;;

//  char const* auths[] { s.c_str() };
//  client.set_auth_context(auths, 1);

//  client.set_auth_context({ s.c_str() });

  hyperdex::google_macaroon_vector mv { m };
  hyperdex::string_vector sv {};
  hyperdex::name_vector nmv {};

  hyperdex::serialize_google_macaroon_vector(nmv, sv, mv);

  client.set_auth_context(nmv);

  ans = client.get_sync("accounts", account, attrs);

  if (ans) LOG(info)  << "Account data received.";
  else   { LOG(error) << "Account data can't be received."; std::cout << ans << std::endl; return; }

  std::cout << attrs << std::endl;

  // Test of put
  ans = client.put_sync("accounts", account, attrs);

  if (ans) LOG(info)  << "New put done.";
  else   { LOG(error) << "New put can't be done."; std::cout << ans << std::endl; }

  // Get after sleep
  std::cout << "Sleeping for 31 seconds............" << std::endl;
  sleep(31);
  std::cout << "After sleep:" << std::endl;

  ans = client.get_sync("accounts", account, attrs);

  if (ans) LOG(info)  << "Account data received.";
  else   { LOG(error) << "Account data can't be received."; std::cout << ans << std::endl; }

  client.clear_auth_context();
}


#endif // TEST30_HPP_INCLUDED
