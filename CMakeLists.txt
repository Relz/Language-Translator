cmake_minimum_required(VERSION 3.8)
project(LanguageTranslator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp CLanguage.cpp CLanguage.h Input/Constant.h UnorderedSetHash.h)
add_executable(LanguageTranslator ${SOURCE_FILES})
