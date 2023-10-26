# Thread Pool

В этой задаче мы построим базовый инфраструктурный компонент для параллельного исполнения задач – _пул потоков_ (_thread pool_).

Наш пул потоков будет _статическим_ – число потоков фиксируется при создании пула и не меняется до остановки пула.

Далее с потоками и параллельностью мы будем работать только через пулы.

## Схема работы

![Thread pool](images/thread_pool.png)

## Конструктор

```cpp
// Создаем пул из 4-х потоков
tp::ThreadPool pool{/*workers=*/4};
```

## `Submit`

Пул потоков исполняет [задачи](tp/task.hpp) – функции без аргументов и возвращаемого значения.

Запланировать задачу на исполнение в пуле можно с помощью метода `Submit`:

```cpp
tp::ThreadPool pool{4};

pool.Submit([]() {
  std::cout << "Hello from pool!" << std::endl;
});
```

Вызов `Submit` не дожидается завершения задачи, он лишь добавляет ее в очередь задач пула, после чего возвращает управление.

Метод `Submit` можно вызывать конкурентно из разных потоков.

### `Current`

Иногда хочется отправить новую задачу в пул прямо из бегущей задачи, т.е. из потока пула. 

Для этого нам пригодится свободная функция `tp::Current()`, которая возвращает
- указатель на текущий пул, если ее вызвали из потока-воркера, и
- `nullptr` в противном случае.

### Пример
```cpp
tp::ThreadPool pool{4};

pool.Submit([]() {
  // Планируем задачу из задачи
  tp::Current()->Submit([]() {
    std::cout << "Works!" << std::endl;
  });
});
```

## `WaitIdle`

Метод `WaitIdle` блокирует вызвавший его поток до тех пор, пока в пуле не закончатся задачи.

Метод `WaitIdle` можно вызывать несколько раз, а можно не вызывать ни разу.

Метод не изменяет состояние пула, он только наблюдает за ним.

### Пример
```cpp
tp::ThreadPool pool{4};

pool.Submit([]() {
  std::this_thread::sleep_for(1s);
  tp::Current()->Submit([]() {
    std::cout << "Hi" << std::endl;
  });
});

// К моменту вызова WaitIdle вложенная задача скорее всего еще не запланирована.
// Но вызов WaitIdle дождется ее завершения.
pool.WaitIdle();
// <-- На экране напечатано "Hi"
```

## Остановка

Пул должен быть остановлен до своего разрушения явно с помощью метода `Stop`.

Вызов `Stop` останавливает потоки-воркеры как можно быстрее, игнорируя уже запланированные, но еще не исполненные задачи. 

Вызов `Stop` возвращает управление, когда все потоки пула остановлены.

Вызвать метод `Stop` можно только один раз. 

Опоздавшие `Submit`-ы (не успевшие запланировать задачу до закрытия очереди) будут игнорироваться пулом.

## `WaitIdle` / `Stop`

Можно считать, что `WaitIdle` и `Stop` вызываются только одним потоком, т.е. только последовательно.

## Очередь

Для распределения задач между потоками пула будем использовать `UnboundedBlockingQueue` – общую блокирующую очередь неограниченной емкости.

### API

| Метод | Семантика |
|---|---|
| `bool Put(T value)` | Если очередь еще не закрыта (вызовом `Close` или `Cancel`), то положить в нее `value` и вернуть `true`, в противном случае вернуть `false`. |
| `std::optional<T> Take()` | Дождаться и извлечь элемент из головы очереди; если же очередь закрыта и пуста, то вернуть `std::nullopt`. |
| `void Close()` | Закрыть очередь для новых `Put`-ов. Уже добавленные в очередь элементы останутся доступными для извлечения. |
| `void Cancel()` | Закрыть очередь для новых `Put`-ов и выбросить все неизвлеченные элементы.

### Bounded vs Unbounded

Обратите внимание: пулу потоков нужна очередь именно неограниченной емкости.

Придумайте сценарий дедлока в пуле с очередью, которая блокирует поток-продьюсер при переполнении буфера.

## Замечания

### Возврат значения / ошибки, зависимости

- Метод `Submit` в пуле не предполагает никакого механизма возврата значения или ошибки из задачи.
  
- Пул потоков не дает возможности задать зависимости между планируемыми задачами.

И то, и другое, безусловно, полезно. Но в нашем пуле потоков ничего этого не будет, он останется максимально простым.

Требуемая функциональность (возврат результата, планирование задач с учетом завимостей) будет достигнута с помощью отдельных инструментов / абстракций, которые будут хорошо комбинироваться с пулом потоков, но при этом будут ортогональны ему.

## Задание

1) Реализуйте блокирующую очередь – [`UnboundedBlockingQueue`](tp/blocking_queue.hpp)
2) Реализуйте пул потоков – [`ThreadPool`](tp/thread_pool.hpp)

Изучите [юнит-тесты](tests/unit) очереди и пула потоков чтобы разобраться с их поведением.

## Замечания по реализации

Вы можете добавлять в библиотеку `tp` новые файлы,

Добавлять новые публичные методы в очередь и пул нельзя.

### `WaitIdle`

Посмотрите на [Go by Example: WaitGroups](https://gobyexample.com/waitgroups), возможно вам будет полезен подобный примитив.

Методы `WaitIdle` и `Stop` должны быть ортогональны друг другу.

### Деструктор

Напишите `assert` в деструктор на случай забытого вызова `Stop`.

### `thread_local`

Задача тестируется под файберами, поэтому реализовать функцию `tp::Current` через [`thread_local`](https://en.cppreference.com/w/cpp/keyword/thread_local) указатель на пул мы не можем.

Вместо `thread_local` мы будем использовать `twist::util::ThreadLocalPtr`:

```cpp
// ThreadLocalPtr<T> - замена thread_local T*
// - Для каждого потока хранит собственное значение указателя
// - Повторяет интерфейс указателя
// - Инициализируется nullptr-ом
ThreadLocalPtr<ThreadPool> pool;
```