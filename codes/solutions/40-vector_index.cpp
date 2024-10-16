#include <iostream>
#include <vector>

int main()
{
    std::vector<int> V{6, 111, 116, 10, 187, 17};

    std::cout << "Enter an index: ";
    int idx{};
    if (std::cin >> idx)
    {
        try
        {
            int element = V.at(idx);
            std::cout << "The element at index " << idx << " is " << element << std::endl;
        }
        catch (const std::out_of_range &a_super_long_and_tedious_name)
        {
            std::cout << "something bad happened " << std::endl;
            std::cout << a_super_long_and_tedious_name.what() << std::endl;
        }
    }
    else
    {
        std::cout << "You didn't enter an index" << std::endl;
    }

    try
    {
        std::cout << "Done!" << std::endl;
    }
    catch (const std::out_of_range &e)
    {
        std::cout << "something bad happened" << std::endl;
    }

    return 0;
}
