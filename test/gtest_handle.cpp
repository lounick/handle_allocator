#include <gtest/gtest.h>
#include <handle_allocator/handle.h>

TEST(TEST_HANDLE, TEST_HANDLE_EQUALS_EQUAL_HANDLES) {
  handle_allocator::Handle h1{0, 0};
  handle_allocator::Handle h2{0, 0};
  EXPECT_TRUE(h1 == h2);
}

TEST(TEST_HANDLE, TEST_HANDLE_EQUALS_INEQUAL_HANDLES_INDEX) {
  handle_allocator::Handle h1{0, 0};
  handle_allocator::Handle h2{1, 0};
  EXPECT_FALSE(h1 == h2);
}

TEST(TEST_HANDLE, TEST_HANDLE_EQUALS_INEQUAL_HANDLES_PATTERN) {
  handle_allocator::Handle h1{0, 0};
  handle_allocator::Handle h2{0, 1};
  EXPECT_FALSE(h1 == h2);
}

TEST(TEST_HANDLE, TEST_HANDLE_EQUALS_INEQUAL_HANDLES_BOTH) {
  handle_allocator::Handle h1{0, 0};
  handle_allocator::Handle h2{1, 1};
  EXPECT_FALSE(h1 == h2);
}

TEST(TEST_HANDLE, TEST_HANDLE_EQUALS_CONST_LHS) {
  const handle_allocator::Handle h1{0, 0};
  handle_allocator::Handle h2{0, 0};
  EXPECT_TRUE(h1 == h2);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
