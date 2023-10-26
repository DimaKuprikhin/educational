package database;

import backend.Contact;
import backend.Date;
import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;

import static org.junit.jupiter.api.Assertions.*;

class DatabaseTest {

    @Test
    void save() {
        Database base;
        try {
            base = new Database();
        }
        catch(IOException ex) {
            fail();
            return;
        }

        ArrayList<String> emails = new ArrayList<>();
        emails.add("mail");
        ArrayList<String> phones = new ArrayList<>();
        phones.add("123456ddf");
        Contact contact = new Contact("a", "b", "c", new Date(1, 1, 3000),
                "adr", emails, phones);
        base.add(contact);

        try {
            base.save();
        }
        catch(IOException ex) {
            fail();
            return;
        }

        try {
            base = new Database();
        }
        catch(IOException ex) {
            fail();
            return;
        }

        Collection<Contact> collection = base.getByPredicate(contact1 -> true);
        Contact lastContact = null;
        for(Contact con : collection) {
            lastContact = con;
        }
        assertEquals(contact, lastContact);
    }
}