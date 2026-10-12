#ifndef STDARG_H
#define STDARG_H

// va_list: اشاره‌گر به آرگومان‌های متغیر
typedef __builtin_va_list va_list;

// شروع خوندن آرگومان‌ها
#define va_start(ap, last)  __builtin_va_start(ap, last)

// آرگومان بعدی رو بگیر
#define va_arg(ap, type)    __builtin_va_arg(ap, type)

// تمیزکاری
#define va_end(ap)          __builtin_va_end(ap)

// کپی
#define va_copy(dest, src)  __builtin_va_copy(dest, src)

#endif