#include "imaging/bitmap.h"
#include <algorithm>
#include <assert.h>

using namespace imaging;


Bitmap::Bitmap(unsigned width, unsigned height)
    : m_pixels(width, height, colors::black())
{
    // NOP
}

unsigned Bitmap::width() const
{
    return m_pixels.width();
}

unsigned Bitmap::height() const
{
    return m_pixels.height();
}

bool Bitmap::is_inside(const Position2D& p) const
{
    return p.x < width() && p.y < height();
}

Color& Bitmap::operator[](const Position2D& p)
{
    assert(is_inside(p));

    return m_pixels[p];
}

const Color& Bitmap::operator[](const Position2D& p) const
{
    assert(is_inside(p));

    return m_pixels[p];
}

void Bitmap::clear(const Color& Color)
{
    for_each_position([this, &Color](const Position2D& p) {
        m_pixels[p] = Color;
    });
}

void Bitmap::for_each_position(std::function<void(const Position2D&)> callback) const
{
    m_pixels.for_each_position(callback);
}

Bitmap& Bitmap::operator +=(const Bitmap& bitmap)
{
    assert(width() == bitmap.width());
    assert(height() == bitmap.height());

    for_each_position([this, &bitmap](const Position2D& p) {
        (*this)[p] += bitmap[p];
    });

    return *this;
}

Bitmap& Bitmap::operator -=(const Bitmap& bitmap)
{
    assert(width() == bitmap.width());
    assert(height() == bitmap.height());

    for_each_position([this, &bitmap](const Position2D& p) {
        (*this)[p] -= bitmap[p];
    });

    return *this;
}

Bitmap& Bitmap::operator *=(double constant)
{
    for_each_position([this, constant](const Position2D& p) {
        (*this)[p] *= constant;
    });

    return *this;
}

Bitmap& Bitmap::operator /=(double constant)
{
    return *this *= 1 / constant;
}

void Bitmap::invert()
{
    for_each_position([this](const Position2D& position) {
        (*this)[position].invert();
    });
}
