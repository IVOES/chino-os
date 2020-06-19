cmake_minimum_required (VERSION 3.13)

target_sources(drivers PRIVATE stm32f10x-usart.cpp)

CHINOC_RENDER(stm32f10x-usart-inl ${CMAKE_CURRENT_BINARY_DIR}/stm32f10x-usart.inl ${CMAKE_CURRENT_SOURCE_DIR}/stm32f10x-usart.inl.razor)
add_dependencies(drivers stm32f10x-usart-inl)
target_include_directories(drivers PRIVATE ${CMAKE_CURRENT_BINARY_DIR})