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

#ifndef STORAGE_H
#define STORAGE_H

//#define DEBUG

namespace openphase
{

template <class T>
class Storage  /// 1D storage template class. Can handle any type of values
{
 public:
    Storage()
    {
        Array = NULL;
    }
    T& operator[](int x)
    {
#ifdef DEBUG
        if(x > Size_X)
        {
            std::cout << "Error in Storage::operator[]\n"
                 << "Access beyond storage range. x = " << x
                 << " > Size_X = " << Size_X <<"\nTerminating!!!"<<std::endl;
            exit(13);
        }
#endif
        return Array[x];
    }
    void Allocate(int nx)
    {
        Size_X = nx;
        Array = new T[Size_X] ();
    }
    void Reallocate(int nx)
    {
        delete[] Array;
        Size_X = nx;
        Array = new T[Size_X] ();
    }
    int Size()
    {
        return Size_X;
    }
    ~Storage()
    {
        delete[] Array;
    }

 protected:
 private:
    T* Array;
    int Size_X;
};

}// namespace openphase
#endif
