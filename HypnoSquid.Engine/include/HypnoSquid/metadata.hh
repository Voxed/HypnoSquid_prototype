#pragma

/*
 * The goal of meta-data is to be simple but powerful enough.
 * Tags are simple names you can attach to classes.
 * Attributes are strings with a specified value.
 *
 * Everything is represented by inline static members, in order to not affect memory speed.
 */

#define HS_TAG(tag) \
    inline static const bool __META_HSTAG_##tag = 0;

#define HS_ATTR(attr, val) \
    inline static const std::string __META_HSATTR_##attr = #val;

#define HS_HAS_TAG(type, tag) \
    requires(type) { type::__META_HSTAG_##tag; }

#define HS_HAS_ATTR(type, attr) \
    requires(type) { type::__META_HSATTR_##attr; }

#define HS_GET_ATTR(type, attr) type::__META_HSATTR_##attr