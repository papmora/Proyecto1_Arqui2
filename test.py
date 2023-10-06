import os
import subprocess
    
def getThreadsPerCore():
    # Run the lscpu command and capture its output
    command = "lscpu | grep 'Thread(s) per core'"
    output = subprocess.check_output(command, shell=True, text=True)

    # Parse the value from the output
    lines = output.strip().split('\n')
    if lines:
        line = lines[0]
        parts = line.split(':')
        if len(parts) == 2:
            threads_per_core = int(parts[1].strip())
            # print("Threads per core:", threads_per_core)
            return threads_per_core
    return None

def getCoresPerSocket():
    # Run the lscpu command and capture its output
    command = "lscpu | grep 'Core(s) per socket'"
    output = subprocess.check_output(command, shell=True, text=True)

    # Parse the value from the output
    lines = output.strip().split('\n')
    if lines:
        line = lines[0]
        parts = line.split(':')
        if len(parts) == 2:
            cores_per_socket = int(parts[1].strip())
            # print("Cores per socket:", cores_per_socket)
            return cores_per_socket
    return None

def main_menu():

    # System Information
    os.system(f"./systemInfo benchmarkOne")

    threadsPerCore = getThreadsPerCore()
    coresPerSocket = getCoresPerSocket()
    totalThreads = threadsPerCore * coresPerSocket
    while True:
        print("Options:")
        print("1. Execute Single-Threading Benchmark")
        print("2. Execute Multi-Threading Benchmark")
        print("3. Exit")
        
        choice = input("Type an option and press ENTER: ")

        if choice == '1':
            os.system(f"./controller benchmarkTwo 1")

        elif choice == '2':
            os.system(f"./controller benchmarkTwo {totalThreads}")

        elif choice == '3':
            break

        else:
            print("Opción no válida. Intente de nuevo.")

if __name__ == "__main__":
    main_menu()
