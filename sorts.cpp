#include <iostream>
#include <fstream>
#include <random>
#include <climits>
#include <chrono>

#define SIZEMAX 1000

using std::cout;
using std::cin;
using std::endl;

void RandomizeArray(unsigned att[], unsigned n)
{
     // 1. Заполняем массив числами от 1 до n
    for (unsigned i = 0; i < n; i++)
    {
        att[i] = i + 1;
    }

    // 2. Перемешиваем алгоритмом Фишера-Йетса
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);

    for (unsigned i = n - 1; i > 0; i--)
    {
        // Генерируем случайный индекс от 0 до i
        std::uniform_int_distribution<unsigned> d(0, i);
        unsigned j = d(e);

        // Меняем местами текущий элемент со случайным
        std::swap(att[i], att[j]);
    }
}

void SortBubble(unsigned arr[], unsigned n)
{
    std::ofstream file("sorts/bubble.csv", std::ios::out);
    /*file << "bubble" << endl;*/
    for (unsigned i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                unsigned t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
            for (unsigned int i = 0; i < n - 1; i++)
            {
                file << arr[i] << ", ";
            }

            file << arr[n - 1] << endl;
        }
    }
}

void SortSimpleChoose(unsigned arr[], unsigned n)
{
    std::ofstream file("sorts/choose.csv", std::ios::out);

    for (unsigned j = 0; j < n - 1; j++) // Внешний цикл до n-1
    {
        unsigned maxn = arr[0]; // Сбрасываем максимум для каждой итерации
        unsigned ind = 0;

        // Ищем максимальный элемент в неотсортированной части
        for (unsigned i = 1; i < n - j; i++)
        {
            if (arr[i] > maxn)
            {
                maxn = arr[i];
                ind = i;
            }
        }

        // Меняем местами найденный максимум с последним элементом неотсортированной части
        unsigned t = arr[n - 1 - j];
        arr[n - 1 - j] = maxn;
        arr[ind] = t;

        // Записываем состояние массива ПОСЛЕ каждой перестановки
        for (unsigned int i = 0; i < n - 1; i++)
        {
            file << arr[i] << ", ";
        }
        file << arr[n - 1] << std::endl;
    }
}

void SortInsert(unsigned arr[], unsigned n)
{
    std::ofstream file("sorts/insert.csv", std::ios::out);

    for (unsigned i = 1; i < n; i++)
    {
        unsigned temp = arr[i];
        unsigned j = i;
        while (j > 0 && arr[j - 1] > temp)
        {
            arr[j] = arr[j - 1];
            j--;

            for (unsigned int i = 0; i < n - 1; i++)
            {
                file << arr[i] << ", ";
            }

            file << arr[n - 1] << endl;
        }
        arr[j] = temp;
    }
}

void d(unsigned arr[], unsigned current_n, unsigned i, std::ofstream &file, unsigned original_n)
{
    // Всегда записываем состояние ПОЛНОГО массива (original_n элементов)
    for (unsigned int k = 0; k < original_n - 1; k++)
    {
        file << arr[k] << ", ";
    }
    file << arr[original_n - 1] << std::endl;

    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < current_n && arr[left] > arr[largest])
    {
        largest = left;
    }

    if (right < current_n && arr[right] > arr[largest])
    {
        largest = right;
    }

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        d(arr, current_n, largest, file, original_n);
    }
}

void HeapSort(unsigned arr[], unsigned n)
{
    std::ofstream file("sorts/heap.csv", std::ios::out);

    // Записываем начальное состояние
    for (unsigned int k = 0; k < n - 1; k++)
    {
        file << arr[k] << ", ";
    }
    file << arr[n - 1] << std::endl;

    // Построение кучи - работаем со всем массивом
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        d(arr, n, i, file, n); // Передаем original_n = n
    }

    // Сортировка
    for (int i = n - 1; i > 0; i--)
    {
        std::swap(arr[0], arr[i]);

        // Записываем после обмена корня с последним элементом
        for (unsigned int k = 0; k < n - 1; k++)
        {
            file << arr[k] << ", ";
        }
        file << arr[n - 1] << std::endl;

        // Восстанавливаем кучу для уменьшенной части, но записываем полный массив
        d(arr, i, 0, file, n); // current_n = i, но original_n = n
    }

    file.close();
}

void rasch_sort(unsigned arr[], unsigned n)
{
    std::ofstream file("sorts/rasch.csv", std::ios::out);

    int gap = n;

    double shrink = 1.3;

    bool swapped = true;

    while (gap > 1 || swapped)
    {
        gap = (int)(gap / shrink);
        if (gap < 1)
        {
            gap = 1;
        }

        swapped = false;

        for (int i = 0; i + gap < n; i++)
        {
            if (arr[i] > arr[i + gap])
            {
                std::swap(arr[i], arr[i + gap]);
                swapped = true;
            }
            for (unsigned int i = 0; i < n - 1; i++)
            {
                file << arr[i] << ", ";
            }

            file << arr[n - 1] << endl;
        }
    }
}

void merge(unsigned arr[], int left, int mid, int right, std::ofstream &file, int size)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    unsigned *leftArr = new unsigned[n1];
    unsigned *rightArr = new unsigned[n2];

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            i++;
        }
        else
        {
            arr[k] = rightArr[j];
            j++;
        }
        k++;

        // Записываем состояние после каждого шага
        for (int idx = 0; idx < size - 1; idx++)
        {
            file << arr[idx] << ", ";
        }
        file << arr[size - 1] << std::endl;
    }

    while (i < n1)
    {
        arr[k] = leftArr[i];
        i++;
        k++;

        for (int idx = 0; idx < size - 1; idx++)
        {
            file << arr[idx] << ", ";
        }
        file << arr[size - 1] << std::endl;
    }

    while (j < n2)
    {
        arr[k] = rightArr[j];
        j++;
        k++;

        for (int idx = 0; idx < size - 1; idx++)
        {
            file << arr[idx] << ", ";
        }
        file << arr[size - 1] << std::endl;
    }

    delete[] leftArr;
    delete[] rightArr;
}

void mergesort(unsigned arr[], int left, int right, std::ofstream &file, int size)
{
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergesort(arr, left, mid, file, size);
    mergesort(arr, mid + 1, right, file, size);
    merge(arr, left, mid, right, file, size);
}

void mergeSort(unsigned arr[], int size)
{
    std::ofstream file("sorts/merge.csv", std::ios::out);

    for (int i = 0; i < size - 1; i++)
    {
        file << arr[i] << ", ";
    }
    file << arr[size - 1] << std::endl;

    if (size > 1)
    {
        mergesort(arr, 0, size - 1, file, size);
    }

    file.close();
}

int main(int argc, char* argv[])
{
    unsigned n = 30;
    unsigned m[SIZEMAX];

    if (argc > 1) {
        n = std::stoi(argv[1]);
    } else {
        // Иначе спрашиваем вручную
        cout << "Введите размер массива: ";
        cin >> n;
    }

    if (n < 1)
        {
            cout << "Число слишком маленькое, использовано значение по умолчанию: " << n << endl;
            n = 20;
        }
    else if (n > SIZEMAX)
        {
            cout << "Число слишком большое (макс. =" << SIZEMAX << "), использовано максимальное значение." << endl;
            n = SIZEMAX;
        }

    RandomizeArray(m, n);
    SortBubble(m, n);

    RandomizeArray(m, n);
    SortSimpleChoose(m, n);

    RandomizeArray(m, n);
    SortInsert(m, n);

    RandomizeArray(m, n);
    HeapSort(m, n);

    RandomizeArray(m, n);
    rasch_sort(m, n);

    RandomizeArray(m, n);
    mergeSort(m, n);

    return 0;
}
