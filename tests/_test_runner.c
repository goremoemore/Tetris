#include "test_config.h"

Suite *field_module(void);
Suite *hard_drop(void);
Suite *lines(void);
Suite *rotate(void);
Suite *shift(void);
Suite *upd(void);
Suite *timer_move(void);
Suite *set_types(void);

int main(void) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(NULL);

  srunner_add_suite(sr, field_module());
  srunner_add_suite(sr, hard_drop());
  srunner_add_suite(sr, lines());
  srunner_add_suite(sr, rotate());
  srunner_add_suite(sr, shift());
  srunner_add_suite(sr, upd());
  srunner_add_suite(sr, timer_move());
  srunner_add_suite(sr, set_types());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}