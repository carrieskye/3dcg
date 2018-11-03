#include "animation/duration.h"
#include <limits>

using namespace animation;


Duration Duration::from_milliseconds(double ms)
{
    return Duration(ms);
}

Duration Duration::from_seconds(double s)
{
    return Duration(s * 1000.0);
}

Duration Duration::zero()
{
    return from_milliseconds(0);
}

Duration Duration::infinite()
{
    return from_milliseconds(std::numeric_limits<double>::infinity());
}

Duration::Duration(double ms)
    : m_milliseconds(ms)
{
    // NOP
}

double Duration::milliseconds() const
{
    return m_milliseconds;
}

double Duration::seconds() const
{
    return m_milliseconds / 1000.0;
}

Duration animation::operator +(const Duration& d1, const Duration& d2)
{
    return Duration::from_milliseconds(d1.milliseconds() + d2.milliseconds());
}

Duration animation::operator -(const Duration& d)
{
    return Duration::from_milliseconds(-d.milliseconds());
}

Duration animation::operator -(const Duration& d1, const Duration& d2)
{
    return Duration::from_milliseconds(d1.milliseconds() - d2.milliseconds());
}

Duration animation::operator *(const Duration& duration, double factor)
{
    return Duration::from_milliseconds(duration.milliseconds() * factor);
}

Duration animation::operator *(double factor, const Duration& duration)
{
    return duration * factor;
}

Duration animation::operator /(const Duration& duration, double factor)
{
    return duration * (1.0 / factor);
}

double animation::operator /(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() / d2.milliseconds();
}

bool animation::operator <(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() < d2.milliseconds();
}

bool animation::operator <=(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() <= d2.milliseconds();
}

bool animation::operator >=(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() >= d2.milliseconds();
}

bool animation::operator >(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() >= d2.milliseconds();
}

bool animation::operator ==(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() == d2.milliseconds();
}

bool animation::operator !=(const Duration& d1, const Duration& d2)
{
    return d1.milliseconds() != d2.milliseconds();
}

std::ostream& animation::operator <<(std::ostream& out, const Duration& duration)
{
    return out << duration.seconds() << "s";
}