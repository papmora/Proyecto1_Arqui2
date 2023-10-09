import time
import tkinter as tk
import os
from PIL import Image,ImageTk,ImageFont
import tkinter.font as tkFont
import datetime
import matplotlib.pyplot as plt
import subprocess


root = tk.Tk()
selectedTest=None
test_buttons={}
compareStatus={"test1_s":0,"test1_m":0,"test1_sol1":0,"test1_sol2":0,"test2_s":0,"test2_m":0,"test2_sol1":0,"test2_sol2":0}
equivalences={"test1_s":"benchmarkOne","test1_m":"benchmarkOne","test1_sol1":"benchmarkOneS1","test1_sol2":"benchmarkOneS2","test2_s":"benchmarkTwo","test2_m":"benchmarkTwo","test2_sol1":"benchmarkTwoS1","test2_sol2":"benchmarkTwoS2"}
compareButtons=[]
def setCompare(i):

    global compareStatus

    colors=["#1f1d20","#47434c"]

    tests=available_for_compare()

    test=tests[i]

    compareStatus[test]=not compareStatus[test]
    compare_button=compareButtons[i]
    compare_button.config(bg=colors[i%2])
    if(compareStatus[test]):

        compare_button.config(bg="#062239")

def doCompare():

    testCompare=[]

    plt.clf()

    for key in list(compareStatus.keys()):

        if(compareStatus[key]):

            testCompare+=[key]

    plt.subplot(3, 1, 1)

    for test in testCompare:

        inputs = parseResults("output_" + test + ".txt")

        iterations = range(len(inputs[list(inputs.keys())[0]]))

        plt.subplot(3, 1, 1)
        plt.plot(iterations, inputs["L1-dcache-load-misses"], label=test)
        plt.title("Iterations Vs Cache Misses")

        plt.subplot(3, 1, 2)
        plt.plot(iterations, inputs["L1-dcache-loads"],label=test)
        plt.title("Iterations Vs Cache Loads")

        plt.subplot(3, 1, 3)
        plt.plot(iterations, inputs["time"],label=test)
        plt.title("Iterations Vs Time per Iteration")

    plt.tight_layout()
    plt.legend()
    plt.savefig("comparison.png")

    compare_window = tk.Toplevel(root)

    compare_window.title("False Sharing Analysis Tool")
    compare_window.geometry("700x565")
    compare_window.configure(bg="#28262b")
    compare_window.resizable(width=False, height=False)
    # compare_window.wm_attributes('-type', 'splash')
    compare_window.configure(borderwidth=2, relief="solid")

    window_tk_font = tk.font.Font(family="Share Tech Mono", size=12)

    benchmark_label = tk.Label(compare_window, text="Comparison Results", font=window_tk_font, fg="#55ed64",bg="#28262b")
    benchmark_label.place(x=0, y=1)

    graph_panel = tk.Frame(compare_window, bg="#353238", width=696, height=536)
    graph_panel.place(x=0, y=25)

    image = Image.open("comparison.png")
    graph_image = ImageTk.PhotoImage(image)

    graph_image_label = tk.Label(graph_panel, image=graph_image, width=640, height=480)
    graph_image_label.image = graph_image
    graph_image_label.place(x=30, y=30)

def kill_compare():
    global compare_window,compareButtons

    for key in list(compareStatus.keys()):
        compareStatus[key]=0

    compareButtons = []
    compare_window.destroy()

def setTest(test):

    global execute_button,selectedTest,test_results_button

    selectedTest=test

    if(test in list(compareStatus.keys())):

        execute_button.config(state="normal")

    else:

        execute_button.config(state="disabled")

    for key in list(test_buttons.keys()):

        if(key == test):

            test_buttons[key].config(bg="#55ed64", fg="#062239")
        else:

            test_buttons[key].config(fg="#55ed64", bg="#062239")

    exists_data=parseResults("output_"+test+".txt")

    if(exists_data!=-1):

        test_results_button.config(state="normal")

    else:

        test_results_button.config(state="disabled")

def available_for_compare():

    available=[]

    for test in equivalences.keys():

        exists_data = parseResults("output_" + test + ".txt")

        if (exists_data != -1):

            available+=[test]

    return available



def getSpecs():

    global wait_window

    specs_path="specs.txt"

    showWaitWindow("Fetching system information, please wait")

    try:
        os.remove(specs_path)
    except:

        None

    file = open(specs_path, "w")
    file.close()

    os.system(f"./systemInfo t >> specs.txt")
    initial_time=datetime.datetime.now()
    fail=False

    while(True):

        current_time=datetime.datetime.now()

        seconds=(current_time-initial_time).seconds

        if (os.path.exists(specs_path)):
            break

        if(seconds>=60):

            fail=True
            break
    if(fail):

        destroyWaitWindow()
        showWaitWindow("Failed to retrieve information")
        time.sleep(2)
        destroyWaitWindow()
        test_window.deiconify()
    else:


        destroyWaitWindow()
        showWaitWindow("Spec can be found at: "+specs_path)
        time.sleep(2)
        destroyWaitWindow()
        os.system(f"gedit specs.txt")
        test_window.deiconify()



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
            #print("Threads per core:", threads_per_core)
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
            #print("Cores per socket:", cores_per_socket)
            return cores_per_socket
    return None


def executeBench():
    test_equivalence=equivalences[selectedTest]
    cores=1
    if(selectedTest in ["test1_m","test1_sol1","test1_sol2","test2_m","test2_sol1","test2_sol2"]):

        threadsPerCore = getThreadsPerCore()
        coresPerSocket = getCoresPerSocket()
        totalThreads = threadsPerCore * coresPerSocket

        cores=totalThreads

    output_path=f"output_{selectedTest}.txt"
    if not os.path.exists(output_path):
        file = open(output_path, "w")
        file.close()

    result=-1
    fail=False
    showWaitWindow("Executing Benchmark, please wait")

    initial_time=datetime.datetime.now()
    result=os.system(f"./controller {test_equivalence} {cores} {selectedTest}")

    while(result==-1):

        current_time = datetime.datetime.now()

        seconds = (current_time - initial_time).seconds

        if (seconds >= 60):
            fail = True
            break

    if (fail):

        destroyWaitWindow()
        showWaitWindow("Failed to execute benchmark")

    else:

        destroyWaitWindow()
        showWaitWindow("Test executed successfully")

    time.sleep(2)
    destroyWaitWindow()
    test_window.deiconify()

def clearOutput():

    os.system("find -type f -name '*output*' -exec rm {} \;")

def init():


    # Customize the window
    root.title("False Sharing Analysis Tool")
    root.geometry("800x300")
    root.configure(bg="#353238")
    root.resizable(width=False, height=False)
    #root.wm_attributes('-type', 'splash')
    root.configure(borderwidth=2, relief="solid")

    left_image = Image.open("Assets/Images/circuitry_left.jpg")  # Replace with your image file path
    left_panel_image = ImageTk.PhotoImage(left_image)

    left_panel = tk.Frame(root, bg="blue", width=150, height=290)
    left_panel.place(x=0,y=0)

    left_line = tk.Frame(root, bg="black", width=2, height=300)
    left_line.place(x=145, y=0)

    left_image_label = tk.Label(left_panel, image=left_panel_image, width=145, height=294)
    left_image_label.image = left_panel_image  # Keep a reference to the image to prevent it from being garbage collected
    left_image_label.pack()

    right_image = Image.open("Assets/Images/circuitry_right.jpg")  # Replace with your image file path
    right_panel_image = ImageTk.PhotoImage(right_image)

    right_panel = tk.Frame(root, bg="blue", width=150, height=290)
    right_panel.place(x=640, y=0)

    right_line = tk.Frame(root, bg="black", width=2, height=300)
    right_line.place(x=640, y=0)

    right_image_label = tk.Label(right_panel, image=right_panel_image, width=154, height=294)
    right_image_label.image = right_panel_image  # Keep a reference to the image to prevent it from being garbage collected
    right_image_label.pack()

    title_tk_font = tkFont.Font(family="Share Tech Mono", size=20)

    title_label_mp=tk.Label(root, text="MP Programming",font=title_tk_font, fg="#55ed64", bg="#353238")
    title_label_mp.place(x=290,y=20)

    title_label_and = tk.Label(root, text="&", font=title_tk_font,fg="#55ed64", bg="#353238")
    title_label_and.place(x=390, y=70)

    title_label_fs = tk.Label(root, text="False Sharing Analysis", font=title_tk_font,fg="#55ed64", bg="#353238")
    title_label_fs.place(x=240, y=120)

    beg_button=tk.Button(root, text="Begin", bg="#062239",borderwidth=1, relief="solid",font=title_tk_font,fg="#55ed64",highlightthickness=0, command=showTestsWindow)
    beg_button.place(x=340, y=190)

    # Start the Tkinter main event loop
    root.mainloop()

def showWaitWindow(message):

    global wait_window,test_window

    root.withdraw()
    test_window.withdraw()

    regular_tk_font = tk.font.Font(family="Share Tech Mono", size=12)

    wait_window = tk.Toplevel(root)
    wait_window.title("False Sharing Analysis Tool")
    wait_window.geometry("500x75")
    wait_window.configure(bg="#062239")
    wait_window.resizable(width=False, height=False)
    wait_window.wm_attributes('-type', 'splash')
    wait_window.configure(borderwidth=2, relief="solid")

    message_label = tk.Label(wait_window, text=message, font=regular_tk_font, fg="#55ed64", bg="#062239")
    message_label.pack(padx=10, pady=20)

    wait_window.lift()
    wait_window.update()

def destroyWaitWindow():

    global wait_window

    wait_window.destroy()
def showTestsWindow():

    global test_window,execute_button,test_results_button

    test_window = tk.Toplevel(root)
    root.withdraw()
    test_window.title("False Sharing Analysis Tool")
    test_window.geometry("800x400")
    test_window.configure(bg="#353238")
    test_window.resizable(width=False, height=False)
    #test_window.wm_attributes('-type', 'splash')
    test_window.configure(borderwidth=2, relief="solid")
    test_window.protocol("WM_DELETE_WINDOW", root.destroy)

    upper_bar = tk.Frame(test_window, bg="#28262b", width=796, height=25)
    upper_bar.place(x=0, y=0)

    left_panel = tk.Frame(test_window, bg="#1f1d20", width=220, height=365)
    left_panel.place(x=0, y=25)

    left_line = tk.Frame(test_window, bg="#0a0a0b", width=2, height=370)
    left_line.place(x=220, y=25)

    l_left_panel = tk.Frame(test_window, bg="#353238", width=220, height=90)
    l_left_panel.place(x=0, y=300)

    l_right_panel = tk.Frame(test_window, bg="#1f1d20", width=574, height=96)
    l_right_panel.place(x=222, y=300)

    image = Image.open("Assets/Images/circuitry_left.jpg")  # Replace with your image file path
    panel_image = ImageTk.PhotoImage(image)

    panel_image_label = tk.Label(l_left_panel, image=panel_image, width=218, height=94)
    panel_image_label.image = panel_image  # Keep a reference to the image to prevent it from being garbage collected
    panel_image_label.pack()

    panel_line = tk.Frame(test_window, bg="#0a0a0b", width=795, height=2)
    panel_line.place(x=0, y=300)

    window_tk_font = tk.font.Font(family="Share Tech Mono", size=12)
    regular_tk_font = tk.font.Font(family="Share Tech Mono", size=14)

    benchmark_label = tk.Label(test_window, text="Benchmarks", font=window_tk_font, fg="#55ed64", bg="#28262b")
    benchmark_label.place(x=0,y=1)

    single_thread_label = tk.Label(test_window, text="> Benchmark 1 (C)", font=regular_tk_font, fg="#55ed64", bg="#1f1d20")
    single_thread_label.place(x=20, y=85)

    multi_thread_label = tk.Label(test_window, text="> Benchmark 2 (C++)", font=regular_tk_font, fg="#55ed64",bg="#1f1d20")
    multi_thread_label.place(x=20, y=205)

    image = Image.open("Assets/Images/compare.png")  # Replace with your image file path
    compare_image = ImageTk.PhotoImage(image)
    compare_button = tk.Button(test_window, image=compare_image, bg="#1f1d20", borderwidth=0,relief="solid", highlightthickness=0, command=showCompare)
    compare_button.photo = compare_image
    compare_button.place(x=694, y=335)


    clear_button = tk.Button(test_window, text="Clear Output", bg="#062239", borderwidth=1, relief="solid", font=regular_tk_font,
                             fg="#55ed64", highlightthickness=0, command=clearOutput)
    clear_button.place(x=230, y=330)

    specs_button = tk.Button(test_window, text="System Specs", bg="#062239", borderwidth=1, relief="solid",
                             font=regular_tk_font,
                             fg="#55ed64", highlightthickness=0, command=getSpecs)
    specs_button.place(x=380, y=330)

    execute_button=tk.Button(test_window, text="Execute", bg="#55ed64", fg="#062239",borderwidth=1,state="disabled" ,relief="solid", font=regular_tk_font, highlightthickness=0, command=executeBench)
    execute_button.place(x=530, y=330)

    image = Image.open("Assets/Images/examen.png")  # Replace with your image file path
    results_image = ImageTk.PhotoImage(image)
    test_results_button = tk.Button(test_window, image=results_image, bg="#1f1d20", borderwidth=0, state="disabled",relief="solid", highlightthickness=0, command=showResults)
    test_results_button.photo = results_image
    test_results_button.place(x=650, y=335)

    test1_panel = tk.Frame(test_window, bg="#47434c", width=574, height=42)
    test1_panel.place(x=222, y=80)

    test2_panel = tk.Frame(test_window, bg="#47434c", width=574, height=42)
    test2_panel.place(x=222, y=200)

    test1_single_button = tk.Button(test_window, text="> Single T.", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test1_s"))
    test1_single_button.place(x=230, y=85)
    test_buttons["test1_s"]=test1_single_button


    test1_multi_button = tk.Button(test_window, text="> Multi T.", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test1_m"))
    test1_multi_button.place(x=360, y=85)
    test_buttons["test1_m"] = test1_multi_button

    test1_sol1_button = tk.Button(test_window, text="> Sol. 1", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test1_sol1"))
    test1_sol1_button.place(x=480, y=85)
    test_buttons["test1_sol1"] = test1_sol1_button

    test1_sol2_button = tk.Button(test_window, text="> Sol. 2", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test1_sol2"))
    test1_sol2_button.place(x=582, y=85)
    test_buttons["test1_sol2"] = test1_sol2_button

    test2_single_button = tk.Button(test_window, text="> Single T.", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test2_s"))
    test2_single_button.place(x=230, y=205)
    test_buttons["test2_s"] = test2_single_button

    test2_multi_button = tk.Button(test_window, text="> Multi T.", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test2_m"))
    test2_multi_button.place(x=360, y=205)
    test_buttons["test2_m"] = test2_multi_button

    test2_sol1_button = tk.Button(test_window, text="> Sol. 1", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test2_sol1"))
    test2_sol1_button.place(x=480, y=205)
    test_buttons["test2_sol1"] = test2_sol1_button

    test2_sol2_button = tk.Button(test_window, text="> Sol. 2", bg="#062239", borderwidth=1, relief="solid",font=window_tk_font,fg="#55ed64", highlightthickness=0, command=lambda:setTest("test2_sol2"))
    test2_sol2_button.place(x=582, y=205)
    test_buttons["test2_sol2"] = test2_sol2_button

    os.chdir("Benchmark_Scripts/")

def showResults():

    global test_window,results_window

    results_window = tk.Toplevel(root)

    results_window.title("False Sharing Analysis Tool")
    results_window.geometry("700x565")
    results_window.configure(bg="#28262b")
    results_window.resizable(width=False, height=False)
    #results_window.wm_attributes('-type', 'splash')
    results_window.configure(borderwidth=2, relief="solid")

    window_tk_font = tk.font.Font(family="Share Tech Mono", size=12)

    benchmark_label = tk.Label(results_window, text=selectedTest+" results", font=window_tk_font, fg="#55ed64", bg="#28262b")
    benchmark_label.place(x=0, y=1)

    graph_panel = tk.Frame(results_window, bg="#353238", width=696, height=536)
    graph_panel.place(x=0, y=25)

    doGraphs(parseResults("output_"+selectedTest+".txt"), "output_"+selectedTest)

    image = Image.open("output_"+selectedTest+".png")  # Replace with your image file path
    graph_image = ImageTk.PhotoImage(image)

    graph_image_label = tk.Label(graph_panel, image=graph_image, width=640, height=480)
    graph_image_label.image = graph_image  # Keep a reference to the image to prevent it from being garbage collected
    graph_image_label.place(x=30, y=30)

def showCompare():

    global test_window, results_window,compare_window

    compare_window = tk.Toplevel(root)
    compare_window.title("False Sharing Analysis Tool")

    window_tk_font = tk.font.Font(family="Share Tech Mono", size=12)

    tests=available_for_compare()

    height=len(tests)*25

    compare_window.geometry("559x100")
    if(len(tests)>0):
        compare_window.geometry("559x"+str(height+25+8+50))
        test_panel = tk.Frame(compare_window, bg="#353238", width=555, height=len(tests)*25+3)
        test_panel.place(x=0, y=25)

    compare_window.configure(bg="#28262b")
    compare_window.resizable(width=False, height=False)
    compare_window.configure(borderwidth=2, relief="solid")
    compare_window.protocol("WM_DELETE_WINDOW", kill_compare)

    available_label = tk.Label(compare_window ,text="Available Tests Results for Comparison:", font=window_tk_font, fg="#55ed64",bg="#28262b")
    available_label.place(x=0, y=2)

    if(len(tests)==0):
        benchmark_label = tk.Label(compare_window ,text="> No test results available to compare", font=window_tk_font, fg="#55ed64",bg="#28262b")
        benchmark_label.place(x=0, y=30)

    else:

        colors=["#1f1d20","#47434c"]

        for i in range(len(tests)):
            compare_button = tk.Button(compare_window,text=tests[i],font=window_tk_font, fg="#55ed64",bg=colors[i%2],borderwidth=0, relief="solid",highlightthickness=0,anchor='w', command=lambda p=i: setCompare(p))
            compare_button.place(x=0, y=25+i*25)
            compare_button.config(width=59)
            compareButtons.append(compare_button)

        compare_button = tk.Button(compare_window, text="Compare", font=window_tk_font, fg="#55ed64", bg=colors[i % 2],borderwidth=1, relief="solid", highlightthickness=0, anchor='w', command=doCompare)
        compare_button.place(x=450, y= len(tests)*25+3+37)

def parseResults(filename):

    file_path = filename  # Replace with your file path

    try:
        with open(file_path, "r") as file:

            inputs=file.read().split("$")

            for j in range(len(inputs)):
                input = inputs[j]
                lines=input.split("\n")
                components=[]
                for line in lines:
                    components+=line.split(" ")

                components = [component for component in components if component != '']
                output={}
                for i in range(len(components)):


                    component = components[i].replace('\u202f',"")
                    components[i]=component

                    if(component == 'L1-dcache-loads'):

                        output[component]=float(components[i-1])

                    elif(component == 'L1-dcache-load-misses'):

                        output[component] = float(components[i - 1])

                    elif (component == 'time'):

                        output[component] = float(components[i - 2].replace(",","."))

                inputs[j]=output

            output={"L1-dcache-load-misses":[],"L1-dcache-loads":[],"time":[]}

            for input in inputs:

                for key in input.keys():

                    output[key]=output[key]+[input[key]]

            return output

    except:

        return -1

def doGraphs(inputs,outputFile):

    plt.clf()

    iterations=range(len(inputs[list(inputs.keys())[0]]))

    plt.subplot(3,1,1)
    plt.plot(iterations,inputs["L1-dcache-load-misses"])
    plt.title("Iterations Vs Cache Misses")

    plt.subplot(3, 1, 2)
    plt.plot(iterations, inputs["L1-dcache-loads"])
    plt.title("Iterations Vs Cache Loads")

    plt.subplot(3, 1, 3)
    plt.plot(iterations, inputs["time"])
    plt.title("Iterations Vs Time per Iteration")

    plt.tight_layout()


    plt.savefig(outputFile)



init()
#doGraphs(parseResults("output_test1_s.txt"),"output_test1_s")