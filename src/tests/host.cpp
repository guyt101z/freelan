/*
 * freelan - An open, multi-platform software to establish peer-to-peer virtual
 * private networks.
 *
 * Copyright (C) 2010-2011 Julien KAUFFMANN <julien.kauffmann@freelan.org>
 *
 * This file is part of freelan.
 *
 * freelan is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * freelan is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 *
 * If you intend to use freelan in a commercial software, please
 * contact me : we may arrange this for a small fee or no fee at all,
 * depending on the nature of your project.
 */

#include <gtest/gtest.h>

#include <sstream>

#include "../internal/host.hpp"
#include "../internal/common.hpp"

using freelan::Host;
using freelan::IPv4Address;
using freelan::IPv6Address;
using freelan::Hostname;
using freelan::from_string;

TEST(Host, default_instantiation) {
	const Host value {};
}

TEST(Host, ipv4_address_instantiation) {
	const auto raw_value = IPv4Address::from_string("9.0.0.0");
	const Host value { raw_value };

	ASSERT_EQ(raw_value, value);
}

TEST(Host, ipv6_address_instantiation) {
	const auto raw_value = IPv6Address::from_string("fe80::a:0");
	const Host value { raw_value };

	ASSERT_EQ(raw_value, value);
}

TEST(Host, hostname_instantiation) {
	const auto raw_value = Hostname::from_string("foo.bar.net");
	const Host value { raw_value };

	ASSERT_EQ(raw_value, value);
}

TEST(Host, ipv4_address_getter) {
	const auto raw_value = IPv4Address::from_string("9.0.0.0");
	const Host value { raw_value };

	ASSERT_TRUE(value.is<IPv4Address>());
	ASSERT_FALSE(value.is<IPv6Address>());
	ASSERT_FALSE(value.is<Hostname>());
	ASSERT_EQ(raw_value, *value.as<IPv4Address>());
	ASSERT_EQ(nullptr, value.as<IPv6Address>());
	ASSERT_EQ(nullptr, value.as<Hostname>());
}

TEST(Host, ipv6_address_getter) {
	const auto raw_value = IPv6Address::from_string("fe80::a:0");
	const Host value { raw_value };

	ASSERT_FALSE(value.is<IPv4Address>());
	ASSERT_TRUE(value.is<IPv6Address>());
	ASSERT_FALSE(value.is<Hostname>());
	ASSERT_EQ(nullptr, value.as<IPv4Address>());
	ASSERT_EQ(raw_value, *value.as<IPv6Address>());
	ASSERT_EQ(nullptr, value.as<Hostname>());
}

TEST(Host, hostname_getter) {
	const auto raw_value = Hostname::from_string("foo.bar.net");
	const Host value { raw_value };

	ASSERT_FALSE(value.is<IPv4Address>());
	ASSERT_FALSE(value.is<IPv6Address>());
	ASSERT_TRUE(value.is<Hostname>());
	ASSERT_EQ(nullptr, value.as<IPv4Address>());
	ASSERT_EQ(nullptr, value.as<IPv6Address>());
	ASSERT_EQ(raw_value, *value.as<Hostname>());
}

TEST(Host, ipv4_address_string_instantiation) {
	const std::string str_value = "9.0.0.1";
	const auto value = Host::from_string(str_value);

	ASSERT_EQ(str_value, value.to_string());
}

TEST(Host, ipv6_address_string_instantiation) {
	const std::string str_value = "fe80::a:0";
	const auto value = Host::from_string(str_value);

	ASSERT_EQ(str_value, value.to_string());
}

TEST(Host, hostname_string_instantiation) {
	const std::string str_value = "foo.bar.net";
	const auto value = Host::from_string(str_value);

	ASSERT_EQ(str_value, value.to_string());
}

TEST(Host, read_from_invalid_stream) {
	std::istringstream iss;
	iss.setstate(std::ios::failbit);
	Host value;

	const auto& result = Host::read_from(iss, value);

	ASSERT_EQ(&iss, &result);
	ASSERT_EQ(Host(), value);
	ASSERT_EQ(std::ios::failbit, iss.rdstate());
}

TEST(Host, string_instantiation_failure) {
	try {
		Host::from_string("invalid");
	} catch (boost::system::system_error& ex) {
		ASSERT_EQ(make_error_condition(boost::system::errc::invalid_argument), ex.code());
	}
}

TEST(Host, string_instantiation_failure_no_throw) {
	boost::system::error_code ec;
	const auto value = Host::from_string("invalid_-_*", ec);

	ASSERT_EQ(Host(), value);
	ASSERT_EQ(make_error_condition(boost::system::errc::invalid_argument), ec);
}

TEST(Host, ipv4_address_implicit_string_conversion) {
	const std::string str_value = "9.0.0.1";
	const auto value = from_string<Host>(str_value);

	ASSERT_EQ(str_value, to_string(value));
}

TEST(Host, ipv6_address_implicit_string_conversion) {
	const std::string str_value = "fe80::a:0";
	const auto value = from_string<Host>(str_value);

	ASSERT_EQ(str_value, to_string(value));
}

TEST(Host, hostname_implicit_string_conversion) {
	const std::string str_value = "foo.bar.net";
	const auto value = from_string<Host>(str_value);

	ASSERT_EQ(str_value, to_string(value));
}

TEST(Host, compare_to_same_instance) {
	const auto value = from_string<Host>("9.0.0.1");

	ASSERT_TRUE(value == value);
	ASSERT_FALSE(value != value);
	ASSERT_FALSE(value < value);
	ASSERT_TRUE(value <= value);
	ASSERT_FALSE(value > value);
	ASSERT_TRUE(value >= value);
}

TEST(Host, compare_to_same_value) {
	const auto value_a = from_string<Host>("9.0.0.1");
	const auto value_b = from_string<Host>("9.0.0.1");

	ASSERT_TRUE(value_a == value_b);
	ASSERT_FALSE(value_a != value_b);
	ASSERT_FALSE(value_a < value_b);
	ASSERT_TRUE(value_a <= value_b);
	ASSERT_FALSE(value_a > value_b);
	ASSERT_TRUE(value_a >= value_b);
}

TEST(Host, compare_to_different_values) {
	const auto value_a = from_string<Host>("9.0.0.1");
	const auto value_b = from_string<Host>("9.0.0.2");

	ASSERT_FALSE(value_a == value_b);
	ASSERT_TRUE(value_a != value_b);
	ASSERT_TRUE(value_a < value_b);
	ASSERT_TRUE(value_a <= value_b);
	ASSERT_FALSE(value_a > value_b);
	ASSERT_FALSE(value_a >= value_b);
}

TEST(Host, compare_to_different_subtypes) {
	const auto value_a = from_string<Host>("9.0.0.1");
	const auto value_b = from_string<Host>("fe80::a:0");

	ASSERT_FALSE(value_a == value_b);
	ASSERT_TRUE(value_a != value_b);
	ASSERT_TRUE(value_a < value_b);
	ASSERT_TRUE(value_a <= value_b);
	ASSERT_FALSE(value_a > value_b);
	ASSERT_FALSE(value_a >= value_b);
}

TEST(Host, ipv4_address_stream_input) {
	const std::string str_value = "9.0.0.1";
	const auto value_ref = from_string<Host>(str_value);

	std::istringstream iss(str_value);
	Host value;

	iss >> value;

	ASSERT_EQ(value_ref, value);
	ASSERT_TRUE(iss.eof());
	ASSERT_TRUE(!iss.good());
	ASSERT_TRUE(!iss.fail());
}

TEST(Host, ipv6_address_stream_input) {
	const std::string str_value = "fe80::80:a";
	const auto value_ref = from_string<Host>(str_value);

	std::istringstream iss(str_value);
	Host value;

	iss >> value;

	ASSERT_EQ(value_ref, value);
	ASSERT_TRUE(iss.eof());
	ASSERT_TRUE(!iss.good());
	ASSERT_TRUE(!iss.fail());
}

TEST(Host, hostname_stream_input) {
	const std::string str_value = "foo.bar.net";
	const auto value_ref = from_string<Host>(str_value);

	std::istringstream iss(str_value);
	Host value;

	iss >> value;

	ASSERT_EQ(value_ref, value);
	ASSERT_TRUE(iss.eof());
	ASSERT_TRUE(!iss.good());
	ASSERT_TRUE(!iss.fail());
}

TEST(Host, ipv4_address_stream_output) {
	const std::string str_value = "9.0.0.1";
	const auto value = from_string<Host>(str_value);

	std::ostringstream oss;
	oss << value;

	ASSERT_EQ(str_value, oss.str());
}

TEST(Host, ipv6_address_stream_output) {
	const std::string str_value = "fe80::80:a";
	const auto value = from_string<Host>(str_value);

	std::ostringstream oss;
	oss << value;

	ASSERT_EQ(str_value, oss.str());
}

TEST(Host, hostname_stream_output) {
	const std::string str_value = "foo.bar.net";
	const auto value = from_string<Host>(str_value);

	std::ostringstream oss;
	oss << value;

	ASSERT_EQ(str_value, oss.str());
}