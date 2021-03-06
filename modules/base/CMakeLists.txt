SET(OPENMA_BASE_SRCS
  src/any.cpp
  src/date.cpp
  src/event.cpp
  src/hardware.cpp
  src/logger.cpp
  src/node.cpp
  src/object.cpp
  src/subject.cpp
  src/timesequence.cpp
  src/trial.cpp
  src/typeid.cpp
)

ADD_LIBRARY(base ${OPENMA_LIBS_BUILD_TYPE} ${OPENMA_BASE_SRCS})
TARGET_INCLUDE_DIRECTORIES(base PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
GENERATE_EXPORT_HEADER(base
                       BASE_NAME OPENMA_BASE
                       EXPORT_FILE_NAME include/openma/base_export.h)

SET_TARGET_PROPERTIES(base PROPERTIES ${OPENMA_LIBRARY_PROPERTIES})
IF (NOT ${OPENMA_BUILD_SHARED_LIBS})
  SET_TARGET_PROPERTIES(base PROPERTIES COMPILE_FLAGS -DOPENMA_BASE_STATIC_DEFINE)
ENDIF()

IF(BUILD_LANGUAGE_BINDINGS)
  ADD_SUBDIRECTORY(swig)
ENDIF()

IF(BUILD_UNIT_TESTS)
  ADD_SUBDIRECTORY(test)
ENDIF()

INSTALL(TARGETS base EXPORT OpenMATargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib  
  INCLUDES DESTINATION lib
)