# Versão mínima do CMake e nome do projeto
cmake_minimum_required(VERSION 3.14)
project(TrabalhoED2 CXX)

# Define o padrão do C++ para C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Define a biblioteca CORE ---

# Encontra automaticamente todos os ficheiros .cpp dentro da pasta src e
# armazena a lista na variável CORE_SOURCES.
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")

# Cria a biblioteca "core" a partir da lista de ficheiros encontrados.
add_library(core STATIC ${CORE_SOURCES})

# Adiciona a pasta "include" ao caminho de busca, permitindo que a biblioteca
# encontre todos os seus ficheiros .h.
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Define o Executável ---
# Manda o CMake processar a subpasta "app", onde o programa principal é criado.
add_subdirectory(app)