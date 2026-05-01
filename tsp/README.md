## Решение 1

Стартуем с первой вершины и выбираем ближайшую из еще не посещенных.

Результат:

- tsp_51_1 got 506.3631653628, passing no threshold and receiving 0 points
- tsp_100_3 got 25138.7854527723, passing no threshold and receiving 0 points
- tsp_200_2 got 36226.2214381415, passing no threshold and receiving 0 points
- tsp_574_1 got 47054.9474467063, passing no threshold and receiving 0 points
- tsp_1889_1 got 391470.4454918891, passing no threshold and receiving 0 points
- tsp_33810_1 got 78478867.03022148, passing small threshold and receiving 3 points

sum points: 3

Видно, что работает отвратительно, но начало положено.

## Решение 2

Так же, только поочередно стартуем с каждой вершины. Также добавил ограничние на общее число операций чтобы большие тесты укладывались в минуту.

Результат:

- tsp_51_1 got 496.4090227267, passing no threshold and receiving 0 points
- tsp_100_3 got 23566.4029464274, passing no threshold and receiving 0 points
- tsp_200_2 got 35394.0087188639, passing small threshold and receiving 3 points
- tsp_574_1 got 44514.8353543506, passing no threshold and receiving 0 points
- tsp_1889_1 got 380279.9955720282, passing no threshold and receiving 0 points
- tsp_33810_1 got 77866836.57404383, passing small threshold and receiving 3 points

sum points: 6

На самом деле довольно значительное улучшение, вполне неплохой бейзлайн получается.

## Решение 3

Пробуем добавить локальный поиск! Пока что самый простой: инициализируем жадным поиском из нулевой вершины, пытаемся сделать 2opt на всех возможных парах вершин, самый лучший применяем (best-improve). Пока находим хоть какое-то улучшение или пока не превысили асимптотический порог числа операций, повторяем.

Результат:

- tsp_51_1 got 442.917448031, passing small threshold and receiving 3 points
- tsp_100_3 got 21431.233218346, passing small threshold and receiving 3 points
- tsp_200_2 got 31797.3380885428, passing small threshold and receiving 3 points
- tsp_574_1 got 42995.040110546, passing no threshold and receiving 0 points
- tsp_1889_1 got 378584.1207974767, passing no threshold and receiving 0 points
- tsp_33810_1 got 77776861.78646962, passing small threshold and receiving 3 points

sum points: 12

Тоже довольно значительно продвинулись. Сейчас буду пытаться разгонять локальный поиск разными эвристиками. Последний тест конечно невероятно огромный, его всего считать очень тяжело, и на нем если ограничивать время, получается что почти ничего не успеваем обойти.