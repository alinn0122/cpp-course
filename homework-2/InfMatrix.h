#pragma once
#include <map>
#include <iostream>
template <typename T, T void_val = T()>
class InfMatrix {
private:
    using Row = std::map<int, T>;
    std::map<int, Row> data;
    size_t element_count = 0;

public:

    class CellProxy {
    private:
        InfMatrix& matrix;
        int row, col;

    public:
        CellProxy(InfMatrix& m, int r, int c)
            : matrix(m), row(r), col(c) {}

        operator T() const {
            auto row_it = matrix.data.find(row);
            if (row_it == matrix.data.end())
                return void_val;

            auto col_it = row_it->second.find(col);
            if (col_it == row_it->second.end())
                return void_val;

            return col_it->second;
        }

        CellProxy& operator=(const T& value) {
            auto& row_map = matrix.data[row];
            auto col_it = row_map.find(col);

            if (value == void_val) {
                if (col_it != row_map.end()) {
                    row_map.erase(col_it);
                    matrix.element_count--;
                    if (row_map.empty())
                        matrix.data.erase(row);
                }
            }
            else {
                if (col_it == row_map.end())
                    matrix.element_count++;
                row_map[col] = value;
            }

            return *this;
        }
    };

    class RowProxy {
    private:
        InfMatrix& matrix;
        int row;

    public:
        RowProxy(InfMatrix& m, int r)
            : matrix(m), row(r) {}

        CellProxy operator[](int col) {
            return CellProxy(matrix, row, col);
        }

        T operator[](int col) const {
            auto row_it = matrix.data.find(row);
            if (row_it == matrix.data.end())
                return void_val;

            auto col_it = row_it->second.find(col);
            if (col_it == row_it->second.end())
                return void_val;

            return col_it->second;
        }
    };

    RowProxy operator[](int row) {
        return RowProxy(*this, row);
    }

    const RowProxy operator[](int row) const {
        return RowProxy(const_cast<InfMatrix&>(*this), row);
    }

    size_t size() const {
        return element_count;
    }

    int max_row() const {
        for (auto it = data.rbegin(); it != data.rend(); ++it)
            if (!it->second.empty()) return it->first;
        return -1;
    }

    int max_col() const {
        int max_c = -1;
        for (typename std::map<int, Row>::const_iterator it = data.begin(); it != data.end(); ++it) {
            if (!it->second.empty())
                max_c = std::max(max_c, it->second.rbegin()->first);
        }
        return max_c;
    }

    class Cell {
    private:
        int r, c;
        T v;

    public:
        Cell(int row, int col, const T& val)
            : r(row), c(col), v(val) {}

        int row() const { return r; }
        int col() const { return c; }
        const T& value() const { return v; }
    };

    class iterator {
    private:
        using OuterIt = typename std::map<int, Row>::iterator;
        using InnerIt = typename Row::iterator;

        OuterIt outer;
        OuterIt outer_end;
        InnerIt inner;

        void skip_empty_rows() {
            while (outer != outer_end && inner == outer->second.end()) {
                ++outer;
                if (outer != outer_end)
                    inner = outer->second.begin();
            }
        }

    public:
        iterator(OuterIt out, OuterIt out_end)
            : outer(out), outer_end(out_end) {
            if (outer != outer_end)
                inner = outer->second.begin();
            skip_empty_rows();
        }

        Cell operator*() const {
            return Cell(outer->first, inner->first, inner->second);
        }

        iterator& operator++() {
            ++inner;
            skip_empty_rows();
            return *this;
        }

        bool operator==(const iterator& other) const {
            return outer == other.outer &&
                (outer == outer_end || inner == other.inner);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    iterator begin() { return iterator(data.begin(), data.end()); }
    iterator end() { return iterator(data.end(), data.end()); }
};
