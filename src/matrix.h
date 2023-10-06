#ifndef FILE_MATRIX_H
#define FILE_MATRIX_H

namespace ASC_bla
{

    // choice of row or column major, for template
    enum ORDERING
    {
        RowMajor,
        ColMajor
    };

template <typename T, ORDERING ORD>
    class Matrix
    {
        size_t height_;
        size_t width_;
        T *data_;

    public:
        Matrix(size_t height, size_t width) : height_(height), width_(width), data_(new T[height * width]){};

        Matrix(const Matrix &A) : Matrix(A.height(), A.width()) { *this = A; };

        //TODO move ctor

        ~Matrix() { delete[] data_; }

        Matrix &operator=(const Matrix &A2)
        {
            for (size_t i = 0; i < height_; i++)
            {
                for (size_t j = 0; j < width_; j++)
                {
                    (*this)(i, j) = A2(i, j);
                };
            }
            return *this;
        }
        //TODO move operator=


        size_t height() const { return this->height_; };
        size_t width() const { return this->width_; };

        // round bracket access operator
        const T &operator()(size_t i, size_t j) const
        {
            if (ORD == RowMajor)
            {
                return data_[i * width_ + j];
            }
            if (ORD == ColMajor)
            {
                return data_[j * height_ + i];
            }
        }

        T &operator()(size_t i, size_t j)
        {
            if (ORD == RowMajor)
            {
                return data_[i * width_ + j];
            }
            if (ORD == ColMajor)
            {
                return data_[j * height_ + i];
            }
        }
    };

}
#endif
