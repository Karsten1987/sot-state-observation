# Copyright 2010, François Bleibel, Olivier Stasse, JRL, CNRS/AIST,
#                 Florent Lamiraux (CNRS/LAAS)
#
# This file is part of sot-dyninv.
# sot-dyninv is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# sot-dyninv is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# sot-dyninv. If not, see <http://www.gnu.org/licenses/>.

INCLUDE(../cmake/python.cmake)

# prefix and suffix each element of list by ${prefix}elemnt${suffix}
macro(ADDPREFIX newlist prefix list_name)
    # create empty list - necessary?
    SET(${newlist})

     # prefix and suffix elements
    foreach(l ${${list_name}})
      list(APPEND ${newlist} ${prefix}${l} )
    endforeach()

endmacro(ADDPREFIX)

ADDPREFIX(${PROJECT_NAME}_ABSOLUTE_HEADERS  "${CMAKE_SOURCE_DIR}/"  ${PROJECT_NAME}_HEADERS)

SET(LIBRARY_NAME ${PROJECT_NAME})

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  dg-imu-attitude-estimation.cc
  dg-imu-flexibility-estimation.cc
  ${${PROJECT_NAME}_ABSOLUTE_HEADERS}
  )

SET_TARGET_PROPERTIES(${LIBRARY_NAME}
  PROPERTIES
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# provide path to dependency libraries
LINK_DIRECTORIES( ${STATE_OBSERVATION_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_DEPENDENCIES(${LIBRARY_NAME} state-observation sot-core dynamic-graph dynamic-graph-python)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} state-observation sot-core dynamic-graph dynamic-graph-python)

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} state-observation)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} sot-core)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph-python)


INSTALL(TARGETS ${LIBRARY_NAME}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

STRING(REPLACE - _ PYTHON_LIBRARY_NAME ${LIBRARY_NAME})




SET(NEW_ENTITY_CLASS
#  DGIMUFlexibilityEstimation
#  DGIMUAttitudeEstimation
  )

DYNAMIC_GRAPH_PYTHON_MODULE("sot/core/${PYTHON_LIBRARY_NAME}" ${LIBRARY_NAME} wrap)
