# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-src"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-build"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/tmp"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/src/googletest-stamp"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/src"
  "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/d/Coding/Codam/Projecten/webserv/build/gtest/googletest-download/googletest-prefix/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
