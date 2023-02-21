import time
from concurrent.futures import ProcessPoolExecutor, as_completed
import subprocess
from math import ceil


def get_duration():
    p = subprocess.Popen(
        ['./main.out', 'black'],
        stdin=subprocess.PIPE, stdout=subprocess.PIPE
    )
    start = time.perf_counter()
    stdout = p.communicate(b"make_move\n")[0]
    end = time.perf_counter()
    duration = end - start
    return stdout, ceil(duration * 100) / 100

def main():
    max_time = 0
    N = 64
    with ProcessPoolExecutor() as executor:
        futures = [executor.submit(get_duration) for _ in range(N)]
        for future in as_completed(futures):
            stdout, duration = future.result()
            max_time = max(max_time, duration)
    print(f'{stdout.decode("utf-8")}')
    print(f'Max time: {max_time:.2f} seconds')


if __name__ == '__main__':
    main()
