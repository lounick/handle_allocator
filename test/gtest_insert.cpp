#include <gtest/gtest.h>
#include <handle_allocator/handle.h>
#include <handle_allocator/handle_allocator.h>

TEST(TEST_INSERT, TEST_INSERT_VAR_SIZE_EMPTY) {
  handle_allocator::HandleAllocator<int> ha_int;
  auto ret = ha_int.Insert(42);
  EXPECT_TRUE(ret.has_value());
  EXPECT_EQ(ret.value().index, 0);
  EXPECT_EQ(ret.value().pattern, 0);
}

TEST(TEST_INSERT, TEST_INSERT_FIXED_SIZE_EMPTY) {
  handle_allocator::HandleAllocator<int> ha_int(1);
  auto ret = ha_int.Insert(42);
  EXPECT_TRUE(ret.has_value());
  EXPECT_EQ(ret.value().index, 0);
  EXPECT_EQ(ret.value().pattern, 0);
}

TEST(TEST_INSERT, TEST_INSERT_FIXED_SIZE_FULL) {
  handle_allocator::HandleAllocator<int> ha_int(1);
  auto ret = ha_int.Insert(42);
  ret = ha_int.Insert(43);
  EXPECT_FALSE(ret.has_value());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
