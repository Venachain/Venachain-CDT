add_custom_command(TARGET VenachainLLVM POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin)
macro(venachain_llvm_install file)
  set(BINARY_DIR ${CMAKE_BINARY_DIR}/venachain_llvm/bin)
  add_custom_command(TARGET VenachainLLVM POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file}${BIN_SUFFIX} ${CMAKE_BINARY_DIR}/bin/${file}${BIN_SUFFIX})
  install(FILES ${BINARY_DIR}/${file}${BIN_SUFFIX}
    DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro(venachain_llvm_install)

macro(venachain_tools_install file)
  set(BINARY_DIR ${CMAKE_BINARY_DIR}/tools/bin)
  add_custom_command(TARGET VenachainTools POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${file}${BIN_SUFFIX} ${CMAKE_BINARY_DIR}/bin/${file}${BIN_SUFFIX})
  install(FILES ${BINARY_DIR}/${file}${BIN_SUFFIX}
    DESTINATION ${CMAKE_INSTALL_FULL_BINDIR}
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endmacro(venachain_tools_install)

macro(venachain_libraries_install)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/lib)
   execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/include)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
   install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})
   #install(DIRECTORY ${CMAKE_SOURCE_DIR}/libraries/boost/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})
   #install(DIRECTORY ${CMAKE_SOURCE_DIR}/libraries/rapidjson/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})
   #install(DIRECTORY ${CMAKE_SOURCE_DIR}/libraries/venachainlib/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})
endmacro(venachain_libraries_install)

set(CLANG clang-7)
if (WIN32)
  set(CLANG clang)
endif()

venachain_llvm_install(${CLANG})
venachain_llvm_install(opt)
venachain_llvm_install(llc)
venachain_llvm_install(lld)
venachain_llvm_install(wasm-ld)
venachain_tools_install(venachain-cpp)
venachain_tools_install(venachain-ld)
venachain_tools_install(venachain-init)
venachain_tools_install(venachain-abigen)
venachain_libraries_install()
