from numpy import random
import time

# ------------------ Quickselect ------------------ #
def quickselect_partition(arr, l, r):
    x = arr[r]
    i = l
    for j in range(l, r):
        if arr[j] <= x:
            arr[i], arr[j] = arr[j], arr[i]
            i += 1
    arr[i], arr[r] = arr[r], arr[i]
    return i

def kthSmallest(arr, l, r, k):
    if k > 0 and k <= r - l + 1:
        index = quickselect_partition(arr, l, r)
        if index - l == k - 1:
            return arr[index]
        if index - l > k - 1:
            return kthSmallest(arr, l, index - 1, k)
        return kthSmallest(arr, index + 1, r, k - index + l - 1)
    print("Index out of bound")

# ------------------ Quicksort ------------------ #
def swap(arr, i, j):
    arr[i], arr[j] = arr[j], arr[i]

def quicksort_partition(arr, low, high):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        if arr[j] < pivot:
            i += 1
            swap(arr, i, j)
    swap(arr, i + 1, high)
    return i + 1

def quickSort(arr, low, high):
    if low < high:
        pi = quicksort_partition(arr, low, high)
        quickSort(arr, low, pi - 1)
        quickSort(arr, pi + 1, high)

# ------------------ Main comparison loop ------------------ #
if __name__ == "__main__":
    sizes = [5000, 10000, 20000, 50000]
    k = 1000  # fixed k for all tests

    for size in sizes:
        print(f"\n=== Array size: {size} ===")

        # Generate original random array
        original_arr = random.randint(0, 100, size=size)

        # Quickselect
        arr_qs = original_arr.copy()
        start_time = time.time()
        kth_qs = kthSmallest(arr_qs, 0, len(arr_qs) - 1, k)
        end_time = time.time()
        print("Quickselect:")
        print(f"  K-th smallest element: {kth_qs}")
        print(f"  Time taken: {end_time - start_time:.6f} seconds")

        # Quicksort
        arr_sort = original_arr.copy()
        start_time = time.time()
        quickSort(arr_sort, 0, len(arr_sort) - 1)
        kth_sorted = arr_sort[k - 1]
        end_time = time.time()
        print("Quicksort:")
        print(f"  K-th smallest element: {kth_sorted}")
        print(f"  Time taken: {end_time - start_time:.6f} seconds")
