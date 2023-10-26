package backend;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.*;

public class Contact {

    /**
     * Имя контакта. Имя не должно быть пустой строкиой
     * или состоять из пробельных символов.
     */
    @NotNull
    public final String firstName;

    /**
     * Отчество или среднее имя. Равно пустой строке, если отсутствует.
     */
    @NotNull
    public final String middleName;

    /**
     * Фамилия контакта. Равно пустой строке, если отсутствует.
     */
    @NotNull
    public final String lastName;

    /**
     * Дата рождения контакта. Должно быть задано для всех контактов.
     */
    @NotNull
    public final Date birthday;

    /**
     * Адрес контакта. Равно пустой строке, если отсутствует.
     */
    @NotNull
    public final String address;

    /**
     * Массив email'ов контакта.
     */
    @NotNull
    private final ArrayList<String> emails;

    /**
     * Массив телефонных номеров контакта.
     */
    @NotNull
    private final ArrayList<String> phoneNumbers;

    @JsonCreator
    public Contact(@JsonProperty("firstName") @NotNull String firstName,
                   @JsonProperty("middleName") @Nullable String middleName,
                   @JsonProperty("lastName") @Nullable String lastName,
                   @JsonProperty("birthday") @NotNull Date birthday,
                   @JsonProperty("address") @Nullable String address,
                   @JsonProperty("emails") @Nullable ArrayList<String> emails,
                   @JsonProperty("phoneNumbers") @Nullable
                               ArrayList<String> phoneNumbers)
                    throws IllegalArgumentException {
        if(firstName.trim().length() == 0) {
            throw new IllegalArgumentException("Имя не может быть пустой" +
                    " строкой");
        }
        this.firstName = Objects.requireNonNull(firstName, "Ссылка на строку" +
                " имени при создании контакта является налом.");
        this.middleName = Objects.requireNonNullElse(middleName, "");
        this.lastName = Objects.requireNonNullElse(lastName, "");
        this.birthday = Objects.requireNonNull(birthday, "Ссылка на дату" +
                " рождения при создании контакта является налом.");
        this.address = Objects.requireNonNullElse(address, "");
        this.emails = Objects.requireNonNullElse(emails, new ArrayList<>());
        this.phoneNumbers =
                Objects.requireNonNullElse(phoneNumbers, new ArrayList<>());
    }

    /**
     * Геттер для email'ов.
     * @return unmodifiable view коллекции email'ов.
     */
    @NotNull
    public Collection<String> getEmails() {
        return Collections.unmodifiableCollection(emails);
    }

    /**
     * Геттер для номеров телефонов.
     * @return unmodifiable view коллекции телефонных номеров.
     */
    @NotNull
    public Collection<String> getPhoneNumbers() {
        return Collections.unmodifiableCollection(phoneNumbers);
    }

    /**
     * Хешкод контакта, вычисляющийся по фио.
     * @return Хешкод конкатенации строк имени, среднего имени и фамилии.
     */
    @Override
    public int hashCode() {
        return (firstName + middleName + lastName).hashCode();
    }

    /**
     * Сравнивает на равентство два экземпляра контакта. Контакты равны, если
     * они имеют одинаковые имена, фамилии, средние имена и даты рождения.
     * @param other Сравниваемый объект.
     * @return True, если объекты равны, false - иначе.
     */
    @Override
    public boolean equals(Object other) {
        if(!(other instanceof Contact)) {
            return false;
        }
        Contact otherContact = (Contact)other;
        return otherContact.firstName.equals(firstName) &&
                otherContact.middleName.equals(middleName) &&
                otherContact.lastName.equals(lastName) &&
                otherContact.birthday.equals(birthday);
    }

    /**
     * Строковое представление контакта, содержащее всю информацию, необходимую
     * для вывода пользователю.
     * @return Строку со всей информацией о контакте.
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        result.append(firstName).append(" ")
                .append(middleName).append(" ")
                .append(lastName)
                .append(", address: ")
                .append(address)
                .append(", birthday: ")
                .append(birthday)
                .append(", emails: ");
        for(int i = 0; i < emails.size(); ++i) {
            if(i != 0) {
                result.append(", ");
            }
            result.append(emails.get(i));
        }
        result.append(", phone numbers: ");
        for(int i = 0; i < phoneNumbers.size(); ++i) {
            if(i != 0) {
                result.append(", ");
            }
            result.append(phoneNumbers.get(i));
        }
        return result.toString();
    }
}
