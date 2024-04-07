import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

def plot_gantt_chart(cores, fpgas, tasks, time_length):
    n_colors = len(tasks)
    colors = sns.color_palette("husl", n_colors=n_colors)
    x = np.arange(len(cores) + len(fpgas))
    y = [0] * (len(fpgas) + len(cores))
    fig, ax = plt.subplots(figsize=(12, 6+len(cores)*0.1))
    ax.barh(x, y)
    plt.xlim(0, time_length+5)
    plt.yticks(x, fpgas+cores)
    # plt.xlabel('Time')
    # plt.ylabel('Resource')
    # plt.title('Scheduling Gantt Chart')

    core_index = len(cores)  # Starting index for FPGA resources
    fpga_index = len(fpgas)

    for i, task in enumerate(tasks):
        if task['core'] in cores:
            resource_index = fpga_index+cores.index(task['core'])
        elif task['core'] in fpgas:
            resource_index = fpgas.index(task['core'])
        else:
            continue

        task_start = task['start']
        task_duration = task['duration']
        text_x = task_start + task_duration/2
        text_y = resource_index-0.01
        ha = 'center'

        # if task_duration >= 2:
        #     ax.text(text_x, text_y, task['name'], ha=ha, va='center')
        # elif task_duration > 1:
        #     ax.text(text_x, text_y, "T"+task['name'].split('Task')[1], ha=ha, va='center')
        # else:
        #     ax.text(text_x, text_y, task['name'].split('Task')[1], ha=ha, va='center')

        color = colors[i]
        if task['core'] in fpgas and task['fpga_index'] is not None:
            resource_index -= 0.1*task['fpga_index']  # Adjust resource index for overlapping tasks under Fpga0
            text_y=resource_index-0.01
            ax.barh(resource_index, task_duration, height=0.1, left=task_start, color=color, alpha=0.4)
            ax.barh(resource_index, task_duration, height=0.1, left=task_start, fill=False, edgecolor='black', linewidth=2)
            
            # if task_duration >= 2:
            ax.text(text_x, text_y, f'\u03C4'+task['name'].split("Task")[1], ha=ha, va='center')
            # elif task_duration > 1:
            #     ax.text(text_x, text_y, "T"+task['name'].split('Task')[1], ha=ha, va='center')
            # else:
            #     ax.text(text_x, text_y, task['name'].split('Task')[1], ha=ha, va='center')
        else:
            ax.barh(resource_index, task_duration, height=0.1, left=task_start, color=color, alpha=0.4)
            ax.barh(resource_index, task_duration, height=0.1, left=task_start, fill=False, edgecolor='black', linewidth=2)
            
            # if task_duration >= 2:
            ax.text(text_x, text_y, f'\u03C4'+task['name'].split("Task")[1], ha=ha, va='center')
            # elif task_duration > 1:
            #     ax.text(text_x, text_y, "T"+task['name'].split('Task')[1], ha=ha, va='center')
            # else:
            #     ax.text(text_x, text_y, task['name'].split('Task')[1], ha=ha, va='center')

        print("graphing"+str(i))

    plt.xticks(np.arange(0, time_length+5, 1))

    handles = [plt.Rectangle((0,0),1,1, color=color, alpha=0.5) for color in colors]
    labels = [f'\u03C4{task["name"].split("Task")[1]}' for task in tasks]
    handles_labels = sorted(zip(handles, labels), key=lambda x: int(x[1].split("\u03C4")[1]))
    handles, labels = zip(*handles_labels)
    ax.legend(handles, labels, loc='upper right')
    ax.grid(color='gray', linestyle=':', linewidth=0.5)
    plt.tight_layout()

    print("start 2 save")
    plt.savefig('output.svg', format='svg')
    print("end  save")
    plt.show()

def main():
    with open('input2.txt', 'r') as file:
        lines = file.readlines()

    cores = lines[0].strip().split(',')
    fpgas = lines[1].strip().split(',')
    tasks = []
    time_length = int(lines[2])

    for line in lines[3:]:
        name, core, start, duration, *fpga_index = line.strip().split(',')
        tasks.append({'name': name, 'core': core, 'start': int(start), 'duration': int(duration), 'fpga_index': int(fpga_index[0]) if fpga_index else None})

    tasks.sort(key=lambda x: x['fpga_index'] if x['fpga_index'] is not None else float('inf'))
    
    print(tasks)
    
    plot_gantt_chart(cores, fpgas, tasks, time_length)

if __name__ == '__main__':
    main()