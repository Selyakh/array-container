// Copyright (c) 2024 MIPT-school of applied mathematics and computer science

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <array>

#include "array.h"

template <class T, class U, size_t N>
void Equals(const Array<T, N>& actual, const std::array<U, N>& required) {
  for (size_t i = 0; i < N; ++i) {
    REQUIRE(actual[i] == required[i]);
  }
}

TEST_CASE("Automatic Storage", "[Array Basics]") {
  static_assert(sizeof(Array<int, 1>) == sizeof(int));
  static_assert(sizeof(Array<int, 17>) == sizeof(int) * 17);
  static_assert(sizeof(Array<double, 13>) == sizeof(double) * 13);
}

TEST_CASE("Size and Empty", "[Array Basics]") {
  const Array<int, 7> array{};
  REQUIRE(array.Size() == 7);
  REQUIRE_FALSE(array.Empty());
}

TEST_CASE("Indexing", "[Data Access]") {
  Array<int, 4> a{};
  a[0] = 1;
  a[1] = -1;
  a[3] = 0;
  (a[2] = a[3]) = 987;
  Equals(a, std::array{1, -1, 987, 0});

  using ResultType = decltype(std::as_const(a)[0]);
  static_assert(std::is_same_v<ResultType, const int&>);
}

TEST_CASE("Front", "[Data Access]") {
  Array<char, 3> a{};
  a[0] = '1';
  a[1] = 'z';
  a[2] = '*';

  REQUIRE(std::as_const(a).Front() == '1');
  a.Front() = '+';
  REQUIRE(std::as_const(a).Front() == '+');

  using ResultType = decltype(std::as_const(a).Front());
  static_assert(std::is_same_v<ResultType, const char&>);
}

TEST_CASE("Back", "[Data Access]") {
  Array<char, 3> a{};
  a[0] = '1';
  a[1] = 'z';
  a[2] = '*';

  REQUIRE(std::as_const(a).Back() == '*');
  a.Back() = '+';
  REQUIRE(std::as_const(a).Back() == '+');

  using ResultType = decltype(std::as_const(a).Back());
  static_assert(std::is_same_v<ResultType, const char&>);
}

TEST_CASE("Data", "[Data Access]") {
  Array<int, 3> a{};
  a[0] = 1;
  a[1] = 2;
  a[2] = -1;

  REQUIRE(std::as_const(a).Data() == &(a[0]));
  REQUIRE(*(std::as_const(a).Data() + 1) == 2);
  static_assert((std::is_same_v<decltype(a.Data()), int*>));
  static_assert((std::is_same_v<decltype(std::as_const(a).Data()), const int*>));
}

TEST_CASE("Fill", "[Data Modification]") {
  Array<int, 7> a{};
  a.Fill(-11);
  Equals(a, std::array{-11, -11, -11, -11, -11, -11, -11});
  a.Fill(1);
  Equals(a, std::array{1, 1, 1, 1, 1, 1, 1});
}

TEST_CASE("Aggregate Initialization", "[Initialization]") {
  Array<int, 3> a{5, 4, -9};
  Equals(a, std::array{5, 4, -9});

  Array<int16_t, 5> c{};
  Equals(c, std::array{0, 0, 0, 0, 0});

  Array<int64_t, 4> d{9, -5};
  Equals(d, std::array{9, -5, 0, 0});

  struct S {
    int i;
    char c;
  };

  Array<S, 3> arr{{{1, 'a'}, {2, 'b'}}};
  REQUIRE(arr[0].i == 1);
  REQUIRE(arr[0].c == 'a');
  REQUIRE(arr[1].i == 2);
  REQUIRE(arr[1].c == 'b');
  REQUIRE(arr[2].i == 0);
  REQUIRE(arr[2].c == '\0');
}

TEST_CASE("Swap", "[Data Modification]") {
  Array<int, 3> a{1, 2, 3};
  Array<int, 3> b{-1, -2, -3};

  a.Swap(b);
  Equals(a, std::array{-1, -2, -3});
  Equals(b, std::array{1, 2, 3});
}