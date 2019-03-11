#!/usr/bin/python



# pyroot can't understand C++ arrays with more than one dimension and just gives a 1D list of length equal to the
# first dimension of the C++ array. So the size of the array must be manually set
# e.g. for a 2D array, read as:
#     python list[0] = C++ array[0][0]
#     python list[1] = C++ array[0][1]
#     python list[2] = C++ array[1][0]
#     python list[3] = C++ array[1][1]
# etc.
# This method supports both 2D and 3D arrays
def c_to_pyroot_array_hack(c_array, c_array_first_dim, c_array_second_dim, c_array_third_dim = 0):
    list = []
    if c_array_third_dim == 0:
        c_array.SetSize(c_array_first_dim * c_array_second_dim)
    else:
        c_array.SetSize(c_array_first_dim * c_array_second_dim * c_array_third_dim)
    for i in range(0, c_array_first_dim):
        list_j = []
        for j in range(0, c_array_second_dim):
            if c_array_third_dim == 0:
                index = j + c_array_second_dim * i
                list_j.append(c_array[index])
            else:
                list_k = []
                for k in range(0, c_array_third_dim):
                    index = k + c_array_third_dim * (j + c_array_second_dim * i)
                    list_k.append(c_array[index])
                list_j.append(list_k)
        list.append(list_j)
    return list