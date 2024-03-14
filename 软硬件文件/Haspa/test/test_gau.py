def read_from_file(file_name):
    input_data = []
    with open(file_name, 'r') as file:
        for line in file:
            input_data.append(line.strip().split())
    return input_data

def write_to_file(data, file_name,num_rows,swtime,hwtime,lut,hardwareconstraint,line):
    with open(file_name, 'w') as file:
        file.write('1' + '\n')
        file.write("\n")
        file.write('0' + '\n')
        file.write("\n")
        file.write(str(num_rows))
        file.write("\n\n")
        file.write(swtime)
        file.write("\n\n")
        file.write(hwtime)
        file.write("\n\n")
        file.write(lut)
        file.write("\n\n")
        file.write(str(hardwareconstraint))
        file.write("\n\n")
        lines=",".join(line)
        file.write(lines)
        file.write("\n\n")
        for line in data:
            modified_line = ['0' if val == '-1' else val for val in line]
            file.write(','.join(modified_line) + '\n')

def process_data(input_data, start_index):
    num_rows = int(input_data[start_index][0])
    sw_time =  ','.join(input_data[start_index+2*num_rows+3])
    sw_time = sw_time.replace('.0', '')
    hw_time = ','.join(input_data[start_index+2*num_rows+5])
    hw_time = hw_time.replace('.0', '')
    lut     = ','.join(input_data[start_index+2*num_rows+5+9])
    hardwareconstaint =  int(input_data[start_index][2])
    data = input_data[start_index+2:start_index+num_rows+2]
    # data = data.replace("-1","0")
    return num_rows, data,sw_time,hw_time,lut,hardwareconstaint

def main():
    input_file = 'gau13.txt'
    input_num=int(input_file.split(".")[0].split("gau")[1])
    print(input_num)
    input_data = read_from_file(input_file)

   
    count = 1
    start_processing = True
    start_index = 0
    
    hardwareconstraintsum=0
    
    num_rows = int(input_data[start_index][0])
    # if start_processing:
    #     print(start_index)
    #     num_rows, processed_data = process_data(input_data, start_index)
    #     output_file = f'output{count}.txt'
    #     write_to_file(processed_data, output_file)
        
        
    for i, line in enumerate(input_data):
        if line == ['0'] * int((num_rows)):
            if start_processing:
                num_rows, processed_data,sw_time,hw_time,lut,hardwareconstraint = process_data(input_data, start_index)
                output_file = 'CORE2FPGANGAUBenchMark/'+'num_'+str(input_num)+f'/output{count}.txt'
                write_to_file(processed_data, output_file,num_rows,sw_time,hw_time,lut,hardwareconstraint,line)
                count += 1
                hardwareconstraintsum+=hardwareconstraint
            start_processing = True
            start_index = i + 2

    print(hardwareconstraintsum)
    print(count-1)
    print("hardwareconstainavg=" + str(hardwareconstraintsum/(count-1)) )


if __name__ == '__main__':
    main()