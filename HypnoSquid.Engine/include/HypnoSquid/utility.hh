#pragma once

namespace hs {

    /// Function running static_cast for use with monadic functions.
    /// \tparam To Type to cast to.
    /// \tparam From Type to cast from.
    /// \param from [in] Value to cast from.
    /// \return Casted value.
    template <typename To, typename From>
    To staticCast(From from) {
        return static_cast<To>(from);
    }

}