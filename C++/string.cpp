#include <iostream>
#include <cstring>
#include <cmath>

void swap(size_t* a, size_t* b) {
  size_t* tmp = a;
  *a = *b;
  *b = *tmp;
}

static const char kStringA[] = "My beautiful string.";
// static const char* kStringB = "My fancy string.";

struct String {
  char* data = nullptr;
  size_t length;
};

// Объявление сигнатур во избежании ошибок при вызове до объявления из других функций
String* NewString(size_t length); // Выделение памяти под новую строку размером length
String* NewStringBy(const char* c_string); // Создать новый объект String и поместить значения из существующего
String* CopyString(const String* string); // хз в чём разница с NewStringBy. может использовать указатели?
String* DeleteString(String* string); // Очистить память

int PushSymbol(String* string, char symbol); // Добавить символ в конец
int PushSubstring(String* string, const String* other); // Добавить подстроку в конец
int SetSymbol(String* string, char symbol, size_t position); // Заменить символ в строке по индексу
int SetSubstring(String* string, const String* substring, size_t start); // tricky, be careful. хз нужно или добавить подстроку целиком по идексу или добавить первый символ подстроки по индексу, второй по идексу+1 и т.д.

// int (const String* string); // Вернуть занимаемый размер строки
char GetSymbol(const String* string, size_t position); // Получить символ по индексу
String* GetSubstring(const String* string, size_t start, size_t finish); // Получить подстроку по срезу

size_t GetCStringSize(const char* c_string) {
  size_t string_size = 0;

  for(size_t i = 0; c_string[i] != '\0'; ++i) {
    ++string_size;
  }
  
  return string_size;
}

String* NewString(size_t length) {
  String* string = new String;

  string->data = new char[length];
  string->length = length;

  return string;
};

String* NewStringBy(const char* c_string) {
  String* string = new String;
  size_t string_size = GetCStringSize(c_string);


  string->data = new char[string_size];
  string->length = string_size;

  for(size_t i = 0; i < string_size; ++i) {
    string->data[i] = c_string[i];
  }

  return string;
};

String* CopyString(const String* string) {
  String* new_string = new String;

  new_string->data = new char[string->length];

  for(size_t i = 0; i < string->length; ++i) {
    new_string->data[i] = string->data[i];
  }
  new_string->length = string->length;

  return new_string;
};

String* DeleteString(String* string) {
  if (string) {
    delete[] string->data;
    string->data = nullptr;
    string->length = 0;
  }
  return nullptr;
};

int PushSymbol(String* string, char symbol) {
  String* new_string = new String;

  size_t string_size = string->length;
  
  new_string->data = new char[string_size + 1];
  new_string->length = string_size + 1;

  for(size_t i = 0; i < string_size; ++i) {
    new_string->data[i] = string->data[i];
  }
  new_string->data[string_size] = symbol;

  string->data = new_string->data;
  ++string->length;

  return 0;
};

int PushSubstring(String* string, const String* other) {
  String* new_string = new String;

  size_t string_size = string->length;
  size_t substring_size = other->length;
  
  new_string->data = new char[string_size + substring_size];
  new_string->length = string_size + substring_size;

  for(size_t i = 0; i < string_size; ++i) {
    new_string->data[i] = string->data[i];
  }
  
  for(size_t i = 0; i < substring_size; ++i) {
    new_string->data[string_size + i] = other->data[i];
  }

  string->data = new_string->data;
  string->length = string->length + other->length;

  return 0;
};

int SetSymbol(String* string, char symbol, size_t position) {
  if (position < string->length) {
    string->data[position] = symbol;
    return 0;
  }
  return 1;
};

int SetSubstring(String* string, const String* substring, size_t start) {
  String* new_string = new String;

  size_t substring_size = substring->length;
  
  new_string->data = new char[start + substring_size];
  new_string->length = start + substring_size;

  for(size_t i = 0; i < start; ++i) {
    new_string->data[i] = string->data[i];
  }
  
  for(size_t i = 0; i < substring_size; ++i) {
    new_string->data[start + i] = substring->data[i];
  }

  string->data = new_string->data;

  return 0;
};

size_t GetSize(const String* string) {
  return string->length;
};

char GetSymbol(const String* string, size_t position) {
  return string->data[position];
};

String* GetSubstring(const String* string, size_t start, size_t finish) {
  if (finish < start) {
    swap(&start, &finish);
  }

  size_t delta = finish - start;

  String* result = new String;

  result->data = new char[delta];
  for(size_t i = 0; i < delta; ++i){
    result->data[i] = string->data[start + i];
  }

  result->length = delta;
  
  return result;
};


int TestLifetime() {
    // Create an empty string
    String* string_a = NewString(0);
    if (GetSize(string_a) != 0) {
      return 1;
    }
    
    // Create a string from a C-string
    String* string_b = NewStringBy("Hello");
    if (GetSize(string_b) != 5) {
      return 2;
    }
    
    // Copy the string
    String* string_c = CopyString(string_b);
    if (GetSize(string_c) != 5 || GetSymbol(string_c, 0) != 'H') {
      return 3;
    }
    
    // Delete strings
    string_a = DeleteString(string_a);
    string_b = DeleteString(string_b);
    string_c = DeleteString(string_c);
    
    if (string_a || string_b || string_c) {
      return 4;
    }

    return 0;
}

int TestAPI() {
    // Create and manipulate a string
    String* string = NewStringBy("Test");
    if (!string) {
      return 1;

    }
    if (PushSymbol(string, '!') != 0) {
      return 2;
    }
    if (GetSize(string) != 5 || GetSymbol(string, 4) != '!') {
      return 3;
    }

    String* suffix = NewStringBy(" API");
    if (PushSubstring(string, suffix) != 0) {
      return 4;
    }
    if (GetSize(string) != 9) {
      return 5;
    }
    if (SetSymbol(string, '?', 8) != 0) {
      return 6;
    }
    if (GetSymbol(string, 8) != '?') {
      return 7;
    }

    String* segment = NewStringBy("String");
    if (SetSubstring(string, segment, 5) != 0) {
      return 8;
    }
    if (GetSize(string) != 9) {
      return 9;
    }

    // Get substring
    String* substring = GetSubstring(string, 0, 6);
    if (!substring || GetSize(substring) != 6) {
      return 10;
    }

    // Clean up
    string = DeleteString(string);
    suffix = DeleteString(suffix);
    segment = DeleteString(segment);
    substring = DeleteString(substring);
    
    if (string || suffix || segment || substring) {
      return 11;
    }

    return 0;
}

int main() {
  std::cout << "Hello world!" << std::endl;

  int test_lifetime_status = TestLifetime();
  if ( test_lifetime_status!= 0 ) {
    std::cout << "Crash on lifetime test: " << test_lifetime_status << std::endl;
  }
  else {
    std::cout << "Lifetime test is OK" << std::endl;
  }

  int test_API_status = TestAPI();
  if (test_API_status != 0) {
    std::cout << "Crash on API test: " << test_API_status << std::endl;
  }
  else {
    std::cout << "API test is OK" << std::endl;
  }

  return 0;
}