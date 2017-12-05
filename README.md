# Magnas Tareas
Magnas Tareas is a todo app inspired by [HaikuToDo](http://github.com/AdrianArroyoCalle/haiku-todo)

The app is divided in a C++ core, a GUI frontend and multiple tasks backends

## Platforms

* Windows
* Linux
* Ubuntu Phone - planned
* Android - planned

## UI frontends

 * Qt 5 with QML
 * Haiku - planned
 * wxWidgets - planned

## Tasks backends

 * Local tasks
 * BeFS Tasks - only on Haiku
 * Google Tasks
 * Remember the Milk
 * Workflowy
 * HabitRPG
 * Doris
 * Hiveminder
 * Nozbe
 * Toodledo

# BUILD

```
(sudo apt install qtbase5-dev qtdeclarative5-dev qml-module-qtquick-controls qml-module-qtquick-dialogs)
mkdir build
cd build
cmake ..
make
```
