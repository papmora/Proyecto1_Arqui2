import os

def main_menu():
    while True:
        print("Menú:")
        print("1. Ejecutar ./programa1")
        print("2. Ejecutar ./programa2")
        print("3. Salir")
        
        choice = input("Elija una opción: ")

        if choice == '1':
            os.system("./benchmark bench1_c 5 4")
        elif choice == '2':
            os.system("./programa2")
        elif choice == '3':
            break
        else:
            print("Opción no válida. Intente de nuevo.")

if __name__ == "__main__":
    main_menu()
