cmake_minimum_required(VERSION 3.14)
project(Trabalho CXX)

# Define o padrão do C++ para C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontra todos os arquivos de codigo fonte na pasta src e suas subpastas
file(GLOB_RECURSE CORE_SOURCES 
    "src/*.cpp" 
    "src/**/*.cpp"
)

# Cria uma biblioteca chamada core com todo o código principal do seu projeto.
add_library(core ${CORE_SOURCES})

# Adiciona a pasta include e todas as suas subpastas ao caminho de busca de cabeçalhos.
target_include_directories(core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/data
    ${CMAKE_CURRENT_SOURCE_DIR}/include/entities
    ${CMAKE_CURRENT_SOURCE_DIR}/include/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/include/structures
)

# --- Seção de Testes ---
enable_testing()
# Manda o CMake entrar na pasta 'tests' e ler o CMakeLists.txt que está lá dentro
# add_subdirectory(tests)
add_subdirectory(app)

