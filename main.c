#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// --- Function Prototypes ---

void get_simulation_parameters(int* num_dice, int* num_sides, long long* num_trials);
void calculate_theoretical_counts(int num_dice, int num_sides, long long num_trials, double expected_counts[]);
long long count_sum_combinations_recursive(int dice_remaining, int target_sum, int num_sides);
void run_simulation(int num_dice, int num_sides, long long num_trials, long long observed_counts[]);
double perform_chi_squared_test(long long observed_counts[], double expected_counts[], int min_sum, int max_sum);
void print_results_report(long long observed_counts[], double expected_counts[], int num_dice, int num_sides, long long num_trials, double chi_squared_stat);


int main() {
    srand(time(NULL));

    int num_dice, num_sides;
    long long num_trials;

    printf("--- Statistical Simulation Toolkit ---\n");
    printf("This tool simulates rolling multiple dice and compares the results\n");
    printf("to the theoretical probabilities using a Chi-Squared test.\n\n");

    get_simulation_parameters(&num_dice, &num_sides, &num_trials);
    int max_sum = num_dice * num_sides;
    int min_sum = num_dice;

    long long* observed_counts = calloc(max_sum + 1, sizeof(long long));
    double* expected_counts = calloc(max_sum + 1, sizeof(double));

    if (!observed_counts || !expected_counts) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }
    printf("\nCalculating theoretical probabilities...\n");
    calculate_theoretical_counts(num_dice, num_sides, num_trials, expected_counts);

    printf("Running simulation with %lld trials...\n", num_trials);
    run_simulation(num_dice, num_sides, num_trials, observed_counts);

    double chi_squared_stat = perform_chi_squared_test(observed_counts, expected_counts, min_sum, max_sum);
    print_results_report(observed_counts, expected_counts, num_dice, num_sides, num_trials, chi_squared_stat);
    free(observed_counts);
    free(expected_counts);

    return 0;
}

/**
 * @brief Prompts the user for simulation parameters with input validation.
 */
void get_simulation_parameters(int* num_dice, int* num_sides, long long* num_trials) {
    printf("Enter simulation parameters:\n");
    do {
        printf("  - Number of dice to roll (e.g., 2): ");
        scanf("%d", num_dice);
    } while (*num_dice <= 0 || *num_dice > 10); // Cap dice to prevent extreme calculations

    do {
        printf("  - Number of sides on each die (e.g., 6): ");
        scanf("%d", num_sides);
    } while (*num_sides <= 1 || *num_sides > 100);

    do {
        printf("  - Total number of trials (e.g., 1000000): ");
        scanf("%lld", num_trials);
    } while (*num_trials <= 0);
}

/**
 * @brief A recursive helper to find the number of ways to achieve a target sum.
 */
long long count_sum_combinations_recursive(int dice_remaining, int target_sum, int num_sides) {
    if (target_sum < dice_remaining || target_sum > dice_remaining * num_sides) {
        return 0;
    }
    if (dice_remaining == 0) {
        return (target_sum == 0) ? 1 : 0;
    }

    long long total_combinations = 0;
    for (int i = 1; i <= num_sides; i++) {
        total_combinations += count_sum_combinations_recursive(dice_remaining - 1, target_sum - i, num_sides);
    }
    return total_combinations;
}

/**
 * @brief Calculates the theoretical expected count for each possible sum.
 */
void calculate_theoretical_counts(int num_dice, int num_sides, long long num_trials, double expected_counts[]) {
    int min_sum = num_dice;
    int max_sum = num_dice * num_sides;
    double total_possible_outcomes = pow(num_sides, num_dice);

    for (int sum = min_sum; sum <= max_sum; sum++) {
        long long ways_to_make_sum = count_sum_combinations_recursive(num_dice, sum, num_sides);
        double probability = ways_to_make_sum / total_possible_outcomes;
        expected_counts[sum] = probability * num_trials;
    }
}

/**
 * @brief Runs the die-rolling simulation to get observed counts.
 */
void run_simulation(int num_dice, int num_sides, long long num_trials, long long observed_counts[]) {
    for (long long i = 0; i < num_trials; i++) {
        int current_sum = 0;
        for (int j = 0; j < num_dice; j++) {
            current_sum += (rand() % num_sides) + 1;
        }
        observed_counts[current_sum]++;
    }
}

/**
 * @brief Performs a Chi-Squared test to measure "goodness of fit".
 * A smaller Chi-Squared value indicates a better fit to the theoretical model.
 */
double perform_chi_squared_test(long long observed_counts[], double expected_counts[], int min_sum, int max_sum) {
    double chi_squared_statistic = 0.0;
    for (int sum = min_sum; sum <= max_sum; sum++) {
        if (expected_counts[sum] > 0) {
            double difference = observed_counts[sum] - expected_counts[sum];
            chi_squared_statistic += (difference * difference) / expected_counts[sum];
        }
    }
    return chi_squared_statistic;
}

/**
 * @brief Prints the final comparison report in a formatted table.
 */
void print_results_report(long long observed_counts[], double expected_counts[], int num_dice, int num_sides, long long num_trials, double chi_squared_stat) {
    int min_sum = num_dice;
    int max_sum = num_dice * num_sides;
    int degrees_of_freedom = max_sum - min_sum;

    printf("\n--- Simulation Results for %lld trials of rolling %dd%d ---\n", num_trials, num_dice, num_sides);
    printf("=================================================================================\n");
    printf("| %-4s | %-18s | %-18s | %-s\n", "Sum", "Expected Count", "Observed Count", "Distribution Bar");
    printf("|------|--------------------|--------------------|--------------------------------\n");

    for (int sum = min_sum; sum <= max_sum; sum++) {
        printf("| %-4d | %-18.2f | %-18lld | ", sum, expected_counts[sum], observed_counts[sum]);

        double max_expected = 0;
        for(int i = min_sum; i <= max_sum; i++) {
            if(expected_counts[i] > max_expected) max_expected = expected_counts[i];
        }

        int bar_width = 0;
        if(max_expected > 0) {
             bar_width = (int)((observed_counts[sum] / max_expected) * 30.0); // 30 is the max bar width
        }

        for (int j = 0; j < bar_width; j++) {
            printf("#");
        }
        printf("\n");
    }
    printf("=================================================================================\n");
    printf("Statistical Analysis:\n");
    printf("  - Chi-Squared (χ²) Statistic: %.4f\n", chi_squared_stat);
    printf("  - Degrees of Freedom: %d\n", degrees_of_freedom);
    printf("\nInterpretation: A smaller Chi-Squared value indicates a better fit between\n");
    printf("the observed results and the theoretical probabilities. As the number of\n");
    printf("trials increases, this value should approach the degrees of freedom.\n");
}
