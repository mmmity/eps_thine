# Решение 1 
Жадный набор множеств в порядке отношения количества покрытых новых элементов к стоимости. Не уверен что есть хорошие оценки на это, но вроде как-то сработало.

Результат:

- test sc_157_0: 102100
- test sc_330_0: 30
- test sc_1000_11: 170
- test sc_5000_1: 36
- test sc_10000_5: 76
- test sc_10000_2: 192

Пробиты все нижние пороги кроме второго теста. Будем дальше оптимизировать...

# Решение 2
Простейший BnB, в котором мы отсекаем ветвь если нижняя граница на нее больше чем текущий оптимальный результат. Нижняя граница считается как решение двойственной задачи к линейной релаксации, а верхняя - жадное решение из решения 1. На самом деле получается сильно лучше, и не так уж и долго. Плюс я добавил всяких оптимизаций, типа пересчитывать upper bound не каждый раз. Пробовал еще сортировать элементы в каком-то порядке, либо менять жадную UB, но от этого только хуже становилось. 

Результат:
- sc_157_0 got 102100, passing small threshold and receiving 3 points
- sc_330_0 got 27, passing small threshold and receiving 3 points
- sc_1000_11 got 161, passing small threshold and receiving 3 points
- sc_5000_1 got 34, passing small threshold and receiving 3 points
- sc_10000_5 got 73, passing small threshold and receiving 3 points
- sc_10000_2 got 192, passing small threshold and receiving 3 points

sum points: 18

# Решение 3

Сделал обход best-first, идти сначала туда где upper bound меньше. Это немножко улучшило результат, но недостаточно

Результат:
- sc_157_0 got 100600, passing small threshold and receiving 3 points
- sc_330_0 got 27, passing small threshold and receiving 3 points
- sc_1000_11 got 158, passing small threshold and receiving 3 points
- sc_5000_1 got 33, passing small threshold and receiving 3 points
- sc_10000_5 got 71, passing small threshold and receiving 3 points
- sc_10000_2 got 191, passing small threshold and receiving 3 points

sum points: 18

# Решение 4

Несколько раз посемплить по малому числу элементов, решить задачу минимального покрытия элементов при помощи дейкстры на масках, взять это решение и добить его до жадного. Сделать так 500 раз.

Результат:

- sc_157_0 got 94400, passing BIG threshold and receiving 5 points
- sc_330_0 got 27, passing small threshold and receiving 3 points
- sc_1000_11 got 155, passing small threshold and receiving 3 points
- sc_5000_1 got 32, passing small threshold and receiving 3 points
- sc_10000_5 got 69, passing small threshold and receiving 3 points
- sc_10000_2 got 185, passing small threshold and receiving 3 points

sum points: 20

Хорошо сработало на тесте в котором всего 29 элементов, так что норм наверное. Остальные кстати тоже в итоге получше чем bnb оказались. Работало максимум полторы минуты у меня на macbook air.

# Решение 5

Пробуем прокрутить локальный поиск: сначала добиваем решение GRASP'ом, на каждой итерации выбирая случайное из топ-k множеств по покрытиям. Также рассчитываем веса элементов через их частоту встречаемости, а веса множеств через веса элементов. Сначала какое-то количество раз прогоняемся по следующей тактике: инициализируем grasp'ом со случайным числом топов, потом много раз удаляем несколько случайных множеств из решения и добиваем до хорошего решения жадно. Если слишком долго не было улучшений, начинаем удалять больше. Если еще дольше не было улучшений, выходим. Также иногда принимаем решение, которое ухудшает результат, но не сильно. Потом чуть дольше крутимся примерно так же, только еще каждый раз обновляем веса элементов - если его покрывает хотя бы два множества из предыдущего решения, уменьшаем вес, иначе увеличиваем.

Результат:

- sc_157_0 got 96400, passing small threshold and receiving 3 points
- sc_330_0 got 23, passing BIG threshold and receiving 5 points
- sc_1000_11 got 146, passing BIG threshold and receiving 5 points
- sc_5000_1 got 30, passing BIG threshold and receiving 5 points
- sc_10000_5 got 64, passing BIG threshold and receiving 5 points
- sc_10000_2 got 167, passing BIG threshold and receiving 5 points

sum points: 28

Работало примерно 30 секунд на самом большом тесте на моем компьютере. Видим, что локальный поиск очень хорошо всё сделал и привел нас почти к идеальному решению. Только на маленьком тесте плохо сработал, полагаю это из-за того, что grasp не очень хорошо работал, плюс удаление небольшого числа множеств из маленького решения не очень хорошо его меняет.

# Решение 6
Остается вспомнить, что на маленьких тестах хорошо работало решение 4, потому что буквально было полным перебором. Будем на маленьких тестах запускать Решение 4, а на остальных Решение 5.

Результат:

- sc_157_0 got 94400, passing BIG threshold and receiving 5 points
- sc_330_0 got 23, passing BIG threshold and receiving 5 points
- sc_1000_11 got 146, passing BIG threshold and receiving 5 points
- sc_5000_1 got 30, passing BIG threshold and receiving 5 points
- sc_10000_5 got 64, passing BIG threshold and receiving 5 points
- sc_10000_2 got 167, passing BIG threshold and receiving 5 points

sum points: 30

Я уверен, что это оптимальные решения, но я не могу это доказать... Считаю задачу решенной.