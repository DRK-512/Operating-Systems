#include "functions.h"

int main(int argc, char *argv[]) {

        // I made a struct that holds all the args
        arguments args = parse_args(argc, argv);

        // Check if the args are valid, if not, return 1
        if (check_args(args)) {
                return 1;
        }

        main_loop(args.address_path, args.process_path);
        return 0;
}
