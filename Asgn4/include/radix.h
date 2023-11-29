/*
 * Copyright (C) 2018-2022 David C. Harrison. All rights reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

/******************  DO NOT MODIFY THIS FILE ****************************
 *
 * It is not included in the subission archive ceated by 'make submit' .
 *
 * If you modify it and your code relies on those modifications, your code
 * will not compile in the automated test harness and will be unable to
 * execute any tests.
 *
 * To put it another way, if you modify this file, you will get a big fat
 * ZERO on this assignment.
 *
 ************************************************************************/

#include <vector>
#include <functional>

/*
 * Parallel Radix Sort with support for Most Significant Digit sorting only.
 *
 * If allowed to use ten cores or more, should sort a large set of randomly
 * selected unsigned integers apromixmately ten times faster than a single
 * threaded sort of the same random set.
 *
 * Whilst the choice of sorting algorithnm is a matter or personal choice,
 * a bucket sort where each bucket is sorted in a different thread will almost 
 * certainly deliver the best results.
 */
class ParallelRadixSort {
public:
    /*
     * Perform an in-place Most Significant Digit Radix Sort on each list of
     * unsigned integers in LISTS using no more that CORES cpu cores.
     */
    void msd(
        std::vector<std::reference_wrapper<std::vector<unsigned int>>> &lists, 
        const unsigned int cores);
};
