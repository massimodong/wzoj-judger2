#include "gtest/gtest.h"

#include "comparer.h"

TEST(COMPARER, A){
  int c11 = open("c1-1.txt", O_RDONLY),
      c12 = open("c1-2.txt", O_RDONLY),
      c13 = open("c1-3.txt", O_RDONLY),
      c21 = open("c2-1.txt", O_RDONLY),
      c22 = open("c2-2.txt", O_RDONLY);

  EXPECT_NE(c11, -1);
  EXPECT_NE(c12, -1);
  EXPECT_NE(c13, -1);
  EXPECT_NE(c21, -1);
  EXPECT_NE(c22, -1);

  Comparer cp11(c11), cp12(c12), cp13(c13), cp21(c21), cp22(c22);

  bool b = cp11 == cp12;
  EXPECT_EQ(b, true);
  cp11.reset();
  cp12.reset();

  b = cp11 == cp13;
  EXPECT_EQ(b, true);
  cp11.reset();
  cp13.reset();

  b = cp11 == cp21;
  EXPECT_EQ(b, false);
  cp11.reset();
  cp21.reset();

  b = cp21 == cp22;
  EXPECT_EQ(b, true);
  cp21.reset();
  cp22.reset();
}
