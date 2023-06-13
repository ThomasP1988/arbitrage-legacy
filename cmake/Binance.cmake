
set(binacpp_src 
${PROJECT_SOURCE_DIR}/external/binance/binacpp/binacpp.cpp 
${PROJECT_SOURCE_DIR}/external/binance/binacpp/binacpp_logger.cpp  
${PROJECT_SOURCE_DIR}/external/binance/binacpp/binacpp_utils.cpp 
)

add_library(binacpp STATIC ${binacpp_src})
target_include_directories(binacpp PUBLIC ${PROJECT_SOURCE_DIR}/external/binance)
link_libraries(binacpp curl)

