cmake_minimum_required(VERSION 3.28)
project(TypeThrill)

set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS FALSE)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY  https://github.com/SFML/SFML
        GIT_TAG         2.6.1
)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         10.2.1
)

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(fmt)


add_executable(TypeThrill
        Globals.cpp
        Globals.h
        main.cpp
        Game/Game.cpp
        Game/Game.h
        Settings.cpp
        Settings.h
        Game/GameInterface.cpp
        Game/GameInterface.h
        UI/Button.cpp
        UI/Button.h
        ResourceManagers/FontManager.cpp
        ResourceManagers/FontManager.h
        UI/Checkbox.cpp
        UI/Checkbox.h
        UI/UIElement.h
        UI/Panel.cpp
        UI/Panel.h
        UI/UIElementFactory.h
        UI/TextField.cpp
        UI/TextField.h
        UI/TextLabel.cpp
        UI/TextLabel.h
        UI/DynamicTextLabel.cpp
        UI/DynamicTextLabel.h
        ResourceManagers/TextureManager.cpp
        ResourceManagers/TextureManager.h
        UI/Word.h
        UI/Word.cpp
        Translator/WordLocales.cpp
        Translator/WordLocales.h
        Game/WordSpawner.cpp
        Game/WordSpawner.h
        Game/GameStatistics.cpp
        Game/GameStatistics.h
        UI/ComboBox.cpp
        UI/ComboBox.h
        Misc/PosRatio.h
        Game/Highscores.cpp
        Game/Highscores.h
        ResourceManagers/GameSaveManager.cpp
        ResourceManagers/GameSaveManager.h
)
target_link_libraries(TypeThrill sfml-graphics sfml-audio fmt)