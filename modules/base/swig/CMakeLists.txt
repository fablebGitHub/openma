SET_SOURCE_FILES_PROPERTIES("ma.i" PROPERTIES CPLUSPLUS ON)
SET(SWIG_MODULE_base_EXTRA_SRCS
  "base/any.i"
  "base/event.i"
  "base/hardware.i"  
  "base/logger.i"
  "base/node.i"
  "base/object.i"
  "base/subject.i"
  "base/timesequence.i"
  "base/trial.i")
SET(SWIG_MODULE_ma_EXTRA_FLAGS
  # "-debug-tmsearch"
  "-I${PROJECT_SOURCE_DIR}/modules/bindings/swig"
  "-I${SWIG_DIR}/Lib")

IF(BUILD_MATLAB_BINDINGS)
  SET(BASE_MATLAB_SRCS
    "ma.i"
    "../../bindings/include/openma/matlab/mexFunction.def")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma")
  SET(SWIG_MODULE_maMEX_EXTRA_DEPS
    ${SWIG_MODULE_base_EXTRA_SRCS}
    "ma_matlab.i")
  SET(SWIG_MODULE_maMEX_EXTRA_FLAGS
    ${SWIG_MODULE_ma_EXTRA_FLAGS}
    "-I${SWIG_DIR}/Lib/matlab")
  SWIG_ADD_MODULE(maMEX matlab "${BASE_MATLAB_SRCS}")
  SWIG_LINK_LIBRARIES(maMEX base bindings ${MATLAB_LIBRARIES})
  TARGET_INCLUDE_DIRECTORIES(${SWIG_MODULE_maMEX_REAL_NAME} PRIVATE "${MATLAB_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(${SWIG_MODULE_maMEX_REAL_NAME} PROPERTIES
    COMPILE_DEFINITIONS "_SWIG_MATLAB"
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/matlab/openma"
    PREFIX ""
    SUFFIX ".${MATLAB_MEXFILE_EXT}")
ENDIF()

IF(BUILD_PYTHON_BINDINGS)
  SET(BASE_PYTHON_SRCS
    "ma.i")
  SET(CMAKE_SWIG_OUTDIR "${OPENMA_EXECUTABLE_PATH}/swig/python/openma/ma")
  SET(SWIG_MODULE_ma_EXTRA_DEPS
    ${SWIG_MODULE_base_EXTRA_SRCS}
    "ma_python.i")
  SET(SWIG_MODULE_ma_EXTRA_FLAGS
    ${SWIG_MODULE_ma_EXTRA_FLAGS}
    "-I${SWIG_DIR}/Lib/python")
  SWIG_ADD_MODULE(ma python "${BASE_PYTHON_SRCS}")
  SWIG_LINK_LIBRARIES(ma base bindings ${PYTHON_LIBRARY})
  TARGET_INCLUDE_DIRECTORIES(${SWIG_MODULE_ma_REAL_NAME} PRIVATE "${PYTHON_INCLUDE_DIR}" "${NUMPY_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(${SWIG_MODULE_ma_REAL_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/python/openma/ma")
  # TODO Is there a way to rename the file without that it triggers a new call of SWIG?
  ADD_CUSTOM_COMMAND(TARGET ${SWIG_MODULE_ma_REAL_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "ma.py" "__init__.py" WORKING_DIRECTORY "${OPENMA_EXECUTABLE_PATH}/swig/python/openma/ma")
ENDIF()
