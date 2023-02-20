## How to build and run

Run `make` in the root directory of the repo to generate the program executable `main.out`.
Run `./main.out white` or `./main.out black` to start the bot, which follows the [tournament communication protocol](https://github.com/UndergraduateArtificialIntelligenceClub/UAIS-2022-Hex-Bot/blob/main/README.md#communication-protocol).

## How to test

Move `test_suite.py` from the root directory of the repo into the `central_program` directory of UAIS repo and run it there with `python3 test_suite.py`. 
The `-h` option provides usage information and a few useful variables are placed near the top of the file for ease of editing.
