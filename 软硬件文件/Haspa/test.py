import random

def uunifast(total_utilization, num_tasks):
    utilizations = []
    sum_utilization = total_utilization

    for i in range(1, num_tasks):
        next_sum_utilization = sum_utilization * random.random() ** (1.0 / (num_tasks - i))
        utilization = sum_utilization - next_sum_utilization
        utilizations.append(utilization)
        sum_utilization = next_sum_utilization

    utilizations.append(sum_utilization)
    return utilizations

# Example usage
total_utilization = 1.0  # Total utilization of the task set
num_tasks = 5  # Number of tasks in the set

utilizations = uunifast(total_utilization, num_tasks)
print("Generated Utilizations:", utilizations)