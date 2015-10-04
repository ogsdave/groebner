#include <gtest/gtest.h>

#include <iostream>

#include "Polynomial.h"

using namespace std;

TEST(PolynomialTest, EmptyConstruction) {
  Polynomial<int, char> p;
  EXPECT_EQ(0, p.lc());
  EXPECT_TRUE(p.lm().isZero());
}

TEST(PolynomialTest, ConstantConstruction) {
  Polynomial<> p(42);
  EXPECT_EQ(42, p.lc());
  EXPECT_TRUE(p.lm().isZero());
}

TEST(PolynomialTest, ConstantAddition) {
  Polynomial<> a(17);
  Polynomial<> b(4);
  Polynomial<> p = a + b;
  EXPECT_EQ(21, p.lc());
  EXPECT_TRUE(p.lm().isZero());
}

TEST(PolynomialTest, Addition) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  EXPECT_EQ(5, a.lc());
  EXPECT_FALSE(a.lm().isZero());
  EXPECT_EQ(e, a.lm());
}

TEST(PolynomialTest, AdditionToZero) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  Polynomial<> b(-17);
  b += Term<>(-5, e);

  EXPECT_TRUE((a + b).lm().isZero());
  EXPECT_EQ(0, (a + b).lc());
}

TEST(PolynomialTest, ConstantMultiplicationToZero) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  a *= 0;

  EXPECT_TRUE(a.lm().isZero());
  EXPECT_EQ(0, a.lc());
}

TEST(PolynomialTest, ConstantMultiplication) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  a *= 3;

  EXPECT_EQ(15, a.lc());
}

TEST(PolynomialTest, TermMultiplication) {
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  Monomial<> f;
  f[0] = 2;

  a *= Term<>(3, f);

  Monomial<> g;
  g[0] = 2;
  g[1] = 12;

  EXPECT_EQ(15, a.lc());
  EXPECT_EQ(g, a.lm());
}

TEST(PolynomialTest, Multiplication) {
  // 5y^12+17
  Polynomial<> a(17);
  Monomial<> e;
  e[1] = 12;
  a += Term<>(5, e);

  // 3x^4+7
  Polynomial<> b(7);
  Monomial<> f;
  f[0] = 4;
  b += Term<>(3, f);

  Term<> x(Monomial<>::x(0));
  Term<> y(Monomial<>::x(1));

  Polynomial<> p = a * b;
  Monomial<> g;
  g[0] = 4;
  g[1] = 12;

  EXPECT_EQ(15, p.lc());
  EXPECT_EQ(g, p.lm());

  Polynomial<> expected = 15*pow(x, 4)*pow(y, 12) + 51*pow(x, 4) + 35*pow(y, 12) + 119;
  EXPECT_EQ(expected, p);
}
