/*
*   This file is a part of the OpenPhase software project.
*   For more details visit www.openphase.de
*
*   Created:    2011
*
*   Authors:    Oleg Shchyglo; Efim Borukhovich; Dmitry Medvedev
*
*   Copyright (c) 2009-2014 Interdisciplinary Centre for Advanced Materials
*                 Simulation (ICAMS). Ruhr-Universitaet Bochum. Germany
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef TENSOR_H
#define TENSOR_H

namespace openphase
{

template <class T, int Rank>
class Tensor                                                                    /// Tensor template class. Can handle any type of values but POD is preferred
{
 public:
 protected:
 private:
};

template <class T>
class Tensor<T, 1>                                                              /// Tensor class specialization of rank 1 (Vector) template class. Can handle any type of values but POD is preferred
{
 public:
    Tensor()
    {
        Array = nullptr;
        Size_A = 0;
        allocated = false;
    }
    Tensor(const Tensor<T, 1>& locTensor)
    {
        Size_A = locTensor.Size_A;
        if(Size_A)
        {
            Array = new T[Size_A] ();
            memmove(Array, locTensor.data(), sizeof(T)*Size_A);
            allocated = true;
        }
        else
        {
            Array = nullptr;
            allocated = false;
        }
    }
    Tensor(T* data_ptr, int Size)
    {
        Array = data_ptr;
        allocated = false;
        Size_A = Size;
    }
    T& operator()(int a)
    {
#ifdef DEBUG
        if(a > Size_A)
        {
            std::cout << "Error in Tensor<T, 1>::operator()\n"
                 << "Access beyond storage range. a = "
                 << a << " > Size_A = "
                 << Size_A
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[a];
    }
    void Allocate(int na)
    {
        if(!allocated)
        {
            Size_A = na;
            Array = new T[na] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 1>: Allocation attempt of already allocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    void Reallocate(int na)
    {
        if(allocated)
        {
            delete[] Array;
            Size_A = na;
            Array = new T[na] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 1>: Reallocation attempt of a nonallocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    int size_A()
    {
        return Size_A;
    }
    ~Tensor()
    {
        if(allocated)
        {
            delete[] Array;
        }
    }
    const T* data(void) const
    {
        return Array;
    }
    T* data(void)
    {
        return Array;
    }
    Tensor<T, 1>& operator=(const Tensor<T, 1>& locTensor)
    {
        if (this == &locTensor)
        {
            return *this;
        }
        if(Size_A == locTensor.Size_A)
        {
            memmove(Array, locTensor.data(), sizeof(T)*Size_A);
            return *this;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 1>: Different tensor size in assignement operator!"
                      << std::endl;
            exit(133);
        }

    }
    Tensor<T, 1> operator+(const Tensor<T, 1>& locTensor)
    {
        if (locTensor.Size_A == Size_A)
        {
            Tensor<T,1> myReturn(locTensor);

            for(int i = 0; i < Size_A; i++)
            {
                myReturn(i) += Array[i];
            }
            return myReturn;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 1>: Different tensor size in summation operator!"
                      << std::endl;
            exit(133);
        }

    }
    Tensor<T, 1> operator*(double num)
    {
        Tensor<T, 1> myReturn(*this);

        for(int i = 0; i < Size_A; i++)
        {
            myReturn(i) *= num;
        }
        return myReturn;
    }
 protected:
    int Size_A;
    bool allocated;
    T*  Array;
 private:
};

template <class T>
class Tensor<T, 2>                                                              /// Tensor of rank 2 (Matrix) template class. Can handle any type of values
{
 public:
    Tensor()
    {
        Array = nullptr;
        allocated = false;
    }
    Tensor(const Tensor<T,2>& locTensor)
    {
        Size_A = locTensor.Size_A;
        Size_B = locTensor.Size_B;
        if(Size_A*Size_B)
        {
            Array = new T[Size_A*Size_B] ();
            memmove(Array, locTensor.data(), sizeof(T)*Size_A*Size_B);
            allocated = true;
        }
        else
        {
            Array = nullptr;
            allocated = false;
        }
    }
    Tensor(T* data_ptr, int Size1, int Size2)
    {
        Array = data_ptr;
        allocated = false;
        Size_A = Size1;
        Size_B = Size2;
    }
    T& operator()(int a, int b)
    {
#ifdef DEBUG
        if(a > Size_A or b > Size_B)
        {
            std::cout << "Error in Tensor<T, 2>::operator()\n"
                 << "Access beyond storage range. (a, b) = ("
                 << a <<", " << b << ") > (Size_A, Size_B) = ("
                 << Size_A << ", " << Size_B <<")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[Size_B*a + b];
    }
    void Allocate(int na, int nb)
    {
        if(!allocated)
        {
            Size_A = na;
            Size_B = nb;
            Array = new T[na*nb] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 2>: Allocation attempt of already allocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    void Reallocate(int na, int nb)
    {
        if(allocated)
        {
            delete[] Array;
            Size_A = na;
            Size_B = nb;
            Array = new T[na*nb] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 2>: Reallocation attempt of a nonallocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    inline void set_to_zero()
    {
        for(int a = 0; a < Size_A; a++)
        for(int b = 0; b < Size_B; b++)
        {
            Array[Size_B*a + b] = 0;
        }
    }
    int size_A()
    {
        return Size_A;
    }
    int size_B()
    {
        return Size_B;
    }
    ~Tensor()
    {
        if(allocated)
        {
            delete[] Array;
        }
    }
    const T* data(void) const
    {
        return Array;
    }
    T* data(void)
    {
        return Array;
    }
    Tensor<T, 2>& operator=(const Tensor<T, 2>& locTensor)
    {
        if (this == &locTensor)
        {
            return *this;
        }

        if(Size_A == locTensor.Size_A and
           Size_B == locTensor.Size_B)
        {
            memmove(Array, locTensor.data(), sizeof(T)*Size_A*Size_B);
            return *this;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 2>: Different tensor size in assignement operator!"
                      << std::endl;
            exit(133);
        }
    }
    Tensor<T, 2> operator+(const Tensor<T, 2>& locTensor)
    {
        if (locTensor.Size_A == Size_A and
            locTensor.Size_B == Size_B)
        {
            Tensor<T, 2> myReturn(locTensor);

            for(int i = 0; i < Size_A; i++)
            for(int j = 0; j < Size_B; j++)
            {
                myReturn(i,j) += Array[Size_B*i + j];
            }
            return myReturn;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 2>: Different tensor size in summation operator!"
                      << std::endl;
            exit(133);
        }
    }
    Tensor<T, 2> operator*(double num)
    {
        Tensor<T, 2> myReturn(*this);

        for(int i = 0; i < Size_A; i++)
        for(int j = 0; j < Size_B; j++)
        {
            myReturn(i,j) *= num;
        }
        return myReturn;
    }
 protected:
    int Size_A;
    int Size_B;
    bool allocated;
    T*  Array;
 private:
};


template <class T>
class Tensor<T, 3>                                                              /// Tensor of rank 3 template class. Can handle any type of values but POD is preferred
{
 public:
    Tensor<T, 3>()
    {
        Array = nullptr;
        allocated = false;
    }
    Tensor(const Tensor<T,3>& locTensor)
    {
        Size_A = locTensor.Size_A;
        Size_B = locTensor.Size_B;
        Size_C = locTensor.Size_C;
        if(Size_A*Size_B*Size_C)
        {
            Array = new T[Size_A*Size_B*Size_C] ();
            memmove(Array, locTensor.data(), sizeof(T)*Size_A*Size_B*Size_C);
            allocated = true;
        }
        else
        {
            Array = nullptr;
            allocated = false;
        }
    }
    Tensor(T* data_ptr, int Size1, int Size2, int Size3)
    {
        Array = data_ptr;
        allocated = false;
        Size_A = Size1;
        Size_B = Size2;
        Size_C = Size3;
    }
    T& operator()(int a, int b, int c)
    {
#ifdef DEBUG
        if(a > Size_A or b > Size_B or c > Size_C)
        {
            std::cout << "Error in Tensor<T, 3>::operator()\n"
                 << "Access beyond storage range. (a, b, c) = ("
                 << a <<", "<< b << ", " << c << ") > (Size_A, Size_B, Size_C) = ("
                 << Size_A << ", " << Size_B << ", " << Size_C << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[(Size_B*a + b)*Size_C + c];
    }
    void Allocate(int na, int nb, int nc)
    {
        if(!allocated)
        {
            Size_A = na;
            Size_B = nb;
            Size_C = nc;
            Array = new T[na*nb*nc] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 3>: Allocation attempt of already allocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    void Reallocate(int na, int nb, int nc)
    {
        if(allocated)
        {
            delete[] Array;
            Size_A = na;
            Size_B = nb;
            Size_C = nc;
            Array = new T[na*nb*nc] ();
            allocated = true;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 3>: Reallocation attempt of a nonallocated Tensor!"
                      << std::endl;
            exit(133);
        }
    }
    int size_A()
    {
        return Size_A;
    }
    int size_B()
    {
        return Size_B;
    }
    int size_C()
    {
        return Size_C;
    }
    ~Tensor()
    {
        if(allocated)
        {
            delete[] Array;
        }
    }
    const T* data(void) const
    {
        return Array;
    }
    T* data(void)
    {
        return Array;
    }
    Tensor<T, 3>& operator=(const Tensor<T, 3>& locTensor)
    {
        if (this == &locTensor)
        {
            return *this;
        }

        if(Size_A == locTensor.Size_A and
           Size_B == locTensor.Size_B and
           Size_C == locTensor.Size_C)
        {
            memmove(Array, locTensor.data(), sizeof(T)*Size_A*Size_B*Size_C);
            return *this;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 3>: Different tensor size in assignement operator!"
                      << std::endl;
            exit(133);
        }
    }
    Tensor<T, 3> operator+(const Tensor<T, 3>& locTensor)
    {
        if (locTensor.Size_A == Size_A and
            locTensor.Size_B == Size_B and
            locTensor.Size_C == Size_C)
        {
            Tensor<T, 2> myReturn(locTensor);

            for(int i = 0; i < Size_A; i++)
            for(int j = 0; j < Size_B; j++)
            for(int k = 0; k < Size_C; k++)
            {
                myReturn(i,j) += Array[Size_C*(Size_B*i + j) + k];
            }
            return myReturn;
        }
        else
        {
            std::cout << "ERROR: Tensor<T, 3>: Different tensor size in summation operator!"
                      << std::endl;
            exit(133);
        }
    }
    Tensor<T, 3> operator*(double num)
    {
        Tensor<T, 3> myReturn(*this);

        for(int i = 0; i < Size_A; i++)
        for(int j = 0; j < Size_B; j++)
        for(int k = 0; k < Size_C; k++)
        {
            myReturn(i,j,k) *= num;
        }
        return myReturn;
    }
 protected:
    int Size_A;
    int Size_B;
    int Size_C;
    bool allocated;
    T*  Array;
 private:
};

}// namespace openphase
#endif
