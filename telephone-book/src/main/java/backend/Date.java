package backend;

import com.fasterxml.jackson.annotation.JsonCreator;
import com.fasterxml.jackson.annotation.JsonProperty;

/**
 * Класс для хранения даты, состоящей из числа, месяца и года.
 */
public class Date  {
    /**
     * Число месяца. Может находится в отрезке [1, 31]. Не учитывает разное
     * количество дней в месяцах.
     */
    public final int day;

    /**
     * Номер месяца. Может находится в отрезке [1, 12].
     */
    public final int month;

    /**
     * Год. Может равняться любому значению int.
     */
    public final int year;

    @JsonCreator
    public Date(@JsonProperty("day") int day,
                @JsonProperty("month") int month,
                @JsonProperty("year") int year) {
        if(day < 1 || day > 31) {
            throw new IllegalArgumentException("Неккоректное значение дня " +
                    "месяца. Должно быть в отрезке [1, 31].");
        }
        if(month < 1 || month > 12) {
            throw new IllegalArgumentException("Неккоректное значение номера " +
                    "месяца. Должно быть в отрезке [1, 12].");
        }
        this.day = day;
        this.month = month;
        this.year = year;
    }

    @Override
    public int hashCode() {
        return day + 31 * month + 365 * year;
    }

    @Override
    public boolean equals(Object other) {
        if(!(other instanceof Date)) {
            return false;
        }
        Date otherDate = (Date)other;
        return otherDate.day == day && otherDate.month == month &&
                otherDate.year == year;
    }

    /**
     * Возвращает строковое представление даты.
     * @return Строковое представление даты в формате DD.MM.Y.
     */
    @Override
    public String toString() {
        return (day < 10 ? "0" : "") + day + "." + (month < 10 ? "0" : "") +
                month + "." + year;
    }
}
