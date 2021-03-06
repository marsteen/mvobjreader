#ifndef CRECTT_H
#define CRECTT_H

#include <sstream>
#include <string>

template<class T>
class CRectT
{
    public:

        void Set(T _left, T _top, T _right, T _bottom)
        {
            left = _left;
            top = _top;
            right = _right;
            bottom = _bottom;
        }


        bool InterSectRect(const CRectT* r2, CRectT* ri) const;
        bool InRect(T x, T y);
        void TrimRect(void);
        void Show(const char* title, std::stringstream& mstr);
        void Add(T xoff, T yoff);

        T Width() const { return right - left; }
        T Height() const { return bottom - top; }

        T left;
        T top;
        T right;
        T bottom;

    protected:

        bool IntersectLine(T A1, T B1, T A2, T B2, T* A3, T* B3) const;
};

#include <CRectT.hpp>

#endif
