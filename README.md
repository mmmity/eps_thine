# eps_thine
May the eps be thine! Discrete Optimization course work

Сборка всего подряд: `mkdir -p build && cmake -B build -DCMAKE_BUILD_TYPE=RELEASE && cmake --build build`

(RELEASE ставит высокий уровень оптимизаций который реально оч сильно ускоряет происходящее. Есть еще DEBUG, в нем стоят санитайзеры и прикольный прогресс бар)

После сборки можно запускать два скрипта из корня репозитория:
- `python3 scripts/check_all_tests_valid.py` запустит актуальные солверы на всех тестах и проверит что они выдают осмысленный ответ. Результаты запусков, в том числе ответы, будут лежать в `<task>/results`
- `python3 scripts/run_benchmark_tests.py` запустит тесты с оценкой и проверит какие пороги они проходят. Результат будет лежать в папке `benchmark_results`

На оба скрипта настроен CI, так что вроде как файлы в `benchmark_results` всегда актуальные.

Отчеты по задачам лежат в файлах `<tartet>/README.md`