#include <gtest/gtest.h>
#include <handle_allocator/handle.h>
#include <handle_allocator/handle_allocator.h>
namespace {
struct Point2D {
  double x;
  double y;
};
}  // namespace

TEST(TEST_GET, TEST_GET_CREF_CORRECT_HANDLE) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  auto ret = ha_point2d.Insert({0.0, 0.0});
  EXPECT_TRUE(ret.has_value());
  handle_allocator::Handle handle = ret.value();
  auto ret_cref = ha_point2d.GetConst(handle);
  EXPECT_TRUE(ret_cref.has_value());
  EXPECT_DOUBLE_EQ(ret_cref.value().get().x, 0.0);
  EXPECT_DOUBLE_EQ(ret_cref.value().get().y, 0.0);
}

TEST(TEST_GET, TEST_GET_CREF_INVALID_PATTERN) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  ha_point2d.Insert({0.0, 0.0});
  handle_allocator::Handle handle{0, 1};
  auto ret_cref = ha_point2d.GetConst(handle);
  EXPECT_FALSE(ret_cref.has_value());
}

TEST(TEST_GET, TEST_GET_CREF_INVALID_INDEX) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  ha_point2d.Insert({0.0, 0.0});
  handle_allocator::Handle handle{1, 0};
  auto ret_cref = ha_point2d.GetConst(handle);
  EXPECT_FALSE(ret_cref.has_value());
}

TEST(TEST_GET, TEST_GET_REF_CORRECT_HANDLE) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  auto ret = ha_point2d.Insert({0.0, 0.0});
  EXPECT_TRUE(ret.has_value());
  handle_allocator::Handle handle = ret.value();
  auto ret_ref = ha_point2d.Get(handle);
  EXPECT_TRUE(ret_ref.has_value());
  EXPECT_DOUBLE_EQ(ret_ref.value().get().x, 0.0);
  EXPECT_DOUBLE_EQ(ret_ref.value().get().y, 0.0);
}

TEST(TEST_GET, TEST_GET_REF_INVALID_PATTERN) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  ha_point2d.Insert({0.0, 0.0});
  handle_allocator::Handle handle{0, 1};
  auto ret_ref = ha_point2d.Get(handle);
  EXPECT_FALSE(ret_ref.has_value());
}

TEST(TEST_GET, TEST_GET_INVALID_INDEX) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  ha_point2d.Insert({0.0, 0.0});
  handle_allocator::Handle handle{1, 0};
  auto ret_ref = ha_point2d.Get(handle);
  EXPECT_FALSE(ret_ref.has_value());
}

TEST(TEST_GET, TEST_GET_CORRECT_HANDLE_AND_UPDATE) {
  handle_allocator::HandleAllocator<Point2D> ha_point2d;
  auto ret = ha_point2d.Insert({0.0, 0.0});
  EXPECT_TRUE(ret.has_value());
  handle_allocator::Handle handle = ret.value();
  auto ret_ref = ha_point2d.Get(handle);
  ret_ref.value().get().x = 42.0;
  auto ret_ref2 = ha_point2d.Get(handle);
  EXPECT_DOUBLE_EQ(ret_ref2.value().get().x, 42.0);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
