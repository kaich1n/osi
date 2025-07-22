#include <gtest/gtest.h>
extern "C" {
#include "config_test.h"
}

TEST(ConfigTest, config_new_empty) {
    ASSERT_TRUE(test_config_new_empty());
}

TEST(ConfigTest, config_new_no_file) {
    ASSERT_TRUE(test_config_new_no_file());
}

TEST(ConfigTest, config_new) {
    ASSERT_TRUE(test_config_new());
}

TEST(ConfigTest, config_free_null) {
    ASSERT_TRUE(test_config_free_null());
}

TEST(ConfigTest, config_has_section) {
    ASSERT_TRUE(test_config_has_section());
}

TEST(ConfigTest, config_has_key_in_default_section) {
    ASSERT_TRUE(test_config_has_key_in_default_section());
}

TEST(ConfigTest, config_has_keys) {
    ASSERT_TRUE(test_config_has_keys());
}

TEST(ConfigTest, config_no_bad_keys) {
    ASSERT_TRUE(test_config_no_bad_keys());
}

TEST(ConfigTest, config_get_int_version) {
    ASSERT_TRUE(test_config_get_int_version());
}

TEST(ConfigTest, config_get_int_default) {
    ASSERT_TRUE(test_config_get_int_default());
}

TEST(ConfigTest, config_remove_section) {
    ASSERT_TRUE(test_config_remove_section());
}

TEST(ConfigTest, config_remove_section_missing) {
    ASSERT_TRUE(test_config_remove_section_missing());
}

TEST(ConfigTest, config_remove_key) {
    ASSERT_TRUE(test_config_remove_key());
}

TEST(ConfigTest, config_remove_key_missing) {
    ASSERT_TRUE(test_config_remove_key_missing());
}

TEST(ConfigTest, config_section_begin) {
    ASSERT_TRUE(test_config_section_begin());
}

TEST(ConfigTest, config_section_next) {
    ASSERT_TRUE(test_config_section_next());
}

TEST(ConfigTest, config_section_end) {
    ASSERT_TRUE(test_config_section_end());
}

TEST(ConfigTest, config_save_basic) {
    ASSERT_TRUE(test_config_save_basic());
}
