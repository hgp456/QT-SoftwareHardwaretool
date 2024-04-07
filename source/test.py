import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

def plot_gantt_chart(cores, tasks, time_length):
    n_colors = len(tasks)
    colors = sns.color_palette("husl", n_colors=n_colors)
    x = np.arange(len(cores))
    y = [0] * len(cores)
    fig, ax = plt.subplots(figsize=(12, 6+len(cores)*0.5))
    ax.barh(x, y)
    plt.xlim(0, time_length+5)
    plt.yticks(x, cores)
    plt.xlabel('Time')
    plt.ylabel('Core')
    plt.title('Scheduling Gannt Chart')
    for i, task in enumerate(tasks):
        core_index = cores.index(task['core'])
        task_start = task['start']
        task_duration = task['duration']
        text_x = task_start + task_duration/2
        text_y = core_index-0.01
        ha = 'center'
        if task_duration >= 2:
            ax.text(text_x, text_y, task['name'], ha=ha, va='center')
        elif task_duration > 1:
            ax.text(text_x, text_y, "T"+task['name'].split('Task')[1], ha=ha, va='center')
        else:
            ax.text(text_x, text_y, task['name'].split('Task')[1], ha=ha, va='center')
        color = colors[i]
        ax.barh(core_index, task_duration, height=0.1, left=task_start, color=color, alpha=0.4)
        ax.barh(core_index, task_duration, height=0.1, left=task_start, fill=False, edgecolor='black', linewidth=2)
        print("graphing"+str(i))
    plt.xticks(np.arange(0, time_length+10, 1))
    
    # handles = [plt.Rectangle((0,0),1,1, color=color, alpha=0.5) for color in colors]
    # # labels = [f'Task {i}' for i in range(len(tasks))]
    # labels = [f'Task {task["name"].split("Task")[1]}' for task in tasks]
    # ax.legend(handles, labels, loc='upper right')
 
    handles = [plt.Rectangle((0,0),1,1, color=color, alpha=0.5) for color in colors]
    labels = [f'Task {task["name"].split("Task")[1]}' for task in tasks]

    # Combine handles and labels, then sort based on handles
    handles_labels = sorted(zip(handles, labels), key=lambda x: int(x[1].split("Task")[1]))

    # Unzip the sorted handles and labels
    handles, labels = zip(*handles_labels)

    # Create the legend
    ax.legend(handles, labels, loc='upper right')

    
    ax.grid(color='gray', linestyle=':', linewidth=0.5)

    plt.tight_layout()
    print("start 2 save")
    plt.savefig('output.svg', format='svg')
    print("end  save")
    plt.show()

def main():
    # 从txt文件中读取信息
    with open('input.txt', 'r') as file:
        lines = file.readlines()
    
    cores = lines[0].strip().split(',')
    tasks = []
    time_length = int(lines[1])
    for line in lines[2:]:
        name, core, start, duration = line.strip().split(',')
        tasks.append({'name': name, 'core': core, 'start': int(start), 'duration': int(duration)})
    
    
    plot_gantt_chart(cores, tasks, time_length)

if __name__ == '__main__':
    main()