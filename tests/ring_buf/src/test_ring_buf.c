/*
 * Ring Buffer Module - Homework Test Skeleton
 *
 * test_fresh_state is provided as a worked example. Fill in the remaining
 * 7 ZTEST bodies according to TEST_SPEC.md. Stubs call ztest_test_skip()
 * so the binary builds and runs cleanly before each test is implemented.
 *
 * Run:
 *   west twister -T tests/ring_buf -p native_sim
 */

#include "ring_buf.h"

#include <errno.h>
#include <zephyr/ztest.h>

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state. Capacity 4 is
 * enough to exercise FIFO order (push 1, 2, 3) and overflow (full at 4).
 */
static void before(void *f) {
  ARG_UNUSED(f);
  rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 *
 * Initial state and re-initialization behaviour.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

/* PROVIDED — study this test before writing the rest. */
ZTEST(ring_buf_init, test_fresh_state) {
  zassert_true(rb_is_empty(), "Fresh buffer must be empty");
  zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state) {
  int v = 3;

  rb_push(v);
  zassert_equal(rb_count(), 1, "Buffer count must be 1");
  rb_init(4);
  zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 *
 * Single push/pop round-trip, FIFO order, full error path.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop) {
  rb_push(42);

  int v;
  rb_pop(&v);
  zassert_equal(42, v, "Popped Value must be 42");
  zassert_equal(rb_is_empty(), true, "Buffer sount must be empty = true");
}

ZTEST(ring_buf_push_pop, test_fifo_order) {
  /* TODO(l8-task1): rb_push(1), rb_push(2), rb_push(3); pop three times
   * and verify the values come out as 1, 2, 3 in that order.
   * See TEST_SPEC.md "Suite ring_buf_push_pop" #2.
   */
  int v[3];
  for (uint8_t i = 0; i < 3; i++) {
    rb_push(i + 1);
  }

  for (uint8_t i = 0; i < 3; i++) {
    rb_pop(&v[i]);
    zassert_equal(v[i], i + 1, "Val must be %d", i + 1);
  }

  zassert_equal(rb_is_empty(), true, "Buffer sount must be empty = true");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc) {
  for (uint8_t i = 0; i < 4; i++) {
    rb_push(i + 1);
  }

  zassert_equal(rb_is_full(), true, "Buffer sount must be full = true");
  zassert_equal(rb_push(99), -ENOSPC, "Buffer must return -ENOSPC");
  zassert_equal(rb_count(), 4, "Buffer count must be 4");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 *
 * Peek semantics and NULL-pointer boundary conditions.
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume) {
  rb_push(7);

  int v;

  zassert_equal(rb_peek(&v), 0, "Must be 0");
  zassert_equal(v, 7, "Must be 7");
  zassert_equal(rb_peek(&v), 0, "Must be 0");
  zassert_equal(v, 7, "Musst be 7");
  zassert_equal(rb_count(), 1, "Must be 1");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval) {
  zassert_equal(rb_pop(NULL), -EINVAL, "Must be -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill) {
  for (uint8_t i = 0; i < 4; i++) {
    zassert_equal(rb_push(i), 0, "Must be 0");
  }

  zassert_equal(rb_is_full(), true, "Must be true");
  zassert_equal(rb_count(), 4, "Must be 4");
}
