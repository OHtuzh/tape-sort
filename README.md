## Запуск программы
#### Билд
```
mkdir build
cd build
cmake --build . --target TapeSort
```
#### Запуск
```
./bin/TapeSort --input=input_path --output=output_path --memory-limit=limit --config=config_path
```
`--input` - путь к входному файлу (обязательно) \
`--output` - путь к выходному файлу (обязательно) \
`--memory-limit` - ограничение по памяти (опционально) \
`--config` - путь к конфигу (опционально)
## Конфиг
```
read_delay=100
write_delay=10
rewind_delay=100
shift_delay=15
```
Имеет такие поля, измеряются в ms.
## Запуск тестов
#### Билд
```
mkdir build
cd build
cmake --build . --target tape-sort-tests
```
#### Запуск
```
./test/tape-sort-tests
```
Тесты рандомно генерируют массив чисел, поэтому рекомендуется запускать их несколько раз, например, вот так:
```
./test/tape-sort-tests --gtest_repeat=1000 --gtest_break_on_failure
```
