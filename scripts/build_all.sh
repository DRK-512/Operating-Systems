#!/usr/bin/env bash

main() {
        SCRIPT_DIR="$(cd -- "$(dirname -- "$(realpath -- "$0")")" && pwd)"

        # Define which constants are readonly
        readonly SCRIPT_DIR

        # Bring in the config.sh for logging
        if [[ -f "$SCRIPT_DIR/config.sh" ]]; then
                # shellcheck source=SCRIPTDIR/config.sh
                source "$SCRIPT_DIR/config.sh"
        else
                echo -e "\e[31mERROR: Cannot find config.sh\e[0m"
                exit 1
        fi

        cd ${SCRIPT_DIR}/../
        cmake -S . -B build || log_error "Failed to build cmake configuration for project"
        cmake --build build || log_error "Failed to build project"

}

main "$@"
