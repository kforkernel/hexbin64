# hexbin64

A C command-line utility that converts signed 64-bit integers into their hexadecimal and 64-bit binary representations across three classic encodings: **Two's complement**, **One's complement**, and **Sign-magnitude**.

## Features

- **Multiple Signed Representations:** Displays 64-bit binary formatted in 8-bit byte groups alongside 16-digit padded hexadecimal (`0x...`) for:
  - Two's complement
  - One's complement
  - Sign-magnitude
- **Flexible Input Modes:** Pass one or multiple numbers directly as command-line arguments, or run with no arguments for an interactive prompt.
- **Safe Bitwise Operations:** Uses unsigned 64-bit (`uint64_t`) casting and portable `<inttypes.h>` macros (`PRId64`, `PRIX64`) to prevent undefined behavior and ensure cross-platform compatibility.
- **Strict Input Validation:** Uses `strtoimax` and `errno` checking to catch non-numeric inputs, trailing characters, and 64-bit integer overflows/underflows (`INT64_MIN` edge cases).
- **Zero External Dependencies:** Built using only the C standard library (`<stdio.h>`, `<stdint.h>`, `<inttypes.h>`, `<stdlib.h>`, `<errno.h>`).

## Compilation

Compile with `gcc` or `clang`:

```bash
gcc -Wall -Wextra -O2 hexbin64.c -o hexbin64
```

## Usage

```text
hexbin64 [number...]
```

### 1. Command-line arguments (single or multiple numbers)

```bash
./hexbin64 -5 42
```

### 2. Interactive mode (no arguments)

```bash
./hexbin64
```

### Example Output (`./hexbin64 -5`)

```text
Number (-5): Two's complement
11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111011
Hex: 0xFFFFFFFFFFFFFFFB
------------------------------------------------------

Number (-5): One's complement
11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111010
Hex: 0xFFFFFFFFFFFFFFFA
------------------------------------------------------

Number (-5): Sign magnitude
10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000101
Hex: 0x8000000000000005
------------------------------------------------------
```

## Supported Range

Because One's complement and Sign-magnitude are symmetric around zero, the valid input range is:

- **Minimum:** `-9,223,372,036,854,775,807` (-(2⁶³ - 1))
- **Maximum:** `9,223,372,036,854,775,807` (2⁶³ - 1)

## License

Copyright (c) 2026 @kforkernel. All rights reserved.  
Licensed under the **kforkernel Use License (KUL) v1.0**. See [LICENSE.md](LICENSE.md) for terms and conditions.
