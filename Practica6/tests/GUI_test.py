import subprocess
import time
import os
import signal

# Define the path to your compiled executable
APP_PATH = "../../bin/calculo_punto_fijo"  # Ensure you compile with: gcc -o puntofijo ...

def run_GUI_test():
    print(f"--- Starting GUI Test for {APP_PATH} ---")
    
    # 1. Check if executable exists
    if not os.path.exists(APP_PATH):
        print(f"ERROR: Executable {APP_PATH} not found. Did you compile?")
        exit(1)

    # 2. Launch the application
    try:
        # Popen starts the process in the background
        process = subprocess.Popen([APP_PATH], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print(f"Process started with PID: {process.pid}")
    except Exception as e:
        print(f"ERROR: Failed to launch application: {e}")
        exit(1)

    # 3. Wait for X seconds to ensure it initializes without segfaulting immediately
    time.sleep(3)

    # 4. Check if it is still running
    if process.poll() is None:
        print("SUCCESS: Application is running stable after 3 seconds.")
        
        # 5. Clean up (Close the app)
        print("Closing application...")
        process.terminate() # Sends SIGTERM
        try:
            process.wait(timeout=2)
        except subprocess.TimeoutExpired:
            process.kill() # Force kill if it hangs
    else:
        # If poll() returns a code, the app crashed or closed unexpectedly
        return_code = process.returncode
        stderr_output = process.stderr.read().decode()
        print(f"FAILURE: Application crashed with code {return_code}")
        print(f"Error Log: {stderr_output}")
        exit(1)

if __name__ == "__main__":
    run_GUI_test()