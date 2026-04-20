import os
import time
import subprocess
import csv

def run_command(cmd):
    # print(f"Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    return result

def benchmark():
    # Configuration
    scales = [1, 2, 3, 4, 5, 6, 7, 8] # Octahedron subdivision levels
    executable = os.path.abspath("./bin/MeshFix")
    results_file = "bench/scalability_results.csv"
    
    print(f"{'Subdiv':<10} | {'Vertices':<10} | {'Triangles':<10} | {'Time (s)':<10}")
    print("-" * 50)
    
    stats = []

    for s in scales:
        # 1. Generate
        print("Creating clean Geometry")
        clean_mesh = f"bench/clean_{s}.off"
        run_command(["python3", "bench/generate_mesh.py", str(s), clean_mesh])
        
        # 2. Corrupt
        print("Corrupting Geometry")
        corrupt_mesh = f"bench/corrupt_{s}.off"
        run_command(["python3", "bench/corrupt_mesh.py", clean_mesh, corrupt_mesh])
        
        # Get counts
        with open(clean_mesh, 'r') as f:
            lines = f.readlines()
            header = lines[1].split()
            nv, nt = header[0], header[1]

        # 3. Benchmark MeshFix
        print("Fixing Geometry")
        fixed_mesh = f"bench/fixed_{s}.off"
        start_time = time.time()
        res = run_command([executable, corrupt_mesh, "-o", fixed_mesh])
        end_time = time.time()
        
        duration = end_time - start_time
        
        if res.returncode == 0:
            print(f"{s:<10} | {nv:<10} | {nt:<10} | {duration:<10.4f}")
            stats.append({"subdiv": s, "vertices": nv, "triangles": nt, "time": duration})
        else:
            print(f"{s:<10} | Error during MeshFix execution")
            
        # Cleanup temp files
        for f in [clean_mesh, corrupt_mesh, fixed_mesh]:
            if os.path.exists(f):
                os.remove(f)

    # Save to CSV
    with open(results_file, 'w', newline='') as csvfile:
        fieldnames = ['subdiv', 'vertices', 'triangles', 'time']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for row in stats:
            writer.writerow(row)
            
    print(f"\nResults saved to {results_file}")

if __name__ == "__main__":
    benchmark()
