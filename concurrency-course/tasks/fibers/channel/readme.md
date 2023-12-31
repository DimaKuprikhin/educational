# Каналы

## Пререквизиты

- [fibers/mutex](/tasks/fibers/mutex)

## Go Proverb

_Do not communicate by sharing memory; instead, share memory by communicating._

[Share Memory By Communicating](https://blog.golang.org/codelab-share)

### Communicating Sequential Processes

- [Original paper](https://www.cs.cmu.edu/~crary/819-f09/Hoare78.pdf)
- [Book](http://www.usingcsp.com/cspbook.pdf)


## Буферизированный канал

[A Tour of Go / Buffered Channels](https://tour.golang.org/concurrency/3)

Реализуйте [`Channel<T>`](mtf/fibers/sync/channel.hpp) – канал с буфером фиксированного размера, который позволяет отправлять данные из одного файбера в другой.

Метод `Send` блокирует _файбер_ (не поток!) если буфер канала заполнен, метод `Receive` – если пуст.

Канал – MPMC (_multiple producers_ / _multiple consumers_).

Для простоты мы обойдемся без `Close` и неблокирующих вариаций `TrySend` / `TryReceive`.

### Синхронизация

Для синхронизации продьюсеров и консьюмеров используйте спинлок (так делают в Golang). Лок-фри реализация не требуется.

### Рандеву

Если в канале есть ждущие консьюмеры, то в `Send` передавайте сообщение консьюмеру напрямую, со стека на стек, минуя буфер канала.

### Аллокации

Методы `Send` / `Receive` не должны выполнять динамических аллокаций памяти:

- Для списка заблокированных продьюсеров / консьюмеров используйте интрузивные списки
- [Опционально] Для буфера сообщений используйте циклический буфер

### Misc

Не пишите в `Send` / `Receive` код, похожий на код с кондварами, в котором есть завернутое в цикл блокирующее ожидание. В обоих методах достаточно заблокироваться только один раз.

## `Select`

[Go by Example: Select](https://gobyexample.com/select)

Реализуйте функцию [`Select(xs, ys)`](mtf/fibers/sync/select.hpp), которая блокирует файбер до появления первого сообщения в одном из двух каналов `xs` / `ys`:

```cpp
 Channel<X> xs;
 Channel<Y> ys;
 
 // ...

 std::variant<X, Y> value = Select(xs, ys);
 switch (value.index()) {
   case 0:
     // Handle std::get<0>(value);
     break;
   case 1:
     // Handle std::get<1>(value);
     break;
 }
```

В `Select` разрешается передавать только разные каналы, вызов `Select(xs, xs)` – UB.

### Сценарии

`Select`-ы могут
- конкурировать как с `Receive`-ами, так и с другими `Select`-ами
- пересекаться по наборам каналов
- ждать на одних и тех же каналах в разном порядке

### Golang

В языке Go `select` более выразительный:
- В нем не фиксировано число каналов
- В нем можно как получать, так и отправлять сообщения
- У него есть неблокирующий вариант (с `default`)

### Число каналов

Придуманный вами алгоритм синхронизации для `Select` должен обобщаться на произвольное число каналов.

Мы ограничили `Select` двумя каналами чтобы избежать сложного шаблонного кода в реализации.

Задача со звездочкой: реализуйте variadic `Select`, работающий с произвольным числом каналов.

### Fairness

Реализация `Select` не должна отдавать приоритет одному из каналов: если `Select` вызывается в цикле, то файбер должен регулярно получать сообщения из обоих каналов.

### Аллокации

Реализация `Select` не должна выполнять динамических аллокаций памяти.

Используйте `IntrusiveList`. Вам пригодится метод `Unlink` у `IntrusiveListNode`.

### Применения

- Ожидание с таймаутом: [Go by Example / Tickers](https://gobyexample.com/tickers)
- Мультиплексирование событий из разных источников: [etcd/raft](https://github.com/etcd-io/etcd/blob/bd4f8e2b6c6a0bdcd52f4593f68d9f2415ab5293/raft/node.go#L341)
