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
