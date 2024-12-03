cmake_minimum_required(VERSION 3.28)
project(ProyectoAlgoritmos)
set(SFML_DIR "C:/SFML/lib/cmake/SFML") # Cambia a la ruta correcta
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_executable(ProyectoAlgoritmos main.cpp
        LinearSearch.cpp
        LinearSearch.h
        BinarySearch.cpp
        BinarySearch.h
        bubbleSort.cpp
        bubbleSort.h
        mergeSort.cpp
        mergeSort.h
        QuickSort.cpp
        QuickSort.h
        insertionSort.cpp
        insertionSort.h
        selectionSort.cpp
        selectionSort.h
        stacks.cpp
        stacks.h
        queues.cpp
        queues.h
        linkedLists.cpp
        linkedLists.h
        dijkstra.cpp
        dijkstra.h
        AVL.cpp
        AVL.h
        grafos.cpp
        grafos.h
)
target_link_libraries(ProyectoAlgoritmos sfml-graphics sfml-window sfml-system)