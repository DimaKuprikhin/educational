#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Программа моделирует работу гостиницы, которая обслуживает клиентов и
// выдает им свои номера. С гостиницой взаимодействуют города, которые
// генерируют клиентов и обращаются к гостиницы для их обслуживания.
// Гостиница при обслуживании либо выдает клиенту комнату, если есть свободные
// номера, на которые у клиента есть деньги, либо сообщает о характере отказа:
// гостиница заполнена или у клиента нет денег ни на один из свободных номеров.
// Все города и гостиница работают в отдельных потоках. Города взаимодействуют
// с гостиницой через семафор.
// При выводе протокола указывается время симуляции, которое обновляет
// гостиница в своем потоке, для чего тоже использует семафор, чтобы в момент
// вывода очередной строки протокола время было корректно.
// Количество городов, общее количество клиентов, продолжительность каждого
// часа симуляции и задержка перед очередной генерацией клиента в городе
// задаются через аргументы командной строки.

// Класс, хранящий всю информацию о клиенте: количество денег, его 
// идентификатор и идентификатор его города.
class Client {
 public:
    Client(int money, int clientId, int cityId) : 
            money(money), clientId(clientId), cityId(cityId) {}

    int money;
    int clientId;
    int cityId;
};

class Hotel {
 public:
    Hotel(int hourDuration, sem_t* sem) :
            hourDuration(hourDuration), semaphor(sem) {}
    
    // Метод, обслуживающий прибывшего клиента. Выдает комнату, если достаточно
    // денег и есть свободные комнаты. Иначе, сосояние гостинницы не изменяется.
    void handleClient(Client& client)
    {
        // Проверяем, есть ли свободные места в гостиннице. Если нет, то 
        // заканчиваем обслуживание клиента с соответствующим сообщением.
        bool isFull = true;
        for(int i = 0; i < rooms.size(); ++i) {
            if(!rooms[i].second) {
                isFull = false;
            }
        }
        if(isFull) {
            printf("Day%d %d:00 -> Client %d from city %d arrived to the hotel"
                    " but it was full\n",
                    day, hour, client.clientId, client.cityId);
            return;
        }

        // Пытаемся найти комнату, проверяя сначала самые дорогие.
        for(int i = 24; i >= 0; --i) {
            if(client.money >= rooms[i].first && !rooms[i].second) {
                printf("Day%d %d:00 -> Client %d from city %d arrived to the hotel"
                        " and took the room %d\n",
                        day, hour, client.clientId, client.cityId, i);
                rooms[i].second = true;
                return;
            }
        }

        // Если клиент не смог купить ни одну комнату, выводим сообщение об этом.
        printf("Day%d %d:00 -> Client %d from city %d arrived to the hotel"
                " but couldn't afford any spare room\n",
                day, hour, client.clientId, client.cityId);
    }
    
    // Поля для хранения текущего времени симуляции.
    int day = 1;
    int hour = 8;
    // Количество милисекунд, равное одному часу в симуляции.
    int hourDuration = 1000;
    // Вектор комнат. Первое значение пары означает цену комнаты, 
    // второе - занята ли она.
    std::vector<std::pair<int, bool>> rooms;
    // Семафор, использующийся для блокировки во время изменения времени 
    // симуляции.
    sem_t* semaphor;
};

// Класс, реализующий сущность города. Из города со случайным промежутком в 
// заданном интервале отправляются клиенты со случайным количеством денег в
// гостинницу.
class City {
 public:
    City() {}

    City(int cityId, int startClientId, int maxClientId, int maxTime, 
            sem_t* sem, Hotel* hotel) :
            cityId(cityId), clientIdCounter(startClientId),
            maxClientId(maxClientId), maxTimeBetweenClients(maxTime),
            semaphor(sem), hotel(hotel) {}

    // Идентификатор города
    int cityId;
    // Счетчик идентификаторов клиентов.
    int clientIdCounter;
    // Максимальное невключительно число, до которого необходимо генерировать
    // клиентов.
    int maxClientId;
    // Максимально возможное время, на которое засыпает поток после отправки
    // клиента в гостинницу.
    int maxTimeBetweenClients = 1000;
    // Семафор для доступа к гостиннице.
    sem_t* semaphor;
    // Указатель на готинницу, в которую отправляются клиенты.
    Hotel* hotel;
};

// Функция, осуществляющая обновление времени симуляции.
void* refreshLocalHotelTime(void* arg) {
    Hotel* hotel = (Hotel*)arg;
    while(true) {
        // Блокируем семафор.
        sem_wait(hotel->semaphor);
        ++hotel->hour;
        // Если прошла ночь, выгоняем всех клинтов из гостинницы.
        if(hotel->hour == 8) {
            std::cout << "Day" << hotel->day << " " << hotel->hour << ":00 -> " <<
                    "All clients left the hotel after the night" << std::endl;
            for(int i = 0; i < hotel->rooms.size(); ++i) {
                hotel->rooms[i].second = false;
            }
        }
        if(hotel->hour == 24) {
            hotel->hour = 0;
            ++hotel->day;
        }
        // Разблокируем семафор и засыпаем на заданное кол-во времени.
        sem_post(hotel->semaphor);
        struct timespec sleepTime = {hotel->hourDuration / 1000, 
                                    hotel->hourDuration % 1000 * (int)1e6};
        nanosleep(&sleepTime, NULL);
    }
    return 0;
}

// Функция, генерирующая клиентов для городов и отправляющая их в гостинницу.
void* produceClients(void* arg) {
    City* city = (City*)arg;
    while(city->clientIdCounter < city->maxClientId) {
        // Создаем клиента со случайным количеством денег и уникальным идентификатором.
        Client client(rand() % 700, city->clientIdCounter++, city->cityId);
        // Выводим сообщение о том, что клиент отправился из города.
        printf("Day%d %d:00 -> Client %d from city %d with $%d went to the hotel\n",
                city->hotel->day, city->hotel->hour, client.clientId, client.cityId, client.money);
        // Блокируем семафор для обращения к гостиннице.
        sem_wait(city->semaphor);
        city->hotel->handleClient(client);
        // Разблокируем семафор и засыпаем на случайное кол-во времени.
        sem_post(city->semaphor);
        int randValue = rand() % city->maxTimeBetweenClients;
        struct timespec sleepTime = {randValue / 1000, randValue % 1000 * (int)1e6};
        nanosleep(&sleepTime, NULL);
    }
    return 0;
}

// argv[1] - количество городов.
// argv[2] - общее количество клиентов.
// argv[3] - кол-во милисекунд для одного часа симуляции (по умолчанию = 1000).
// argv[4] - максимальное кол-во милисекунд, на которое засыпают потоки
//           после отправки клиента в гостинницу (по умолчанию = 1000).
int main(int argc, char** argv) {
    
    if(argc < 3) {
        std::cout << "Too few arguments\n";
        return 1;
    }
    srand(time(0));

    // Получаем аргументы командной строки.
    int numberOfCities = std::stoi(argv[1]);
    int numberOfClients = std::stoi(argv[2]);
    int hourDuration = 1000;
    int maxTimeBetweenClients = 1000;
    if(argc >= 4)
        hourDuration = std::stoi(argv[3]) > 0 ? std::stoi(argv[3]) : 1000;
    if(argc >= 5)
        maxTimeBetweenClients = std::stoi(argv[4]) > 0 ? std::stoi(argv[4]) : 1000;

    // Создаем общий семафор.
    sem_t semaphor;
    sem_init(&semaphor, 1, 1);

    // Заполняем отель комнатами из задания.
    Hotel hotel(hourDuration, &semaphor);
    for(int i = 0; i < 25; ++i) {
        if(i < 10)
            hotel.rooms.push_back(std::make_pair(200, false));
        else if(i < 20)
            hotel.rooms.push_back(std::make_pair(400, false));
        else
            hotel.rooms.push_back(std::make_pair(600, false));
    }

    // Создаем города.
    std::vector<City> cities;
    for(int i = 0; i < numberOfCities; ++i) {
        cities.emplace_back(i + 1, numberOfClients * i / numberOfCities, 
                        numberOfClients * (i + 1) / numberOfCities, 
                        maxTimeBetweenClients, &semaphor, &hotel);
    }

    // Запускаем поток для обновления времени симуляции.
    pthread_t hotelThread;
    pthread_create(&hotelThread, NULL, refreshLocalHotelTime, &hotel);

    // Запускаем потоки для каждого города.
    std::vector<pthread_t> threads(numberOfCities);
    for(int i = 0; i < threads.size(); ++i) {
        pthread_create(&threads[i], NULL, produceClients, &cities[i]);
    }
    // Ждем завершения потоков.
    for(int i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
