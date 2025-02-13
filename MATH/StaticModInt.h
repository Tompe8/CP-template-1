#ifndef __OY_STATICMODINT__
#define __OY_STATICMODINT__

#include <cstdint>
#include <functional>
#include "Modular.h"

namespace OY {
    template <typename _ModType, _ModType _P, bool _IsPrime = false>
    struct StaticModInt {
        using mint = StaticModInt<_ModType, _P, _IsPrime>;
        _ModType m_val;
        static_assert(_P > 1 && _P < 1ull << 63);
        constexpr StaticModInt() : m_val(0) {}
        template <typename _Tp, std::enable_if_t<std::is_signed_v<_Tp>, bool> = true>
        constexpr StaticModInt(_Tp __val) : m_val(0) {
            int64_t x = int64_t(__val) % int64_t(mod());
            if (x < 0) x += mod();
            m_val = x;
        }
        template <typename _Tp, std::enable_if_t<std::is_unsigned_v<_Tp>, bool> = true>
        constexpr StaticModInt(_Tp __val) : m_val(__val % mod()) {}
        static constexpr mint raw(_ModType __val) {
            mint res;
            res.m_val = __val;
            return res;
        }
        static constexpr _ModType mod() { return _P; }
        constexpr _ModType val() const { return m_val; }
        constexpr mint pow(uint64_t __n) const { return Modular<_ModType, _P>::pow(m_val, __n); }
        constexpr mint inv() const {
            if constexpr (_IsPrime)
                return inv_Fermat();
            else
                return inv_exgcd();
        }
        constexpr mint inv_exgcd() const {
            _ModType x = mod(), y = m_val, m0 = 0, m1 = 1;
            while (y) {
                _ModType z = x / y;
                x -= y * z;
                m0 -= m1 * z;
                std::swap(x, y);
                std::swap(m0, m1);
            }
            if (m0 >= mod()) m0 += mod() / x;
            return raw(m0);
        }
        constexpr mint inv_Fermat() const { return pow(mod() - 2); }
        constexpr mint &operator++() {
            if (++m_val == mod()) m_val = 0;
            return *this;
        }
        constexpr mint &operator--() {
            if (m_val == 0) m_val = mod();
            m_val--;
            return *this;
        }
        constexpr mint operator++(int) {
            mint old(*this);
            ++*this;
            return old;
        }
        constexpr mint operator--(int) {
            mint old(*this);
            --*this;
            return old;
        }
        constexpr mint &operator+=(const mint &__other) {
            m_val = Modular<_ModType, _P>::plus(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator-=(const mint &__other) {
            m_val = Modular<_ModType, _P>::minus(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator*=(const mint &__other) {
            m_val = Modular<_ModType, _P>::multiply(m_val, __other.m_val);
            return *this;
        }
        constexpr mint &operator/=(const mint &__other) { return *this *= __other.inv(); }
        constexpr mint operator+() const { return *this; }
        constexpr mint operator-() const { return raw(m_val ? mod() - m_val : 0); }
        constexpr bool operator==(const mint &__other) const { return m_val == __other.m_val; }
        constexpr bool operator!=(const mint &__other) const { return m_val != __other.m_val; }
        constexpr bool operator<(const mint &__other) const { return m_val < __other.m_val; }
        constexpr bool operator>(const mint &__other) const { return m_val > __other.m_val; }
        constexpr bool operator<=(const mint &__other) const { return m_val <= __other.m_val; }
        constexpr bool operator>=(const mint &__other) const { return m_val <= __other.m_val; }
        template <typename _Tp>
        constexpr explicit operator _Tp() const { return _Tp(m_val); }
        constexpr friend mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
        constexpr friend mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
        constexpr friend mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
        constexpr friend mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
        template <typename _Istream>
        friend _Istream &operator>>(_Istream &is, mint &self) { return is >> self.m_val; }
        template <typename _Ostream>
        friend _Ostream &operator<<(_Ostream &os, const mint &self) { return os << self.m_val; }
    };
    template <uint32_t _P, bool _IsPrime>
    using StaticModInt32 = StaticModInt<uint32_t, _P, _IsPrime>;
    template <uint64_t _P, bool _IsPrime>
    using StaticModInt64 = StaticModInt<uint64_t, _P, _IsPrime>;
}

#endif