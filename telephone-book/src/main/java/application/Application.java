package application;

import backend.Presenter;
import database.Database;
import frontend.ConsoleUserInterface;

import java.io.IOException;

/**
 * Класс, запускающий программу.
 */
public class Application {
    public static void main(String[] argv) {
        Database database;
        try {
            database = new Database();
        }
        catch(IOException ex) {
            System.out.println("Ошибка при загрузке базы данных" +
                    ex.getMessage());
            return;
        }

        Presenter presenter = new Presenter(database);

        ConsoleUserInterface userInterface =
                new ConsoleUserInterface(presenter);
        userInterface.run();
    }
}
