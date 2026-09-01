function(check_format MAIN_TARGET)
   add_custom_target(
      FORMAT_CHECK
      COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/CMakeLists.txt
      COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/cmake
      COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/scripts
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/linux_operations
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/scheduling
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/process_synchronization
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/memory_management
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/security
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/networking
      #COMMAND bash ${SCRIPT_DIR}/format_checker.sh ${TOPDIR}/misc

      WORKING_DIRECTORY ${TOPDIR}
      COMMENT "This checks the format of all files in the repository"
   )
   add_dependencies(${MAIN_TARGET} FORMAT_CHECK)
endfunction()
