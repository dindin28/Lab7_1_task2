#include <vector>
#include <random>
#include <limits>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <windows.h>

const int array_size = 20;

DWORD MaximumNotEvenElement(LPVOID lpParameter);
DWORD AverageWithNine(LPVOID lpParameter);

int main()
{
    // Making random mechanism
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, 100);

    // Generating array
    std::vector<int> vec(array_size);
    std::generate(vec.begin(), vec.end(), [&]() { return distrib(gen); });
    std::cout << "Array: ";
    for(const auto& iter : vec)
    { std::cout << iter << ' '; }
    std::cout << std::endl << std::endl;

    HANDLE maximum_not_even_element
        = CreateThread(NULL, 0, MaximumNotEvenElement, &vec, 0, NULL);
    HANDLE average_with_nine
        = CreateThread(NULL, 0, AverageWithNine, &vec, 0, NULL);

    HANDLE handle_array[2] { maximum_not_even_element, average_with_nine };
    WaitForMultipleObjects(2, handle_array, true, INFINITE);

    CloseHandle(maximum_not_even_element);
    CloseHandle(average_with_nine);

    return 0;
}

DWORD MaximumNotEvenElement(LPVOID lpParameter)
{
    const std::vector<int>& vec = *((std::vector<int>*)lpParameter);

    std::vector<int> even_vector;
    std::for_each(vec.begin(), vec.end(), [&](int a) {
        if (a % 2 != 0)
        {
            even_vector.push_back(a);
        }
    });

    std::stringstream output_msg;
    output_msg << "Even vector: ";
    for(const auto& iter : even_vector)
    {
        output_msg << iter << " ";
    }
    output_msg << std::endl
        << "Maximum not even element: "
        << *(std::max_element(even_vector.begin(), even_vector.end()))
        << std::endl << std::endl;

    std::cout << output_msg.rdbuf();

    return 0;
}

DWORD AverageWithNine(LPVOID lpParameter)
{
    const std::vector<int>& vec = *((std::vector<int>*)lpParameter);

    int sum = 0;
    int count = 0;

    std::stringstream output_msg;

    output_msg << "Multiple of 9: ";
    std::for_each(vec.begin(), vec.end(), [&](const int& a){
        if (a % 9 == 0)
        {
            output_msg << a << " ";
            sum += a;
            ++count;
        }
    });

    output_msg << std::endl
        << "Average with nine: "
        << (static_cast<double>(sum) / count)
        << std::endl << std::endl;

    std::cout << output_msg.rdbuf();

    return 0;
}