#pragma once

#include <gtest/gtest.h>

#define CHECK(statement) \
  GTEST_AMBIGUOUS_ELSE_BLOCKER_ \
  if (::testing::internal::AlwaysTrue()) { \
    ::testing::internal::HasNewFatalFailureHelper gtest_fatal_failure_checker; \
    GTEST_SUPPRESS_UNREACHABLE_CODE_WARNING_BELOW_(statement); \
    if (gtest_fatal_failure_checker.has_new_fatal_failure()) { \
      goto GTEST_CONCAT_TOKEN_(gtest_label_testnofatal_, __LINE__); \
    } \
  } else \
    GTEST_CONCAT_TOKEN_(gtest_label_testnofatal_, __LINE__): \
    GTEST_FATAL_FAILURE_("")
