CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(rmsc)


# include system-wide packages
FIND_PACKAGE(FLEX)
FIND_PACKAGE(BISON)


############################################################################
# Global settings                                                          #
############################################################################
INCLUDE(rmsc.cmake)

ADD_DEFINITIONS(
# catch lazy errors during compilation
-pedantic -std=gnu99 -Wall -Werror

# use GNU extensions
-D_GNU_SOURCE

# verbose bison
-DYYERROR_VERBOSE
)

INCLUDE_DIRECTORIES(include ${JANSSON_INCLUDE_DIRS})

# support subdirectory builds
SET(RMSC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)


############################################################################
# rmsc sources                                                             #
############################################################################
BISON_TARGET(RMSC_PARSER source/rmsc.y ${CMAKE_BINARY_DIR}/rmsc.tab.c)
FLEX_TARGET(RMSC_SCANNER source/rmsc.l ${CMAKE_BINARY_DIR}/rmsc.lex.c
COMPILE_FLAGS "--header-file=${CMAKE_BINARY_DIR}/rmsc.lex.h"
)
ADD_FLEX_BISON_DEPENDENCY(RMSC_SCANNER RMSC_PARSER)

ADD_EXECUTABLE(rmsc
include/flexbison.h
source/main.c

include/ast.h
source/ast.c

include/ast_restful.h
source/ast_restful.c

include/ast_graphviz.h
source/ast_graphviz.c

source/rmsc.l
source/rmsc.y
${FLEX_RMSC_SCANNER_OUTPUTS}
${BISON_RMSC_PARSER_OUTPUTS}
)

# don't treat warning as error
SET_SOURCE_FILES_PROPERTIES(${CMAKE_BINARY_DIR}/rmsc.lex.c
PROPERTIES COMPILE_FLAGS -Wno-unused-function
)

# rmsc libraries sources
ADD_LIBRARY(librmsccore OBJECT
include/rmsc.h
source/rmsc.c
)

SET_PROPERTY(TARGET librmsccore PROPERTY COMPILE_FLAGS "-fPIC")


############################################################################
# targets                                                                  #
############################################################################
ADD_LIBRARY(librmsc SHARED $<TARGET_OBJECTS:librmsccore>)
TARGET_LINK_LIBRARIES(librmsc ${JANSSON_LIBRARIES})
SET_TARGET_PROPERTIES(librmsc PROPERTIES OUTPUT_NAME "rmsc")
SET_TARGET_PROPERTIES(librmsc PROPERTIES VERSION "0.1.1")

ADD_LIBRARY(librmsc_static STATIC $<TARGET_OBJECTS:librmsccore>)
SET_TARGET_PROPERTIES(librmsc_static PROPERTIES OUTPUT_NAME "rmsc")


############################################################################
# install targets                                                          #
############################################################################
INSTALL(TARGETS rmsc DESTINATION bin)
INSTALL(TARGETS librmsc librmsc_static DESTINATION lib)
INSTALL(FILES include/rmsc/rmsc.h DESTINATION include)
