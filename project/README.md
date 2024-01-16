Scheduling Application

Description
This application is a process scheduler simulator that interfaces with a MySQL database. It is designed to manage and schedule processes based on various scheduling algorithms like First-In-First-Out (FIFO), Shortest Job First (SJF), and Round Robin (RR). The application dispatches processes based on their arrival and execution times, handling each process according to the specified scheduling algorithm. For the RoundRobin algorithm, the period for which a process is served depends on the service time allocated. The application uses a priority queue to store processes. The advantage of using a priority queue was to ensure that processes were sorted based on their arrival time regardless of when they entered the queue. The application logs scheduling actions and process details into the database, providing a comprehensive record of the scheduling process. The application keeps track of a timer and updates it such that there is a record even when the scheduler is idle. The program prompts the user for input for the scheduling algorithm type as well as the textfile that where processes are stored.

Key Features
Scheduling Algorithms: Supports FIFO, SJF, and RR scheduling strategies.
Database Integration: Connects to a MySQL database to store detailed logs of scheduling actions and process details.
Process Management: Efficiently manages processes using a priority queue, considering factors like arrival time and required execution time.
Dynamic Scheduling: Adapts to different scheduling scenarios, dynamically handling processes based on the chosen algorithm.
Time Tracking: Maintains a timer to log the sequence of events and actions taken during the scheduling.

Technical Overview
FIFO Scheduler: Processes tasks in the order they arrive, maintaining a simple queue structure.
SJF Scheduler: Prefers tasks with the shortest execution time, reducing overall process waiting time.
Round Robin Scheduler: Allocates a fixed time slice for each process in a rotating manner, ensuring fairness among processes.

Database Logging: Each scheduler function logs scheduling events, such as process fetching, execution, and idle times, into the MySQL database.
Process Table Updates: Inserts or updates process information, including process ID, arrival time, and execution time, into the database.

Requirements
GCC Compiler
Python 3.x
MySQL Server 8.0
MySQL C API Libraries

Installation
Compiling the C Library
Compile the C source code to a shared library (DLL) with MySQL integration:


gcc -shared -o libscheduler.dll -fPIC -I"C:\Program Files\MySQL\MySQL Server 8.0\include" -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" scheduler.c process.c pqueue.c data.c -lmysqlclient

Running the Application
Run the main.py Python script in a terminal:

python main.py
Follow the prompts to input a text file with processes and select a scheduling algorithm.
Observe the scheduling process output and check the MySQL database for logs and process details.



Database Schema
SchedulerLog Table: Stores logs of scheduler actions. Fields include process_id, scheduler_type, action, and time_stamp.
Processes Table: Contains details of each process, including process_id, arrival_time, and execution_time.