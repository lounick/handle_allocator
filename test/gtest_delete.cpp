#include <gtest/gtest.h>
#include <handle_allocator/handle.h>
#include <handle_allocator/handle_allocator.h>

TEST(TEST_DELETE, TEST_DELETE_CORRECT_HANDLE) {
  handle_allocator::HandleAllocator<int> ha_int;
  auto ret = ha_int.Insert(42);
  bool del_ret = ha_int.Delete(ret.value());
  EXPECT_TRUE(del_ret);
}

TEST(TEST_DELETE, TEST_DELETE_INVALID_INDEX) {
  handle_allocator::HandleAllocator<int> ha_int;
  ha_int.Insert(42);
  handle_allocator::Handle bad_handle{1, 0};
  bool del_ret = ha_int.Delete(bad_handle);
  EXPECT_FALSE(del_ret);
}

TEST(TEST_DELETE, TEST_DELETE_INVALID_PATTERN) {
  handle_allocator::HandleAllocator<int> ha_int;
  ha_int.Insert(42);
  handle_allocator::Handle bad_handle{0, 1};
  bool del_ret = ha_int.Delete(bad_handle);
  EXPECT_FALSE(del_ret);
}

TEST(TEST_DELETE, TEST_DELETE_GET_DELETED_FAILS) {
  handle_allocator::HandleAllocator<int> ha_int;
  auto ret = ha_int.Insert(42);
  ha_int.Delete(ret.value());
  auto get_ret = ha_int.Get(ret.value());
  EXPECT_FALSE(get_ret.has_value());
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
