#!/usr/bin/env bash
# For information on this script, look at the usage function below
usage() {
   cat << EOF
Usage: ./$0 [OPTIONS] [PATH] [DEPTH]

  The purpose of this script is to check or fix the format of all the following types of files:
        - CMakeLists.txt
        - Bash Scripts (.sh)
        - Rust (.rs)
        - C (.c .h)
        - C++ (.cpp .hpp)
        - Python (.py)

  For changing the format of each file, visit the ci/format-files directory

Options:
  -h, --help          Display this help message
  -f, --fix           Automatically apply formatting instead of just checking diffs
   <Some path>        Will check/fix a single file OR all subdirectories (default: ./services)
   <Num>              Max depth for file search (applies to directories only)

Example:
  ./$0 --fix ./scripts/my_script.sh  # Will FIX ONLY this single script
  ./$0 -f ./scripts                  # Will FIX all files in the scripts directory
  ./$0 ./scripts 2                   # Will CHECK scripts dir and 2 layers of subdirectories
EOF
}

parse_args() {
   FIX_MODE=0
   local positional=()

   # Parse all arguments dynamically
   while [[ $# -gt 0 ]]; do
      case "$1" in
         -h | --help)
            usage
            exit 0
            ;;
         -f | --fix)
            FIX_MODE=1
            shift
            ;;
         -*)
            log_error "Unknown option: $1"
            usage
            exit 1
            ;;
         *)
            positional+=("$1")
            shift
            ;;
      esac
   done

   local target_arg="${positional[0]:-}"
   local depth_arg="${positional[1]:-}"

   # First param = target we check (file or directory)
   if [[ -z "$target_arg" ]]; then
      SEARCH_TARGET="."
      IS_FILE=0
   elif [[ -f "$target_arg" ]]; then
      SEARCH_TARGET="$target_arg"
      IS_FILE=1
      MAXDEPTH_ARGS=()
      MAXDEPTH_DESC="N/A (Single File)"
   elif [[ -d "$target_arg" ]]; then
      SEARCH_TARGET="$target_arg"
      IS_FILE=0
   else
      usage
      log_error "Path does not exist: ${target_arg}"
   fi

   # Parse max depth (only applies if we are searching a directory)
   if [[ "$IS_FILE" -eq 0 ]]; then
      if [[ -z "$depth_arg" ]]; then
         MAXDEPTH_ARGS=() # empty => unbounded recursion
         MAXDEPTH_DESC="unbounded"
      elif [[ "$depth_arg" =~ ^[0-9]+$ ]]; then
         if ((depth_arg < 1)); then
            usage
            log_error "Depth must be >= 1"
         else
            MAXDEPTH_ARGS=(-maxdepth "$depth_arg")
            MAXDEPTH_DESC="$depth_arg"
         fi
      else
         usage
         log_error "Invalid depth (must be a positive integer): ${depth_arg}"
      fi
   fi
}

# Check we have all the format checking tools
check_tools() {
   local tools=(cmake-format clang-format rustfmt shfmt)

   # Pylint is a linter and can't apply PEP8, autopep8 handles the fix mode
   if [[ "$FIX_MODE" -eq 1 ]]; then
      tools+=(autopep8)
   else
      tools+=(pylint)
   fi

   for tool in "${tools[@]}"; do
      if ! command -v "$tool" > /dev/null 2>&1; then
         log_error "Tool not found, related checks will fail: ${tool}"
      fi
   done
}

# ---- Generic file-format runner ----------------------------------------------
# Args:
#   $1 - human-readable label
#   $2 - tool binary
#   $3 - check command template
#   $4 - fix command template
#   $5.. - find name patterns
run_check() {
   local label="$1"
   shift
   local tool="$1"
   shift
   local check_cmd_template="$1"
   shift
   local fix_cmd_template="$1"
   shift
   local patterns=("$@")

   # Select the correct command template based on flag
   local active_template
   if [[ "$FIX_MODE" -eq 1 ]]; then
      active_template="$fix_cmd_template"
   else
      active_template="$check_cmd_template"
   fi

   # Build the find expression: \( -name 'a' -o -name 'b' \)
   local find_expr=()
   local first=1 pat
   for pat in "${patterns[@]}"; do
      if ((first)); then
         find_expr+=('(' -name "$pat")
         first=0
      else
         find_expr+=(-o -name "$pat")
      fi
   done
   find_expr+=(')')

   local file
   if [[ "$IS_FILE" -eq 1 ]]; then
      if [[ -n $(find "$SEARCH_TARGET" "${find_expr[@]}" 2> /dev/null) ]]; then
         local cmd
         # shellcheck disable=SC2059
         printf -v cmd "$active_template" "$SEARCH_TARGET"
         if eval "$cmd" > /dev/null 2>&1; then
            if [[ "$FIX_MODE" -eq 1 ]]; then
               log_info "[FIXED]  ${label}: ${SEARCH_TARGET}"
            else
               log_info "[OK]   ${label}: ${SEARCH_TARGET}"
            fi
         else
            log_error_no_exit "[FAIL] ${label}: ${SEARCH_TARGET}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
         fi
      fi
   else
      while IFS= read -r -d '' file; do
         local cmd
         # shellcheck disable=SC2059
         printf -v cmd "$active_template" "$file"
         if eval "$cmd" > /dev/null 2>&1; then
            if [[ "$FIX_MODE" -eq 1 ]]; then
               log_info "[FIXED]  ${label}: ${file}"
            else
               log_info "[OK]   ${label}: ${file}"
            fi
         else
            log_error_no_exit "[FAIL] ${label}: ${file}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
         fi
      done < <(
         find -- "$SEARCH_TARGET" "${MAXDEPTH_ARGS[@]}" -type f "${find_expr[@]}" -print0
      )
   fi
}

# ---- Per-category checks -----------------------------------------------------
check_all() {
   local cfg_dir="${SCRIPT_DIR}/../ci/format_files"

   # CMake
   run_check "CMake" "cmake-format" \
      "cmake-format -c '${cfg_dir}/cmake-format.yaml' --check '%s'" \
      "cmake-format -c '${cfg_dir}/cmake-format.yaml' -i '%s'" \
      "CMakeLists.txt" "*.cmake"

   # Bash
   run_check "Bash" "shfmt" \
      "shfmt -d '%s'" \
      "shfmt -w '%s'" \
      "*.sh"

   # C / C++
   run_check "C/C++" "clang-format" \
      "clang-format --dry-run -style=file:${cfg_dir}/clang-format '%s'" \
      "clang-format -i -style=file:${cfg_dir}/clang-format '%s'" \
      "*.c" "*.h" "*.cpp" "*.hpp"

   # Rust
   run_check "Rust" "rustfmt" \
      "rustfmt --check --config-path '${cfg_dir}/rustfmt.toml' '%s'" \
      "rustfmt --config-path '${cfg_dir}/rustfmt.toml' '%s'" \
      "*.rs"

   # Python (pylint for check, autopep8 for fix)
   run_check "Python" "python-tools" \
      "pylint --rcfile='${cfg_dir}/.pylintrc' '%s'" \
      "autopep8 --in-place '%s'" \
      "*.py"
}

main() {
   SEARCH_TARGET=""
   FAIL_COUNT=0
   MAXDEPTH_ARGS=()
   MAXDEPTH_DESC="unbounded"
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

   parse_args "$@"

   if [[ "$IS_FILE" -eq 1 ]]; then
      log_info "Targeting single file: ${SEARCH_TARGET}"
   else
      log_info "Search dir: ${SEARCH_TARGET} (max depth ${MAXDEPTH_DESC})"
   fi

   if [[ "$FIX_MODE" -eq 1 ]]; then
      log_info "Mode: AUTO-FIX enabled"
   else
      log_info "Mode: DRY-RUN CHECK only"
   fi

   check_tools
   check_all

   if ((FAIL_COUNT == 0)); then
      if [[ "$FIX_MODE" -eq 1 ]]; then
         log_success "All files formatted successfully"
      else
         log_success "All files in scope are correctly formatted"
      fi
      exit 0
   else
      if [[ "$FIX_MODE" -eq 1 ]]; then
         log_warning "${FAIL_COUNT} file(s) encountered formatting tool errors"
      else
         log_warning "${FAIL_COUNT} file(s) failed formatting checks"
      fi
      exit 1
   fi
}

main "$@"
