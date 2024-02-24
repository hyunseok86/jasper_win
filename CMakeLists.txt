cmake_minimum_required(VERSION 3.10)
project(JASPER VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()



# DLL 파일 경로 설정
set(PostgreSQL_LIB_DIR "C:/JASPER/workspace/JASPER_WIN_DEMON/libs")
set(KCSAPI4_DLL_PATH "C:/JASPER/workspace/JASPER_WIN_DEMON/libs/KCSAPI4.dll")

# pugixml 다운로드 및 빌드
include(FetchContent)
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    GIT_TAG v1.11.4 # 릴리즈 태그 사용 (최신 버전 확인 필요)
)
FetchContent_MakeAvailable(pugixml) 

# 헤더 파일 디렉토리 추가
include_directories(${pugixml_SOURCE_DIR}/src)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(JASPER src/main.cpp)
target_link_libraries(JASPER PRIVATE pugixml)

set(CPACK_PROJECT_NAME ${PROJECT_NAME}) 
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# DLL 파일을 실행 파일과 같은 디렉토리로 복사
add_custom_command(
    TARGET JASPER POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${KCSAPI4_DLL_PATH}"
        $<TARGET_FILE_DIR:JASPER>
)

# config.ini 파일을 빌드 디렉토리로 복사
add_custom_command(
    TARGET JASPER POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.ini"
        $<TARGET_FILE_DIR:JASPER>
)
