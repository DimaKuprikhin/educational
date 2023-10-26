package backend;

import database.AbstractDatabase;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.Collection;
import java.util.Objects;
import java.util.function.Predicate;

/**
 * Класс, реализующий представителя в паттерне MVP. Используется
 * пользовательским для обращения к базе данных.
 */
public class Presenter {

    /**
     * Ссылка на базу контактов.
     */
    @NotNull
    private final AbstractDatabase database;

    /**
     * Логгер представителя.
     */
    private static final Logger logger =
            LogManager.getLogger(Presenter.class.getName());

    public Presenter(@NotNull AbstractDatabase database) {
        this.database = Objects.requireNonNull(database, "База данных" +
                " является налом.");
    }

    /**
     * Сохраняет изменения в базе.
     * @throws IOException При ошибке записи изменений.
     */
    public void saveDatabase() throws IOException {
        database.save();
        logger.info("Сохраняем базу.");
    }

    /**
     * Получение коллекции контактов по запросу.
     * @param predicate Предикат, указывающий, какие элементы необходимо
     *                  вернуть.
     * @return Unmodifiable коллекцию контактов, удовлетворяющих предикату.
     */
    @NotNull
    public Collection<Contact> getByPredicate(
            @NotNull Predicate<Contact> predicate) {
        Objects.requireNonNull(predicate, "Ссылка не предикат поиска является" +
                " налом.");
        return database.getByPredicate(predicate);
    }

    /**
     * Добавляет контакт в базу. Если в базе уже есть дубликат этого контакта,
     * то база не меняется.
     * @param contact Добавляемый контакт.
     * @return True, если этот контакт был добавлен, false, если такой контакт
     * уже находился в базе.
     */
    public boolean addContact(@NotNull Contact contact) {
        Objects.requireNonNull(contact, "Ссылка на добавляемый контакт" +
                " является налом.");
        logger.info("Пытаемся добавить контакт " + contact.toString());
        if(database.getByPredicate(
                contactInContacts -> contactInContacts.equals(contact))
                .size() > 0) {
            return false;
        }
        database.add(contact);
        return true;
    }

    /**
     * Удаляет контакт из базы. При отсутствии удаляемого контакта в базе,
     * состояние базы не меняется.
     * @param contact Удаляемый контакт.
     * @return True, если элемент был удален, false, если удаляемого элемента
     * не было в базе.
     */
    public boolean removeContact(@NotNull Contact contact) {
        Objects.requireNonNull(contact, "Ссылка на удаляемый контакт" +
                " является налом.");
        logger.info("Пытаемся удалить контакт " + contact.toString());
        return database.remove(contact);
    }
}
