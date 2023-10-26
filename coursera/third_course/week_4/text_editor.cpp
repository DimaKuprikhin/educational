#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor
{
public:
    friend std::ostream& operator<<(std::ostream& stream, Editor& editor);

    Editor() {}
    void Left() {
        // AdvanceCursor(cursor, -1);
        if(cursor != text.begin()) {
            --cursor;
        }
    }
    void Right() {
        // AdvanceCursor(cursor, 1);
        if(cursor != text.end()) {
            ++cursor;
        }
    }
    void Insert(char token) {
        text.insert(cursor, token);
        ++size;
    }
    void Cut(size_t tokens = 1) {
        buffer.clear();
        auto cutEnd = cursor;
        size -= AdvanceCursor(cutEnd, tokens);
        text.splice(buffer.begin(), text, cursor, cutEnd);
        cursor = cutEnd;
    }
    void Copy(size_t tokens = 1) {
        buffer.clear();
        for(auto it = cursor; tokens-- > 0 && it != text.end(); ++it) {
            buffer.push_back(*it);
        }
    }
    void Paste() {
        for(auto symbol : buffer) {
            text.insert(cursor, symbol);
            ++size;
        }
    }
    string GetText() const {
        std::string result;
        result.reserve(size);
        for(auto symbol : text) {
            result.push_back(symbol);
        }
        return result;
    }

private:
    int AdvanceCursor(std::list<char>::iterator& iter, int positions) {
        int restPositions = positions;
        while(restPositions > 0 && iter != text.end()) {
            ++iter;
            --restPositions;
        }
        while(restPositions++ < 0 && iter != text.begin()) {
            --iter;
            ++restPositions;
        }
        return positions - restPositions;
    }

    std::list<char> text;
    std::list<char> buffer;
    std::list<char>::iterator cursor = text.begin();
    size_t size = 0;
};

std::ostream& operator<<(std::ostream& stream, Editor& editor) {
    for(auto it = editor.text.begin(); it != editor.text.end(); ++it) {
        if(it == editor.cursor) {
            stream << '|';
        }
        stream << *it;
    }
    if(editor.cursor == editor.text.end()) {
        stream << "|";
    }
    stream << " " << editor.size << " ";
    int bufferSize = 0;
    for(auto symbol : editor.buffer) {
        stream << symbol;
        ++bufferSize;
    }
    return stream << " " << bufferSize;
}

void TypeText(Editor &editor, const string &text)
{
    for (char c : text)
    {
        editor.Insert(c);
    }
}

void TestEditing()
{
    {
        Editor editor;

        const size_t text_len = 12;
        const size_t first_part_len = 7;
        TypeText(editor, "hello, world");
        for (size_t i = 0; i < text_len + 3; ++i)
        {
            editor.Left();
        }
        editor.Cut(first_part_len);
        for (size_t i = 0; i < text_len - first_part_len + 3; ++i)
        {
            editor.Right();
        }
        TypeText(editor, ", ");
        editor.Paste();
        editor.Left();
        editor.Left();
        editor.Cut(3);

        ASSERT_EQUAL(editor.GetText(), "world, hello");
    }
    {
        Editor editor;

        TypeText(editor, "misprnit");
        editor.Left();
        editor.Left();
        editor.Left();
        editor.Cut(1);
        editor.Right();
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "misprint");
    }
    {
        Editor editor;
        TypeText(editor, "abcdef");
        editor.Left();
        editor.Left();
        editor.Cut(2);
        editor.Paste();
        editor.Paste();
        editor.Left();
        editor.Insert('A');
        ASSERT_EQUAL(editor.GetText(), "abcdefeAf");
    }
    {
        Editor editor;
        TypeText(editor, "abcdef");
        for(int i = 0; i < 6; ++i)
            editor.Left();
        editor.Copy(3);
        editor.Paste();
        editor.Paste();
        editor.Insert('A');
        ASSERT_EQUAL(editor.GetText(), "abcabcAabcdef");
    }
}

void TestReverse()
{
    Editor editor;

    const string text = "esreveR";
    for (char c : text)
    {
        editor.Insert(c);
        editor.Left();
    }

    ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText()
{
    {
        Editor editor;
        ASSERT_EQUAL(editor.GetText(), "");

        editor.Left();
        editor.Left();
        editor.Right();
        editor.Right();
        editor.Copy(3);
        editor.Cut(3);
        editor.Paste();

        ASSERT_EQUAL(editor.GetText(), "");
    }
    {
        Editor editor;
        editor.Insert('a');
        ASSERT_EQUAL(editor.GetText(), "a");
    }
    {
        Editor editor;
        editor.Left();
        editor.Insert('a');
        ASSERT_EQUAL(editor.GetText(), "a");
    }
    {
        Editor editor;
        editor.Right();
        editor.Right();
        editor.Insert('a');
        ASSERT_EQUAL(editor.GetText(), "a");
    }
    {
        Editor editor;
        editor.Right();
        editor.Left();
        editor.Right();
        editor.Left();
        editor.Left();
        editor.Insert('a');
        editor.Left();
        editor.Insert('b');
        ASSERT_EQUAL(editor.GetText(), "ba");
    }
}

void TestEmptyBuffer()
{
    Editor editor;

    editor.Paste();
    TypeText(editor, "example");
    editor.Left();
    editor.Left();
    editor.Paste();
    editor.Right();
    editor.Paste();
    editor.Copy(0);
    editor.Paste();
    editor.Left();
    editor.Cut(0);
    editor.Paste();

    ASSERT_EQUAL(editor.GetText(), "example");
}

void Test()
{
	Editor editor;
	TypeText(editor, "1234567");
	editor.Left();
	editor.Left();
	editor.Cut(1);
	editor.Paste();
	editor.Paste();
	editor.Paste();
	ASSERT_EQUAL(editor.GetText(), "123456667");
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestEditing);
    RUN_TEST(tr, TestReverse);
    RUN_TEST(tr, TestNoText);
    RUN_TEST(tr, TestEmptyBuffer);
    RUN_TEST(tr, Test);
    return 0;
}