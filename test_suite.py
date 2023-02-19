import multiprocessing
import shlex
import subprocess
import time

RUST_PREAMBLE = "cargo run --bin central_controller --release -- matchup"
BOARD_SIZE = 2
BLACK_PATH = "../../hexbot/cpp_bot/main.out"
WHITE_PATH = "../../hexbot/cpp_bot/main.out"
RUN_TIME_SECS = 20

args = shlex.split("{} {} {} {}".format(RUST_PREAMBLE, BOARD_SIZE, BLACK_PATH, WHITE_PATH))


def play_game(black_wins, white_wins):
    p = subprocess.Popen(args,
                         stdin=subprocess.PIPE,
                         stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE,
                         universal_newlines=True,
                         bufsize=0)

    p.stdin.write("run " + str(BOARD_SIZE ** 2 + 1))  # swap move and fill board
    p.stdin.close()

    for line in p.stdout:
        if "Black has won" in line:
            with black_wins.get_lock():
                black_wins.value += 1
        elif "White has won" in line:
            with white_wins.get_lock():
                white_wins.value += 1


procs = []
BLACK_WINS = multiprocessing.Value('i', 0)
WHITE_WINS = multiprocessing.Value('i', 0)

start_time = time.perf_counter()

while time.perf_counter() - start_time < RUN_TIME_SECS:
    for cpu in range(0, multiprocessing.cpu_count()):
        proc = multiprocessing.Process(target=play_game, args=(BLACK_WINS, WHITE_WINS))
        procs.append(proc)
        proc.start()
    for proc in procs:
        proc.join()

print("BLACK WIN", BLACK_WINS.value, "WHITE WIN", WHITE_WINS.value)
