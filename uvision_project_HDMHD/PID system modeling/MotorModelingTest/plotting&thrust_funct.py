import os
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import linregress


# Check if the file exists in the current directory
file_path = 'C:/Users/Hugo/Documents/GitHub/PropellerArm/uvision_project_HDMHD/PID system modeling/MotorModelingTest/data_motor_testbench.txt'
if not os.path.isfile(file_path):
    print(f"Error: File '{file_path}' not found in the current directory.")
else:
    # Read data from the text file
    data = []
    try:
        with open(file_path, 'r') as file:
            for line in file:
                parts = line.strip().split(';')
                if len(parts) == 3:
                    data.append((int(parts[0]), int(parts[1]), int(parts[2])))
    except Exception as e:
        print(f"Error reading file: {e}")
    else:
        # Separate the data into two lists: x and y
        throttle = [item[0] for item in data]
        thrust = [item[1] for item in data]
        ampere_cons = [item[2] for item in data]
        # Compute the linear regression
        slope, intercept, r_value, p_value, std_err = linregress(
            throttle[:70], thrust[:70])
        equation = f"y = {slope:.2f}x + {intercept:.2f}"
        print(equation)
        print(throttle)
        print(0.17*1350 + -176.21)
        # Plot the original data
        plt.figure(figsize=(10, 6))
        plt.scatter(throttle, thrust, color='blue', label='Original Data')

        # Plot the linear regression line
        regression_line = [slope * x + intercept for x in throttle]
        plt.plot(throttle, regression_line, color='red',
                 label=f"Linear Regression Line:\n y={slope: .2f}x + {intercept: .2f}")
        plt.legend()
        plt.grid(True)
        plt.xlabel('PWM Throttle in us')
        plt.ylabel('Thrust generated in g')
        plt.title('Thrust generated in function of the command PWM in us')
        plt.grid(True)

        # Plot the data
        plt.figure(figsize=(10, 6))
        plt.plot(throttle, [thr / ampere for thr, ampere in zip(throttle, ampere_cons)],
                 marker='o', linestyle='-', color='r')
        plt.xlabel('PWM Throttle in us')
        plt.ylabel('Motor efficiency in g/A')
        plt.title('Motor efficiency in function of the command PWM in us')
        plt.grid(True)
        plt.show()
