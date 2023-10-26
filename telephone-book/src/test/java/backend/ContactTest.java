package backend;

import org.junit.jupiter.api.Test;

import java.lang.reflect.Array;
import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*;

class ContactTest {

    @Test
    void testToString() {
        ArrayList<String> emails = new ArrayList<>();
        emails.add("dk@gmail.com");
        emails.add("mail");
        ArrayList<String> phones = new ArrayList<>();
        phones.add("123");
        phones.add("987");
        phones.add("8-666");
        assertEquals("Dima Alexeevich Kuprikhin, " +
                        "address: Pushkin st., Kalatushkin's Building, " +
                        "birthday: 01.01.2000, " +
                        "emails: dk@gmail.com, mail, " +
                        "phone numbers: 123, 987, 8-666",
                (new Contact("Dima", "Alexeevich", "Kuprikhin",
                        new Date(1,1,2000),
                        "Pushkin st., Kalatushkin's Building",
                        emails, phones)).toString());

        assertEquals("a  , address: , birthday: 02.03.2001, emails: , " +
                "phone numbers: ",
                (new Contact("a", null, null, new Date(2, 3, 2001), null, null,
                        null)).toString());

    }
}