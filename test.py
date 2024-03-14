# This Python file uses the following encoding: utf-8
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

def plot_gantt_chart(cores, tasks, time_length):
    n_colors = len(tasks)
    colors = sns.color_palette("dark", n_colors=n_colors)
    x = np.arange(len(cores))
    y = [0] * len(cores)
    fig, ax = plt.subplots(figsize=(12, 6+len(cores)*0.5))
    ax.barh(x, y)
    plt.xlim(0, time_length+5)
    plt.yticks(x, cores)
    plt.xlabel('Time')
    plt.ylabel('Core')
    plt.title('Scheduling Gantt Chart')
    for i, task in enumerate(tasks):
        core_index = cores.index(task['core'])
        task_start = task['start']
        task_duration = task['duration']
        text_x = task_start + task_duration/2
        text_y = core_index-0.01
        ha = 'center'
        if task_duration >= 3:
            ax.text(text_x, text_y, task['name'], ha=ha, va='center')
        elif task_duration > 1:
            ax.text(text_x, text_y, "T"+task['name'].split('Task')[1], ha=ha, va='center')
        else:
            ax.text(text_x, text_y, task['name'].split('Task')[1], ha=ha, va='center')
        color = colors[i]
        ax.barh(core_index, task_duration, height=0.1, left=task_start, color=color, alpha=0.4)
        ax.barh(core_index, task_duration, height=0.1, left=task_start, fill=False, edgecolor='black', linewidth=2)

    # Add a vertical dashed line and label for the last completed task
    # last_task = tasks[-1]
    # last_task_end = last_task['start'] + last_task['duration']
    # ax.axvline(x=last_task_end, color='black', linestyle='dashed')
    # ax.text(last_task_end, -0.5, str(last_task_end), ha='center', va='center')

    plt.xticks(np.arange(0, time_length+10, 5))

    # 添加图例
    handles = [plt.Rectangle((0,0),1,1, color=color, alpha=0.5) for color in colors]
    labels = [f'Task {i}' for i in range(len(tasks))]
    ax.legend(handles, labels, loc='upper right')
    plt.tight_layout()
    plt.show()

cores = ['Core0', 'Core1','Core2']
tasks = [
    {'name': 'Task0', 'core': 'Core0', 'start': 0, 'duration': 1},
    {'name': 'Task1', 'core': 'Core0', 'start': 1, 'duration': 5},
    {'name': 'Task2', 'core': 'Core1', 'start': 1, 'duration': 5},
    {'name': 'Task3', 'core': 'Core0', 'start': 10, 'duration': 2},
    {'name': 'Task4', 'core': 'Core1', 'start': 20, 'duration': 3},
    {'name': 'Task5', 'core': 'Core0', 'start': 30, 'duration': 4},
    {'name': 'Task6', 'core': 'Core2', 'start': 30, 'duration': 5},
]

time_length = 35

#plot_gantt_chart(cores, tasks, time_length)
def test():
    plot_gantt_chart(cores, tasks, time_length)
