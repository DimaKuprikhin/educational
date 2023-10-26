package database;

import backend.Contact;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.Collection;
import java.util.function.Predicate;

/**
 * Интерфейс для взаимодействия с базой данных контактов.
 */
public interface AbstractDatabase {

    /**
     * Сохраняет изменеия в базе.
     * @throws IOException При ошибке записи изменений.
     */
    void save() throws IOException;

    /**
     * Получение коллекции контактов по запросу.
     * @param predicate Предикат, указывающий, какие элементы необходимо
     *                  вернуть.
     * @return Unmodifiable коллекцию контактов, удовлетворяющих предикату.
     */
    Collection<Contact> getByPredicate(@NotNull Predicate<Contact> predicate);

    /**
     * Безусловно добавляет контакт в базу.
     * @param contact Добавляемый контакт.
     */
    void add(@NotNull Contact contact);

    /**
     * Удаляет контакт из базы. При отсутствии удаляемого контакта в базе,
     * состояние базы не меняется.
     * @param contact Удаляемый контакт.
     * @return True, если элемент был удален, false, если удаляемого элемента
     * не было в базе.
     */
    boolean remove(@NotNull Contact contact);

}
