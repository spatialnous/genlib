// genlib - a component of the depthmapX - spatial network analysis platform
// Copyright (C) 2018, Christian Sailer

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <stdexcept>

namespace depthmapX {

/**
 *  Base class for 2 dimensional matrices. This can be used as reference/pointer, but you cannot
 *  create this directly - you need to create either a row or a column matrix (the difference
 *  being the memory layout either, contiguous rows, or contiguous columns.
 */
template<typename T> class BaseMatrix {
protected:
    BaseMatrix(size_t rows, size_t columns ){
        m_data = new T[rows * columns];
        m_rows = rows;
        m_columns = columns;
    }

    BaseMatrix<T>(const BaseMatrix<T> &other) : BaseMatrix<T>(other.m_rows, other.m_columns){
        std::copy(other.begin(), other.end(), m_data);
    }

    BaseMatrix<T>(BaseMatrix<T> &&other): m_data(other.m_data), m_rows(other.m_rows), m_columns(other.m_columns){
        other.m_data = nullptr;
        other.m_rows = 0;
        other.m_columns =0;
    }

public:
    virtual ~BaseMatrix<T>(){
        delete[] m_data;
    }

    /**
     * @brief operator () access operator uses () instead of [] to allow giving two coordinates
     * @param row row to access
     * @param column column to access
     * @return non-const reference to the data
     */
    virtual T & operator()(size_t row, size_t column) = 0;

    /**
     * @brief operator () access operator uses () instead of [] to allow giving two coordinates
     * @param row row to access
     * @param column column to access
     * @return const reference to the data
     */
    virtual T const & operator()(size_t row, size_t column) const = 0;

    /**
     * @brief begin get a pointer to the data array (complies with std::iterator definitions)
     * @return pointer to the first element
     */
    T* begin(){
        return m_data;
    }

    /**
     * @brief end pointer marking the end of the data array
     * @return pointer behind the last element of the data array
     */
    T* end(){
        return m_data + size();
    }

    /**
     * @brief begin get a pointer to the data array (complies with std::iterator definitions)
     * @return const pointer to the first element
     */
    T const * begin() const {
        return m_data;
    }

    /**
     * @brief end pointer marking the end of the data array
     * @return const pointer behind the last element of the data array
     */
    T const * end() const {
        return m_data + size();
    }

    /**
     * @brief size
     * @return size of the data array in elements
     */
    size_t size() const {
        return m_rows * m_columns;
    }

    /**
     * @brief rows
     * @return number of rows
     */
    size_t rows() const {
        return m_rows;
    }

    /**
     * @brief columns
     * @return number of columns
     */
    size_t columns() const {
        return m_columns;
    }

protected:
    T* m_data;
    size_t m_rows;
    size_t m_columns;

    void access_check( size_t row, size_t column) const {
        if ( row >= m_rows ){
            throw std::out_of_range("row out of range");
        }
        if ( column >= m_columns){
            throw std::out_of_range("column out of range");
        }

    }
};


/**
 * Row matrix implementation - the data for each row is contiguous in memory, columns jump by the
 * number of rows.
 */
template<typename T> class RowMatrix : public BaseMatrix<T>{
public:
    RowMatrix(size_t rows, size_t columns) : BaseMatrix<T>(rows, columns){}
    RowMatrix(RowMatrix const & other) : BaseMatrix(other){}
    RowMatrix(RowMatrix && other) : BaseMatrix(std::move(other)){}

    RowMatrix<T> & operator= ( RowMatrix<T> const & other){
        RowMatrix tmp(other);
        std::swap(m_data, tmp.m_data);
        std::swap(m_rows, tmp.m_rows);
        std::swap(m_columns, tmp.m_columns);
        return *this;
    }

    T & operator () (size_t row, size_t column){
        this->access_check(row, column);
        return m_data[column + row * m_columns];
    }

    T const & operator ()(size_t row, size_t column) const {
        this->access_check(row, column);
        return m_data[column + row * m_columns];
    }
};

/**
 * Column matrix implementation - the data for each column is contiguous in memory, rows jump by the
 * number of columns.
 */
template<typename T> class ColumnMatrix : public BaseMatrix<T>{
public:
    ColumnMatrix(size_t rows, size_t columns) : BaseMatrix<T>(rows, columns){}
    ColumnMatrix(ColumnMatrix const & other) : BaseMatrix(other){}
    ColumnMatrix(ColumnMatrix && other) : BaseMatrix(std::move(other)){}

    ColumnMatrix<T> & operator= ( ColumnMatrix<T> const & other){
        ColumnMatrix tmp(other);
        std::swap(m_data, tmp.m_data);
        std::swap(m_rows, tmp.m_rows);
        std::swap(m_columns, tmp.m_columns);
        return *this;
    }

    T & operator () (size_t row, size_t column){
        this->access_check(row, column);
        return m_data[row + column * m_rows];
    }

    T const & operator ()(size_t row, size_t column) const {
        this->access_check(row, column);
        return m_data[row + column * m_rows];
    }
};


}
