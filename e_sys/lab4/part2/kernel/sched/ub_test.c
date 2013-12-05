/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Alvin Zheng <dongxuez@andrew.cmu.edu>
 *         Minghao Wang <minghaow@andrew.cmu.edu>
 *         Yining Yang <yiningy@andrew.cmu.edu>
 * @date   4 Dec, 2013 16:09
 */

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

#define UB_NUMBER    64

/* The limit of UB test value. If the task number exceeds UB_NUMBER,
 * use it to perform UB test */
static double ub_min __attribute__((unused)) = 0.693147;

// Pre-defined UB test value
static double ub_table[UB_NUMBER] __attribute__((unused)) =
{
1.000000, 0.828427, 0.779763, 0.756828, 0.743492, 0.734772, 0.728627, 0.724062,
0.720538, 0.717735, 0.715452, 0.713557, 0.711959, 0.710593, 0.709412, 0.708381,
0.707472, 0.706666, 0.705946, 0.705298, 0.704713, 0.704182, 0.703698, 0.703254,
0.702846, 0.702469, 0.702121, 0.701798, 0.701497, 0.701217, 0.700954, 0.700709,
0.700478, 0.700261, 0.700056, 0.699863, 0.699681, 0.699508, 0.699344, 0.699188,
0.699040, 0.698898, 0.698764, 0.698636, 0.698513, 0.698396, 0.698284, 0.698176,
0.698073, 0.697974, 0.697879, 0.697788, 0.697700, 0.697615, 0.697533, 0.697455,
0.697379, 0.697306, 0.697235, 0.697166, 0.697100, 0.697036, 0.696974, 0.696914
};

/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks __attribute__((unused)), size_t num_tasks __attribute__((unused)))
{
    // Sort tasks in ascending order
    size_t i, j;
    task_t temp;
    for (i = 0; i < num_tasks; i++) {
        for (j = i + 1; j < num_tasks; j++) {
            if ((*tasks)[i].T > (*tasks)[j].T) {
                temp = (*tasks)[i];
                (*tasks)[i] = (*tasks)[j];
                (*tasks)[j] = temp;
            }
        }
    }

    // UB Test
    double C, T, B, sum, b_value, ub_line;
    for (i = 0, sum = 0; i < num_tasks; i++) {
        C = (double)(*tasks)[i].C;
        T = (double)(*tasks)[i].T;
        B = (double)(*tasks)[i].B;
        sum += C / T;
        b_value = B / T;
        ub_line = i < UB_NUMBER ? ub_table[i] : ub_min;
        if (sum + b_value > ub_line) return 0;
    }

    return 1;
}
