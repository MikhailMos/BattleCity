# BattleCity

Попытка сделать 2D игру. Делалось по видео канала [SimpleCoding](https://www.youtube.com/watch?v=wf37ukItjE8&list=PL6x9Hnsyqn2XU7vc8-oFLojbibK91fVd-)

## Сборка
Проект планируется быть кроссплатформенным, в настоящее время протестирован на Windows, Linux и Mac.
Для сборки необходимы только Git, CMake и компилятор

```powershell
git clone https://github.com/MikhailMos/BattleCity.git
cd BattleCity
mkdir build
cmake ..
cmake --build .
```

## Что реализовано:
* карта уровня
* танки
* различные коллизии

## Что нужно сделать:
* хаотичное движение вражеских танков
* массив вражеских танков
* жизни и урон танкам
* очьки
* бонусы
* переход на следующий уровень