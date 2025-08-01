# Statistical Simulation Toolkit

This is a command-line tool written in C designed to explore the principles of probability and statistics by simulating dice rolls. The program allows users to define custom experiments (e.g., rolling multiple, multi-sided dice) and rigorously compares the simulated (empirical) results against the true theoretical probabilities.

The core feature is the implementation of a **Chi-Squared (χ²) Goodness-of-Fit test**, which provides a quantitative measure of how well the simulation's outcome matches the expected mathematical model, demonstrating the Law of Large Numbers in action.

---

## Features

* **User-Defined Simulations:** Configure experiments by specifying:
    * The number of dice to roll per trial.
    * The number of sides on each die (e.g., 6 for d6, 20 for d20).
    * The total number of trials to simulate (from one to billions).
* **Theoretical Probability Calculation:** Automatically calculates the exact mathematical probability for every possible sum.
* **Empirical Simulation:** Runs a high-speed simulation using a pseudo-random number generator to gather experimental data.
* **Side-by-Side Comparison:** Displays the results in a clear, formatted table showing the expected counts versus the observed counts from the simulation.
* **Visual Distribution Chart:** Includes a simple text-based bar chart to visually represent the frequency distribution of the outcomes.
* **Chi-Squared (χ²) Goodness-of-Fit Test:** Provides a formal statistical score to measure how well the observed data fits the theoretical predictions.

---

## How to Compile and Run

This program is written in standard C and requires a C compiler like `gcc`.

1.  **Compile the code:**
    Open your terminal, navigate to the directory containing the source file (`main.c`), and run the following command. The `-lm` flag is crucial for linking the math library.

    ```bash
    gcc main.c -o simulation -lm
    ```

2.  **Run the executable:**
    Execute the compiled program from your terminal.

    ```bash
    ./simulation
    ```

The program will then prompt you to enter the simulation parameters.

---

## Technical Deep Dive

This toolkit is built on three key technical and mathematical pillars: a recursive combinatorial calculator for theoretical probabilities, a pseudo-random simulation engine, and a statistical validation test.

### 1. Theoretical Probability Calculation

The probability of rolling a specific sum with multiple dice is not uniform. To find the true probability, we must first solve a combinatorics problem: "In how many ways can *N* dice, each with *S* sides, sum to a target value *T*?"

This is solved using the recursive function `long long count_sum_combinations_recursive(int dice_remaining, int target_sum, int num_sides)`.

* **Recursive Strategy:** The function solves the problem by breaking it down into smaller, identical subproblems. To find the number of ways 3d6 can sum to 10, it considers rolling a 1 on the first die and then recursively asks, "In how many ways can 2d6 sum to 9?". It does this for every possible value of the first die (1 through 6) and sums the results.
* **Base Cases:** The recursion terminates when one of two conditions is met:
    1.  **Success:** `dice_remaining == 0` and `target_sum == 0`. This means we have successfully found one valid combination.
    2.  **Failure (Pruning):** The `target_sum` becomes impossible to achieve with the `dice_remaining` (e.g., the target is too high or too low). This path returns 0, pruning the search tree.

Once the number of combinations for a sum is known, the probability is calculated as:
$$P(\text{sum}) = \frac{\text{Number of Ways to Make Sum}}{\text{Total Possible Outcomes}}$$
where Total Possible Outcomes = $(\text{Number of Sides})^{\text{Number of Dice}}$.

### 2. The Simulation Engine

The simulation is performed in the `run_simulation()` function. It uses the `rand()` function from `<stdlib.h>`, seeded by `srand(time(NULL))` for a different sequence on each run.

The core of the simulation is the line:
```c
current_sum += (rand() % num_sides) + 1;
````

  * `rand() % num_sides` generates a pseudo-random integer between `0` and `num_sides - 1`.
  * `+ 1` shifts this range to `1` to `num_sides`, correctly modeling a physical die roll.

The results are aggregated in an array, `observed_counts`, which stores the frequency of each sum over millions of trials.

### 3\. The Chi-Squared (χ²) Goodness-of-Fit Test

This is the key statistical test that validates our simulation. It quantifies the deviation between the observed frequencies from the simulation and the expected frequencies from our theoretical model.

The formula is:
$$\chi^2 = \sum \frac{(O - E)^2}{E}$$
Where:

  * **O**: The **Observed** frequency for a given sum (from `observed_counts`).
  * **E**: The **Expected** frequency for that same sum (from `expected_counts`).

**Interpretation of the $\\chi^2$ Statistic:**

  * A **low $\\chi^2$ value** indicates a small deviation between the observed and expected results, meaning our simulation is a "good fit" for the theoretical model.
  * A **high $\\chi^2$ value** indicates a large deviation.
  * According to the **Law of Large Numbers**, as the number of trials increases, the observed frequencies should converge on the expected frequencies. Therefore, the $\\chi^2$ value should get progressively smaller (relative to the number of trials), providing mathematical proof of this principle.

-----

## Sample Output

Running the program for **2 dice with 6 sides for 1,000,000 trials** produces the following output:
```
--- Simulation Results for 1000000 trials of rolling 2d6 ---
=================================================================================
| Sum  | Expected Count     | Observed Count     | Distribution Bar
|------|--------------------|--------------------|--------------------------------
| 2    | 27777.78           | 27769              | ##########
| 3    | 55555.56           | 55604              | ####################
| 4    | 83333.33           | 83129              | ##############################
| 5    | 111111.11          | 111234             | ########################################
| 6    | 138888.89          | 138850             | ##################################################
| 7    | 166666.67          | 166589             | ############################################################
| 8    | 138888.89          | 139021             | ##################################################
| 9    | 111111.11          | 111039             | ########################################
| 10   | 83333.33           | 83444              | ##############################
| 11   | 55555.56           | 55531              | ####################
| 12   | 27777.78           | 27790              | ##########
=================================================================================
Statistical Analysis:
  - Chi-Squared (χ²) Statistic: 1.2515
  - Degrees of Freedom: 10

Interpretation: A smaller Chi-Squared value indicates a better fit between
the observed results and the theoretical probabilities. As the number of
trials increases, this value should approach the degrees of freedom.
```

## License

This project was made as part of our submission for CSE160F Lab Project and is licensed under the MIT License. See the `LICENSE` file for details.

## Team
[<img src="https://github.com/0xSalik.png" width="40" height="40" alt="Salik Khan">](https://github.com/0xSalik)
[<img src="https://github.com/RuhaabAbroo.png" width="40" height="40" alt="Ruhaab Abroo">](https://github.com/RuhaabAbroo)
