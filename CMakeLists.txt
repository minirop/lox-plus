cmake_minimum_required(VERSION 3.8)
project(loxplus)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp Scanner.cpp Scanner.h Lox-plus.cpp Lox-plus.h Token.h TokenType.h Object.h ast.h Parser.cpp Parser.h ParseError.cpp ParseError.h Interpreter.cpp Interpreter.h Environment.cpp Environment.h Object.cpp LoxCallable.h LoxFunction.cpp LoxFunction.h Return.cpp Return.h RuntimeError.cpp RuntimeError.h Resolver.cpp Resolver.h LoxClass.cpp LoxClass.h LoxInstance.cpp LoxInstance.h)
add_executable(loxplus ${SOURCE_FILES})

add_executable(ast-generator generator.cpp)
