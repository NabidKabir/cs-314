import os
import subprocess

test_word = "hello"
decimal_places = 6
run_count = 20

# Get time string from program output s
def get_time_string(s):
    return s.split(":")[1].split("\n")[0].strip()

# Get time difference of own to sol (own - sol)
# - time means own is faster than sol
def get_time_dif(own_time_string, sol_time_string):
    dif = float(own_time_string) - float(sol_time_string)
    return round(dif, decimal_places)

def spell_t2_singleloop():
    print("====================")
    print("spell_t2_singleloop")
    print("====================")
    print("Making...")
    subprocess.run(["make", "spell_t2_singleloop"])
    print("====================")
    print("Sol\t\tOwn\t\tDif")
    faster_count = 0
    close_count = 0
    for i in range(run_count):
        own_output = subprocess.run(["./spell_t2_singleloop", test_word], capture_output=True).stdout.decode()
        sol_output = subprocess.run(["./spell_t2_singleloop-sol", test_word], capture_output=True).stdout.decode()

        own_output_time = get_time_string(own_output)
        sol_output_time = get_time_string(sol_output)
        dif_time = get_time_dif(own_output_time, sol_output_time)
        if (dif_time < 0):
            faster_count += 1
        if (dif_time >= 0 and dif_time <= 0.01):
            close_count += 1

        print(f"{sol_output_time}\t{own_output_time}\t{dif_time}")
    total_count = faster_count + close_count
    faster_percent = (faster_count / run_count) * 100
    close_percent = (close_count / run_count) * 100
    total_percent = (total_count / run_count) * 100
    print(f"# of faster runs: {faster_count}/{run_count} ({faster_percent}%)")
    print(f"# of close runs: {close_count}/{run_count} ({close_percent}%)")
    print(f"Total 'good' runs: {total_count}/{run_count} ({total_percent}%)")
    print()

def spell_t2_fastest():
    print("====================")
    print("spell_t2_fastest")
    print("====================")
    print("Making...")
    subprocess.run(["make", "spell_t2_fastest"])
    print("====================")
    print("Sol\t\tOwn\t\tDif")
    faster_count = 0
    close_count = 0
    for i in range(run_count):
        own_output = subprocess.run(["./spell_t2_fastest", test_word], capture_output=True).stdout.decode()
        sol_output = subprocess.run(["./spell_t2_fastest-sol", test_word], capture_output=True).stdout.decode()

        own_output_time = get_time_string(own_output)
        sol_output_time = get_time_string(sol_output)
        dif_time = get_time_dif(own_output_time, sol_output_time)
        if (dif_time < 0):
            faster_count += 1
        if (dif_time >= 0 and dif_time <= 0.01):
            close_count += 1

        print(f"{sol_output_time}\t{own_output_time}\t{dif_time}")
    total_count = faster_count + close_count
    faster_percent = (faster_count / run_count) * 100
    close_percent = (close_count / run_count) * 100
    total_percent = (total_count / run_count) * 100
    print(f"# of faster runs: {faster_count}/{run_count} ({faster_percent}%)")
    print(f"# of close runs: {close_count}/{run_count} ({close_percent}%)")
    print(f"Total 'good' runs: {total_count}/{run_count} ({total_percent}%)")
    print()

spell_t2_singleloop()
# spell_t2_fastest()
