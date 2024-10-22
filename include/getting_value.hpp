#pragma once

template <typename T>
void get_smth_from_cin (T* smth)
{
    std::cin >> *smth;
    if (!std::cin.good ())
        throw "Input error!\n";
}

template <typename T>
void get_positive_val_from_cin (T* value)
{
    get_smth_from_cin (value);

    while (*value <= 0)
    {
        std::cout << "You should enter positive number. Try again, please." << std::endl;

        get_smth_from_cin (value);
    }
}
