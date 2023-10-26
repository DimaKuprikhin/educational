package database;

import backend.Contact;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.*;
import java.util.function.Predicate;

/**
 * Класс, реализующий базу для хранения списка контактов в текстовом файле в
 * формате JSON.
 */
public class Database implements AbstractDatabase {

    /**
     * Хранимые контакты. Ни один элемент не равен null.
     */
    @NotNull
    private final ArrayList<Contact> contacts = new ArrayList<>();

    /**
     * Строковая константа имени файла, хранящего базу контактов.
     */
    private final String DATABASE_FILE_NAME = "database.txt";

    /**
     * Маппер JSON сериализации.
     */
    private static final ObjectMapper mapper = new ObjectMapper();

    /**
     * Логгер базы.
     */
    private static final Logger logger =
            LogManager.getLogger(Database.class.getName());

    /**
     * Конструктор базы. Десериализует контакты из файла database.txt, если
     * возможно открыть файл.
     * @throws IOException При ошибке чтения из файла или ошибке десериализации.
     */
    public Database() throws IOException {
        BufferedReader reader;
        try {
            File file = new File(DATABASE_FILE_NAME);
            if(file.exists()) {
                reader = new BufferedReader(
                        new FileReader(DATABASE_FILE_NAME));
                logger.info("Удалось открыть database.txt.");
            }
            else {
                logger.info("database.txt не существует при запуске программы.");
                return;
            }
        } catch (FileNotFoundException ex) {
            // Обрабатываем FileNotFoundEx в случаях, когда файл существует,
            // но файл не удалось открыть при создании FileReader.
            logger.log(Level.ERROR,
                    "Не удалось открыть database.txt для чтения.", ex);
            return;
        }
        catch(SecurityException ex) {
            logger.log(Level.ERROR,
                    "Отказано в доступе чтения database.txt.", ex);
            return;
        }

        String serialized;
        // Построчно считываем из файла и десериализуем каждую строку.
        while ((serialized = reader.readLine()) != null) {
            contacts.add(mapper.readValue(serialized, Contact.class));
            logger.info("Десериализован контакт " +
                    contacts.get(contacts.size() - 1));
        }
        reader.close();
    }

    /**
     * Сохраняет изменеия в базе.
     * @throws IOException При ошибке записи изменений.
     */
    @Override
    public void save() throws IOException{
        BufferedWriter writer =
                new BufferedWriter(new FileWriter(DATABASE_FILE_NAME));
        // Записываем сериализованные элементы построчно в файл.
        for(int i = 0; i < contacts.size(); ++i) {
            writer.write(mapper.writeValueAsString(contacts.get(i)));
            writer.newLine();
        }
        writer.close();
        logger.info("База успешно сохранена.");
    }

    /**
     * Получение коллекции контактов по запросу.
     * @param predicate Предикат, указывающий, какие элементы необходимо
     *                  вернуть.
     * @return Unmodifiable коллекцию контактов, удовлетворяющих предикату.
     */
    @Override
    @NotNull
    public Collection<Contact> getByPredicate(
            @NotNull Predicate<Contact> predicate) {
        Objects.requireNonNull(predicate, "Предикат поиска является нулевой" +
                " ссылкой.");
        return Collections.unmodifiableCollection(new ArrayList<>(Arrays.asList(
                contacts.stream().filter(predicate).toArray(Contact[]::new))));
    }

    /**
     * Безусловно добавляет контакт в базу.
     * @param contact Добавляемый контакт.
     */
    @Override
    public void add(@NotNull Contact contact) {
        contacts.add(Objects.requireNonNull(contact, "Нал при добавлении " +
                "контакта в базу."));
        logger.info("Добавлен контакт " + contact.toString());
    }

    /**
     * Удаляет контакт из базы. При отсутствии удаляемого контакта в базе,
     * состояние базы не меняется.
     * @param contact Удаляемый контакт.
     * @return True, если элемент был удален, false, если удаляемого элемента
     * не было в базе.
     */
    @Override
    public boolean remove(@NotNull Contact contact) {
        if(contacts.remove(Objects.requireNonNull(contact,
                "Нал при удалении контакта из базы."))) {
            logger.info("Удален контакт " + contact.toString());
            return true;
        }
        return false;
    }
}
