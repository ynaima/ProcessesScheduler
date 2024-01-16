import ctypes

# Defining the Data struct as it is defined in C
class Data(ctypes.Structure):
    _fields_ = [("PID", ctypes.c_long), ("time", ctypes.c_int), ("arrival", ctypes.c_int)]

# Loading the shared library
shared_lib = ctypes.CDLL('./libscheduler.dll')

# using the function in scheduler.c
def my_c_function_wrapper():
    # Allocating  array for Data
    array_type = Data * 30  # Defining  an array of Data
    data_array = array_type()

    # Getting filename input from user
    filename = input("Enter the filename with extension (e.g., processes1.txt): ").encode('utf-8')


    # Get scheduler type input from user
    scheduler_type = input("Enter the scheduler type (e.g., FIFO, RRx, SJF): ").encode('utf-8')

    # Setting the argument types and return types for these functions
    shared_lib.read_processes.argtypes = [ctypes.c_char_p, ctypes.POINTER(Data)]
    shared_lib.read_processes.restype = ctypes.c_int

    shared_lib.dispatcher.argtypes = [ctypes.c_char_p, ctypes.POINTER(Data), ctypes.c_int]
    shared_lib.dispatcher.restype = None

    size = shared_lib.read_processes(filename, data_array)
    shared_lib.dispatcher(scheduler_type, data_array, size)

# Call the function
result = my_c_function_wrapper()
