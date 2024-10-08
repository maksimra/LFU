import random

cache_capacity = int(input("Введите размер кэша\n"))
pages_number =   int(input("Введите число страниц\n"))
tests_number =   int(input("Введите число тестов\n"))
for i in range(1, tests_number + 1):
    file = open(f"{i}test.txt", 'w')

    file.write(f"{cache_capacity} {pages_number} ")
    for i in range(pages_number):
        file.write(f"{random.randint(1, 1000)} ")
print("Тесты сгенерированы")
