begin_task()
add_task_library(futures)
add_task_test(unit_tests tests/unit.cpp)
add_task_test(traits_tests tests/traits.cpp)
add_task_test(stress_tests tests/stress.cpp)
add_playground(play)
end_task()
