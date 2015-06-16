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

#ifndef STORAGE3D_H
#define STORAGE3D_H

namespace openphase
{


template <typename A, typename T, typename = void>
struct ClearCaller;

template <typename T>
class has_clear
{
    typedef char one;
    typedef long two;

    template <typename C> static one test( decltype(&C::clear) ) ;
    template <typename C> static two test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <typename A, typename T>
class ClearCaller<A, T, typename std::enable_if<has_clear<T>::value>::type> {
public:
    static T call(A& self) {
        for (int i = 0; i < self.size(); ++i) self.Array[i].clear();
        return T();
    }
};
  
template <typename A, typename T>
class ClearCaller<A, T, typename std::enable_if<!has_clear<T>::value>::type> {
public:
    static T call(A& self) {
        std::cerr << "Error in Storage3D.h: Clear() called for non-POD with no clear method." << std::endl;
        exit(1);
        return T();
    }
};


template <class T, int Num>
class Storage3D                                                                 /// 3D storage template class. Can handle any type of numerical values
{
 public:
    /*Storage3D<T, Num>()
    {
        Array = NULL;
    }
    T& operator()(int x, int y, int z)
    {
        return Array[(Size_Y*x + y)*Size_Z + z];
    }
    T& operator[](int idx)
    {
        return Array[idx];
    }
    void Allocate(int nx, int ny, int nz)
    {
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = new T[Size_X*Size_Y*Size_Z] ();
    }
    void Reallocate(int nx, int ny, int nz)
    {
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = new T[Size_X*Size_Y*Size_Z] ();
    }
    bool IsEmpty()
    {
        return (Array==NULL);
    }
    void Remesh(int nx, int ny, int nz)
    {
        T* tempArray = new T[nx*ny*nz] ();

        double Xscale = double(Size_X)/double(nx);
        double Yscale = double(Size_Y)/double(ny);
        double Zscale = double(Size_Z)/double(nz);

        for(int x = 1; x < nx - 1; x++)
        for(int y = 1; y < ny - 1; y++)
        for(int z = 1; z < nz - 1; z++)
        {
            int x0 = floor((x - nx*0.5)*Xscale + Size_X * 0.5);
            int y0 = floor((y - ny*0.5)*Yscale + Size_Y * 0.5);
            int z0 = floor((z - nz*0.5)*Zscale + Size_Z * 0.5);
            double dx = x*Xscale - x0;
            double dy = y*Yscale - y0;
            double dz = z*Zscale - z0;

            tempArray[(ny*x + y)*nz + z] =
                  Array[(Size_Y* x0    +  y0   )*Size_Z +  z0   ]*((1.0 - dx)*(1.0 - dy)*(1.0 - dz)) +
                  Array[(Size_Y*(x0+1) +  y0   )*Size_Z +  z0   ]*(dx*(1.0- dy)*(1.0 - dz)) +
                  Array[(Size_Y* x0    + (y0+1))*Size_Z +  z0   ]*((1.0 - dx)*dy*(1.0 - dz)) +
                  Array[(Size_Y* x0    +  y0   )*Size_Z + (z0+1)]*((1.0 - dx)*(1.0 - dy)*dz) +
                  Array[(Size_Y*(x0+1) + (y0+1))*Size_Z +  z0   ]*(dx*dy*(1.0 - dz)) +
                  Array[(Size_Y*(x0+1) +  y0   )*Size_Z + (z0+1)]*(dx*(1.0 - dy)*dz) +
                  Array[(Size_Y* x0    + (y0+1))*Size_Z + (z0+1)]*((1.0 - dx)*dy*dz) +
                  Array[(Size_Y*(x0+1) + (y0+1))*Size_Z + (z0+1)]*(dx*dy*dz);
        }
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;

        Array = tempArray;
    }
    ~Storage3D<T, Num>()
    {
        delete[] Array;
    }
    Storage3D<T, Num>& operator=(Storage3D<T, Num>& locStorage3D)
    {
        if(Array != NULL and std::is_trivial<T>::value)
        {
            if (locStorage3D.Size_X == Size_X and
                locStorage3D.Size_Y == Size_Y and
                locStorage3D.Size_Z == Size_Z)
            {
                memcpy(Array, locStorage3D.data(), sizeof(T)*Size_X*Size_Y*Size_Z);
            }
            else
            {
                std::cout << "ERROR: Storage3D<T, Num>: Wrong storage size in assignement operator! \n"
                          << "Terminating!!!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "Storage3D<T, Num>::operator=() on non-trivial type \n"
                      << "storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }

        return *this;
    }
    void Clear(void)
    {
        if(Array != NULL and std::is_trivial<T>::value)
        {
            memset(Array, 0, sizeof(T)*Size_X*Size_Y*Size_Z);
        }
        else
        {
            std::cout << "Storage3D::Clear() operation of non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
    }
    T* data(void)
    {
        return Array;
    }
    int sizeX()
    {
        return Size_X;
    }
    int sizeY()
    {
        return Size_Y;
    }
    int sizeZ()
    {
        return Size_Z;
    }
 protected:
 private:
    T* Array;
    int Size_X;
    int Size_Y;
    int Size_Z;*/
};

template <class T>
class Storage3D<T, 0>                                                           /// 3D storage template class. Can handle any type of numerical values
{
 public:
    friend class ClearCaller< Storage3D<T, 0> ,T>; 
    int size()
    {
        return Size_X*Size_Y*Size_Z;
    }
    Storage3D<T, 0>()
    {
        Array = nullptr;
    }
    T& operator()(int x, int y, int z)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z)
        {
            std::cout << "Error in Storage3D<T, 0>::operator()\n"
                 << "Access beyond storage range. (x, y, z) = ("
                 << x << "," << y << "," << z << ")"
                 << " > (Size_x, Size_y, Size_z) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[(Size_Y*x + y)*Size_Z + z];
    }
    T& operator[](int idx)
    {
#ifdef DEBUG
        if(idx > Size_X*Size_Y*Size_Z)
        {
            std::cout << "Error in Storage3D<T, 0>::operator[]\n"
                 << "Access beyond storage range. idx = "
                 << idx << " > Size_x*Size_y*Size_z = "
                 << Size_X * Size_Y * Size_Z
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[idx];
    }
    void Allocate(int nx, int ny, int nz)
    {
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = new T[Size_X*Size_Y*Size_Z] ();
    }
    void Reallocate(int nx, int ny, int nz)
    {
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = new T[Size_X*Size_Y*Size_Z] ();
    }
    bool IsEmpty()
    {
        return (Array==nullptr);
    }
    void Remesh(int nx, int ny, int nz)
    {
        T* tempArray = new T[nx*ny*nz] ();

        double Xscale = double(Size_X)/double(nx);
        double Yscale = double(Size_Y)/double(ny);
        double Zscale = double(Size_Z)/double(nz);

        for(int x = 1; x < nx - 1; x++)
        for(int y = 1; y < ny - 1; y++)
        for(int z = 1; z < nz - 1; z++)
        {
            int x0 = floor((x - nx*0.5)*Xscale + Size_X * 0.5);
            int y0 = floor((y - ny*0.5)*Yscale + Size_Y * 0.5);
            int z0 = floor((z - nz*0.5)*Zscale + Size_Z * 0.5);
            double dx = x*Xscale - x0;
            double dy = y*Yscale - y0;
            double dz = z*Zscale - z0;

            tempArray[(ny*x + y)*nz + z] =
                  Array[(Size_Y* x0    +  y0   )*Size_Z +  z0   ]*((1.0 - dx)*(1.0 - dy)*(1.0 - dz)) +
                  Array[(Size_Y*(x0+1) +  y0   )*Size_Z +  z0   ]*(dx*(1.0- dy)*(1.0 - dz)) +
                  Array[(Size_Y* x0    + (y0+1))*Size_Z +  z0   ]*((1.0 - dx)*dy*(1.0 - dz)) +
                  Array[(Size_Y* x0    +  y0   )*Size_Z + (z0+1)]*((1.0 - dx)*(1.0 - dy)*dz) +
                  Array[(Size_Y*(x0+1) + (y0+1))*Size_Z +  z0   ]*(dx*dy*(1.0 - dz)) +
                  Array[(Size_Y*(x0+1) +  y0   )*Size_Z + (z0+1)]*(dx*(1.0 - dy)*dz) +
                  Array[(Size_Y* x0    + (y0+1))*Size_Z + (z0+1)]*((1.0 - dx)*dy*dz) +
                  Array[(Size_Y*(x0+1) + (y0+1))*Size_Z + (z0+1)]*(dx*dy*dz);
        }
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;

        Array = tempArray;
    }
    ~Storage3D<T, 0>()
    {
        delete[] Array;
    }
    Storage3D<T, 0>& operator=(Storage3D<T, 0>& locStorage3D)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            if (locStorage3D.Size_X == Size_X and
                locStorage3D.Size_Y == Size_Y and
                locStorage3D.Size_Z == Size_Z)
            {
                memcpy(Array, locStorage3D.data(), sizeof(T)*Size_X*Size_Y*Size_Z);
            }
            else
            {
                std::cout << "ERROR: Storage3D<T, 0>: Wrong storage size in assignement operator! \n"
                          << "Terminating!!!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "Storage3D<T, 0>::operator=() on non-trivial type \n"
                      << "storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }

        return *this;
    }
    void Clear(void)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            memset(Array, 0, sizeof(T)*Size_X*Size_Y*Size_Z);
        }
        else
        {
            ClearCaller< Storage3D<T, 0> ,T> K;
            K.call(*this);
            /*std::cout << "Storage3D<T, 0>::Clear() operation on non-trivial type \n"
                      << "storage is not supported! Terminating!!!" << std::endl;
            exit(1); */
        }
    }
    void set_to_value(T val)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            for(int i = 0; i < Size_X*Size_Y*Size_Z; i++)
            {
                Array[i] = val;
            }
        }
        else
        {
            std::cout << "Storage3D<T, 0>::set_to_value() operation on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
    }
    T* data(void)
    {
        return Array;
    }
    int sizeX()
    {
        return Size_X;
    }
    int sizeY()
    {
        return Size_Y;
    }
    int sizeZ()
    {
        return Size_Z;
    }
 protected:
 private:
    T* Array;
    int Size_X;
    int Size_Y;
    int Size_Z;
};


template <class T>
class Storage3D<T, 1>                                                         /// 3D storage template class of vector values. Can handle any type of values
{
 public:
    friend class ClearCaller< Storage3D<T, 1> ,T>; 
    int size()
    {
        return Size_X*Size_Y*Size_Z*Size_N;
    }
    Storage3D<T, 1>()
    {
        Array = nullptr;
    }
    T& operator()(int x, int y, int z, int n)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z or n > Size_N)
        {
            std::cout << "Error in Storage3D<T, 1>::operator()\n"
                 << "Access beyond storage range. (x, y, z, n) = ("
                 << x << "," << y << "," << z << "," << n << ")"
                 << " > (Size_x, Size_y, Size_z, Size_N) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ", " << Size_N << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[((Size_Y*x + y)*Size_Z + z)*Size_N + n];
    }
    Tensor<T,1> operator()(int x, int y, int z)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z)
        {
            std::cout << "Error in Storage3D<T, 1>::operator()\n"
                 << "Access beyond storage range. (x, y, z) = ("
                 << x << "," << y << "," << z << ")"
                 << " > (Size_x, Size_y, Size_z) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        Tensor<T,1> locReturn(&Array[((Size_Y*x + y)*Size_Z + z)*Size_N], Size_N);

        return locReturn;
    }
    T& operator[](int idx)
    {
#ifdef DEBUG
        if(idx > Size_X*Size_Y*Size_Z)
        {
            std::cout << "Error in Storage3D<T, 1>::operator[]\n"
                 << "Access beyond storage range. idx = "
                 << idx << " > Size_x*Size_y*Size_z = "
                 << Size_X * Size_Y * Size_Z
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[idx];
    }
    void Allocate(int nx, int ny, int nz, int nn)
    {
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N = nn;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N] ();
    }
    void Reallocate(int nx, int ny, int nz, int nn)
    {
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N = nn;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N] ();
    }
    bool IsEmpty()
    {
        return (Array==nullptr);
    }
    void Remesh(int nx, int ny, int nz)
    {
        T* tempArray = new T[nx*ny*nz*Size_N] ();

        double Xscale = double(Size_X)/double(nx);
        double Yscale = double(Size_Y)/double(ny);
        double Zscale = double(Size_Z)/double(nz);
        for(int x = 1; x < nx - 1; x++)
        for(int y = 1; y < ny - 1; y++)
        for(int z = 1; z < nz - 1; z++)
        for(int n = 0; n < Size_N; n++)
        {
            int x0 = floor((x - nx*0.5)*Xscale + Size_X * 0.5);
            int y0 = floor((y - ny*0.5)*Yscale + Size_Y * 0.5);
            int z0 = floor((z - nz*0.5)*Zscale + Size_Z * 0.5);
            double dx = x*Xscale - x0;
            double dy = y*Yscale - y0;
            double dz = z*Zscale - z0;

            tempArray[((ny*x + y)*nz + z)*Size_N + n] =
                  Array[((Size_Y* x0    +  y0   )*Size_Z +  z0   )*Size_N + n]*((1.0 - dx)*(1.0 - dy)*(1.0 - dz)) +
                  Array[((Size_Y*(x0+1) +  y0   )*Size_Z +  z0   )*Size_N + n]*(dx*(1.0- dy)*(1.0 - dz)) +
                  Array[((Size_Y* x0    + (y0+1))*Size_Z +  z0   )*Size_N + n]*((1.0 - dx)*dy*(1.0 - dz)) +
                  Array[((Size_Y* x0    +  y0   )*Size_Z + (z0+1))*Size_N + n]*((1.0 - dx)*(1.0 - dy)*dz) +
                  Array[((Size_Y*(x0+1) + (y0+1))*Size_Z +  z0   )*Size_N + n]*(dx*dy*(1.0 - dz)) +
                  Array[((Size_Y*(x0+1) +  y0   )*Size_Z + (z0+1))*Size_N + n]*(dx*(1.0 - dy)*dz) +
                  Array[((Size_Y* x0    + (y0+1))*Size_Z + (z0+1))*Size_N + n]*((1.0 - dx)*dy*dz) +
                  Array[((Size_Y*(x0+1) + (y0+1))*Size_Z + (z0+1))*Size_N + n]*(dx*dy*dz);
        }
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = tempArray;
    }
    ~Storage3D<T, 1>()
    {
        delete[] Array;
    }
    Storage3D<T, 1>& operator=(Storage3D<T, 1>& locStorage3D)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            if (locStorage3D.Size_X == Size_X and
                locStorage3D.Size_Y == Size_Y and
                locStorage3D.Size_Z == Size_Z and
                locStorage3D.Size_N == Size_N)
            {
                memcpy(Array, locStorage3D.data(), sizeof(T)*Size_X*Size_Y*Size_Z*Size_N);
            }
            else
            {
                std::cout << "ERROR: Storage3D<T, 1>: Wrong storage size in assignement operator! \n"
                          << "Terminating!!!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "Storage3D<T, 1>::operator=() on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
        return *this;
    }
    void Clear(void)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            memset(Array, 0, sizeof(T)*Size_X*Size_Y*Size_Z*Size_N);
        }
        else
        {
            ClearCaller< Storage3D<T, 1> ,T> K;
            K.call(*this);
            /*std::cout << "Storage3D<T, 1>::Clear() operation on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1); */
        }
    }
    void set_to_value(T val)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            for(int i = 0; i < Size_X*Size_Y*Size_Z*Size_N; i++)
            {
                Array[i] = val;
            }
        }
        else
        {
            std::cout << "Storage3D<T, 1>::set_to_value() operation on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
    }
    T* data(void)
    {
        return Array;
    }
    int sizeX()
    {
        return Size_X;
    }
    int sizeY()
    {
        return Size_Y;
    }
    int sizeZ()
    {
        return Size_Z;
    }
    int sizeN()
    {
        return Size_N;
    }
 protected:
 private:
    T* Array;
    int Size_X;
    int Size_Y;
    int Size_Z;
    int Size_N;
};


template <class T>
class Storage3D<T, 2>                                                           /// 3D storage template class of vector of vectors. Can handle any type of values
{
 public:
    friend class ClearCaller< Storage3D<T, 2> ,T>; 
    int size()
    {
        return Size_X*Size_Y*Size_Z*Size_N1*Size_N2;
    }
    Storage3D<T, 2>()
    {
        Array = nullptr;
    }
    T& operator()(int x, int y, int z, int n1, int n2)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z or n1 > Size_N1 or n2 > Size_N2)
        {
            std::cout << "Error in Storage3D<T, 2>::operator()\n"
                 << "Access beyond storage range. (x, y, z, n1, n2) = ("
                 << x << "," << y << "," << z << "," << n1 << "," << n2 << ")"
                 << " > (Size_X, Size_Y, Size_Z, Size_N1, Size_N2) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ", " << Size_N1 << ", " << Size_N2 << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[(((Size_Y*x + y)*Size_Z + z)*Size_N1 + n1)*Size_N2 + n2];
    }
    T& operator[](int idx)
    {
#ifdef DEBUG
        if(idx > Size_X*Size_Y*Size_Z*Size_N1*Size_N2)
        {
            std::cout << "Error in Storage3D<T, 2>::operator[]\n"
                 << "Access beyond storage range. idx = "
                 << idx << " > Size_X*Size_Y*Size_Z*Size_N1*Size_N2 = "
                 << Size_X * Size_Y * Size_Z * Size_N1 * Size_N2
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[idx];
    }
    void Allocate(int nx, int ny, int nz, int nn1, int nn2)
    {
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N1 = nn1;
        Size_N2 = nn2;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N1*Size_N2] ();
    }
    void Reallocate(int nx, int ny, int nz, int nn1, int nn2)
    {
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N1 = nn1;
        Size_N2 = nn2;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N1*Size_N2] ();
    }
    bool IsEmpty()
    {
        return (Array==nullptr);
    }
    void Remesh(int nx, int ny, int nz)
    {
        T* tempArray = new T[nx*ny*nz*Size_N1*Size_N2] ();

        double Xscale = double(Size_X)/double(nx);
        double Yscale = double(Size_Y)/double(ny);
        double Zscale = double(Size_Z)/double(nz);
        for(int x = 1; x < nx - 1; x++)
        for(int y = 1; y < ny - 1; y++)
        for(int z = 1; z < nz - 1; z++)
        for(int n1 = 0; n1 < Size_N1; n1++)
        for(int n2 = 0; n2 < Size_N2; n2++)
        {
            int x0 = floor((x - nx*0.5)*Xscale + Size_X * 0.5);
            int y0 = floor((y - ny*0.5)*Yscale + Size_Y * 0.5);
            int z0 = floor((z - nz*0.5)*Zscale + Size_Z * 0.5);
            double dx = x*Xscale - x0;
            double dy = y*Yscale - y0;
            double dz = z*Zscale - z0;

            tempArray[(((ny*x + y)*nz + z)*Size_N1 + n1)*Size_N2 + n2] =
                  Array[(((Size_Y* x0    +  y0   )*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2]*((1.0 - dx)*(1.0 - dy)*(1.0 - dz)) +
                  Array[(((Size_Y*(x0+1) +  y0   )*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2]*(       dx *(1.0 - dy)*(1.0 - dz)) +
                  Array[(((Size_Y* x0    + (y0+1))*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2]*((1.0 - dx)*       dy *(1.0 - dz)) +
                  Array[(((Size_Y* x0    +  y0   )*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2]*((1.0 - dx)*(1.0 - dy)*       dz)  +
                  Array[(((Size_Y*(x0+1) + (y0+1))*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2]*(       dx *       dy *(1.0 - dz)) +
                  Array[(((Size_Y*(x0+1) +  y0   )*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2]*(       dx *(1.0 - dy)*       dz)  +
                  Array[(((Size_Y* x0    + (y0+1))*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2]*((1.0 - dx)*       dy *       dz) +
                  Array[(((Size_Y*(x0+1) + (y0+1))*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2]*(       dx *       dy *       dz);
        }
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = tempArray;
    }
    ~Storage3D<T, 2>()
    {
        delete[] Array;
    }
    Tensor<T, 2> operator()(int x, int y, int z)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z)
        {
            std::cout << "Error in Storage3D<T, 2>::operator()\n"
                 << "Access beyond storage range. (x, y, z) = ("
                 << x << "," << y << "," << z << ")"
                 << " > (Size_x, Size_y, Size_z) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        Tensor<T,2> locReturn(&Array[((Size_Y*x + y)*Size_Z + z)*Size_N1*Size_N2], Size_N1, Size_N2);

        return locReturn;
    }
    Storage3D<T, 2>& operator=(Storage3D<T, 2>& locStorage3D)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            if (locStorage3D.Size_X == Size_X and
                locStorage3D.Size_Y == Size_Y and
                locStorage3D.Size_Z == Size_Z and
                locStorage3D.Size_N1 == Size_N1 and
                locStorage3D.Size_N2 == Size_N2)
            {
                memcpy(Array, locStorage3D.data(), sizeof(T)*Size_X*Size_Y*Size_Z*Size_N1*Size_N2);
            }
            else
            {
                std::cout << "ERROR: Storage3D<T, 2>: Wrong storage size in assignement operator! \n"
                          << "Terminating!!!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "Storage3D<T, 2>::operator=() on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
        return *this;
    }
    void Clear(void)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            memset(Array, 0, sizeof(T)*Size_X*Size_Y*Size_Z*Size_N1*Size_N2);
        }
        else
        {
            ClearCaller< Storage3D<T, 2> ,T> K;
            K.call(*this);
            /*std::cout << "Storage3D<T, 2>::Clear() operation of non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);*/
        }
    }
    void set_to_value(T val)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            for(int i = 0; i < Size_X*Size_Y*Size_Z*Size_N1*Size_N2; i++)
            {
                Array[i] = val;
            }
        }
        else
        {
            std::cout << "Storage3D<T, 2>::set_to_value() operation on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
    }
    T* data(void)
    {
        return Array;
    }
    int sizeX()
    {
        return Size_X;
    }
    int sizeY()
    {
        return Size_Y;
    }
    int sizeZ()
    {
        return Size_Z;
    }
    int sizeN1()
    {
        return Size_N1;
    }
    int sizeN2()
    {
        return Size_N2;
    }
 protected:
 private:
    T* Array;
    int Size_X;
    int Size_Y;
    int Size_Z;
    int Size_N1;
    int Size_N2;
};


template <class T>
class Storage3D<T, 3>                                                           /// 3D storage template class of rank 3 tensors. Can handle any type of values
{
 public:
    friend class ClearCaller< Storage3D<T, 0> ,T>; 
    int size()
    {
        return Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3;
    }
    Storage3D<T, 3>()
    {
        Array = nullptr;
    }
    T& operator()(int x, int y, int z, int n1, int n2, int n3)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z or n1 > Size_N1 or n2 > Size_N2 or n3 > Size_N3)
        {
            std::cout << "Error in Storage3D<T, 3>::operator()\n"
                 << "Access beyond storage range. (x, y, z, n1, n2, n3) = ("
                 << x << "," << y << "," << z << "," << n1 << "," << n2 << "," << n3 << ")"
                 << " > (Size_X, Size_Y, Size_Z, Size_N1, Size_N2, Size_N3) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ", "
                 << Size_N1 << ", " << Size_N2 << ", " << Size_N3 << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[((((Size_Y*x + y)*Size_Z + z)*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3];
    }
    T& operator[](int idx)
    {
#ifdef DEBUG
        if(idx > Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3)
        {
            std::cout << "Error in Storage3D<T, 3>::operator[]\n"
                 << "Access beyond storage range. idx = "
                 << idx << " > Size_x*Size_y*Size_z*Size_N1*Size_N2*Size_N3 = "
                 << Size_X * Size_Y * Size_Z * Size_N1 * Size_N2 * Size_N3
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        return Array[idx];
    }
    void Allocate(int nx, int ny, int nz, int nn1, int nn2, int nn3)
    {
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N1 = nn1;
        Size_N2 = nn2;
        Size_N3 = nn3;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3] ();
    }
    void Reallocate(int nx, int ny, int nz, int nn1, int nn2, int nn3)
    {
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Size_N1 = nn1;
        Size_N2 = nn2;
        Size_N3 = nn3;
        Array = new T[Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3] ();
    }
    bool IsEmpty()
    {
        return (Array==nullptr);
    }
    void Remesh(int nx, int ny, int nz)
    {
        T* tempArray = new T[nx*ny*nz*Size_N1*Size_N2*Size_N3] ();

        double Xscale = double(Size_X)/double(nx);
        double Yscale = double(Size_Y)/double(ny);
        double Zscale = double(Size_Z)/double(nz);
        for(int x = 1; x < nx - 1; x++)
        for(int y = 1; y < ny - 1; y++)
        for(int z = 1; z < nz - 1; z++)
        for(int n1 = 0; n1 < Size_N1; n1++)
        for(int n2 = 0; n2 < Size_N2; n2++)
        for(int n3 = 0; n3 < Size_N3; n3++)
        {
            int x0 = floor((x - nx*0.5)*Xscale + Size_X * 0.5);
            int y0 = floor((y - ny*0.5)*Yscale + Size_Y * 0.5);
            int z0 = floor((z - nz*0.5)*Zscale + Size_Z * 0.5);
            double dx = x*Xscale - x0;
            double dy = y*Yscale - y0;
            double dz = z*Zscale - z0;

            tempArray[((((ny*x + y)*nz + z)*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3] =
                  Array[((((Size_Y* x0    +  y0   )*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*((1.0 - dx)*(1.0 - dy)*(1.0 - dz)) +
                  Array[((((Size_Y*(x0+1) +  y0   )*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*(       dx *(1.0 - dy)*(1.0 - dz)) +
                  Array[((((Size_Y* x0    + (y0+1))*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*((1.0 - dx)*       dy *(1.0 - dz)) +
                  Array[((((Size_Y* x0    +  y0   )*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*((1.0 - dx)*(1.0 - dy)*       dz)  +
                  Array[((((Size_Y*(x0+1) + (y0+1))*Size_Z +  z0   )*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*(       dx *       dy *(1.0 - dz)) +
                  Array[((((Size_Y*(x0+1) +  y0   )*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*(       dx *(1.0 - dy)*       dz)  +
                  Array[((((Size_Y* x0    + (y0+1))*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*((1.0 - dx)*       dy *       dz) +
                  Array[((((Size_Y*(x0+1) + (y0+1))*Size_Z + (z0+1))*Size_N1 + n1)*Size_N2 + n2)*Size_N3 + n3]*(       dx *       dy *       dz);
        }
        delete[] Array;
        Size_X = nx;
        Size_Y = ny;
        Size_Z = nz;
        Array = tempArray;
    }
    ~Storage3D<T, 3>()
    {
        delete[] Array;
    }

    Tensor<T, 3> operator()(int x, int y, int z)
    {
#ifdef DEBUG
        if(x > Size_X or y > Size_Y or z > Size_Z)
        {
            std::cout << "Error in Storage3D<T, 3>::operator()\n"
                 << "Access beyond storage range. (x, y, z) = ("
                 << x << "," << y << "," << z << ")"
                 << " > (Size_x, Size_y, Size_z) = ("
                 << Size_X << ", " << Size_Y << ", " << Size_Z << ")"
                 << "\nTerminating!!!" << std::endl;
            exit(13);
        }
#endif
        Tensor<T,3> locReturn(&Array[((Size_Y*x + y)*Size_Z + z)*Size_N1*Size_N2*Size_N3], Size_N1, Size_N2, Size_N3);

        return locReturn;
    }
    Storage3D<T, 3>& operator=(Storage3D<T, 3>& locStorage3D)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            if (locStorage3D.Size_X == Size_X and
                locStorage3D.Size_Y == Size_Y and
                locStorage3D.Size_Z == Size_Z and
                locStorage3D.Size_N1 == Size_N1 and
                locStorage3D.Size_N2 == Size_N2 and
                locStorage3D.Size_N3 == Size_N3)
            {
                memcpy(Array, locStorage3D.data(), sizeof(T)*Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3);
            }
            else
            {
                std::cout << "ERROR: Storage3D<T, 3>: Wrong storage size in assignement operator! \n"
                          << "Terminating!!!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "Storage3D<T, 3>::operator=() on non-trivial type \n"
                      << "storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
        return *this;
    }
    void Clear(void)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            memset(Array, 0, sizeof(T)*Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3);
        }
        else
        {
            ClearCaller< Storage3D<T, 3> ,T> K;
            K.call(*this);
            /*std::cout << "Storage3D<T, 3>::Clear() operation of non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);*/
        }
    }
    void set_to_value(T val)
    {
        if(Array != nullptr and std::is_trivial<T>::value)
        {
            for(int i = 0; i < Size_X*Size_Y*Size_Z*Size_N1*Size_N2*Size_N3; i++)
            {
                Array[i] = val;
            }
        }
        else
        {
            std::cout << "Storage3D<T, 3>::set_to_value() operation on non-trivial type \n"
                      << " storage is not supported! Terminating!!!" << std::endl;
            exit(1);
        }
    }
    T* data(void)
    {
        return Array;
    }
    int sizeX()
    {
        return Size_X;
    }
    int sizeY()
    {
        return Size_Y;
    }
    int sizeZ()
    {
        return Size_Z;
    }
    int sizeN1()
    {
        return Size_N1;
    }
    int sizeN2()
    {
        return Size_N2;
    }
    int sizeN3()
    {
        return Size_N3;
    }
 protected:
 private:
    T* Array;
    int Size_X;
    int Size_Y;
    int Size_Z;
    int Size_N1;
    int Size_N2;
    int Size_N3;
};

}// namespace openphase
#endif
