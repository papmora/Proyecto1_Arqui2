import os

def getCores():
    try:
        # Get Number of Cores
        cores = os.sysconf(os.sysconf_names['SC_NPROCESSORS_ONLN'])
        print("Cores:", cores)
        return int(cores)
    except Exception as e:
        print("Error:", e)
        return None

def main_menu():

    # System Information
    os.system(f"./systemInfo benchmarkOne")

    cores = getCores()

    while True:
        print("Options:")
        print("1. Execute Single-Threading Benchmark")
        print("2. Execute Multi-Threading Benchmark")
        print("3. Exit")
        
        choice = input("Type an option and press ENTER: ")

        if choice == '1':
            os.system(f"./controller benchmarkTwo 1")

        elif choice == '2':
            os.system(f"./controller benchmarkTwo {cores}")

        elif choice == '3':
            break

        else:
            print("Opción no válida. Intente de nuevo.")

if __name__ == "__main__":
    main_menu()
