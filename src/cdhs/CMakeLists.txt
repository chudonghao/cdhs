file(GLOB SRCS *.cpp *.h */*.cpp */*.h)

add_library(cdhs STATIC ${SRCS})
target_link_libraries(cdhs PRIVATE LLVMCore)