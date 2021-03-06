# The MIT License (MIT)
#
# Copyright (c) 2020 Scott Aron Bloom
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.1)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

project(TestProject)
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/SABUtils/Modules ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
cmake_policy(SET CMP0020 NEW)
cmake_policy(SET CMP0043 NEW)

include( include.cmake )

add_executable( TestProject
                 ${project_SRCS} 
                 ${project_H} 
                 ${qtproject_SRCS} 
                 ${qtproject_QRC} 
                 ${qtproject_QRC_SRCS} 
                 ${qtproject_UIS_H} 
                 ${qtproject_MOC_SRCS} 
                 ${qtproject_H} 
                 ${qtproject_UIS}
                 ${qtproject_QRC_SOURCES}
                 ${_CMAKE_FILES}
                 ${_CMAKE_MODULE_FILES}
          )
set_target_properties( TestProject PROPERTIES FOLDER Apps )
          
target_link_libraries( TestProject )
