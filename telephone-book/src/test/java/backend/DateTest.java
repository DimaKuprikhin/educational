package backend;

import static org.junit.jupiter.api.Assertions.*;

class DateTest {

    @org.junit.jupiter.api.Test
    void testToString() {
        assertEquals("01.01.1900", (new Date(1, 1, 1900)).toString());
        assertEquals("31.12.30000", (new Date(31, 12, 30000)).toString());
        assertEquals("01.01.1", (new Date(1, 1, 1)).toString());
        assertEquals("08.09.-753", (new Date(8, 9, -753)).toString());
    }
}