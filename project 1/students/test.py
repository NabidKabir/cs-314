import os
import subprocess

make_targets = ["compile", "constfolding"]
#make_targets = ["compile", "constfolding", "strengthreduct", "deadcode"]

def test_compile():
    print("Tests for compile:")

    for filename in os.listdir("tests"):
        if not filename.startswith("test"):
            continue
        
        file_path = "tests/" + filename

        subprocess.run(["./compile.sol", file_path], capture_output=True)
        with open("tinyL.out", "r") as file:
            sample_solution = file.read()

        subprocess.run(["./compile", file_path], capture_output=True)
        with open("tinyL.out", "r") as file:
            student_solution = file.read()

        result = "PASS" if sample_solution == student_solution else "FAIL"

        print(f"{filename} - {result}")

def test_constfolding():
    print("Tests for constfolding:")

    for filename in os.listdir("tests"):
        if not filename.startswith("test"):
            continue

        file_path = "tests/" + filename

        subprocess.run(["./compile.sol", file_path], capture_output=True)

        sample_solution = subprocess.run(["./constfolding.sol"], stdin=open("tinyL.out"), capture_output=True).stdout
        student_solution = subprocess.run(["./constfolding"], stdin=open("tinyL.out"), capture_output=True).stdout

        result = "PASS" if sample_solution == student_solution else "FAIL"

        print(f"{filename} - {result}")

def test_strength_reduction():
    print("Tests for strength reduction:")

    for filename in os.listdir("tests"):
        if not filename.startswith("test"):
            continue

        file_path = "tests/" + filename

        subprocess.run(["./compile.sol", file_path], capture_output=True)

        sample_solution = subprocess.run(["./strengthreduct.sol"], stdin=open("tinyL.out"), capture_output=True).stdout
        student_solution = subprocess.run(["./strengthreduct"], stdin=open("tinyL.out"), capture_output=True).stdout

        result = "PASS" if sample_solution == student_solution else "FAIL"

        print(f"{filename} - {result}")

def test_deadcode_elimination():
    print("Tests for deadcode elimination:")

    for filename in os.listdir("tests"):
        if not filename.startswith("test"):
            continue

        file_path = "tests/" + filename

        subprocess.run(["./compile.sol", file_path], capture_output=True)

        sample_solution = subprocess.run(["./deadcode.sol"], stdin=open("tinyL.out"), capture_output=True).stdout
        student_solution = subprocess.run(["./deadcode"], stdin=open("tinyL.out"), capture_output=True).stdout

        result = "PASS" if sample_solution == student_solution else "FAIL"

        print(f"{filename} - {result}")

def test_valgrind():
    print("Tests for valgrind:")

    for filename in os.listdir("tests"):
        if not filename.startswith("test"):
            continue

        file_path = "tests/" + filename

        print(f"{filename}:")

        result = "PASS" if "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" in subprocess.run(["valgrind", "--leak-check=full", "--show-reachable=yes", "--track-origins=yes", "./compile", file_path], capture_output=True).stderr.decode() else "FAIL"
        print(f"\tcompile - {result}")
        for target in make_targets:
            if target != "compile":
                result = "PASS" if "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" in subprocess.run(["valgrind", "--leak-check=full", "--show-reachable=yes", "--track-origins=yes", f"./{target}"], stdin=open("tinyL.out"), capture_output=True).stderr.decode() else "FAIL"
                print(f"\t{target} - {result}")

print("Making targets:")
subprocess.run(["make", "clean"])
for target in make_targets:
    subprocess.run(["make", target], capture_output=False)

test_compile()
print()
test_constfolding()
print()
#test_strength_reduction()
print()
#test_deadcode_elimination()
print()
#test_valgrind()