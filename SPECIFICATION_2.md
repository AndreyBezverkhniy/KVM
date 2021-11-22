# KIRPITCH 2
# Спецификация интерпретатора

____

## ОГЛАВЛЕНИЕ

1 Структура исходных файлов

2 Определение строительныъх блоков программы

3 Назначение строительных блоков программы

____

## 1 Структура исходных файлов

Файл парсится как последовательность лексем. Типы лексем:

- Пустая лексема - не имее символьного представления, искусственная лексема
- Пробельная лексема - пробелы, табуляции, переносы строки
- Строка - последовательность символов, кроме двойной кавычки, заключённая в двойные кавички
- Слово - последовательность латинский букв и цифр
- Однострочный комментарий, заключён между // и переносом строки, либо концом файла
- Многострочный комментарий, заключён между /* и */
- Конец файла - искусственно добавляемая лексемма в конец массива лексем
____

## 2 Структура программы

### 2.1 Основные строительные блоки (Brick) программы:

- модуль (набор инструкций) Module
- блок (набор инструкций) {} Block
- пустой оператор Nop
- выражение Expression
- оператор объявления переменной Var
- условный оператор if
- оператор цикла while
- оператор объявления функции
- оператор подключения модуля import

Программа в загруженном виде хранится как объект Module

### 2.2 Псевдокод

```
Brick{}
```
```
Module{
    array<Brick> block;
}
```
```
Block:Brick{
    array<Brick> bricks;
}
```
```
Nop:Brick{}
```
```
Expression:Brick{
    Operator operator;
}
```
```
Operator{}
```
```
BinaryOperator:Operator{
    Operand operand1;
    Operand operand2;
}
```
```
UnaryOperator:Operator{
    Operand operand;
}
```
```
LeftUnaryOperator:UnaryOperator{}
```
```
RightUnaryOperator:UnaryOperator{}
```
```
Operand{}
```
```
Constant:Operand{
    int n;
}
```
```
Variable:Operand{
    string name;
}
```
```
FunctionCall:Operand{
    string functionName;
    array<Expression> arguments;
}
```
```
Var:Brick{
    array<Declaration> declarationList;
}
```
```
Declaration{
    string name;
    Expression expression;
}
```
```
If:Brick{
    Expression condition;
    Brick success;
    Brick failure;
}
```
```
While:Brick{
    Expression condition;
    Brick body;
}
```
```
FunctionDefinition:Brick{
    string functionName;
    array<Variable> arguments;
    Block body;
}
```
```
Import:Brick{
    Module module;
}
```
____

## 3 Назначение строительных блоков программы
____

