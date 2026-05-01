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

