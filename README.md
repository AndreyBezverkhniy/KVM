# KIRPITCH 2

Учебный проект по реализации интерпретатора собственного языка, работающего
только с переменными целочисленного типа.

Примеры исходников имеют расширение .ksc

## РАЗБОР ИСХОДНЫХ ФАЙЛОВ

Исходные файлы из набора символов переводятся в наборы лексем, наборы лексем переводятся в древовидные структуры инструкций, и уже последниие имеют удобное представление программы для интерпретирования.

## LEXEME. ТИПЫ ЛЕКСЕМ

- Пустая лексема - единственная, состоящая из 0 символов.
- Пробельная - строка, включающая только симолы: пробел, табуляцию, конец строки.
- Знак - состоит из одного или двух символов. Список возможных знаков:
    1. ++ -- && ||
    2. += -= *= /= %= != >= <=
    3. \> < ! + - * / % = , ( ) { }
- Слово - состоит из букв и цифр
- Строка - начинается и заканчивается двойной кавычкой, между которыми двойная ковычка не встречается.
- Однострочный комментарий - начинается с двух слешей / и продолжается до конца строки.
- Многострочный комментарий - начинается комбинацией двух символов /* и заканчивается первой же (вложенность не принимается) комбинацией двух символов \*/.

## LEXEME. КОНЕЧНЫЙ АВТОМАТ

Автомат лексем имеет состояния, характеризующиеся одним из маркеров:

- complited - лексема завершена и не может быть продолжена никаким символом, т.е. следующий символ точно начинает следующую лексему
- extendable - лексема может считаться завершённой, но также может быть продолжена некоторым символом
- not complited - на данном этапе лексема обязана быть продолжена некоторым символом и не может пока считаться завершённой. Завершение на данном состоянии лексемы означает ошибку

Новый символ продолжает лексему из текущего состояния автомата. Если из текущего состояния нет перехода по принятому символу, то текущая лексема обрубается, а этот символ начинает новую лексему. Если лексему обрубили в состоянии, отличном от complited или extendable, то обнаружена синтаксическая ошибка в тексте. Если из стартового состояния автомата нет перехода по символу, то обнаружена синтаксическая ошибка в тексте.

![alt text](img/lexeme/automat.png)

## ЯЗЫКОВЫЕ КОНСТРУКЦИИ

![alt text](img/constructions/program_module.png)

![alt text](img/constructions/instruction.png)

![alt text](img/constructions/var_declaration.png)

![alt text](img/constructions/block.png)

![alt text](img/constructions/if.png)

![alt text](img/constructions/while.png)

![alt text](img/constructions/return.png)

![alt text](img/constructions/import.png)

![alt text](img/constructions/function_declaration.png)

![alt text](img/constructions/expression.png)

![alt text](img/constructions/operand.png)

![alt text](img/constructions/element.png)

![alt text](img/constructions/variable_name.png)

![alt text](img/constructions/function_call.png)

![alt text](img/constructions/function_name.png)

![alt text](img/constructions/number.png)

![alt text](img/constructions/identifier.png)

bin - бинарный оператор: + - * / % = < > == != <= >= += -= *= /= %= && ||

left unary - левый унарный оператор: ! ++ -- + -

right unary - правый унарный оператор: ++ -- + -

## ОПИСАНИЕ ЯЗЫКОВЫХ КОНСТРУКЦИЙ

### Модуль программы

Программа состоит из модулей - исходных файлов (см. МОДУЛЬНАЯ РЕАЛИЗАЦИЯ). Модуль содержит последовательность инструкций (возможно, пустую). Выполнение программы осуществляется с первой инструкции основного модуля программы (в котором подключаются другие модули).

### Инструкция

Инструкция представляет собой одну из следующих конструкций:

- Объявление переменной
- Выражение
- Блок кода (составной оператор)
- Условный оператор
- Оператор цикла while
- Объявление функции
- Оператор return
- Директива import подключения модуля

Инструкции import и объявления фунции обрабатываются в первую очередь. Перед началом выполнения интерпретатор загружает в память все модули программы и сканирует код в поисках объявлений функций.

### Объявление переменных

Инструкцией var объявляются имена новых переменных, которым можно присваивать начальное значение. Начальное значение по умолчанию - 0. Имена переменных не могут повторяться внутри одной инструкции объявления переменных, а также в разных инструкциях объявления переменных расположенных на одном уровне вложенности блоков {}. Повторное объявление в блоке с большим уровнем вложенности перекрывает предыдущее объявление переменной с таким именем внутри блока, на уровне которого произошло повторное объявление. После выъода из такого блока, копия уничтожается, и с этим именем снова связана старая переменная. Переменные становятся доступны толькопосле их объявления (в отличии от оператора var в javascript).

### Блок

Представляет собой последовательность инструкций, объединяет их в одну составную инструкцию.

### Условный оператор
Сначала вычисляет выражение. Если оно не равно, то оно считается истиным, если равно 0, то ложным. Если условие истино выполняется инструкция после выражения в скобках. При наличии продолжения "else" инструкции в случае ложности условия выполняется инструкция после else.

### Оператор цикла while

Аналогично условному оператору сначала вычисляется выражение-условие цикла. Тело цикла - инструкция, следующая после условия в скобках - выполняется после успешной проверки выражения на истиность. Если условие ложно, выполнение программы переходит к следующей после этого опратора цикла инструкции.

### Объявление функции

Так как подключение модулей и объявление функций объявляются в первую очередь, функции в коде могут использоваться до их объявления.

Объявляемые функции должны иметь разные сигнатуры. В сигнатуру входит только имя функции и количество принимаемых аргументов. По этим параметрам функции совпадать не могут, перегрузка не поддерживается.

Любая функция возвращает значение. По умолчанию это 0. Для возврата иного значения используется инструкция return.

### Оператор return

Оператор завершает выполнение функции. При использовании оператора с выражением функция возвращает значение этого выражения, вычисляемого непосредственно перед выходом из функции. При использовании return без выражения, используется возвращаемое значение по умолчанию - 0.

### Директива import подключения модуля

Сигнализирует интерпретатору о необходимости загрузить модуль к программе. Директива import не заменяется текстом подключаемого модуля. Если модуль уже был загружен, директива игнорируется.

## ТИП ДАННЫХ ПЕРЕМЕННЫХ

В языке используется единственный тип данных - целочисленный со знаком int32.

0 считается ложью, не 0 - истиной.

## ОПЕРАТОРЫ

Левый/правый инкремент/декремент, +, -, * работают как в других языках.

Отрицание ! переводит не 0 - в 0, а 0 в 1.

Результаты логических операций И (&&), ИЛИ (||) и операторов сравнения - 0 или 1.

Деление / целочисленное, % - деление по модулю.

Конструкция variable Х= expression, где Х - это +, -, *, / или %, работает как variable = variable X (expression).

Оператор = и его модификации Х= возвращают значение, присвоенное переменной, которой обязан быть левый аргумент оператора.

## ПОРЯДОК ВЫПОЛНЕНИЯ ОПЕРАТОРОВ

Если операндом оператора является выражение в скобках, то перед вычислением этого оператора (будь он унарным или бинарным) сначала вычисляется выражение в скобках.

Порядок выполнения операторов:

1. Левые унарные (в обратном порядке: справа налево)
2. Правые унарные (в прямом порядке: слева на права)
3. Бинарные

Бинарные операторы выполняются в последнюю очередь в порядке уменьшения приоритета операций: если соседние операторы имеют разный приоритет, то оператор с большим приоритетом выполняется раньше. Если соседние операторы имеют одинаковый приоритет, то эти операторы выполняются в порядке, определённом их правой или левой ассоциативностью.

Правоассоциативные операторы: = и его модификации Х=, остальные левоассоциативные.

Приоритеты операторов:

1. = и его модификации Х=
2. ||
3. &&
4. == != < > <= >=
5. \+ -
6. \* / %

## ВНУТРЕННЯЯ СТРУКТУРА ИСПОЛНЯЕМОЙ ПРОГРАММЫ

Каждый модуль программы загружается как одельный объект, который сохраняется в ассоциативный массив с ключом, который является строкой - путём к модулю (в том виде, в котором он указан в директиве import, либо в котором послан аргументом командной строки для основного модуля).

![alt text](img/classes/class.modules.png)

Код модуля не обязан выделяться фигурными скобками в блок, он автоматически целиком рассматривается как блок самого верхнего уровня - корневой блок.

![alt text](img/classes/class.module.png)

Классы инструкций наследуются от одного интерфейса Instruction.

![alt text](img/classes/class.block.png)

При объявлении начальное значение либо задаётся присваиваемым выражением, либо определяется равным 0 по умолчанию.

![alt text](img/classes/class.variable_declaration.png)

В условном операторе часть else опциональна.

![alt text](img/classes/class.if.png)

![alt text](img/classes/class.while.png)

Возвращаемое значение определяется выражением, либо приравнивается 0 по умолчанию.

![alt text](img/classes/class.return.png)

Выражение ссылается либо на операнд, либо на бинарный оператор.

![alt text](img/classes/class.expression.png)

На этом классы инструкций закончиваются. Далее идут вспомогательные классы для выражений.

Бинарный оператор ссылается на два своих операнда и определяет бинарную операцию, применяемую к ним.

![alt text](img/classes/class.bin.png)

Операнд является либо элементом, либо сначала ссылается на цепочку унарных операторов.

![alt text](img/classes/class.operand.png)

![alt text](img/classes/class.left-right_unary.png)

Элемент - это "элементарный операнд", который может быть одним из 4-х типов.

![alt text](img/classes/class.element.png)

![alt text](img/classes/class.number.png)

![alt text](img/classes/class.variable_name.png)

![alt text](img/classes/class.function_call.png)

Перегрузка функций запрещена, проверка на несовпадение сигнатур производится по именам функций и количеству принимаемых ими аргументов. При совпадении обоих этих параметров обнаруживается перегрузка, запрещённая языком.

![alt text](img/classes/class.function_signature.png)

Объявления функций не входят в объект модуля, а сохраняются отдельно в ассоциативном массиве функций. В нём хранятся функции всех модулей.

![alt text](img/classes/class.functions.png)

Каждый уровень вложенности блока связан с контекстом, в котором хранится информация об объявленных на этом уровне вложенности блока переменных. Повторное объявление переменной запрещено, если в контексте этого блока переменная с таким именем уже объявлена. Однако не запрещено повторять имена переменных, объявленных в блоках уровнями выше текущего. Доступ к переменным по имени в контекстах производится по порядку повышения уровня вложенности, испольуется первое встреченное не пути объявление переменной. Для этого в контексте храится ссылка на контекст родительского блока. У корневого контекста ссылка никуда не ссылается.

![alt text](img/classes/class.context.png)

По такой древовидной структуре модуля происходит рекурсивное выполнение блоков инструкций и вычисление выражений. Перед выполнением программа приводится к этому виду.

## РАЗБОР ПОТОКА ЛЕКСЕМ

Каждая составная часть программы описана выше конечным автоматом. Каждая законченная последовательность лексем выделяется из текста программы и используются для построения объекта, который добавляется в структуру, основанную на списках и деревьях и пригодную для интерпретации программы.
При этом используется следующий принцип разбора законченной последовательности и обнаружения грамматических ошибок. Сохраняем разобранные лексемы и объекты, полученные в результате разбора частей конструкции, в законченную последовательность, которую преобразуем в объект соответствющего типа. Если при этом ни один из вариантов очередной ожидаемой части конструкции не имеет места быть, это означает, что вся предполагаемая текущая конструкция не имеет места быть. Т.е. предпринята попытка обнаружить конструкцию отличную от фактической. При иссякании вариантов конструкций к рассмотрению считается обнаруженной синтаксическая ошибка.

## РАЗБОР ВЫРАЖЕНИЙ

### Определение порядка выполнения бинарных операторов

Определим строгий порядок выполнения в последовательности бинарных операторов в выражении. Он однозначно диктуется приоритетами и (правой/левой) ассоциативностью операторов. Пусть после bin1 следует bin2. Будем считать, что если bin1 вычисляется раньше bin2, то bin1<bin2, а если bin2 вычисляется раньше bin1, то bin1\>bin2.

Здесь priority(bin) возвращает номер приоритета оператора: 1..6, isLeftAssociative(bin) возвращает истину, если оператор левоассоциативный.

![alt text](img/expression/expression.bin_order.png)

### Алгоритм

После разбора синтаксической конструкции выражение у нас есть n>=1 операнд (ops) и n-1 бинарных операторов (bins).

Всё выражение разделяется на левую часть (left), операнд (operand) и правую часть (right). Изначально левая часть пустая, операнд - первый же операнд в выражении, а правая часть - всё остальное.

```
left = new stack();
right = new stack();
operand = ops[0];
for (i = 1; i < n; i ++) {
    right.push(bins[i],ops[i]);
}
```

В процессе выполняется инфариант: в left все бинарные операторы выполняются справа налево.

Далее пытаемся перетянуть очередную пару <оператор, операнд> в левую часть <left, operand>, при этом при необходимости для выполнения инфарианта сокращая left.

```
loop {
    while(not left.isEmpty() and (right.isEmpty() or left.top().bin < right.top().bin)) {
        opL = left.top().op;
        bin = left.top().bin;
        left.pop();
        opR = operand;
        operand = calculate(opL, bin, opR);
    }
    if (right.isEmpty()) break;
    op = right.top().op;
    bin = right.top().bin;
    right.pop();
    left.push(operand, bin);
    operand = op;
}
expressionObj = operand;
```