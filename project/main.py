from c_wrapper import my_c_function_wrapper
import db

db.database()
#calling the main function (the dispatcher)
my_c_function_wrapper()
