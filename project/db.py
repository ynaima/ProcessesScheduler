import mysql.connector

def database():
    conn = mysql.connector.connect(
        host='localhost',
        user='username',
        password='password'
    )
    cursor = conn.cursor()

    cursor.execute("CREATE DATABASE IF NOT EXISTS scheduler_database")
    cursor.execute("USE scheduler_database")

    # creating tables processes, scheduler log and scheduler_id
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS Processes (
            process_id INT PRIMARY KEY,,
            arrival_time INT,
            execution_time INT)
        """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS SchedulerType (
            scheduler_type_id INT AUTO_INCREMENT PRIMARY KEY,
            scheduler_type VARCHAR(255)
        )
        """)

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS SchedulerLog (
            log_id INT AUTO_INCREMENT PRIMARY KEY,
            time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            action VARCHAR(255),
            process_id INT,
            scheduler_type_id INT,
            FOREIGN KEY (process_id) REFERENCES Processes(process_id),
            FOREIGN KEY (scheduler_type_id) REFERENCES SchedulerType(scheduler_type_id)
        )
        """)

    


    conn.commit()
    cursor.close()
    conn.close()

if __name__ == "__main__":
    database()
